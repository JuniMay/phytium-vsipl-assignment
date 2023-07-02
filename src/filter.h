#ifndef RADAR_FILTER_H_
#define RADAR_FILTER_H_

#include "vsip.h"

void hilbert(vsip_vview_f *p_vector_src, vsip_scalar_i n_filter_length,
             vsip_cvview_f *p_vector_dst);
void pulse_compress(vsip_cvview_f *p_vector_signal_src, vsip_cvview_f* p_vector_signal_ref, vsip_cvview_f *p_vector_dst);

#endif