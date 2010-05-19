// Auto-generated Jump Table
// generated on Tue Apr 13 16:51:39 MDT 2004
// Tablesize: 1000
// Argument bytes: 256
// Jump Index Variable: _jt_func_index
// Jump Table Name: _jt_jumptable
// Jump Function Name: _jt_jumppoint
// First entry is at _jt_jumptable + 3, each entry is 15 bytes
// Entry address equation is (_jt_jumptable + 3 + i*15), i is
// index of entry (starting at 0)
//

#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "table.h"

#define ARGS double p1,double p2,double p3,double p4,double p5,double p6
#define CALC_OFFSET(x) (3 + 15 * x)
#define call_func(ndx,return_val)   if (!_jt_symboltable[ndx].func_ptr) {\
      _jt_symboltable[ndx].func_ptr = dlsym((void *)-1l,\
                                           _jt_symboltable[ndx].fname);\
      if (!_jt_symboltable[ndx].func_ptr) {\
            _jt_symboltable[ndx].func_ptr = dlsym((void *)0,\
                                           _jt_symboltable[ndx].fname);\
            if (!_jt_symboltable[ndx].func_ptr)\
            {    fprintf(stderr,"Error: function %s cannot be found\n",\
                     _jt_symboltable[ndx].fname);\
                exit(1);\
            }\
      }\
   }\
   return_val=(_jt_symboltable[ndx].func_ptr)(p1,p2,p3,p4,p5,p6);


//
// Global var that is set by the jump table and used in the wrapper
// to know which function it came from
//

Link_table _jt_symboltable[1000];
int _jt_func_index = 0;
int _jt_nextstentry = 0;
int _jt_nextoffset = 0;

extern int do_redirect;   

//
// Sample tracing function used by jump table
//
   
int _jt_jumppoint (ARGS)
{
   int retval;
//#ifdef DEBUG   
   printf("stringtable.c: Calling %s at %d\n",_jt_symboltable[_jt_func_index].fname,_jt_func_index);
//#endif
   call_func(_jt_func_index,retval);
   
   return retval;
}


int _jt_get_tableoffset(const char *funcname,const char *caller_libname)
{
   int i;
   for(i=0;i < _jt_nextstentry;i++) // Search for entry
   {
       if(0==strcmp(funcname,_jt_symboltable[i].fname) &&
          0==strcmp(caller_libname,_jt_symboltable[i].libname))
       {
            return _jt_nextoffset;
       }
   }
   // Add the new entry if not found
   if (1000 <= _jt_nextstentry)
      return -1;
      
   _jt_symboltable[_jt_nextstentry].fname = strdup(funcname);
   _jt_symboltable[_jt_nextstentry].libname = strdup(caller_libname);
   _jt_symboltable[_jt_nextstentry].func_ptr = 0;
   _jt_nextoffset = CALC_OFFSET(_jt_nextstentry);
   _jt_symboltable[_jt_nextstentry].offset=_jt_nextoffset;
   _jt_nextstentry++;
   
   return _jt_nextoffset;
}

void _fini(void)
{
    int i;
    //printf("_fini stringtable\n");
    for(i=0;i < _jt_nextstentry;++i)
    {
        free(_jt_symboltable[i].fname);
        free(_jt_symboltable[i].libname);
        _jt_symboltable[i].fname=0;
        _jt_symboltable[i].libname=0;
        _jt_symboltable[i].func_ptr=0;
        _jt_symboltable[i].offset=0;
    }
}

//
// Jump table: just a repetition of movl and jump instructions
//


void _jt_jumptable(void)
{
   asm("\
	movl $0, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $1, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $2, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $3, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $4, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $5, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $6, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $7, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $8, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $9, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $10, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $11, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $12, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $13, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $14, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $15, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $16, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $17, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $18, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $19, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $20, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $21, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $22, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $23, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $24, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $25, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $26, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $27, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $28, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $29, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $30, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $31, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $32, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $33, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $34, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $35, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $36, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $37, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $38, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $39, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $40, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $41, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $42, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $43, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $44, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $45, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $46, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $47, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $48, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $49, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $50, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $51, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $52, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $53, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $54, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $55, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $56, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $57, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $58, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $59, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $60, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $61, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $62, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $63, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $64, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $65, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $66, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $67, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $68, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $69, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $70, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $71, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $72, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $73, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $74, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $75, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $76, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $77, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $78, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $79, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $80, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $81, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $82, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $83, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $84, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $85, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $86, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $87, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $88, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $89, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $90, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $91, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $92, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $93, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $94, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $95, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $96, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $97, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $98, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $99, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $100, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $101, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $102, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $103, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $104, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $105, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $106, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $107, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $108, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $109, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $110, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $111, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $112, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $113, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $114, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $115, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $116, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $117, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $118, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $119, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $120, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $121, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $122, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $123, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $124, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $125, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $126, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $127, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $128, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $129, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $130, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $131, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $132, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $133, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $134, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $135, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $136, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $137, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $138, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $139, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $140, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $141, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $142, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $143, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $144, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $145, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $146, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $147, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $148, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $149, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $150, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $151, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $152, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $153, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $154, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $155, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $156, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $157, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $158, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $159, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $160, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $161, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $162, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $163, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $164, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $165, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $166, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $167, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $168, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $169, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $170, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $171, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $172, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $173, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $174, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $175, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $176, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $177, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $178, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $179, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $180, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $181, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $182, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $183, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $184, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $185, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $186, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $187, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $188, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $189, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $190, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $191, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $192, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $193, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $194, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $195, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $196, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $197, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $198, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $199, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $200, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $201, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $202, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $203, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $204, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $205, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $206, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $207, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $208, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $209, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $210, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $211, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $212, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $213, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $214, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $215, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $216, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $217, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $218, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $219, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $220, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $221, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $222, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $223, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $224, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $225, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $226, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $227, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $228, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $229, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $230, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $231, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $232, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $233, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $234, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $235, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $236, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $237, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $238, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $239, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $240, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $241, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $242, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $243, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $244, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $245, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $246, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $247, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $248, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $249, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $250, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $251, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $252, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $253, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $254, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $255, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $256, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $257, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $258, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $259, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $260, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $261, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $262, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $263, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $264, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $265, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $266, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $267, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $268, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $269, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $270, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $271, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $272, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $273, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $274, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $275, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $276, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $277, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $278, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $279, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $280, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $281, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $282, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $283, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $284, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $285, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $286, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $287, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $288, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $289, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $290, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $291, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $292, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $293, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $294, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $295, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $296, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $297, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $298, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $299, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $300, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $301, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $302, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $303, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $304, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $305, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $306, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $307, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $308, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $309, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $310, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $311, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $312, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $313, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $314, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $315, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $316, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $317, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $318, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $319, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $320, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $321, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $322, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $323, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $324, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $325, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $326, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $327, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $328, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $329, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $330, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $331, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $332, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $333, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $334, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $335, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $336, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $337, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $338, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $339, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $340, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $341, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $342, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $343, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $344, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $345, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $346, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $347, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $348, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $349, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $350, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $351, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $352, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $353, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $354, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $355, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $356, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $357, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $358, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $359, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $360, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $361, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $362, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $363, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $364, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $365, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $366, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $367, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $368, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $369, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $370, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $371, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $372, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $373, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $374, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $375, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $376, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $377, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $378, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $379, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $380, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $381, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $382, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $383, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $384, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $385, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $386, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $387, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $388, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $389, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $390, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $391, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $392, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $393, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $394, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $395, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $396, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $397, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $398, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $399, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $400, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $401, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $402, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $403, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $404, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $405, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $406, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $407, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $408, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $409, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $410, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $411, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $412, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $413, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $414, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $415, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $416, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $417, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $418, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $419, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $420, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $421, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $422, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $423, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $424, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $425, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $426, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $427, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $428, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $429, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $430, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $431, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $432, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $433, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $434, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $435, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $436, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $437, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $438, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $439, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $440, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $441, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $442, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $443, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $444, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $445, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $446, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $447, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $448, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $449, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $450, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $451, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $452, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $453, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $454, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $455, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $456, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $457, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $458, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $459, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $460, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $461, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $462, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $463, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $464, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $465, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $466, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $467, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $468, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $469, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $470, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $471, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $472, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $473, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $474, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $475, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $476, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $477, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $478, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $479, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $480, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $481, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $482, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $483, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $484, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $485, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $486, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $487, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $488, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $489, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $490, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $491, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $492, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $493, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $494, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $495, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $496, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $497, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $498, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $499, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $500, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $501, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $502, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $503, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $504, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $505, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $506, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $507, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $508, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $509, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $510, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $511, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $512, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $513, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $514, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $515, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $516, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $517, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $518, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $519, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $520, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $521, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $522, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $523, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $524, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $525, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $526, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $527, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $528, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $529, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $530, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $531, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $532, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $533, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $534, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $535, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $536, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $537, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $538, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $539, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $540, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $541, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $542, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $543, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $544, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $545, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $546, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $547, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $548, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $549, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $550, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $551, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $552, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $553, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $554, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $555, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $556, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $557, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $558, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $559, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $560, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $561, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $562, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $563, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $564, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $565, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $566, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $567, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $568, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $569, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $570, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $571, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $572, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $573, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $574, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $575, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $576, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $577, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $578, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $579, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $580, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $581, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $582, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $583, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $584, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $585, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $586, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $587, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $588, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $589, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $590, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $591, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $592, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $593, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $594, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $595, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $596, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $597, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $598, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $599, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $600, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $601, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $602, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $603, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $604, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $605, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $606, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $607, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $608, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $609, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $610, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $611, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $612, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $613, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $614, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $615, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $616, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $617, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $618, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $619, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $620, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $621, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $622, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $623, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $624, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $625, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $626, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $627, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $628, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $629, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $630, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $631, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $632, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $633, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $634, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $635, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $636, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $637, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $638, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $639, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $640, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $641, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $642, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $643, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $644, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $645, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $646, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $647, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $648, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $649, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $650, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $651, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $652, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $653, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $654, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $655, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $656, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $657, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $658, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $659, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $660, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $661, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $662, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $663, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $664, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $665, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $666, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $667, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $668, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $669, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $670, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $671, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $672, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $673, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $674, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $675, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $676, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $677, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $678, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $679, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $680, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $681, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $682, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $683, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $684, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $685, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $686, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $687, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $688, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $689, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $690, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $691, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $692, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $693, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $694, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $695, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $696, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $697, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $698, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $699, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $700, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $701, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $702, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $703, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $704, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $705, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $706, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $707, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $708, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $709, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $710, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $711, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $712, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $713, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $714, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $715, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $716, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $717, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $718, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $719, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $720, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $721, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $722, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $723, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $724, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $725, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $726, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $727, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $728, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $729, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $730, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $731, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $732, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $733, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $734, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $735, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $736, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $737, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $738, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $739, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $740, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $741, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $742, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $743, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $744, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $745, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $746, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $747, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $748, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $749, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $750, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $751, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $752, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $753, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $754, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $755, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $756, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $757, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $758, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $759, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $760, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $761, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $762, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $763, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $764, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $765, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $766, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $767, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $768, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $769, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $770, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $771, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $772, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $773, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $774, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $775, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $776, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $777, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $778, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $779, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $780, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $781, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $782, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $783, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $784, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $785, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $786, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $787, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $788, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $789, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $790, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $791, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $792, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $793, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $794, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $795, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $796, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $797, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $798, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $799, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $800, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $801, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $802, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $803, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $804, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $805, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $806, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $807, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $808, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $809, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $810, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $811, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $812, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $813, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $814, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $815, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $816, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $817, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $818, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $819, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $820, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $821, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $822, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $823, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $824, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $825, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $826, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $827, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $828, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $829, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $830, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $831, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $832, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $833, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $834, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $835, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $836, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $837, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $838, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $839, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $840, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $841, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $842, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $843, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $844, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $845, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $846, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $847, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $848, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $849, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $850, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $851, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $852, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $853, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $854, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $855, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $856, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $857, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $858, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $859, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $860, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $861, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $862, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $863, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $864, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $865, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $866, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $867, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $868, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $869, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $870, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $871, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $872, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $873, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $874, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $875, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $876, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $877, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $878, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $879, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $880, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $881, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $882, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $883, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $884, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $885, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $886, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $887, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $888, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $889, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $890, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $891, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $892, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $893, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $894, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $895, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $896, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $897, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $898, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $899, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $900, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $901, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $902, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $903, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $904, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $905, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $906, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $907, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $908, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $909, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $910, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $911, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $912, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $913, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $914, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $915, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $916, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $917, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $918, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $919, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $920, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $921, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $922, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $923, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $924, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $925, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $926, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $927, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $928, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $929, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $930, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $931, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $932, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $933, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $934, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $935, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $936, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $937, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $938, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $939, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $940, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $941, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $942, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $943, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $944, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $945, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $946, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $947, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $948, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $949, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $950, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $951, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $952, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $953, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $954, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $955, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $956, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $957, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $958, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $959, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $960, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $961, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $962, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $963, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $964, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $965, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $966, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $967, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $968, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $969, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $970, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $971, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $972, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $973, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $974, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $975, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $976, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $977, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $978, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $979, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $980, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $981, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $982, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $983, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $984, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $985, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $986, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $987, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $988, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $989, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $990, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $991, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $992, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $993, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $994, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $995, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $996, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $997, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $998, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
	movl $999, _jt_func_index\n\
	jmp  _jt_jumppoint\n\
   ");
}
