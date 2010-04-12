#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define FLANK5_TAG_LENGTH 12
#define FLANK3_TAG_LENGTH 12
#define START_TAG_LENGTH 7
#define STOP_TAG_LENGTH 6

size_t size = 0;

class BatchInfo
{
  static double * flank_5;
  static double * flank_3;
  static int * start;
  static int * stop;

  static size_t count;

  static double f5_average;
  static double f3_average;
  static int start_average;
  static int stop_average;

  size_t index;

  void createBatch(size_t size) 
  { 
    flank_5 = new double [size];
    flank_3 = new double [size];
    start = new int [size];
    stop = new int [size];
  }

  void destroyBatch()
  {
    delete [] flank_5;
    delete [] flank_3;
    delete [] start;
    delete [] stop;
    flank_5 = 0;
    flank_3 = 0;
    start = 0;
    stop = 0;
  }

  static void *calc_flank5_avg (void *)
  {
    double sum = 0;
    for (size_t i = 0;i < count; ++i)
      sum += flank_5[i];
    
    f5_average = sum/count;
    return 0;
  }

  static void *calc_flank3_avg (void *)
  {
    double sum = 0;
    for (size_t i = 0;i < count; ++i)
      sum += flank_3[i];
    
    f3_average = sum/count;
    return 0;
  }

  static void *calc_start_avg (void *)
  {
    long long sum = 0;
    for (size_t i = 0;i < count; ++i)
      sum += start[i];
    
    start_average = sum/count;
    return 0;
  }

  static void *calc_stop_avg (void *)
  {
    long long sum = 0;
    for (size_t i = 0;i < count; ++i)
      sum += stop[i];
    
    stop_average = sum/count;
    return 0;  
  }


public:
  inline explicit BatchInfo () 
    : index(count++) 
  {
    if (!flank_5)
      createBatch (size);
  }
  
  ~BatchInfo()
  {
    if (--count == 0)  
      destroyBatch();
  }

  inline double get_flank_5() { return flank_5[index]; }
  inline double get_flank_3() { return flank_3[index]; }
  inline int get_start() { return start[index]; }
  inline int get_stop() { return stop[index]; }
  
  inline void set_flank_5(char const *s) { sscanf (s, "%lf", &flank_5[index]); }
  inline void set_flank_3(char const *s) { sscanf (s, "%lf", &flank_3[index]); }
  inline void set_start(char const *s) { sscanf (s, "%d", &start[index]); }
  inline void set_stop(char const *s) { sscanf (s, "%d", &stop[index]); }

  inline double get_batch_size() { return count; }


  static void calculate_averages ()
  {
    pthread_t t1, t2, t3;
    pthread_attr_t attr;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   
    pthread_create (&t1, &attr, BatchInfo::calc_flank5_avg, 0);
    pthread_create (&t2, &attr, BatchInfo::calc_flank3_avg, 0);
    pthread_create (&t3, &attr, BatchInfo::calc_start_avg, 0);
    calc_stop_avg (0);

    pthread_join (t1, 0 );
    pthread_join (t2, 0 );
    pthread_join (t3, 0 );
    
    pthread_attr_destroy(&attr);
  }
  
  static double flank5_avg ()  { return f5_average; }
  static double flank3_avg ()  { return f3_average; }
  static int start_avg ()  { return start_average; }
  static int stop_avg ()  { return stop_average; }
};

double * BatchInfo::flank_5 = 0;
double * BatchInfo::flank_3 = 0;
int * BatchInfo::start = 0;
int * BatchInfo::stop = 0;

size_t BatchInfo::count = 0;

double BatchInfo::f5_average = 0;
double BatchInfo::f3_average = 0;
int BatchInfo::start_average = 0;
int BatchInfo::stop_average = 0;

void parse (const char *, BatchInfo *, size_t batch_size);

int main (int argc, char *argv[])
{
  size = atoi(argv[2]);
  BatchInfo * batch = new BatchInfo [size];
  
  parse(argv[1], batch, size);
/*
  for (size_t i = 0;i < size; ++i)
  {
    printf("batch[%d].flank_5 = %lf\n", i, batch[i].get_flank_5());
    printf("batch[%d].flank_3 = %lf\n", i, batch[i].get_flank_3());
    printf("batch[%d].start = %d\n", i, batch[i].get_start());
    printf("batch[%d].stop = %d\n", i, batch[i].get_stop());
  }
*/
  BatchInfo::calculate_averages();
  
  printf("flank_5 average = %lf\n", BatchInfo::flank5_avg());
  printf("flank_3 average = %lf\n", BatchInfo::flank3_avg());
  printf("start average = %d\n", BatchInfo::start_avg());
  printf("stop average = %d\n", BatchInfo::stop_avg());
  
  delete [] batch;
  pthread_exit((void *) 0);
  return 0;
}

char * search_flank_5(char * str, BatchInfo & binfo)
{
  char * begin = strstr (str, "<hm:flank_5>") + FLANK5_TAG_LENGTH;
  char * end = strstr (begin, "</hm:flank_5>");
  *end = 0;
  binfo.set_flank_5(begin);
  *end = '<';
  return (end + FLANK5_TAG_LENGTH + 1);
}

char * search_flank_3(char * str, BatchInfo & binfo)
{
  char * begin = strstr (str, "<hm:flank_3>") + FLANK3_TAG_LENGTH;
  char * end = strstr (begin, "</hm:flank_3>");
  *end = 0;
  binfo.set_flank_3(begin);
  *end = '<';
  return (end + FLANK3_TAG_LENGTH + 1);
}

char * search_start(char * str, BatchInfo & binfo)
{
  char * begin = strstr (str, "start=\"") + START_TAG_LENGTH;
  char * end = strstr (begin, "\"");
  *end = 0;
  binfo.set_start(begin);
  *end = '"';
  return (end + 1);
}

char * search_stop(char * str, BatchInfo & binfo)
{
  char * begin = strstr (str, "stop=\"") + STOP_TAG_LENGTH;
  char * end = strstr (begin, "\"");
  *end = 0;
  binfo.set_stop(begin);
  *end = '"';
  return (end + 1);
}

void parse (const char * filename, BatchInfo * batch, size_t batch_size)
{
  std::ostringstream temp;
  std::ifstream infile (filename);
  temp << infile.rdbuf();
  std::string file_str (temp.str());
  char * fileptr  = (char *) file_str.c_str();

  for (size_t count = 0; count < batch_size; ++count)
  {
    fileptr = search_flank_5(fileptr, batch[count]);
    fileptr = search_flank_3(fileptr, batch[count]);
    fileptr = search_start(fileptr, batch[count]);
    fileptr = search_stop(fileptr, batch[count]);
  }
  
}

