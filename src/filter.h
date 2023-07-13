#ifndef RADAR_FILTER_H_
#define RADAR_FILTER_H_

#include "vsip.h"

/*
 * 内部接口：希尔伯特滤波
 * 参数：p_vector_src    -- 输入信号
 *      n_filter_length -- 滤波器长度
 *      p_vector_dst    -- 输出信号
 * 功能：对输入信号进行希尔伯特滤波
 */
void hilbert(vsip_vview_f *p_vector_src, vsip_scalar_i n_filter_length,
             vsip_cvview_f *p_vector_dst);

#endif