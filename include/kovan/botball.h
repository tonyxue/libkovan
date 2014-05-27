#ifndef _BOTBALL_H_
#define _BOTBALL_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYM void shut_down_in(double s);
EXPORT_SYM void wait_for_light(int light_port_);
EXPORT_SYM void run_for(double sec, void* function_name);

#ifdef __cplusplus
}
#endif

#endif
