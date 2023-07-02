#ifndef RADAR_SIGNAL_H_
#define RADAR_SIGNAL_H_

#include "vsip.h"

void generate_lfm_signal(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                         vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                         vsip_cvview_f *p_vector_dst);

void generate_lfm_signal_real(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                              vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                              vsip_vview_f *p_vector_dst);

void generate_radar_signal(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                           vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                           vsip_scalar_f f_distance, vsip_vview_f *p_vector_dst);

void generate_wgn_signal(vsip_vview_f *p_vector_signal, vsip_scalar_f f_snr,
                         vsip_vview_f *p_vector_dst);

#endif