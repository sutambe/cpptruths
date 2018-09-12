CC=clang++
#CPPFLAGS=-std=c++11 -D__GXX_MERGED_TYPEINFO_NAMES
CPPFLAGS=-std=c++14 

all: libfoo.so robust_typeid

libfoo.so: foo.h foo.cpp
	$(CC) $(CPPFLAGS) -fpic -shared foo.cpp -o libfoo.so

robust_typeid: libfoo.so robust_typeid.cpp 
# Assertion failure when __GXX_MERGED_TYPEINFO_NAMES is enabled without -rdynamic
# on RHEL7 clang++ 6.0 and g++ 4.8.5. MacOS+clang does not have this issue for
# fast typeid
	$(CC) $(CPPFLAGS) -ldl -o robust_typeid robust_typeid.cpp

.PHONY: clean

clean:
	rm -f libfoo.so robust_typeid
