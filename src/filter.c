#include "filter.h"
#include "utils.h"

void hilbert(vsip_vview_f *p_vector_src, vsip_scalar_i n_filter_length, vsip_cvview_f *p_vector_dst)
{
    vsip_length n_signal_length = vsip_vgetlength_f(p_vector_src);

    if (p_vector_src == NULL)
    {
        fprintf(stderr, "hilbert: p_vector_src is NULL\n");
        return;
    }
    else if (vsip_cvgetlength_f(p_vector_dst) != n_signal_length)
    {
        fprintf(stderr, "hilbert: p_vector_dst length is not equal to p_vector_src\n");
        return;
    }
    else
    {
        ;
    }

    vsip_cvview_f *p_vector_hilbert_transformer = vsip_cvcreate_f(n_filter_length, VSIP_MEM_NONE);

    // 正负半轴元素个数
    vsip_scalar_i n_half_filter_length = n_filter_length / 2;

    for (vsip_length n_index = 0; n_index < n_filter_length; n_index++)
    {
        // 计算索引偏移
        vsip_scalar_i n_index_offset = (vsip_scalar_i)n_index - n_half_filter_length;
        // 默认 0
        vsip_cscalar_f c_value = vsip_cmplx_f(0.0f, 0.0f);

        if (n_index_offset == 0)
        {
            // 0 处的冲激响应
            c_value = vsip_cmplx_f(1.0f, 0.0f);
        }
        else if (n_index_offset % 2 != 0)
        {
            // 2j / (pi * n)
            c_value = vsip_cmplx_f(0.0f, 2.0f / (VSIP_PI * n_index_offset));
        }
        else
        {
            ;
        }
        vsip_cvput_f(p_vector_hilbert_transformer, n_index, c_value);
    }

    cvdebug_f(p_vector_hilbert_transformer, "./data/hilbert_coefficients.txt");

    // 得到滤波器系数的实部和虚部
    vsip_vview_f *p_vector_hilbert_transformer_real =
        vsip_vcreate_f(n_filter_length, VSIP_MEM_NONE);
    vsip_vview_f *p_vector_hilbert_transformer_imag =
        vsip_vcreate_f(n_filter_length, VSIP_MEM_NONE);
    vsip_vreal_f(p_vector_hilbert_transformer, p_vector_hilbert_transformer_real);
    vsip_vimag_f(p_vector_hilbert_transformer, p_vector_hilbert_transformer_imag);

    // 用于卷积结果的实部和虚部
    vsip_vview_f *p_vector_cmplx_signal_real = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);
    vsip_vview_f *p_vector_cmplx_signal_imag = vsip_vcreate_f(n_signal_length, VSIP_MEM_NONE);

    // 当前 VSIPL 版本中没有复数卷积的操作，由于希尔伯特滤波器系数为实数或虚数而没有复合，
    // 所以可以分别对实部和虚部进行卷积操作。

    // 实部卷积核
    vsip_conv1d_f *p_filter_real =
        vsip_conv1d_create_f(p_vector_hilbert_transformer_real, VSIP_NONSYM, n_signal_length, 1,
                             VSIP_SUPPORT_SAME, 0, VSIP_ALG_TIME);
    // 虚部卷积核
    vsip_conv1d_f *p_filter_imag =
        vsip_conv1d_create_f(p_vector_hilbert_transformer_imag, VSIP_NONSYM, n_signal_length, 1,
                             VSIP_SUPPORT_SAME, 0, VSIP_ALG_TIME);

    // 实部卷积
    vsip_convolve1d_f(p_filter_real, p_vector_src, p_vector_cmplx_signal_real);
    // 虚部卷积
    vsip_convolve1d_f(p_filter_imag, p_vector_src, p_vector_cmplx_signal_imag);

    // 合成复信号
    for (vsip_length n_index = 0; n_index < n_signal_length; n_index++)
    {
        vsip_cscalar_f c_value = vsip_cmplx_f(vsip_vget_f(p_vector_cmplx_signal_real, n_index),
                                              vsip_vget_f(p_vector_cmplx_signal_imag, n_index));
        vsip_cvput_f(p_vector_dst, n_index, c_value);
    }

    // 释放
    vsip_conv1d_destroy_f(p_filter_real);
    vsip_conv1d_destroy_f(p_filter_imag);
    vsip_vdestroy_f(p_vector_cmplx_signal_imag);
    vsip_vdestroy_f(p_vector_cmplx_signal_real);
    vsip_vdestroy_f(p_vector_hilbert_transformer_imag);
    vsip_vdestroy_f(p_vector_hilbert_transformer_real);
    vsip_cvalldestroy_f(p_vector_hilbert_transformer);
}