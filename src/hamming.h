#ifndef RADAR_HAMMING_H_
#define RADAR_HAMMING_H_

#include "vsip.h"

/*
 * 内部接口：生成汉明窗
 * 参数：p_vector_dst -- 输出信号
 * 功能：根据输出信号的长度生成汉明窗
 */
void vcreate_hamming_f(vsip_vview_f *p_vector_dst);

#endif