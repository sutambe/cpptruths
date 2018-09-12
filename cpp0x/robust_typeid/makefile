CC=clang++
#CPPFLAGS=-std=c++11 -D__GXX_MERGED_TYPEINFO_NAMES
CPPFLAGS=-std=c++14 -D__GXX_MERGED_TYPEINFO_NAMES

all: libfoo.so robust_typeid

libfoo.so: foo.h foo.cpp
	$(CC) $(CPPFLAGS) -fpic -shared foo.cpp -o libfoo.so

robust_typeid: libfoo.so robust_typeid.cpp 
# Two assertion failures on Linux if you are not careful.
# 1. When __GXX_MERGED_TYPEINFO_NAMES is enabled without -rdynamic
#    on RHEL7 clang++ 6.0 and g++ 4.8.5. std::type_info pointer comparison
#    becomes wrong because there are two independent instances.
# 2. When loading a shared library that uses a global static variable
#    (e.g., a class-level variable template) -rdynamic must be provided.
#    Otherwise, two independent instances of the variable will be created.
# MacOS+clang does not have both of these issues.
	$(CC) $(CPPFLAGS) -rdynamic -ldl -o robust_typeid robust_typeid.cpp

.PHONY: clean

clean:
	rm -f libfoo.so robust_typeid
