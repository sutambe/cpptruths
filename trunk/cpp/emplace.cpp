#include <vector>
#include <iostream>

struct Point
{
  int x, y;
  Point(int a, int b) : x(a), y(b) 
  {
    std::cout << "Point::Point\n";
  }
  Point(const Point &p) : x(p.x), y (p.y) 
  {
    std::cout << "Point::Point(Point const &)\n";
  }
  Point(Point &&p) : x(p.x), y (p.y) 
  {
    std::cout << "Point::Point(Point &&)\n";
  }
  ~Point()
  {
    std::cout << "~Point\n";
  }
};

int main(void)
{
  std::vector<Point> v;
  v.emplace_back(1,2); // Creates one less Point object than the two below.
  //v.push_back(Point(11,22));
  //v.emplace_back(Point(11,22));
}
