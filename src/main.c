#include "filter.h"
#include "hamming.h"
#include "signal.h"
#include "utils.h"
#include "vsip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    vsip_init((void *)0);

    // 脉冲宽度
    const vsip_scalar_f f_tau = 7e-6;
    // 采样率
    const vsip_scalar_f f_freq_sampling = 20e6;
    // 下限频率
    const vsip_scalar_f f_freq_low = 222e6;
    // 带宽
    const vsip_scalar_f f_band_width = 6e6;
    // 目标距离
    const vsip_scalar_f f_distance = 600.0f;
    // LFM 信号长度
    const vsip_length n_signal_length = (vsip_length)(0.5f + f_tau * f_freq_sampling);
    // 延迟时间
    const vsip_scalar_f f_delay_time = 2.0f * f_distance / 3e8;
    // 总时间
    const vsip_scalar_f f_total_time = f_tau + f_delay_time;
    // 延迟信号长度（样本数量）
    const vsip_length n_delay_signal_length = (vsip_length)(0.5f + f_delay_time * f_freq_sampling);
    // 总信号长度（样本数量）
    const vsip_length n_total_signal_length = (vsip_length)(0.5f + f_total_time * f_freq_sampling);
    // 希尔伯特系数
    const vsip_length n_hilbert_length = 11;

    // 生成雷达接收的实信号
    vsip_vview_f *p_vector_radar_signal = vsip_vcreate_f(n_total_signal_length, VSIP_MEM_NONE);
    generate_radar_signal(f_tau, f_freq_sampling, f_freq_low, f_band_width, 600.0f,
                          p_vector_radar_signal);

    // DEBUG
    vdebug_f(p_vector_radar_signal, "./data/radar_signal.txt");

    // 希尔伯特滤波
    vsip_cvview_f *p_vector_radar_signal_filtered =
        vsip_cvcreate_f(n_total_signal_length, VSIP_MEM_NONE);
    hilbert(p_vector_radar_signal, 11, p_vector_radar_signal_filtered);

    // DEBUG
    cvdebug_f(p_vector_radar_signal_filtered, "./data/radar_signal_filtered.txt");

    // 匹配滤波参考信号
    vsip_cvview_f *p_vector_signal_ref = vsip_cvcreate_f(n_signal_length, VSIP_MEM_NONE);
    generate_lfm_signal(f_tau, f_freq_sampling, f_freq_low, f_band_width, p_vector_signal_ref);

    // 傅里叶变换长度
    vsip_length n_fft_len = 2 * n_total_signal_length - 1;
    n_fft_len = (vsip_length)ceil(0.5 + log2(n_fft_len));
    n_fft_len = (vsip_length)floor(0.5 + pow(2, n_fft_len));

    // DEBUG
    printf("radar: n_fft_len = %ld\n", n_fft_len);

    // 脉冲压缩
    vsip_cvview_f *p_vector_radar_signal_compressed = vsip_cvcreate_f(n_fft_len, VSIP_MEM_NONE);
    pulse_compress(p_vector_radar_signal_filtered, p_vector_signal_ref,
                   p_vector_radar_signal_compressed);

    // DEBUG
    cvdebug_f(p_vector_radar_signal_compressed, "./data/radar_signal_compressed.txt");

    vsip_cvview_f *p_vector_radar_signal_reduced = vsip_cvcreate_f(n_fft_len, VSIP_MEM_NONE);
    detect_signal(p_vector_radar_signal_compressed, 30.0f, p_vector_radar_signal_reduced);

    // DEBUG
    cvdebug_f(p_vector_radar_signal_reduced, "./data/radar_signal_reduced.txt");

    vsip_scalar_f f_delta_time = 1.0f / f_freq_sampling;
    vsip_length n_detect = 0;

    vsip_scalar_f f_prev_time = 0.0f;
    for (vsip_length n_index = 0; n_index < n_fft_len; n_index++)
    {
        vsip_scalar_f f_time = (vsip_scalar_f)n_index * f_delta_time;
        vsip_scalar_f f_real = vsip_real_f(vsip_cvget_f(p_vector_radar_signal_reduced, n_index));

        if (f_real > 0.0f)
        {
            if (f_prev_time == 0.0f)
            {
                f_prev_time = f_time;
                printf("detect: %f\n", f_time);
            }
            else
            {
                vsip_scalar_f distance = 3e8 * (f_time - f_prev_time) / 2.0f;
                printf("detect: time = %fs, distance = %fm\n", f_time, distance);
            }
        }
        else
        {
            ;
        }
    }
    // 释放内存
    vsip_cvalldestroy_f(p_vector_radar_signal_reduced);
    vsip_cvalldestroy_f(p_vector_radar_signal_compressed);
    vsip_cvalldestroy_f(p_vector_signal_ref);
    vsip_valldestroy_f(p_vector_radar_signal);
    vsip_cvalldestroy_f(p_vector_radar_signal_filtered);

    vsip_finalize((void *)0);

    return 0;
}