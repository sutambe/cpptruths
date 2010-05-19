/** \file hash.h
    \brief HashTable service API, typedefs and enumerations.
*/


#ifndef __HASH_H
#define __HASH_H

#include "list.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/**
    \brief A pointer to a function which returns string representation of a ListElement.

    \e ElementToStringFuncType is required by several hashtable operations.
    A hash value is computed by hashtable library using the string representation
    of ListElement. Naturally, uniqueness of this string affects the overall
    performance of hash table operations.
*/
typedef const char *(* ElementToStringFuncType)(const ListElement *);

/**
    \brief A logical hash bucket implemented as a \e List.

    \e HashBucket is the building block of \e HashTable.
*/
typedef struct Hash_tag
{
    List list; //!< A bucket is implemented as a dynamic list.
}HashBucket;

/**
    \brief A HashTable.

    \e A HashTable knows its size and how to convert a ListElement into its string
    representation.
*/
typedef struct HashTable_tag
{
    const int size; //!< Size of HashTable. \b Immutable.
    ElementToStringFuncType elmtostr_fptr; //!< Pointer to converter function.
    HashBucket *hashbucket; //!< The HashTable itself. Sequence of \e size buckets.
}HashTable;

//! Initialises a hash table.
/*! This function initialises a \e HashTable of given size. It
    also "remembers" how to convert a \e ListElement into its string
    representation.
\param table Pointer to the HashTable structure. \b Nonnull.
\param size Desired size of the HashTable. \b +ve \b Nonzero.
\param elmtostr_fptr A pointer to a function which is called everytime to
       convert a ListElement into its string representation before it is hashed
       into one of the buckets of the hash table. \b Nonnull.
\return Nothing
\sa hash_deinitialise()
*/
void hash_initialise(HashTable *table,int size,ElementToStringFuncType elmtostr_fptr);

//! Deinitialises a hash table.
/*! This function deinitialises a \e HashTable. The hash table can't
    be used again unless it is initialised again by \e hash_initialise()
\param table Pointer to the HashTable structure. \b Nonnull.
\param destroy A pointer to a function which is called for every
       element in the \e HashBucket. It is not supposed to free the
       element pointer passed to it. It is freed inside
       \e hash_deinitialise. You may erase contents of \e ListElement
       in function pointed by \e destroy. Even \e next pointer.
\return Nothing
\sa hash_initialise()
*/
void hash_deinitialise(HashTable *table,DestroyElementFuncType destroy);

//! Returns whether a bucket is empty or not.
/*! This function returns one if bucket has non-zero elements, zero
    otherwise.
\param table Pointer to a \e HashTable structure. \b Nonnull.
\param ndx Index of bucket in the hash table. \b [0..hashtablesize]
\return An integer, zero or one.
\sa hash_initialise(), hash_deinitialise()
*/
int ishashbucket_empty(const HashTable *table, int ndx);

//! Adds a copy of given list element to the \e HashTable.
/*! This function allocates memory for a new \e ListElement. Next,
    user given \e copy function is called to copy the contents
    of \e source \e ListElement into this newly created \e destination
    \e ListElement. \e ListElement given as 2nd parameter is
    unchanged as long as  user given \e copy function does not modify it.

    The idea here is that, the hash operations (search, add) are
    independent of \e ListElement contents. Some \e ListElements may
    require non-trivial copy semantics. For example, pointers holding
    malloc'ed memory may need a deepcopy.
\param table Pointer to the HashTable you wanna add into. \b Nonnull.
\param element Pointer to \e prototype ListElement. \b Nonnull.
       A copy of this ListElement is added into the list.
\param copy Pointer to a function which accepts 2 \e ListElement pointer
       parameters. \b Nonnull. This function is supposed to
       copy the contents of the \e source \e ListElement into \e destination
       \e ListElement.
\return Pointer to the \e ListElement, if found. NULL otherwise.
\sa hash_initialise(), hash_deinitialise(), hash_search()
*/
ListElement * hash_add(HashTable *table,const ListElement *element,CopyElementFuncType copy);

//! Removes an element from given hash table, if found.
/*! This function searches a copy of the dummy \e element.
    If found, it removes it from hash table and deallocates memory
    allocated for it. Before deleting memory of element in the list
    itself, \e destroy is called to free any member string pointers.
\param table Pointer to the hash table. \b Nonnull.
\param element Pointer to \e prototype ListElement. \b Nonnull.
       A copy of this ListElement is searched and deleted from the list.
\param equal Pointer to a function which accepts two \e ListElement
       pointer parameters. \b Nonnull. This function is supposed
       to compare the two \e ListElements and return 1 or 0 based
       on result of comparison.
\param destroy Pointer to function which 'knows' how to 'destroy' the element.
       Some list elements may require non-trivial destruction semantics.
       For example, pointers holding malloc'ed memory need to be freed.
       User gets control of it through function \e destroy.
\return Zero if deletion is successful, non-zero otherwise.
\sa hash_search(), hash_add()
*/

int hash_erase(HashTable *table,const ListElement *element,CompareElementFuncType equal,DestroyElementFuncType destroy);

//! Searches a copy of given \e ListElement in the given HashTable.
/*! This function finds a copy of given \e ListElement.
    \b Equality is determined by user given function of type
    CompareElementFuncType.

    to_search element is hashed and a value of HashBucket is computed.
    Lateron, \e to_search is searched in the HashBucket using sequencial
    search. Function \e equal is supposed to compare the keys in the
    elements and return 1 if two keys are same, 0 otherwise.

\param table Pointer to the \e HashTable you want to search. \b Nonnull.
\param to_search Pointer to a dummy \e ListElement which holds
       same \b key as you are looking for. \b Nonnull.
\param equal Pointer to a function which accepts two \e ListElement
       pointer parameters. \b Nonnull. This function is supposed
       to compare the two \e ListElements and return 1 or 0 based
       on result of comparison.
\return Pointer to the \e ListElement, if found. NULL otherwise.
\sa hash_initialise(), hash_deinitialise(), hash_add()
*/
ListElement * hash_search(const HashTable *table,const ListElement *to_search,CompareElementFuncType equal);

//! Computes a hash value given a string.
/*! This function computes a hash value from a string of characters.
    The computed hash value is always less than parameter \e size.
    Hash value always lies between 0 and size-1, inclusive.

\param str Pointer to string. \b Nonnull.
\param size Hash value always lies between 0 and size-1, inclusive.
       size must be greater than zero and logically less than size
       of hash table.
\return A hash value representing a bucket in a HashTable.
\sa hash_initialise(), hash_deinitialise()
*/
unsigned int hash_value(const char *str,int size);

/**
    \brief An instance of the hash table iterator.

    This structure maintains the state of a hash table iterator. It could be a links' hash table
    iterator or a definitions' hash table iterator.
 */
typedef struct HashTableIterator_tag
{
    ListIterator list_itr; //!< An instance of the list iterator (for buckets)
    HashTable *hashtable;  //!< A pointer to the hash table on which the iterator is traversing.
    int index;             //!< The hash table array index which represents the bucket on which the list iterator is traversing.
} * HashTableIterator;

//! This function initiates an iterator on the specified hash table.
/*! The hash table could be links' hash table or definitions' hash table.
    This function returns a dynamically allocated HashTableIterator.
    It should be freed at the appropriate time.
\param table A pointer to the hash table to being iterator on.
\return An instance of the hash table iterator. This function returns a dynamically 
        allocated HashTableIterator. It should be freed at the appropriate time.
\sa hash_new_itr_begin_at(), hash_itr_at(), hash_itr_next()
 */
HashTableIterator hash_new_itr_begin(const HashTable *table);

//! This function initiates an iterator on the specified hash table on the specified entry if it is found.
/*! The hash table could be links' hash table or definitions' hash table. It first searches 
    the given \e ListElement and if found, it initiates a hash table iterator at that point.
    This function returns a dynamically allocated HashTableIterator.
    It should be freed at the appropriate time.
\param table A pointer to the hash table to being iterator on.
\param to_search A List Element to search (The iterator begins at this location)
\param equal A function pointer to perform comparision.
\return An instance of the hash table iterator. This function returns a dynamically 
        allocated HashTableIterator. It should be freed at the appropriate time.
\sa hash_new_itr_begin(), hash_itr_at(), hash_itr_next()
 */
HashTableIterator hash_new_itr_begin_at(const HashTable *table,const ListElement *to_search,
                                        CompareElementFuncType equal);

//! Returns the \e ListElement pointed to by the hash table iterator.
/*! Returns the \e ListElement pointed to by the hash table iterator.
\param itr An instance of a hash table iterator
\return 1 if the iterator points at a valid entry, null otherwise
\sa hash_new_itr_begin(), hash_itr_begin_at(), hash_itr_next()
 */
ListElement *hash_itr_at(HashTableIterator itr);

//! Moves the hash table iterator to the next \e ListElement in the list.
/*! This function advances the hash table iterator to the next
    \e ListElement in the hash table. The order in which the it traverses
    the list elements is unspecified because it is hard, if not impossible,
    to predict the order of the contents in a hash table.
\param itr An instance of a hash table iterator
\return An hash table iterator advanced by one position, null if it reaches at the end.
\sa hash_new_itr_begin(), hash_itr_at(), hash_new_itr_begin_at()
 */
HashTableIterator hash_itr_next(HashTableIterator itr);


#endif
