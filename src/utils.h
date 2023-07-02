#ifndef RADAR_UTILS_H_
#define RADAR_UTILS_H_

#include "vsip.h"

void vdump_f(vsip_vview_f *p_vector, FILE *p_file);
void cvdump_f(vsip_cvview_f *p_vector, FILE *p_file);

void vdebug_f(vsip_vview_f *p_vector, char *p_name);
void cvdebug_f(vsip_cvview_f *p_vector, char *p_name);

void cvflip_f(vsip_cvview_f *p_vector_src, vsip_cvview_f *p_vector_dst);
void cvpad_f(vsip_cvview_f *p_vector_src, vsip_cvview_f *p_vector_dst);

#endif