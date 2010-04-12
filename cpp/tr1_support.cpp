#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <tr1/functional>
#include <tr1/array>

class point {
public:
    point(double x, double y) : m_x(x), m_y(y) { }
    double mag() const {
        return std::sqrt(m_x * m_x + m_y * m_y); }
    double dist(const point& p) const {
        return std::sqrt(pow(m_x - p.m_x, 2)
            + pow(m_y - p.m_y, 2)); }
private:
    double m_x;
    double m_y;
};

int main(void)
{
  std::tr1::array<int, 5> a = {{ 1,2,3,4,5 }};

  std::vector<point> v;
  v.push_back(point(3, 4));
  v.push_back(point(5, 12));
  v.push_back(point(8, 15));

  std::transform(v.begin(), v.end(),
      std::ostream_iterator<double>(std::cout, "\n"),
      std::tr1::mem_fn(&point::mag));

  std::transform(v.begin(), v.end(),
      std::ostream_iterator<double>(std::cout, "\n"),
      std::mem_fun_ref(&point::mag));

/******************************************************/
  
  std::vector<const point *> pv;
  pv.push_back(new point(3, 4));
  pv.push_back(new point(5, 12));
  pv.push_back(new point(8, 15));

  std::transform(pv.begin(), pv.end(),
      std::ostream_iterator<double>(std::cout, "\n"),
      std::tr1::mem_fn(&point::mag));

  std::transform(pv.begin(), pv.end(),
      std::ostream_iterator<double>(std::cout, "\n"),
      std::mem_fun(&point::mag));

/******************************************************/

  using namespace std::tr1::placeholders;
  
  std::vector<int> iv;
  iv.push_back(10);
  iv.push_back(20);
  iv.push_back(30);
  
  std::transform(iv.begin(), iv.end(),
      std::ostream_iterator<int>(std::cout, "\n"),
      std::tr1::bind(std::multiplies<int>(), _1, _1));

  std::transform(iv.begin(), iv.end(),
      std::ostream_iterator<int>(std::cout, "\n"),
      std::bind1st(std::minus<int>(), 10));

  return 0;
}
