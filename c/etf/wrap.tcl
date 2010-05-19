
 set Linkdef_hook_tid 0
 set i 0
 catch {destroy .a} {} ;# this destroys the old button
 proc evt {d} { global i; incr i $d; puts "Event $i"}
 pack [button .a -text "Event Increment" -command "evt 1"]
 pack [button .b -text "Event Decrement" -command "evt -1"]
 

proc myputchar_begin {c} {
   puts " "
   puts "Inside Tcl procedure myputchar: $c"
}

set recurse_level 0

proc mydllmain_begin {a b c} {
    puts "$::recurse_level dllmain $a $b ($c) ([string index $c 31])"
    incr ::recurse_level
}

proc myputchar_end {c} {
  puts "putchar ends"
}

#
# to putchar, recursion traverses third argument c (a string) until a
# character equal to b is found at front, then putchars the 31st char
# in c. the parameter a appears to play no role?
#
# To begin the recursion, the value passed in b is the first char in
# the string c, when a is 0? and b is a pointer? (found exception at
# dllmain(0,1073758408) recurses to dllmain(-87 -1) then
# dllmain -1 -87 then on and on down the string c until ?
# and
#

proc mydllmain_end {a b c} {
   incr ::recurse_level -1
   puts "dllmain_end $::recurse_level"
}

proc wrap_printf_begin {s n} {
  puts "before printf ($s,$n)"
}

proc wrap_printf_end {dummy s} {
  puts "after printf ($s)"
}

proc wrap_malloc_begin {numbytes} {
  puts "BEFORE malloc $numbytes"
}

proc wrap_malloc_end {ptr numbytes} {
  puts "after malloc $numbytes -> [format %x $ptr]"
}

proc wrap_free_begin {ptr} {
  puts "before free [format %x $ptr]"
}

proc wrap_free_end {dummy ptr} {
  puts "after free [format %x $ptr]"
}

proc redirect_init {} {
  puts "TCL: redirect_init"
}

proc wrap_strcmp { s1 s2 } {
    puts "$s1:$s2"
}

proc redirect_lookup {linkfname linklibname} {
  global Linkdef_hook_tid;
  puts "TCL: redirect_lookup: $linkfname, $linklibname, $Linkdef_hook_tid"
  if { $linkfname == "exit" } {
      set links_itr [LinkDef_get_links_itr]
      while { $links_itr != 0 } {
            set alink [LinkDef_itr_at $links_itr]
            set fname [LinkDef_getlinkfname $alink]
            set adef [LinkDef_getdefoflink $alink]
            set deflibname [LinkDef_getdeflibname $adef]
            puts "$fname: $deflibname"
            set links_itr [LinkDef_itr_next $links_itr]
        }
      puts "Traversing backlink"
      set mallocdef [LinkDef_getdef _jt_jumptable libstringtable.so]
      set backlinks_itr [LinkDef_backlink_itr_begin $mallocdef]
      while { $backlinks_itr != 0 } {
            set alink [LinkDef_backlink_itr_at $backlinks_itr]
            set fname [LinkDef_getlinkfname $alink]
            puts $fname
            set backlinks_itr [LinkDef_backlink_itr_next $backlinks_itr]
      }
      LinkDef_free_itr $links_itr
}
  if { $linkfname == "malloc" } {      
      puts "Detected exit, redirecting"
      Redirect2sym $linkfname * wrap_malloc libwrappers3.so
      set result [Redirect2table printf main _jt_jumptable libstringtable.so]
      puts "Result is: $result"
  }  
  Redirect_ignore_next printf main  
  set printf_link [Linkdef_getlink printf main]
  puts $Linkdef_hook_tid
}

proc redirect_offset {linkfname linklibname} {
    puts "TCL: redirect_offset: $linkfname, $linklibname"
}

proc redirect_definition {new_funcname new_libname new_func_address 
                          orig_funcname orig_libname orig_func_GOT_addr} {
  puts "TCL: redirect_definition: $new_funcname, $new_libname"
  set link [Linkdef_getlink strrchr main]
  if { $link != "NULL" } {
     puts $link
     set gotaddr [Linkdef_getGOTaddr $link]
     puts "GOT address = [format %x $gotaddr]"
  }
}

proc redirect_symdef {symbol libname address caller_libname} {
  puts "TCL: redirect_symdef: $symbol, $libname, [format %x $address], $caller_libname"
}
