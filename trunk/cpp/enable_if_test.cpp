#include <cstdio>

namespace details {

/**** Substitution Failure Is Not An Error (SFINAE) idiom helper meta-functions ****/

struct true_type {  
    enum { value = 1 }; 
};

struct false_type { 
    enum { value = 0 }; 
};

template <typename T> struct is_char_pointer              : false_type {};
template <> struct is_char_pointer<char *>                : true_type  {};
template <> struct is_char_pointer<const char *>          : true_type  {};
template <> struct is_char_pointer<volatile char *>       : true_type  {};
template <> struct is_char_pointer<const volatile char *> : true_type  {};

template <unsigned N> struct is_char_pointer<char[N]>                 : true_type  {};
template <unsigned N> struct is_char_pointer<const char[N]>           : true_type  {};
template <unsigned N> struct is_char_pointer<volatile char[N]>        : true_type  {};
template <unsigned N> struct is_char_pointer<volatile const char[N]>  : true_type  {};

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

template <typename T, typename U>
struct are_both_char_pointers
{ 
    enum { value = is_char_pointer<T>::value && 
                   is_char_pointer<U>::value };
};

} //namespace details

typedef int RequestHandle;

template <typename TReq, typename TRep>
class Replier
{
public:

	template <typename URep>
    void send_reply(const URep & rep, 
                    const RequestHandle & handle,
                    typename details::disable_if<details::is_char_pointer<URep>::value>::type * = 0);
	
    // VC++ does not like a boolean expression in enable_if.
    // That's why the boolean expression is refactored in are_both_char_pointers.
    template <typename URep>
    void send_reply(URep rep, 
                    const RequestHandle & handle,
                    typename details::enable_if<details::are_both_char_pointers<TRep, URep>::value>::type * = 0);

};

template <typename TReq, typename TRep>
template <typename URep>
void Replier<TReq, TRep>::send_reply(const URep & rep, 
                                     const RequestHandle & handle,
                                     typename details::disable_if<details::is_char_pointer<URep>::value>::type *)
{
    printf("URep is a NOT pointer.\n");
}

template <typename TReq, typename TRep>
template <typename URep>
void Replier<TReq, TRep>::send_reply(URep rep, 
                                     const RequestHandle & handle,
                                     typename details::enable_if<details::are_both_char_pointers<TRep, URep>::value>::type *)
{
    printf("URep is a pointer.\n");
}

int main(void)
{
	Replier<char *, char *> r1;
    r1.send_reply("RTI", 999);

	Replier<int, int> r2;
    r2.send_reply(999, 999);
}
/*
foo   foo     OK            OK
foo   char*   LONG ERRORS   LESS ERRORS
char* foo     LONG ERRORS   LONG ERRORS
char* char*   OK            OK
*/
