#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

class BatchInfo
{
  double flank_5;
  double flank_3;
  int start;
  int stop;

public:
  BatchInfo () 
    : flank_5(0), flank_3(0), start(0), stop(0) 
  {  }
  
  double get_flank_5() { return flank_5; }
  double get_flank_3() { return flank_3; }
  int get_start() { return start; }
  int get_stop() { return stop; }
  
  void set_flank_5(char const *s) { sscanf (s, "%lf", &flank_5); }
  void set_flank_3(char const *s) { sscanf (s, "%lf", &flank_3); }
  void set_start(char const *s) { start = atoi(s); }
  void set_stop(char const *s) { stop = atoi(s); }
};

void calculate_averages(BatchInfo * binfoptr, size_t count);
void parse (const char *, BatchInfo *, size_t batch_size);

int main (int argc, char *argv[])
{
  size_t size = atoi(argv[2]);
  BatchInfo * batch = new BatchInfo [size];
 
  try 
  {
    parse(argv[1], batch, size);
  }
  catch (std::exception & e)
  {
    std::cerr << "ERROR: " << e.what () << std::endl;
    delete [] batch;
    exit(EXIT_FAILURE);
  }
  
  calculate_averages(batch, size);
  
  delete [] batch;
  return 0;
}

void calculate_averages(BatchInfo * batch, size_t count)
{
  double f5avg = 0, f3avg = 0;
  long  long startavg = 0, stopavg = 0;
  
  for(size_t i = 0;i < count;++i)
  {
    f5avg    += batch[i].get_flank_5();
    f3avg    += batch[i].get_flank_3();
    startavg += batch[i].get_start();
    stopavg  += batch[i].get_stop();
  }

  f5avg /= count;
  f3avg /= count;
  startavg /= count;
  stopavg /= count;
  
  printf("flank_5 average = %lf\n", f5avg);
  printf("flank_3 average = %lf\n", f3avg);
  printf("start average = %Ld\n", startavg);
  printf("stop average = %Ld\n", stopavg);
}

bool match_chars(char c)
{
  switch (c)
  {
    case '<':
    case '>':
    case '"':
    case '=':
    case '/':
        return true;
    default:
        return false;
  }
}

void skip_whitespaces(char * &str)
{
  while (str && isspace(*str)) ++str;
}

char * tokenize (char *& str)
{
  static char old = '\0';
  static char * end = 0;
  
  if (!str)
    throw std::runtime_error ("Input stream terminated abruptly");
  
  if (end) 
    *end = old;
  
  skip_whitespaces(str);
  
  char * begin = str;
  if (str && match_chars(*str))
  {
    end = ++str;
  }
  else
  {
    while (str && !isspace(*str) && !match_chars(*str)) ++str;
    end = str;
  }
  old = *end;
  *end = '\0';
  return begin;
}

void match (char * &str, const char * m)
{
  if (strcmp(tokenize(str), m)) 
    throw std::runtime_error ("Invalid XML");
}

void match_half_tag (char * &str, const char * tagname)
{
  match(str, "<"); 
  match(str, tagname); 
}

void match_close_tag (char * &str, const char * tagname)
{
  match(str, "<");
  match(str, "/");
  match(str, tagname); 
  match(str, ">"); 
}

void match_open_tag (char * &str, const char * tagname)
{
  match(str, "<");
  match(str, tagname); 
  match(str, ">"); 
}

char * match_attribute (char * &str, const char * id)
{
  match(str, id);
  match(str, "="); 
  match(str, "\""); 
  char * val = tokenize (str);
  match(str, "\""); 
  return val;
}

char * match_open_close_tag (char * &str, const char * tagname)
{
  match_open_tag (str, tagname);
  char * val = tokenize(str);
  match_close_tag(str, tagname);
  return val;
}

void skip_upto (char *&str, char ch)
{
  while(str && (*str++ != ch));
}

void match_xml (char * &str)
{
  match_half_tag(str, "?xml");
  match_attribute(str, "version");
  match_attribute(str, "encoding");
  skip_upto(str, '>');
}

void match_batch_allocation (char * &str)
{
  match_half_tag(str, "hm:batch_allocation");
  match_attribute(str, "lsid");
  skip_upto(str, '>');
}

void match_sequence(char *& str, BatchInfo & binfo)
{
  match_open_tag (str, "hm:sequence");
  char * flank5 = match_open_close_tag (str, "hm:flank_5");
  char * flank3 = match_open_close_tag (str, "hm:flank_3");
  match_close_tag (str, "hm:sequence");
  binfo.set_flank_5(flank5);
  binfo.set_flank_3(flank3);
}

void match_coordinates (char *&str, BatchInfo & binfo)
{
  match_half_tag(str, "hm:coordinates");
  char * start = match_attribute(str, "start");
  binfo.set_start(start);
  char * stop = match_attribute(str, "stop");
  binfo.set_stop(stop);
  match(str, "/");
  match(str, ">");
}

void match_genomic_location (char *& str, BatchInfo & binfo)
{
  match_open_tag (str, "hm:genomic_location");
  match_coordinates(str, binfo);
  match_close_tag (str, "hm:genomic_location");
}

void match_snp (char *& str, BatchInfo & binfo)
{
  match_half_tag(str, "hm:snp");
  char * lsid = match_attribute(str, "lsid");
  match (str, ">");

  match_sequence(str, binfo);
  match_genomic_location(str, binfo);

  match_close_tag (str, "hm:snp");
}

void match_batchinfo (char * &str, BatchInfo & binfo)
{
  match_open_tag(str, "hm:BatchInfo");
  match_snp(str, binfo);
  match_close_tag (str, "hm:BatchInfo");
}

void parse (const char * filename, BatchInfo * batch, size_t batch_size)
{
  std::ifstream infile (filename);
  if(infile)
  {    
    std::ostringstream temp;
    temp << infile.rdbuf();
    std::string file_str (temp.str());
    char * fileptr  = (char *) file_str.c_str();

    match_xml (fileptr);
    match_batch_allocation (fileptr);
  
    for (size_t count = 0; count < batch_size; ++count)
    {
      match_batchinfo (fileptr, batch[count]);
    }
  }
  else
  {
    throw std::runtime_error("Can't open file for reading.");
  }
}

