#include <iterator>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <sstream>
#include <fstream>

int main (int argc, char *argv [])
{
  //std::istream_iterator <char> start (std::cin), end;
  //std::istreambuf_iterator <char> start (std::cin), end;
  //std::string str (start, end);
  //str.clear();
  
  std::ostringstream str_stream;
  //str_stream << std::cin.rdbuf();  
  //std::string str (str_stream.str());  

  //std::cout << str << std::endl << str.size () << std::endl;

  //std::ifstream infile (argv[1]); 
  std::istream_iterator <int> start (std::cin), end;
  std::set <int> integers (start, end);
  //std::vector <int> integers;
  //std::copy (start, end, std::back_inserter (integers));
  std::copy (integers.begin (), integers.end (), std::ostream_iterator <int> (str_stream, "\n"));
  std::cout << str_stream.str();

  return 0;
}


