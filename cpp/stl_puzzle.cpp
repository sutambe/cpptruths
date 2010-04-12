#include <set>
#include <map>
#include <functional>
#include <algorithm>
#include <string>

struct : public std::unary_function<std::string, 
                                    std::pair<std::string,bool> > 
{
  std::pair<std::string,bool> operator()(const std::string& s) 
  { 
    return std::pair<std::string,bool> (s,false); 
  }
} word_map_inserter;


int main (int /* argc */, char* /* argv */[])
{
  // the goal of this code is to take every entry of words and
  // use it as a key to insert a "false" into the map

  std::set<std::string> words;

  words.insert ("This");
  words.insert ("is");
  words.insert ("a");
  words.insert ("tricky");
  words.insert ("STL");
  words.insert ("problem");

  std::map <std::string, bool> word_map;

//  std::transform (words.begin (),
//		  words.end (),
//		  std::inserter (word_map,
//				 word_map.end()),
//		  word_map_inserter);
  word_map_inserter ("Sumant");

  return 0;
}
