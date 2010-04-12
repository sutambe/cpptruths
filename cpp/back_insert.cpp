#include <list>
#include <vector>
#include <iterator>
#include <iostream>

int main (void)
{
  std::list <int> l;
  std::vector <int> v;
  std::back_insert_iterator <std::list <int> > back_iter (l);
  std::front_insert_iterator <std::list <int> > front_iter (l);
  back_iter = 10;
  *back_iter = 20;
  front_iter = 30;
  *front_iter = 40;

  std::copy (l.begin(), l.end(), std::ostream_iterator <int> (std::cout, "\n"));
  return 0;
}

