#/usr/bin/python
# Python program for creating web-analytics report of the More C++ Idioms book.
# Author: Sumant Tambe (sutambe@yahoo.com) (http://en.wikibooks.org/wiki/User:Sutambe)
# Released under BSD license.

import pprint
import urllib.request
import json
import sys

idioms = [ "Adapter_Template", 
           "Address_Of", 
           "Algebraic_Hierarchy", 
           "Attach_by_Initialization",
           "Friendship_and_the_Attorney-Client",
           "Barton-Nackman_trick",
           "Base-from-Member",
           "Boost_mutant",
           "Calling_Virtuals_During_Initialization",
           "Capability_Query",
           "Checked_delete",
           "Clear-and-minimize",
           "Coercion_by_Member_Template",
           "Computational_Constructor",
           "Concrete_Data_Type",
           "Const_auto_ptr",
           "Construct_On_First_Use",
           "Construction_Tracker",
           "Copy-and-swap",
           "Copy-on-write",
           "Counted_Body",
           "Curiously_Recurring_Template_Pattern",
           "Detached_Counted_Body",
           "Empty_Base_Optimization",
           "enable-if",
           "Envelope_Letter",
           "Erase-Remove",
           "Execute-Around_Pointer",
           "Export_Guard_Macro",
           "Expression-template",
           "Fake_Vtable",
           "Fast_Pimpl",
           "Final_Class",
           "Free_Function_Allocators",
           "Function_Object",
           "Generic_Container_Idioms",
           "Hierarchy_Generation",
           "Include_Guard_Macro",
           "Inline_Guard_Macro",
           "Inner_Class",
           "Int-To-Type",
           "Interface_Class",
           "Iterator_Pair",
           "Making_New_Friends",
           "Metafunction",
           "Move_Constructor",
           "Multi-statement_Macro",
           "Member_Detector",
           "Named_Constructor",
           "Named_External_Argument",
           "Named_Loop",
           "Named_Parameter",
           "Named_Template_Parameters",
           "Nifty_Counter",
           "Non-copyable_Mixin",
           "Non-member_Non-friend_Function",
           "Non-throwing_swap",
           "Non-Virtual_Interface",
           "nullptr",
           "Object_Generator",
           "Object_Template",
           "Parameterized_Base_Class",
           "Handle_Body",
           "Policy_Clone",
           "Policy-based_Design",
           "Polymorphic_Exception",
           "Recursive_Type_Composition",
           "Requiring_or_Prohibiting_Heap-based_Objects",
           "Resource_Acquisition_Is_Initialization",
           "Resource_Return",
           "Return_Type_Resolver",
           "Runtime_Static_Initialization_Order_Idioms",
           "Safe_bool",
           "Scope_Guard",
           "SFINAE",
           "Shortening_Long_Template_Names",
           "Shrink-to-fit",
           "Small_Object_Optimization",
           "Smart_Pointer",
           "Storage_Class_Tracker",
           "Tag_Dispatching",
           "Temporary_Base_Class",
           "Temporary_Proxy",
           "The_result_of_technique",
           "Thin_Template",
           "Traits",
           "Type_Erasure",
           "Type_Generator",
           "Type_Safe_Enum",
           "Type_Selection",
           "Virtual_Constructor",
           "Virtual_Friend_Function" ]

#url_latest_30='http://stats.grok.se/json/en.b/latest30/More_C++_Idioms/'
#url_latest_60='http://stats.grok.se/json/en.b/latest60/More_C++_Idioms/'
url_latest_90='http://stats.grok.se/json/en.b/latest90/More_C++_Idioms/'
total_views = {}

url=url_latest_90 + 'Virtual_Constructor'
url_str=urllib.request.urlopen(url).read().decode("utf-8")
pageview_obj = json.loads(url_str)
sorted_keys = sorted(pageview_obj["daily_views"])
sys.stdout.write("Name, ")
print(", ".join(sorted_keys))

for idiom_name in idioms:
  url=url_latest_90 + idiom_name
  url_str=urllib.request.urlopen(url).read().decode("utf-8")
  pageview_obj = json.loads(url_str)
  counts = []
  for key in sorted_keys:
    if key in total_views:
      total_views[key] = total_views[key] + pageview_obj["daily_views"][key]
    else:
      total_views[key] = pageview_obj["daily_views"][key]
    
    counts.append(str(pageview_obj["daily_views"][key]))
  
  sys.stdout.write(idiom_name + ", ")
  sys.stdout.write(", ".join(counts))
  sys.stdout.write("\n")
  sys.stdout.flush()  

sys.stdout.write("TOTAL, ")
for key in sorted_keys:
  sys.stdout.write(str(total_views[key]) + ", ")

