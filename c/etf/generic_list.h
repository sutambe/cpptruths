
#ifndef __GENERIC_LIST_H
#define __GENERIC_LIST_H

#include <stdlib.h>

/**
    \brief Clearly shows the type of elements of a generic list at the declaration.
    
    GLIST macro allows the programmer to clearly state the type of the elements
    in the generic list. 
*/
#define GLIST(x) Glist
#define GLIST_ITERATOR(x) Glist_iterator

/**
    \brief A structure to make another structure storable in a generic list.

    This functionality of this structure is based on the philosophy of doing
    object oriented programming in a non-object oriented programming language.
    If you want to store a structure in a list all you need to do is to "inherit" 
    the \e Glist_element structure in your structure. Because C does not provide 
    language level support for inheritance, it needs to be programmed. 
    When \e Glist_element is declared as the first data field in a structure, that 
    structure can be added, searched, duplicated, deleted, in the list using
    the API provided by \e generic_list library. It is very important to note that a
    \e Glist_element data field must be the first data field because the generic_list
    library depends upon the memory layout of the structure. In this way, your custom 
    structure has an "is-a" relationship with Glist_element. Therefore, it is quite similar to
    the concept of inheritance and polymorphism. I would say it is one way of doing object 
    oriented programming in C.
 */
typedef struct Glist_element_tag
{
    struct Glist_element_tag *next; //!< Points to the next element in the linked list.
    //struct Glist_element_tag *prev;
} Glist_element;

/**
    \brief Implements a list.

    It maintains all the state necessary to maintain a singly linked list. New elements
    are added at the tail.
*/
typedef struct Glist_tag
{
    int count; //!< number of elements in the list.
    Glist_element *head; //!< points to the head of the list.
    Glist_element *tail; //!< points to the tail of the list.
} Glist;

typedef Glist_element * Glist_iterator;
typedef void (*GlistCopyElementFuncType)(void *dest,const void *source);
typedef int  (*GlistCompareElementFuncType)(const void *to_search,
                                            const void *inlist);
typedef void (*GlistDestroyElementFuncType)(void *);

void Glist_initialise(Glist *l);
void Glist_deinitialise(Glist *l,GlistDestroyElementFuncType destroy);
int Glist_empty(const Glist *l);
int Glist_count(const Glist *l);
void *Glist_add(Glist *l,const void *element,ssize_t size,GlistCopyElementFuncType copy);
int Glist_erase(Glist *l,const void *element,
                GlistCompareElementFuncType equal,
                GlistDestroyElementFuncType destroy);
int Glist_eraseptr(Glist *l,const void *element,
                   GlistDestroyElementFuncType destroy);
void *Glist_search(const Glist *l,const void *to_search,
                   GlistCompareElementFuncType equal);
void Glist_element_initialise(void *element,ssize_t size);
void Glist_element_deinitialise(void *element,GlistDestroyElementFuncType destroy);

/* The iterators pretty similar to the iterators defined in list.h */

Glist_iterator Glist_itr_begin(const Glist *l);
Glist_iterator Glist_itr_begin_at(const Glist *l,const void *to_search,
                                  GlistCompareElementFuncType equal);
void *Glist_itr_at(Glist_iterator itr);
Glist_iterator Glist_itr_next(Glist_iterator itr);


#endif

