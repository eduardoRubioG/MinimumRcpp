#ifndef KINTERSECTION_CPP
#define KINTERSECTION_CPP
#include <stdio.h>
#include "kintersection.h"

int kintersection( const int i ){
    printf("Hello, world\n");
    return inner(i) + 2;
}

int inner( const int i ){
  printf("On the inside\n");
  return i + 1;
}

#endif // KINTERSECTION_CPP

