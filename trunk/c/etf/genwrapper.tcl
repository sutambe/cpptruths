#!/usr/bin/tclsh
#=================================================================
# Tcl-wrapper generator for DDL (Dynamic Dynamic Loader)
#
# Usage: genwrappers < inputfile > outputfile.c
#
#  input file is a text file with each line containing, e.g.,
#      void putchar(int c) -> myputchar;
#
#  This will generate C code to intercept putchar() and call a Tcl proc
#  before and after the call to the real putchar(). The Tcl procs are
#  called, e.g., myputchar_begin and myputchar_end, and will have the
#  arguments. The _end Tcl proc will have the return value as its first
#  argument, or "void" if the return value was void. (E.g., 
#  "proc myputchar_begin {c} {...}" and "proc myputchar_end {ret c} {...}"
#  are the correct definitions). 
#
#  This script currently only handles int, long, size_t, char* and
#  void* types. (actually, any type with '*' in it except the char*
#  type will be passed to Tcl as a long integer. This at least gives
#  access to the pointer value, if not the data.) Char* is assumed
#  to point to a string, so if your function uses it differently, then
#  declare that argument as void* in the redirection spec. It also 
#  handles "..." by just assuming another 40 bytes of arguments (for 
#  an example, see the wrapper generated for printf in the test demo.)
#=================================================================

#=================================================================
# Split a single type+name string into a 2-element list with
# type and name separated
#=================================================================
proc SplitDecl {declstr} {
    set dlist [split [string trim $declstr]]
    set typelist {}
    foreach e $dlist {
	if {"" != $e} {
	    lappend typelist $e
	}
    }
    set name [lindex $typelist end]
    set typelist [lrange $typelist 0 [expr [llength $typelist]-2]]
    if {[string index $name 0] == "*"} {
	set name [string range $name 1 end]
        lappend typelist "*"
    }
    set type [join $typelist]
    return [list $type $name]
}

#=================================================================
# Translate C types into Tcl internal types
#=================================================================
proc TranslateToTclTypes {typestr} {
    regsub {unsigned} [string trim $typestr] {} typestr
    regsub { } $typestr {} typestr
    if {"char*" == $typestr} {
	return [list String "char*"]
    }
    if {"*" == [string index $typestr end]} {
	return [list Long long]
    }
    if {"int" == $typestr} {
	return [list Int int]
    }
    if {"long" == $typestr || "size_t" == $typestr} {
	return [list Long long]
    }
    return [list none none]
}
    
#=================================================================
# Main program
#=================================================================

puts {#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <tcl.h>

extern int do_redirect;
extern void _dl_ignore_next(char* name);
extern Tcl_Interp* TclInterp;

}

puts -nonewline stderr {setenv LD_REDIRECT 'list:}

while {[gets stdin line]>=0} {
    catch {unset argdecls}
    # read a wrapper map definition
    if {[regexp {([^\(]*)\(([^\)]*)\) *-> *([^;]*);} $line dummy funcspec \
         argspec redirectname]} {
	#puts "funcspec: ($funcspec)"
	set funcdecl [SplitDecl $funcspec]
	#puts "funcdecl: $funcdecl"
	#puts "argspec:  ($argspec)"
	set argsl [split $argspec ,]
	foreach argspec $argsl {
	    lappend argdecls [SplitDecl $argspec]
	}
    }
    # Now generate the wrapper
    puts -nonewline "[lindex $funcdecl 0] $redirectname ("
    set ai 0
    foreach ad $argdecls {
	if {$ai} {puts -nonewline ", "}
	if {"..." != [lindex $ad 1]} {
	    puts -nonewline "[lindex $ad 0] [lindex $ad 1]"
	} else {
            # handle ... arg declaration with some fixed amount of arg space
            puts -nonewline "double d1,double d2,double d3,double d4,double d5"
	}
	incr ai
    }
    puts ")"
    puts "\{"
    puts "  Tcl_Obj *cmdvector\[[expr $ai+2]\];"
    if {"void" != [lindex $funcdecl 0]} {
	puts "  [lindex $funcdecl 0] retval;"
    }
    puts "  do_redirect = 0;"

    #
    # Create the Tcl pre-call interceptor call and invoke it
    #
    # [lindex $funcdecl 1]
    puts "  cmdvector\[0\] = Tcl_NewStringObj(\"${redirectname}_begin\",-1);"
    set ai 1
    foreach ad $argdecls {
	if {"..." == [lindex $ad 1]} break
	set adt [TranslateToTclTypes [lindex $ad 0]]
	set ot [lindex $adt 0]
	if {"String"==$ot} {
	    puts "  cmdvector\[$ai\] = Tcl_New${ot}Obj([lindex $ad 1],-1);"
	} else {
	    puts "  cmdvector\[$ai\] = Tcl_New${ot}Obj([lindex $ad 1]);"
	}
	incr ai
    }
    puts "  Tcl_EvalObjv(TclInterp,$ai,cmdvector,TCL_EVAL_GLOBAL);"

    #
    # Invoke the actual function
    #
    puts "  _dl_ignore_next(\"[lindex $funcdecl 1]\");"
    puts "  do_redirect = 1;"
    if {"void" != [lindex $funcdecl 0]} {
	puts -nonewline "  retval = "
    } else {
	puts -nonewline "  "
    }
    puts -nonewline "[lindex $funcdecl 1] ("
    set ai 0
    foreach ad $argdecls {
	if {$ai} {puts -nonewline ", "}
	if {"..." != [lindex $ad 1]} {
	    puts -nonewline "[lindex $ad 1]"
	} else {
            puts -nonewline "d1,d2,d3,d4,d5"
	}
	incr ai
    }
    puts ");"
    puts "  do_redirect = 0;"
    puts "  _dl_ignore_next(\"\");"

    #
    # Create the Tcl post-call interceptor call and invoke it
    #
    # [lindex $funcdecl 1]
    puts "  cmdvector\[0\] = Tcl_NewStringObj(\"${redirectname}_end\",-1);"
    if {"void" != [lindex $funcdecl 0]} {
	set rt [TranslateToTclTypes [lindex $funcdecl 0]]
	puts "  cmdvector\[1\] = Tcl_New[lindex $rt 0]Obj(retval);"
    } else {
	puts "  cmdvector\[1\] = Tcl_NewStringObj(\"void\",-1);"
    }
    set ai 2
    foreach ad $argdecls {
	if {"..." == [lindex $ad 1]} break
	set adt [TranslateToTclTypes [lindex $ad 0]]
	set ot [lindex $adt 0]
	if {"String"==$ot} {
	    puts "  cmdvector\[$ai\] = Tcl_New${ot}Obj([lindex $ad 1],-1);"
	} else {
	    puts "  cmdvector\[$ai\] = Tcl_New${ot}Obj([lindex $ad 1]);"
	}
	incr ai
    }
    puts "  Tcl_EvalObjv(TclInterp,$ai,cmdvector,TCL_EVAL_GLOBAL);"

    #
    #  Create the return from the C wrapper
    #
    puts "  do_redirect = 1;"
    if {"void" != [lindex $funcdecl 0]} {
	set rt [TranslateToTclTypes [lindex $funcdecl 0]]
	puts "  return retval;"
    } else {
	puts "  return;"
    }
    puts "\}\n"
    puts -nonewline stderr ([lindex $funcdecl 1]:$redirectname)
}
puts stderr {'}

exit 0
