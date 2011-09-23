#!/usr/bin/python

import sys

def main():
    master    = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    functions = int(sys.argv[2]) if len(sys.argv) > 2 else 1000
    overloads = int(sys.argv[3]) if len(sys.argv) > 3 else 10
    depth     = int(sys.argv[4]) if len(sys.argv) > 4 else 50
    arity     = int(sys.argv[5]) if len(sys.argv) > 5 else 5
    
    sys.stdout.write("template<class T>\n"
               "struct h0_ {};\n")
    
    for i in range(1, depth):
        sys.stdout.write("template<class T>\n"
                   "struct h" + str(i) + "_ : h" + str(i-1) + "_<T> {};\n")
        
    sys.stdout.write("\n")
        
    for i in range(functions):
        sys.stdout.write("struct f" + str(i) + "_ {};\n")
        for j in range(overloads):
            sys.stdout.write("template<")
            for k in range(arity):
                if(k != 0):
                    sys.stdout.write(", ")
                sys.stdout.write("class A" + str(k))
            sys.stdout.write(">\n")
            
            if(master):
                sys.stdout.write("void dispatching(f" + str(i) + "_, ")
            else:
                sys.stdout.write("void dispatching_f" + str(i) + "_(")
                
            for k in range(arity):
                if(k != 0):
                    sys.stdout.write(", ")
                sys.stdout.write("h" + str(j) + "_<A" + str(k) + ">")
            sys.stdout.write(");\n")
        
    sys.stdout.write("\n")
    
    sys.stdout.write("int main()\n{\n    ")

    if(master):
        sys.stdout.write("dispatching(f0_(), ");
    else:
        sys.stdout.write("dispatching_f0_(");
               
    for i in range(arity):
        if(i != 0):
            sys.stdout.write(", ")
        sys.stdout.write("h" + str(depth-1) + "_<int>()")
    
    sys.stdout.write(");\n}\n")

if __name__ == "__main__":
    main()
