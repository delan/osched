#ifndef OS200_SJF_BACKEND_H
#define OS200_SJF_BACKEND_H

#include "result.h"
#include "list.h"

os200_result os200_sjf_file(const char *filename);

os200_result os200_sjf_core(os200_list jobs);

#endif
