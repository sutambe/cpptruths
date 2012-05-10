#include <cstdio>
#include <string>
#include <typeinfo>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

template<bool> struct Write_Sample_Types_Match_Assertion_Failure;   
template<>     struct Write_Sample_Types_Match_Assertion_Failure<true>{};
#define MATCH_TYPES_STATIC_ASSERT(e) (Write_Sample_Types_Match_Assertion_Failure <(e)>())

struct true_type {  
    enum { value = 1 }; 
};

struct false_type { 
    enum { value = 0 }; 
};

template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};

template <typename T> struct is_char_pointer              : false_type {};
template <> struct is_char_pointer<char *>                : true_type  {};
template <> struct is_char_pointer<const char *>          : true_type  {};
template <> struct is_char_pointer<volatile char *>       : true_type  {};
template <> struct is_char_pointer<const volatile char *> : true_type  {};

template <unsigned N> struct is_char_pointer<char[N]>                 : true_type  {};
template <unsigned N> struct is_char_pointer<const char[N]>           : true_type  {};
template <unsigned N> struct is_char_pointer<volatile char[N]>        : true_type  {};
template <unsigned N> struct is_char_pointer<volatile const char[N]>  : true_type  {};

template <typename T, typename U>
struct are_both_char_pointers
{ 
	enum { value = is_char_pointer<T>::value && 
	               is_char_pointer<U>::value };
};

template <typename T, typename U>
struct are_both_same_and_not_char_pointers
{
    enum { value = is_same<T, U>::value       &&
                   !is_char_pointer<T>::value && 
                   !is_char_pointer<U>::value     };
};

template <typename T> struct is_string              : false_type {};
template <> struct is_string<std::string>           : true_type  {};
template <> struct is_string<std::string &>         : true_type  {};
template <> struct is_string<const std::string>     : true_type  {};
template <> struct is_string<const std::string &>   : true_type  {};

template<typename T, typename U>
struct are_both_strings
{
    enum { value = is_char_pointer<T>::value &&
                   is_string<U>::value            };
};

template <bool, typename T = void>
struct enable_if { };

template <typename T>
struct enable_if<true, T> 
{ 
    typedef T type;
};

template <bool, typename T = void>
struct disable_if 
{ 
    typedef T type;
};

template <typename T>
struct disable_if<true, T> { };

template <class T>
struct remove_const;

template <class T>
struct remove_const<const T>
{
    typedef T type;
};

typedef int RequestHandle;

template <typename T>
struct WriteSample
{
    WriteSample() {}
    WriteSample(const WriteSample &) 
    {
        printf("************** WriteSample Copied ****************\n");
    }
};

template <typename TReq, typename TRep>
class Replier
{
public:

	template <typename URep>
    void send_reply(const URep & rep, 
        const RequestHandle & handle,
        typename disable_if<is_char_pointer<URep>::value>::type * = 0);
	
    // VC++ does not like a boolean expression in enable_if.
    // That's why the boolean expression is refactored in are_both_char_pointers.
    template <typename URep>
    void send_reply(URep rep, 
                    const RequestHandle & handle,
                    typename enable_if<are_both_char_pointers<TRep, URep>::value>::type * = 0);

};

template <typename TReq, typename TRep>
template <typename URep>
void Replier<TReq, TRep>::send_reply(const URep & rep, 
                                     const RequestHandle & handle,
                                     typename disable_if<is_char_pointer<URep>::value>::type *)
{
    printf("URep is a NOT pointer.\n");
}

template <typename TReq, typename TRep>
template <typename URep>
void Replier<TReq, TRep>::send_reply(URep rep, 
    const RequestHandle & handle,
    typename enable_if<are_both_char_pointers<TRep, URep>::value>::type *)
{
    printf("URep is a pointer.\n");
}

template <typename TReq, typename TRep>
class Requester
{
    static void send_request_impl(const char *)
    {
      printf("send_request_impl(const char *)\n");
    }

    static void send_request_impl(const std::string &)
    {
      printf("send_request_impl(const std::string &)\n");
    }

    static void send_request_impl(WriteSample<char *> &)
    {
      printf("send_request_impl(WriteSample<char *> &)\n");
    }

    static void send_request_impl(WriteSample<const char *> &)
    {
      printf("send_request_impl(WriteSample<char *> &)\n");
    }

    template <class T>
    static void send_request_impl(WriteSample<T> &)
    {
      printf("send_request_impl(WriteSample<T> &)\n");
    }

    template <class T>
    static void send_request_impl(const T &)
    {
      printf("send_request_impl(const T &)\n");
    }

public:

/*
    void send_request(const TReq & req);

    template <typename UReq>
    typename enable_if<are_both_strings<TReq, UReq>::value>::type
    send_request(const UReq & req) {}

    template <typename UReq>
    typename enable_if<are_both_char_pointers<TReq, UReq>::value>::type
    send_request(WriteSample<UReq> & req) {}

    template <typename UReq>
    typename enable_if<are_both_same_and_not_char_pointers<TReq, UReq>::value>::type
    send_request(WriteSample<UReq> & req){}
*/

    template <typename UReq>
    void send_request_fixed(const UReq & ureq)
    {
#ifdef __GNUG__
      int s;
      printf("type = %s\n", abi::__cxa_demangle(typeid(const UReq &).name(), 0, 0, &s));
#else
      printf("type = %s\n", typeid(const UReq &).name());
#endif
      send_request_impl(ureq);
    }

    template <typename UReq>
    void send_request_fixed(WriteSample<UReq> & ureq)
    {
      MATCH_TYPES_STATIC_ASSERT((is_same<TReq, UReq>::value));
#ifdef __GNUG__
      int s;
      printf("WriteSample type = %s\n", abi::__cxa_demangle(typeid(WriteSample<UReq> &).name(), 0, 0, &s));
#else
      printf("WriteSample type = %s\n", typeid(WriteSample<UReq> &).name());
#endif
      send_request_impl(ureq);
    }

    template <typename UReq>
    void send_request_fixed(const WriteSample<UReq> & ureq);
};

struct Foo
{
    Foo() {}
    Foo(const Foo &)
    {
        printf("Foo copied\n");
    }
};

int main(void)
{
    Replier<char *, char *> r1;
    r1.send_reply("RTI", 999);

    Replier<Foo, Foo> r2;
    r2.send_reply(999, 999);
    
    Requester<char *, char *> requester1;
    requester1.send_request_fixed("RTI");
    
    WriteSample<char *> ws;
    requester1.send_request_fixed(ws);
    std::string str = "RTI";
    requester1.send_request_fixed(str.c_str());
    
    Requester<Foo, Foo> requester2;
    const Foo f;
    requester2.send_request_fixed(f);
    WriteSample<Foo> foo_ws;
    requester2.send_request_fixed(foo_ws);
}

