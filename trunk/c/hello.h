#ifndef __HELLO_H
#define __HELLO_H

#ifdef COMPILING_HELLO_C

static void _init(void);
static void _fini(void);

#endif //COMPILING_HELLO_C

extern void usehello(int n);


#endif //__HELLO_H

