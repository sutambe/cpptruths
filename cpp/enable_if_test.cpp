#include <cstdio>
#include <string>
#include <typeinfo>

template<bool> struct CompileTimeAssert;   
template<> struct CompileTimeAssert<true>{};
#define STATIC_ASSERT(e) (CompileTimeAssert <(e)>())

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

typedef int RequestHandle;

template <typename T>
class WriteSample
{};

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
	void send_request_impl(const char *)
	{
		printf("send_request_impl(const char *)\n");
	}

	void send_request_impl(const std::string &)
	{
		printf("send_request_impl(const std::string &)\n");
	}

	void send_request_impl(WriteSample<char *> &)
	{
		printf("send_request_impl(WriteSample<char *> &)\n");
	}

	void send_request_impl(WriteSample<const char *> &)
	{
		printf("send_request_impl(WriteSample<char *> &)\n");
	}

    template <class T>
	void send_request_impl(WriteSample<T> &)
	{
		printf("send_request_impl(WriteSample<T> &)\n");
	}

    template <class T>
	void send_request_impl(const T &)
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
    void send_request_fixed(UReq ureq)
	{
        printf("type = %s\n", typeid(UReq).name());
		send_request_impl(ureq);
	}
};

int main(void)
{
    typedef int Foo;
    
	Replier<char *, char *> r1;
    r1.send_reply("RTI", 999);

	Replier<Foo, Foo> r2;
    r2.send_reply(999, 999);
    
    Requester<char *, char *> requester1;
    requester1.send_request_fixed("RTI");
    
    const WriteSample<char *> ws;
    requester1.send_request_fixed(ws);
    
    Requester<Foo, Foo> requester2;
    requester2.send_request_fixed(999);
}

