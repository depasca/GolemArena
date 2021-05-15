#ifndef __GAUTILS_H__
#define __GAUTILS_H__

#include <stdio.h>

bool GAPAUSE=false;

void gapause() { GAPAUSE=!GAPAUSE; printf("\nGAPAUSE=%d ",GAPAUSE); }


#endif