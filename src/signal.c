#include "signal.h"
#include "hamming.h"
#include "utils.h"
#include <time.h>

void generate_lfm_signal(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                         vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                         vsip_cvview_f *p_vector_dst)
{
    // 信号长度（样本数量）
    vsip_length n_signal_length = (vsip_length)(0.5f + f_tau * f_freq_sampling);

    if (p_vector_dst == NULL)
    {
        fprintf(stderr, "generate_lfm_signal: p_vector_dst is NULL.\n");
        return;
    }
    else if (vsip_cvgetlength_f(p_vector_dst) != n_signal_length)
    {
        fprintf(stderr,
                "generate_lfm_signal: p_vector_dst length is not equal to n_signal_length.\n");
        return;
    }
    else
    {
        ;
    }

    // 初始化时间向量
    vsip_vview_f *p_vector_time = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);
    // 初始化中间变量
    vsip_vview_f *p_vector_temp_0 = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);
    // 初始化中间变量
    vsip_vview_f *p_vector_temp_1 = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);

    // 生成时间向量
    vsip_vramp_f(0.0f, 1.0f / f_freq_sampling, p_vector_time);

    vsip_svmul_f(2.0 * VSIP_PI * f_freq_low, p_vector_time, p_vector_temp_0);

    vsip_vsq_f(p_vector_time, p_vector_temp_1);
    vsip_svmul_f(VSIP_PI * f_band_width / f_tau, p_vector_temp_1, p_vector_temp_1);

    // 相加得到相位
    vsip_vadd_f(p_vector_temp_0, p_vector_temp_1, p_vector_temp_0);

    // 应用欧拉函数
    vsip_veuler_f(p_vector_temp_0, p_vector_dst);

    // 释放内存
    vsip_valldestroy_f(p_vector_time);
    vsip_valldestroy_f(p_vector_temp_0);
    vsip_valldestroy_f(p_vector_temp_1);
}

void generate_lfm_signal_real(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                              vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                              vsip_vview_f *p_vector_dst)
{
    // 信号长度（样本数量）
    vsip_length n_signal_length = (vsip_length)(0.5f + f_tau * f_freq_sampling);

    if (p_vector_dst == NULL)
    {
        fprintf(stderr, "generate_lfm_signal: p_vector_dst is NULL.\n");
        return;
    }
    else if (vsip_vgetlength_f(p_vector_dst) != n_signal_length)
    {
        fprintf(stderr,
                "generate_lfm_signal: p_vector_dst length is not equal to n_signal_length.\n");
        return;
    }
    else
    {
        ;
    }

    // 初始化时间向量
    vsip_vview_f *p_vector_time = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);
    // 初始化中间变量
    vsip_vview_f *p_vector_temp_0 = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);
    // 初始化中间变量
    vsip_vview_f *p_vector_temp_1 = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);

    // 生成时间向量
    vsip_vramp_f(0.0f, 1.0f / f_freq_sampling, p_vector_time);

    vsip_svmul_f(2.0 * VSIP_PI * f_freq_low, p_vector_time, p_vector_temp_0);

    vsip_vsq_f(p_vector_time, p_vector_temp_1);
    vsip_svmul_f(VSIP_PI * f_band_width / f_tau, p_vector_temp_1, p_vector_temp_1);

    // 相加得到相位
    vsip_vadd_f(p_vector_temp_0, p_vector_temp_1, p_vector_temp_0);

    // 应用余弦函数
    vsip_vcos_f(p_vector_temp_0, p_vector_dst);

    // 释放内存
    vsip_valldestroy_f(p_vector_time);
    vsip_valldestroy_f(p_vector_temp_0);
    vsip_valldestroy_f(p_vector_temp_1);
}

void generate_radar_signal(vsip_scalar_f f_tau, vsip_scalar_f f_freq_sampling,
                           vsip_scalar_f f_freq_low, vsip_scalar_f f_band_width,
                           vsip_scalar_f f_distance, vsip_vview_f *p_vector_dst)
{

    // 延迟时间
    vsip_scalar_f f_delay_time = 2.0f * f_distance / 3e8;
    // 总时间
    vsip_scalar_f f_total_time = f_tau + f_delay_time;

    // 延迟信号长度（样本数量）
    vsip_length n_delay_signal_length = (vsip_length)(0.5f + f_delay_time * f_freq_sampling);
    // 总信号长度（样本数量）
    vsip_length n_total_signal_length = (vsip_length)(0.5f + f_total_time * f_freq_sampling);

    if (p_vector_dst == NULL)
    {
        fprintf(stderr, "generate_radar_signal: p_vector_dst is NULL.\n");
        return;
    }
    else if (vsip_vgetlength_f(p_vector_dst) != n_total_signal_length)
    {
        fprintf(
            stderr,
            "generate_radar_signal: p_vector_dst length is not equal to n_total_signal_length.\n");
        return;
    }
    else
    {
        ;
    }

    // LFM 信号长度
    vsip_length n_signal_length = (vsip_length)(0.5f + f_tau * f_freq_sampling);
    // 第一个目标返回的信号
    vsip_vview_f *p_vector_signal_0 = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);
    // 第二个目标返回的信号
    vsip_vview_f *p_vector_signal_1 = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);

    generate_lfm_signal_real(f_tau, f_freq_sampling, f_freq_low, f_band_width, p_vector_signal_0);
    generate_lfm_signal_real(f_tau, f_freq_sampling, f_freq_low, f_band_width, p_vector_signal_1);

    // 延长原始信号向量
    vsip_vview_f *p_vector_signal_0_extended = vsip_vcreate_f(n_total_signal_length, VSIP_MEM_NONE);
    // 延长原始信号向量
    vsip_vview_f *p_vector_signal_1_extended = vsip_vcreate_f(n_total_signal_length, VSIP_MEM_NONE);

    // 零填充
    vsip_vfill_f(0.0f, p_vector_signal_0_extended);
    vsip_vfill_f(0.0f, p_vector_signal_1_extended);

    // 拷贝数据
    for (vsip_length n_index = 0; n_index < n_signal_length; n_index++)
    {
        // 原始信号
        vsip_vput_f(p_vector_signal_0_extended, n_index, vsip_vget_f(p_vector_signal_0, n_index));
        // 延迟信号
        vsip_vput_f(p_vector_signal_1_extended, n_index + n_delay_signal_length,
                    vsip_vget_f(p_vector_signal_1, n_index));
    }

    // 叠加信号
    vsip_vadd_f(p_vector_signal_0_extended, p_vector_signal_1_extended, p_vector_dst);

    // 高斯白噪声，SNR=0dB
    vsip_vview_f *p_vector_wgn_signal = vsip_vcreate_f(n_total_signal_length, VSIP_MEM_NONE);
    generate_wgn_signal(p_vector_dst, 0.0f, p_vector_wgn_signal);

    // 叠加噪声
    vsip_vadd_f(p_vector_dst, p_vector_wgn_signal, p_vector_dst);

    // 释放内存
    vsip_valldestroy_f(p_vector_signal_0);
    vsip_valldestroy_f(p_vector_signal_1);
    vsip_valldestroy_f(p_vector_signal_0_extended);
    vsip_valldestroy_f(p_vector_signal_1_extended);
    vsip_valldestroy_f(p_vector_wgn_signal);
}

void generate_wgn_signal(vsip_vview_f *p_vector_signal, vsip_scalar_f f_snr,
                         vsip_vview_f *p_vector_dst)
{
    vsip_length n_length = vsip_vgetlength_f(p_vector_signal);

    if (p_vector_dst == NULL)
    {
        fprintf(stderr, "generate_wgn_signal: p_vector_dst is NULL.\n");
        return;
    }
    else if (vsip_vgetlength_f(p_vector_dst) != n_length)
    {
        fprintf(stderr, "generate_wgn_signal: p_vector_dst length is not equal to n_length.\n");
        return;
    }
    else
    {
        ;
    }

    // 功率信噪比
    vsip_scalar_f f_snr_power = vsip_pow_f(10.0f, f_snr / 10.0f);
    // 噪声功率
    vsip_scalar_f f_noise_power = vsip_vsumsqval_f(p_vector_signal) / (n_length * f_snr_power);
    // 随机生成
    vsip_randstate *p_rand_state = vsip_randcreate(time(NULL), 1, 1, VSIP_PRNG);
    vsip_vrandn_f(p_rand_state, p_vector_dst);
    vsip_svmul_f(vsip_sqrt_f(f_noise_power), p_vector_dst, p_vector_dst);

    // 释放内存
    vsip_randdestroy(p_rand_state);
}

/// 脉冲压缩
void pulse_compress(vsip_cvview_f *p_vector_signal_src, vsip_cvview_f *p_vector_signal_ref,
                    vsip_cvview_f *p_vector_dst)
{

    if (p_vector_signal_src == NULL)
    {
        fprintf(stderr, "pulse_compress: p_vector_signal_src is NULL.\n");
        return;
    }
    else if (p_vector_signal_ref == NULL)
    {
        fprintf(stderr, "pulse_compress: p_vector_signal_ref is NULL.\n");
        return;
    }
    else if (p_vector_dst == NULL)
    {
        fprintf(stderr, "pulse_compress: p_vector_dst is NULL.\n");
        return;
    }
    else
    {
        ;
    }

    vsip_length n_signal_src_length = vsip_cvgetlength_f(p_vector_signal_src);
    vsip_length n_signal_ref_length = vsip_cvgetlength_f(p_vector_signal_ref);

    // 求参考信号共轭
    vsip_cvconj_f(p_vector_signal_ref, p_vector_signal_ref);
    // 翻转参考信号
    vsip_cvview_f *p_vector_signal_ref_flipped =
        vsip_cvcreate_f(n_signal_ref_length, VSIP_MEM_NONE);
    cvflip_f(p_vector_signal_ref, p_vector_signal_ref_flipped);
    // 加汉明窗
    vsip_vview_f *p_vector_window = vsip_vcreate_f(n_signal_ref_length, VSIP_MEM_NONE);
    vcreate_hamming_f(p_vector_window);

    vsip_rcvmul_f(p_vector_window, p_vector_signal_ref_flipped, p_vector_signal_ref);

    cvdebug_f(p_vector_signal_ref, "./data/reference_signal.txt");

    // 傅里叶变换长度
    vsip_length n_fft_len = 2 * n_signal_src_length - 1;
    n_fft_len = (vsip_length)ceil(0.5 + log2(n_fft_len));
    n_fft_len = (vsip_length)floor(0.5 + pow(2, n_fft_len));

    if (vsip_cvgetlength_f(p_vector_dst) != n_fft_len)
    {
        fprintf(stderr, "pulse_compress: p_vector_dst length is not equal to n_fft_len.\n");
        return;
    }
    else
    {
        ;
    }

    // 补零向量
    vsip_cvview_f *p_vector_signal_src_padded = vsip_cvcreate_f(n_fft_len, VSIP_MEM_NONE);
    vsip_cvview_f *p_vector_signal_ref_padded = vsip_cvcreate_f(n_fft_len, VSIP_MEM_NONE);

    // 补零
    cvpad_f(p_vector_signal_src, p_vector_signal_src_padded);
    cvpad_f(p_vector_signal_ref, p_vector_signal_ref_padded);

    // 傅里叶变换
    vsip_fft_f *p_fft = vsip_ccfftop_create_f(n_fft_len, 1.0, VSIP_FFT_FWD, 1, 0);
    vsip_fft_f *p_ifft = vsip_ccfftop_create_f(n_fft_len, 1.0 / n_fft_len, VSIP_FFT_INV, 1, 0);
    vsip_cvview_f *p_vector_signal_src_fft = vsip_cvcreate_f(n_fft_len, VSIP_MEM_NONE);
    vsip_cvview_f *p_vector_signal_ref_fft = vsip_cvcreate_f(n_fft_len, VSIP_MEM_NONE);

    vsip_ccfftop_f(p_fft, p_vector_signal_src_padded, p_vector_signal_src_fft);
    vsip_ccfftop_f(p_fft, p_vector_signal_ref_padded, p_vector_signal_ref_fft);

    // 相乘
    vsip_cvmul_f(p_vector_signal_src_fft, p_vector_signal_ref_fft, p_vector_signal_src_fft);

    // 傅里叶逆变换
    vsip_ccfftop_f(p_ifft, p_vector_signal_src_fft, p_vector_dst);

    // 释放内存
    vsip_cvalldestroy_f(p_vector_signal_src_fft);
    vsip_cvalldestroy_f(p_vector_signal_ref_fft);
    vsip_fft_destroy_f(p_fft);
    vsip_fft_destroy_f(p_ifft);
    vsip_cvalldestroy_f(p_vector_signal_src_padded);
    vsip_cvalldestroy_f(p_vector_signal_ref_padded);
    vsip_cvalldestroy_f(p_vector_signal_ref_flipped);
    vsip_valldestroy_f(p_vector_window);
}

void detect_signal(vsip_cvview_f *p_vector_signal, vsip_scalar_f f_threshold,
                   vsip_cvview_f *p_vector_dst)
{
    if (p_vector_signal == NULL)
    {
        fprintf(stderr, "detect_signal: p_vector_signal is NULL.\n");
        return;
    }
    else if (p_vector_dst == NULL)
    {
        fprintf(stderr, "detect_signal: p_vector_dst is NULL.\n");
        return;
    }
    else
    {
        ;
    }

    vsip_length n_signal_length = vsip_cvgetlength_f(p_vector_signal);

    if (vsip_cvgetlength_f(p_vector_dst) != n_signal_length)
    {
        fprintf(stderr, "detect_signal: p_vector_dst length is not equal to n_signal_length.\n");
        return;
    }
    else
    {
        ;
    }

    for (vsip_length n_index = 0; n_index < n_signal_length; n_index++)
    {
        vsip_scalar_f f_abs = vsip_cmag_f(vsip_cvget_f(p_vector_signal, n_index));
        if (fabs(f_abs) < f_threshold && fabs(f_abs) < f_threshold)
        {
            // 低于阈值的信号置零
            vsip_cvput_f(p_vector_dst, n_index, vsip_cmplx_f(0.0f, 0.0f));
        }
        else
        {
            // 大于阈值的信号保留
            vsip_cvput_f(p_vector_dst, n_index, vsip_cvget_f(p_vector_signal, n_index));
        }
    }
}