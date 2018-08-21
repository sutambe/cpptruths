CC=clang++
CPPFLAGS=-std=c++11 -D__GXX_MERGED_TYPEINFO_NAMES

all: libfoo.so robust_typeid

libfoo.so: foo.h foo.cpp
	$(CC) $(CPPFLAGS) -fpic -shared foo.cpp -o libfoo.so

robust_typeid: libfoo.so robust_typeid.cpp 
# Assertion failure when __GXX_MERGED_TYPEINFO_NAMES is enabled without -rdynamic
	$(CC) $(CPPFLAGS) -rdynamic -ldl -o robust_typeid robust_typeid.cpp

.PHONY: clean

clean:
	rm -f libfoo.so robust_typeid
