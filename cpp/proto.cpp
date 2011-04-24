#include <iostream>
#include <vector>
#include <boost/proto/proto.hpp>
#include <boost/typeof/std/ostream.hpp>

using namespace boost;

class Catalog {};
class Book {};
class Author {};

proto::terminal<Catalog>::type const _catalog = {{}};
proto::terminal<Book>::type const _book = {{}};
proto::terminal<Author>::type const _author = {{}};

int main()
{
	proto::display_expr(_catalog >>= _author >>= (_book, _book, _book));
		
    return 0;
}
