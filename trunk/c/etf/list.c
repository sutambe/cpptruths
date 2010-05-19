/** \file list.c
    \brief Implements List.
*/


#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "list.h"

/*
    Note that newhead->next=l->head is done after (*copy)(newhead,element).
    This does not let the user erase newhead->next pointer.
*/

ListElement *list_add(List *l,const ListElement *element,CopyElementFuncType copy)
{
    assert(l && element && copy);
    if(l->head)
    {
        ListElement *newhead=(ListElement *)calloc(1,sizeof(ListElement));
        if(!newhead) return 0;
        (*copy)(newhead,element);
        newhead->next=l->head;
        l->head=newhead;
        l->count++;
        return newhead;
    }
    else
    {
        l->head=(ListElement *)calloc(1,sizeof(ListElement));
        if(!l->head) return 0;
        copy(l->head,element);
        l->head->next=0;
        l->count++;
        return l->head;
    }
}

int list_erase(List *l,const ListElement *element,CompareElementFuncType equal,DestroyElementFuncType destroy)
{
    ListElement *erase;

    assert(l && element && destroy);
    if(!l->count) return 1;
    if((*equal)(l->head,element))
    {
        erase=l->head;
        l->head=erase->next;
        (*destroy)(erase);
        free(erase);
        --l->count;
        return 0;
    }
    else
    {
        ListElement *parent=l->head;
        while(parent->next)
        {
            if((*equal)(parent->next,element))
            {
                erase=parent->next;
                parent->next=erase->next;
                (*destroy)(erase);
                free(erase);
                --l->count;
                return 0;
            }
            parent=parent->next;
        }
        return 1;
    }
}

ListElement * list_search(const List *l,const ListElement *to_search,CompareElementFuncType equal)
{
    ListElement *element;
    assert(l && to_search && equal);
    element=l->head;

    while(element)
    {
        if((*equal)(to_search,element)) return element;
        element=element->next;
    }
    return 0;
}

void list_deinitialise(List *l,DestroyElementFuncType destroy)
{
    ListElement *element, *next=0;
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
}

void list_initialise(List *l)
{
    assert(l);
    l->head=0;
    l->count=0;
}

int list_empty(const List *l)
{
    assert(l);
    return !l->count;
}

ListIterator list_itr_begin(const List *l)
{
    assert(l);
    return l->head;
}

ListIterator list_itr_begin_at(const List *l,const ListElement *to_search,
                               CompareElementFuncType equal)
{
    // assert(l && to_search && equal); done in list_search
    return list_search(l,to_search,equal);
}

ListElement *list_itr_at(ListIterator itr)
{
    return itr;
}

ListIterator list_itr_next(ListIterator itr)
{
    return itr ? itr->next : itr;
}

void list_element_initialise(ListElement *element)
{
   assert(element);
   memset(element,0,sizeof(ListElement));
   element->strmem=MEM_REFERENCE;
}

void list_element_deinitialise(ListElement *element,DestroyElementFuncType destroy)
{
    assert(element && destroy);
    (*destroy)(element);
}

BackLinkIterator backlink_itr_begin(const ListElement *def)
{
    assert(def);
    return def->element.def.backlink;
}

ListElement *backlink_itr_at(BackLinkIterator itr)
{
    return itr;
}

BackLinkIterator backlink_itr_next(BackLinkIterator itr)
{
    return itr ? itr->element.link.backlink : itr;
}
