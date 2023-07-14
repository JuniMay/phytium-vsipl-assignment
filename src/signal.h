#ifndef RADAR_SIGNAL_H_
#define RADAR_SIGNAL_H_

#include "vsip.h"

/*
 * 内部接口：生成线性调频信号
 * 参数：f_tau           -- 脉冲宽度
 *      f_freq_sampling -- 采样频率
 *      f_freq_low      -- 起始频率
 *      f_band_width    -- 带宽
 *      p_vector_dst    -- 输出信号
 * 功能：生成线性调频信号（复信号）
 */
void generate_lfm_signal(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                         vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                         vsip_cvview_f *p_vector_dst);

/*
 * 内部接口：生成线性调频信号
 * 参数：f_tau           -- 脉冲宽度
 *      f_freq_sampling -- 采样频率
 *      f_freq_low      -- 起始频率
 *      f_band_width    -- 带宽
 *      p_vector_dst    -- 输出信号
 * 功能：生成线性调频信号（实信号）
 */
void generate_lfm_signal_real(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                              vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                              vsip_vview_f *p_vector_dst);

/*
 * 内部接口：生成雷达回波信号
 * 参数：f_tau           -- 脉冲宽度
 *      f_freq_sampling -- 采样频率
 *      f_freq_low      -- 起始频率
 *      f_band_width    -- 带宽
 *      f_distance      -- 两个物体之间的距离
 *      p_vector_dst    -- 输出信号
 * 功能：生成两个有一定距离的物体反射叠加得到的雷达回波信号
 */
void generate_radar_signal(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                           vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                           vsip_scalar_f f_distance, vsip_vview_f *p_vector_dst);

/*
 * 内部接口：生成雷达回波信号
 * 参数：p_vector_signal -- 输入信号
 *      f_snr           -- 目标信号信噪比
 *      p_vector_dst    -- 输出信号
 * 功能：生成可以叠加到原信号上的给定信噪比的高斯白噪声
 */
void generate_wgn_signal(vsip_vview_f *p_vector_signal, vsip_scalar_f f_snr,
                         vsip_vview_f *p_vector_dst);

/*
 * 内部接口：脉冲压缩
 * 参数：p_vector_signal_src -- 输入信号
 *      p_vector_signal_ref -- 参考信号
 *      p_vector_dst        -- 输出信号
 * 功能：使用给定的参考信号对输入信号进行脉冲压缩
 */
void pulse_compress(vsip_cvview_f *p_vector_signal_src, vsip_cvview_f *p_vector_signal_ref,
                    vsip_cvview_f *p_vector_dst);

/*
 * 内部接口：检测信号
 * 参数：p_vector_signal -- 脉冲压缩之后得到的信号
 *      f_threshold     -- 阈值
 *      p_vector_dst    -- 输出信号
 * 功能：对脉冲压缩之后的信号进行进一步检测，依据阈值进行筛选
 */
void detect_signal(vsip_cvview_f *p_vector_signal, vsip_scalar_f f_threshold,
                   vsip_cvview_f *p_vector_dst);

#endif