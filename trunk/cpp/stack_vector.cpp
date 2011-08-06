/*
 * http://home.roadrunner.com/~hinnant/stack_alloc.html
 */

#include "stack_alloc.h"
#include <vector>
#include <list>
#include <new>
#include <iostream>
#include <cstdlib>

std::size_t memory = 0;
std::size_t alloc = 0;

void* operator new(std::size_t s) throw(std::bad_alloc)
{
    memory += s;
    ++alloc;
    return malloc(s);
}

void  operator delete(void* p) throw()
{
    --alloc;
    free(p);
}

void memuse()
{
    std::cout << "memory = " << memory << '\n';
    std::cout << "alloc = " << alloc << '\n';
}

template <class Container>
void test (void)
{
    Container c;
    c.push_back(1);
    memuse();
    c.push_back(2);
    memuse();
    c.push_back(3);
    memuse();
    c.push_back(4);
    memuse();
    for (typename Container::const_iterator i = c.begin(), e = c.end(); i != e; ++i)
        std::cout << *i << ' ';
    std::cout << '\n';
}

int main()
{
    typedef std::vector<int, stack_alloc<int, 100> > StackVector;
    typedef std::list<int, stack_alloc<int, 100> > StackList;
    
    test<StackVector>();
    test<StackList>();
}

