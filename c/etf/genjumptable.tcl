#!/usr/bin/tclsh

set TableSize 5
set MaxArgBytes 16
set JumpIndexName "_jt_func_index"
set JumpTableName "_jt_jumptable"
set JumpFunctionName "_jt_jumppoint"

proc PrintHelp {} {
   global TableSize MaxArgBytes JumpIndexName JumpTableName JumpFunctionName
   puts [subst {This utility generates a jump table in C for use with
DDL. Options are:

  --entries <num>   : The number of entries in the jump table
                      (default $TableSize)

  --argbytes <num>  : The max number of argument bytes (default $MaxArgBytes)

  --jumpindex <name>: The name of the global index variable
                      (default $JumpIndexName)

  --jumptable <name>: The name of the jump table
                      (default $JumpTableName)

  --jumpfunction <name>: The name of the function that jump table
                         entries jump to (default $JumpFunctionName)

You must redirect stdout in order to save the output.}]
}

foreach {a v} $argv {
   if {"--entries" == $a} {
      set TableSize $v
   } elseif {"--argbytes" == $a} {
      set MaxArgBytes $v
   } elseif {"--jumpindex" == $a} {
      set JumpIndexName $v
   } elseif {"--jumptable" == $a} {
      set JumpTableName $v
   } elseif {"--jumpfunction" == $a} {
      set JumpFunctionName $v
   } elseif {"--help" == $a} {
      PrintHelp
      exit 0
   } else {
      puts "Unknown option: ($a) ($v)"
      exit 1
   }
}

puts "// Auto-generated Jump Table"
puts "// generated on [clock format [clock seconds]]"
puts "// Tablesize: $TableSize\n// Argument bytes: $MaxArgBytes"
puts "// Jump Index Variable: $JumpIndexName"
puts "// Jump Table Name: $JumpTableName"
puts "// Jump Function Name: $JumpFunctionName"
puts "// First entry is at $JumpTableName + 3, each entry is 15 bytes"
puts "// Entry address equation is ($JumpTableName + 3 + i*15), i is"
puts "// index of entry (starting at 0)\n//"
puts {
//
// Global var that is set by the jump table and used in the wrapper
// to know which function it came from
//}
puts "\nint $JumpIndexName;\n"
puts {
//
// Jump table: just a repetition of movl and jump instructions
//}
puts "void [set JumpTableName]()\n{"
puts "   asm(\"\\"
for {set i 0} {$i < $TableSize} {incr i} {
   puts "\tmovl \$$i, $JumpIndexName\\n\\"
   puts "\tjmp  $JumpFunctionName\\n\\"
}
puts "   \");"
puts "}\n\n"

puts "//==========================================================\n\n"

puts "//=====================Sample redirection code================\n"

puts "#include <stdio.h>\n#include <dlfcn.h>\n#include<string.h>\n"

set sv 0
puts "struct jt_args {"
while {$MaxArgBytes > 0} {
   puts "   int a$sv;"
   incr sv
   incr MaxArgBytes -4
}
puts "};\n"

puts [subst -nocommands -nobackslashes {
char* _jt_symboltable[$TableSize];
int (*_jt_ptrtable[$TableSize])(struct jt_args args);
int _jt_nextstentry = 0;
int _jt_nextoffset = 0;

//
// redirect_lookup allocates a table entry, saves the original symbol,
// calculates the offset for the table entry, and returns the table name
//
char *redirect_lookup(char *name, char *libname, void *addr)
{
   // if table is full, don't redirect
   if (_jt_nextstentry >= $TableSize)
      return name;
   _jt_symboltable[_jt_nextstentry] = strdup(name);
   _jt_ptrtable[_jt_nextstentry] = 0;
   _jt_nextoffset = 3 + 15 * _jt_nextstentry;
   _jt_nextstentry++;
   return "$JumpTableName";
}

//
// returns true (nonzero) if we just redirected to the table name
//
int redirect_offsetneeded()
{
   return _jt_nextoffset;
}

//
// returns the actual offset, and clears it for safety
//
int redirect_offset()
{
   int tmp = _jt_nextoffset;
   _jt_nextoffset = 0;
   return tmp;
}
}]

puts [subst -nocommands -nobackslashes {
//
// Sample tracing function used by jump table -- NOT TESTED!
//
int $JumpFunctionName (struct jt_args args)
{
   int retval;
   fprintf(stderr,"Begin %s\n",_jt_symboltable[$JumpIndexName]);
   if (!_jt_ptrtable[$JumpIndexName]) {
      // NOTE: My compiler is not finding the symbol RTLD_DEFAULT.
      //       This is why the first arg to dlsym is hardcoded.
      _jt_ptrtable[$JumpIndexName] = dlsym((void *)-1l,
                                           _jt_symboltable[$JumpIndexName]);
      if (!_jt_ptrtable[$JumpIndexName]) {
         fprintf(stderr,"Error: function %s cannot be found\n",
                 _jt_symboltable[$JumpIndexName]);
         exit(1);
      }
   }
   retval = (_jt_ptrtable[$JumpIndexName])(args);
   fprintf(stderr,"End %s\n",_jt_symboltable[$JumpIndexName]);
   return retval;
}
}]

exit 0
