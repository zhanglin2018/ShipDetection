#ifndef __DEBUGINFO_H__
#define __DEBUGINFO_H__

#include <stdlib.h>
#include <stdio.h>


// input Debug information
#define DEBUG_INFO(format, ...)  printf("Files: %s, Line: %d, Function: %s, %s", \
	__FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)



#endif