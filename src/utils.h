#ifndef RADAR_UTILS_H_
#define RADAR_UTILS_H_

#include "vsip.h"

/*
 * 内部接口：输出实向量
 * 参数：p_vector -- 输入向量
 *      p_file   -- 输出文件
 * 功能：将实向量的数据输出到文件
 */
void vdump_f(vsip_vview_f *p_vector, FILE *p_file);

/*
 * 内部接口：输出复向量
 * 参数：p_vector -- 输入向量
 *      p_file   -- 输出文件
 * 功能：将复向量的数据输出到文件
 */
void cvdump_f(vsip_cvview_f *p_vector, FILE *p_file);

/*
 * 内部接口：实向量调试
 * 参数：p_vector -- 输入向量
 *      p_name   -- 输出文件名
 * 功能：将实向量的数据输出到指定文件名的文件
 */
void vdebug_f(vsip_vview_f *p_vector, char *p_name);

/*
 * 内部接口：复向量调试
 * 参数：p_vector -- 输入向量
 *      p_name   -- 输出文件名
 * 功能：将复向量的数据输出到指定文件名的文件
 */
void cvdebug_f(vsip_cvview_f *p_vector, char *p_name);

/*
 * 内部接口：复向量翻转
 * 参数：p_vector_src -- 输入向量
 *      p_vector_dst -- 输出向量
 * 功能：将输入向量的数据翻转后输出到输出向量
 */
void cvflip_f(vsip_cvview_f *p_vector_src, vsip_cvview_f *p_vector_dst);

/*
 * 内部接口：复向量填充
 * 参数：p_vector_src -- 输入向量
 *      p_vector_dst -- 输出向量
 * 功能：根据输出向量的长度对输入向量进行零填充得到输出
 */
void cvpad_f(vsip_cvview_f *p_vector_src, vsip_cvview_f *p_vector_dst);

#endif