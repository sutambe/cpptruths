/** \file hash.c
    \brief Implements HashTable.
*/

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include "hash.h"

void hash_initialise(HashTable *table,int size, ElementToStringFuncType elmtostr_fptr)
{
    int i;
    assert(table && (size > 0) && elmtostr_fptr);
    *(int *)&table->size=size;
    table->elmtostr_fptr=elmtostr_fptr;
    table->hashbucket=(HashBucket *)calloc(size,sizeof(HashBucket));
    if(!table->hashbucket)
    {
        fprintf(stderr,"ERROR: Insufficient memory\n");
        exit(EXIT_FAILURE);
    }
    for(i=0;i < size;++i)
    {
        list_initialise(&table->hashbucket[i].list);
    }
}
int ishashbucket_empty(const HashTable *table,int ndx)
{
    assert(table && (ndx < table->size));
    return list_empty(&table->hashbucket[ndx].list);
}
ListElement *hash_add(HashTable *table,const ListElement *element,CopyElementFuncType copy)
{
    unsigned int hash=0;
    assert(table && element && copy);

    hash=hash_value((*table->elmtostr_fptr)(element),table->size);
    return list_add(&table->hashbucket[hash].list,element,copy);
}

int hash_erase(HashTable *table,const ListElement *element,CompareElementFuncType equal,DestroyElementFuncType destroy)
{
    unsigned int hash=0;
    assert(table && element && equal && destroy);

    hash=hash_value((*table->elmtostr_fptr)(element),table->size);
    return list_erase(&table->hashbucket[hash].list,element,equal,destroy);
}
ListElement * hash_search(const HashTable *table,const ListElement *to_search,CompareElementFuncType equal)
{
    unsigned int hash=0;
    assert(table && to_search && equal);

    hash=hash_value((*table->elmtostr_fptr)(to_search),table->size);
    // DEBUG fprintf(stderr,"\n\n hash_search: before list_search\n\n");
    return list_search(&table->hashbucket[hash].list,to_search,equal);
}
unsigned int hash_value(const char *str,int size)
{
    unsigned long int hash_value=0;
    assert(str && size);
    for(;*str;str++)
        hash_value+=*str;

    return hash_value % size;
}
void hash_deinitialise(HashTable *table,DestroyElementFuncType destroy)
{
    int i;
    assert(table && destroy);
    for(i=0;i < table->size;++i)
    {
        list_deinitialise(&table->hashbucket[i].list,destroy);
    }
    free(table->hashbucket);
    table->hashbucket=0;
    *(int *)&table->size=0;
    table->elmtostr_fptr=0;
}

HashTableIterator hash_new_itr_begin(const HashTable *table)
{
    int i=0;
    HashTableIterator hash_itr = 0;
    assert(table);
    
    while((i < table->size) && list_empty(&table->hashbucket[i].list))
        i++;
    
    if (i >= table->size)
    {
        return 0;   
    }
    else
    {
        hash_itr=calloc(1,sizeof(struct HashTableIterator_tag));
        hash_itr->hashtable=(HashTable *)table;
        hash_itr->index=i;
        hash_itr->list_itr=list_itr_begin(&table->hashbucket[i].list);
        return hash_itr;
    }
}

HashTableIterator hash_new_itr_begin_at(const HashTable *table,const ListElement *to_search,
                                        CompareElementFuncType equal)
{
    unsigned int hash=0;
    HashTableIterator hash_itr = 0;
    ListIterator litr = 0;
    assert(table && to_search && equal);

    hash=hash_value((*table->elmtostr_fptr)(to_search),table->size);
    litr=list_itr_begin_at(&table->hashbucket[hash].list,to_search,equal);
    if(litr)
    {
        hash_itr=calloc(1,sizeof(struct HashTableIterator_tag));
        hash_itr->index=hash;
        hash_itr->hashtable=(HashTable *)table;
        hash_itr->list_itr=litr;
        return hash_itr;
    }
    else
        return 0;
}

ListElement *hash_itr_at(HashTableIterator itr)
{
    return list_itr_at(itr->list_itr);
}

HashTableIterator hash_itr_next(HashTableIterator itr)
{
    if (!itr) 
        return 0;
    
    itr->list_itr = list_itr_next(itr->list_itr);
    if(itr->list_itr)
    {
        return itr;   
    }
    else if(itr->index >= itr->hashtable->size-1)   
    {
        itr->list_itr = 0;
        itr->index = 0;
        itr->hashtable = 0;   
        return 0;
    }
    else
    {
        while((++itr->index < itr->hashtable->size) && list_empty(&itr->hashtable->hashbucket[itr->index].list)); 
        if (itr->index >= itr->hashtable->size)
        {
            itr->list_itr = 0;
            itr->index = 0;
            itr->hashtable = 0;   
            return 0;   
        }
        else
        {
            itr->list_itr=list_itr_begin(&itr->hashtable->hashbucket[itr->index].list);
            return itr;
        }
    }
}
