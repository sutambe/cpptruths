/*
namespace dds { namespace sub {
  template <class T, template <class> class DELEGATE>
  class DataReader
  {};

} }

namespace rti {
  template <class T>
  class DataReader
  { };
}

namespace dds { namespace sub { namespace detail {
//  template <class T>
//  class DataReader 
//  {};
    using rti::DataReader;

} } }

namespace dds { namespace sub {

template <typename T,
          template <typename Q> class DELEGATE = dds::sub::detail::DataReader>
class DataReader;

} }
*/

namespace rti {
  template <class T>
  class DataReader
  { };
}

namespace dds { namespace sub { namespace detail {
//  template <class T>
//  class DataReader 
//  {};
    using rti::DataReader;

} } }

namespace dds { namespace sub {
  template <typename T,
            template <typename Q> class DELEGATE = dds::sub::detail::DataReader>
  class DataReader
  { };

} }

int main(void)
{
  dds::sub::DataReader<int> dr;
}
