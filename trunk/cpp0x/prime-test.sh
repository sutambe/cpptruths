#!/bin/sh

cc=g++47

function constexpr_compile() {
  echo $cc is_prime.cpp -o is_prime -Wall -std=c++0x -D CONSTEXPR -D NUMBER=$1
  time $cc is_prime.cpp -o is_prime -Wall -std=c++0x -D CONSTEXPR -D NUMBER=$1
}

# Default depth = 900
function template_compile() {
  echo $cc is_prime.cpp -o is_prime -Wall -std=c++0x -D NUMBER=$1 -ftemplate-depth-2100
  time $cc is_prime.cpp -o is_prime -Wall -std=c++0x -D NUMBER=$1 -ftemplate-depth-2100
}

function gen_prime () {
for i in {1..30}
do
  n=`calc $i*10000`
  prime=`./nth-prime $n`
  echo $prime
done
}

for prime in  104729  224737  350377  479909  611953  746773  \
              882377  1020379 1159523 1299709 1441049 1583539 \
              1726943 1870667 2015177 2160553 2307229 2454587 \
              2601857 2750159 2898527 3047767 3196933 3346601 \
              3497861 3648923 3800201 3951161 4103629 4256233 
do
 echo $prime
template_compile $prime
#constexpr_compile $prime
done

