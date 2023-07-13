#include "hamming.h"

void vcreate_hamming_f(vsip_vview_f *p_vector_dst)
{
    if (p_vector_dst == NULL)
    {
        fprintf(stderr, "vcreate_hamming_f: p_vector_dst is NULL\n");
        return;
    }
    else
    {
        ;
    }

    vsip_length n_length = vsip_vgetlength_f(p_vector_dst);

    // 汉明窗有关参数
    const vsip_scalar_f f_a0 = (float)0.53836;
    const vsip_scalar_f f_a1 = (float)(1.0f - f_a0);

    for (vsip_length n_index = 0; n_index < n_length; n_index++)
    {
        vsip_scalar_f f_element =
            f_a0 - f_a1 * (float)vsip_cos_f(2.0 * VSIP_PI * n_index / (n_length - 1));
        vsip_vput_f(p_vector_dst, n_index, f_element);
    }
}