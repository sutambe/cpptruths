#include <streambuf>  // for std::streambuf
#include <ostream>    // for std::ostream
#include <unistd.h>

class fdoutbuf
    : public std::streambuf
{
public:
    explicit fdoutbuf(int fd) : fd_(1) {}
    int overflow(int ch) 
    {
      write(fd_, &ch, 1);
      return 0;
    }
    int fd_;
};

struct fdostream_pbase // A newly introduced class
{
    fdoutbuf sbuffer; // The member moved ‘up’ the hierarchy.

    explicit fdostream_pbase( int fd )
        : sbuffer( fd )
    {}
};

class fdostream
    : protected fdostream_pbase // This class will be initialized before the next one.
    , public std::ostream
{
public:
    explicit fdostream( int fd )
        : fdostream_pbase( fd ), // Initialize the newly added base before std::ostream.
          std::ostream( &sbuffer ) //  Now safe to pass the pointer 
    {}
    //...
};
int main(void)
{
  fdostream out(1);
  out << "Hello, World\n";

  return 0;
}
