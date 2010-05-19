
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "generic_list.h"

/*
    Note that newhead->next=l->head is done after (*copy)(newhead,element).
    This does not let the user erase newhead->next pointer.
*/

void *Glist_add(Glist *l,const void *element,ssize_t size,
                         GlistCopyElementFuncType copy)
{
    assert(l && element && copy);
    if(l->tail)
    {
        Glist_element *newtail=(Glist_element *)calloc(1,size);
        (*copy)(newtail,element);
        newtail->next=0;
        l->tail->next=newtail;
        l->tail=newtail;
        l->count++;
        return newtail;
    }
    else
    {
        l->tail=l->head=(Glist_element *)calloc(1,size);
        if(!l->tail) return 0;
        (*copy)(l->tail,element);
        l->tail->next=0;
        l->count++;
        return l->tail;
    }
}

int Glist_erase(Glist *l,const void *element,
                GlistCompareElementFuncType equal,
                GlistDestroyElementFuncType destroy)
{
    Glist_element *erase=0;

    assert(l && element && destroy && equal);
    if(!l->count) return 1; // ERASE FAILED
    if((*equal)(l->head,element))
    {
        erase=l->head;
        if(l->head == l->tail)
           l->tail=0;
        l->head=erase->next;
        (*destroy)(erase);
        free(erase);
        l->count--;
        return 0; // ERASED
    }
    else
    {
        Glist_element *parent=l->head;
        while(parent->next)
        {
            if((*equal)(parent->next,element))
            {
                erase=parent->next;
                if(erase == l->tail)
                   l->tail=parent;

                parent->next=erase->next;
                (*destroy)(erase);
                free(erase);
                l->count--;
                return 0; // ERASED
            }
            parent=parent->next;
        }
        return 1; // ERASE FAILED
    }
}

int compare_absolute_ptr(const void *p1, const void *p2)
{
    return p1==p2;
}

int Glist_eraseptr(Glist *l,const void *element,
                   GlistDestroyElementFuncType destroy)
{
    return Glist_erase(l,element,compare_absolute_ptr,destroy);
}

void * Glist_search(const Glist *l,const void *to_search,
                    GlistCompareElementFuncType equal)
{
    Glist_element *element=0;
    assert(l && to_search && equal);
    element=l->head;

    while(element)
    {
    
        if((*equal)(to_search,element)) return element;
        element=element->next;
    }
    return 0;
}

void Glist_deinitialise(Glist *l,GlistDestroyElementFuncType destroy)
{
    Glist_element *element=0, *next=0;
    assert(l && destroy);
    element=l->head;
    while(element)
    {
        next=element->next;
        (*destroy)(element);
        element->next=0;
        free(element);
        element=next;
    }
    l->tail=l->head=0;
    l->count=0;
}

void Glist_initialise(Glist *l)
{
    assert(l);
    l->tail=l->head=0;
    l->count=0;
}

int Glist_empty(const Glist *l)
{
    assert(l);
    return !l->count;
}

int Glist_count(const Glist *l)
{
    assert(l);
    return l->count;
}

Glist_iterator Glist_itr_begin(const Glist *l)
{
    assert(l);
    return l->head;
}

Glist_iterator Glist_itr_begin_at(const Glist *l,const void *to_search,
                                  GlistCompareElementFuncType equal)
{
    // assert(l && to_search && equal); done in glist_search
    return Glist_search(l,to_search,equal);
}

void *Glist_itr_at(Glist_iterator itr)
{
    return itr;
}

Glist_iterator Glist_itr_next(Glist_iterator itr)
{
    return itr ? itr->next : itr;
}

void Glist_element_initialise(void *element,ssize_t size)
{
   assert(element);
   memset(element,0,size);
}

void Glist_element_deinitialise(void *element,
                                GlistDestroyElementFuncType destroy)
{
    assert(element && destroy);
    (*destroy)(element);
}
