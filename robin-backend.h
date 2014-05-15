#ifndef OS200_ROBIN_BACKEND_H
#define OS200_ROBIN_BACKEND_H

#include "result.h"
#include "list.h"

os200_result os200_robin_file(const char *filename);

os200_result os200_robin_core(os200_list jobs, double quantum);

#endif
