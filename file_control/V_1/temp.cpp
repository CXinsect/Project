#include <iostream>
#include <stdio.h>
#include <stdlib.h>
int main (void) {
    char buf[1024];
    sprintf(buf,"%x",4);
    std::cout<<buf<<std::endl;
    return 0;
}