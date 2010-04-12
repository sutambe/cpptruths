#include "analogliterals.hpp"
#include <cassert>


int main ()
{
  http://www.xs4all.nl/~weegen/eelis/analogliterals.xhtml    NO NEED TO COMMENT THIS LINE!
  
  
  using namespace analog_literals::symbols;

// Consider:

  unsigned int a = 4;

// Have you ever felt that integer literals like "4" don't convey the true size of the value they denote? If so, use an analog integer literal instead:

  unsigned int b = I---------I;

  assert( a == b );

// Due to the way C++ operators work, we must use N*2+1 dashes between the I's to get a value of N:

  assert( I-I == 0 );
  assert( I---I == 1 );
  assert( I-----I == 2 );
  assert( I-------I == 3 );

// These one-dimensional analog literals are of type analog_literals::line<N>, which is convertible to unsigned int.

// In some cases, two-dimensional analog literals are appropriate:

  unsigned int c = ( o-----o
                     |     !
                     !     !
                     !     !
                     o-----o ).area;

  assert( c == (I-----I) * (I-------I) );

  assert( ( o-----o
            |     !
            !     !
            !     !
            !     !
            o-----o ).area == ( o---------o
                                |         !
                                !         !
                                o---------o ).area );

// Two-dimensional analog literals are of type analog_literals::rectangle<X, Y> which exposes static member constants width, height, and area.

/* As an example use-case, imagine specifying window dimensions in a GUI toolkit API using:

   window.dimensions = o-----------o
                       |           !
                       !           !
                       !           !
                       !           !
                       o-----------o ;

Who said C++ was unintuitive!? */

// But wait, there's more. We can use three-dimensional analog literals, too:

  assert( ( o-------------o
            |L             \
            | L             \
            |  L             \
            |   o-------------o
            |   !             !
            !   !             !
            o   |             !
             L  |             !
              L |             !
               L|             !
                o-------------o ).volume == ( o-------------o
                                              |             !
                                              !             !
                                              !             !
                                              o-------------o ).area * int(I-------------I) );

// Three-dimensional analog literals are of type analog_literals::cuboid<X, Y, Z> which exposes static member constants width, height, depth, and volume. In addition, three free-standing functions top, side, and front are provided which yield rectangles:

  assert( top( o-------o
               |L       \
               | L       \
               |  o-------o
               |  !       !
               !  !       !
               o  |       !
                L |       !
                 L|       !
                  o-------o ) == ( o-------o
                                   |       !
                                   !       !
                                   o-------o ) );

// The current implementation has one restriction on cuboid dimensions: the height of a cuboid literal must be at least its depth + 2.

// Note that storing these literals directly in a variable requires you to specify the dimension sizes:

  analog_literals::rectangle<4, 2> r = o---------o
                                       |         !
                                       !         !
                                       o---------o;

// This of course defeats the purpose of using the analog literal. C++0x's proposed `auto' feature would come in quite handy here. We can actually fix this problem partially (using the stack-ref-to-temporary's-base trick used by Alexandrescu's ScopeGuard), but we would no longer be able to use the values in ICE's, and frankly I think this madness has gone far enough already.


using namespace analog_literals::symbols;
  using namespace analog_literals::shapes;

  line<3>(I-------I);

  rectangle<2, 3>(o-----o
                  |     !
                  !     !
                  !     !
                  o-----o);

  cuboid<6, 6, 3>(o-------------o
                  |L             \
                  | L             \
                  |  L             \
                  |   o-------------o
                  |   !             !
                  !   !             !
                  o   |             !
                   L  |             !
                    L |             !
                     L|             !
                      o-------------o );

  cuboid<3, 4, 2>(o-------o
                  |L       \
                  | L       \
                  |  o-------o
                  |  !       !
                  o  |       !
                   L |       !
                    L|       !
                     o-------o);

}
