#include "utils.h"

void vdump_f(vsip_vview_f *p_vector, FILE *p_file)
{
    if (p_vector == NULL)
    {
        fprintf(stderr, "vdump_f: p_vector is NULL\n");
        return;
    }
    else
    {
        ;
    }
    vsip_length n_length = vsip_vgetlength_f(p_vector);
    for (vsip_index n_index = 0; n_index < n_length; n_index++)
    {
        fprintf(p_file, "%f\n", vsip_vget_f(p_vector, n_index));
    }
}

void cvdump_f(vsip_cvview_f *p_vector, FILE *p_file)
{
    if (p_vector == NULL)
    {
        fprintf(stderr, "vdump_f: p_vector is NULL\n");
        return;
    }
    else
    {
        ;
    }
    vsip_length n_length = vsip_cvgetlength_f(p_vector);
    for (vsip_index n_index = 0; n_index < n_length; n_index++)
    {
        vsip_cscalar_f c_value = vsip_cvget_f(p_vector, n_index);
        fprintf(p_file, "%+9.4f%+9.4fj\n", vsip_real_f(c_value), vsip_imag_f(c_value));
    }
}

void vdebug_f(vsip_vview_f *p_vector, char *p_name)
{
    FILE *p_file = fopen(p_name, "w");
    if (p_file == NULL)
    {
        fprintf(stderr, "vdebug_f: unable to open file `%s`\n", p_name);
        return;
    }
    else
    {
        ;
    }
    vdump_f(p_vector, p_file);
    fclose(p_file);
}

void cvdebug_f(vsip_cvview_f *p_vector, char *p_name)
{
    FILE *p_file = fopen(p_name, "w");
    if (p_file == NULL)
    {
        fprintf(stderr, "cvdebug_f: unable to open file `%s`\n", p_name);
        return;
    }
    else
    {
        ;
    }
    cvdump_f(p_vector, p_file);
    fclose(p_file);
}

void cvflip_f(vsip_cvview_f *p_vector_src, vsip_cvview_f *p_vector_dst)
{
    if (p_vector_src == NULL)
    {
        fprintf(stderr, "cvflip_f: p_vector is NULL\n");
        return;
    }
    else
    {
        ;
    }

    vsip_length n_length = vsip_cvgetlength_f(p_vector_src);

    if (p_vector_dst == NULL)
    {
        fprintf(stderr, "cvflip_f: p_vector_dst is NULL\n");
        return;
    }
    else if (n_length != vsip_cvgetlength_f(p_vector_dst))
    {
        fprintf(stderr, "cvflip_f: p_vector_src and p_vector_dst have different lengths\n");
        return;
    }
    else
    {
        ;
    }

    for (vsip_index n_index = 0; n_index < n_length; n_index++)
    {
        vsip_cvput_f(p_vector_dst, n_index, vsip_cvget_f(p_vector_src, n_length - n_index - 1));
    }
}

void cvpad_f(vsip_cvview_f *p_vector_src, vsip_cvview_f *p_vector_dst)
{
    if (p_vector_src == NULL)
    {
        fprintf(stderr, "cvpad_f: p_vector is NULL\n");
        return;
    }
    else
    {
        ;
    }

    vsip_length n_length_src = vsip_cvgetlength_f(p_vector_src);

    if (p_vector_dst == NULL)
    {
        fprintf(stderr, "cvpad_f: p_vector_dst is NULL\n");
        return;
    }
    else if (n_length_src > vsip_cvgetlength_f(p_vector_dst))
    {
        fprintf(stderr, "cvpad_f: p_vector_src is longer than p_vector_dst\n");
        return;
    }
    else
    {
        ;
    }

    vsip_length n_length_dst = vsip_cvgetlength_f(p_vector_dst);

    for (vsip_index n_index = 0; n_index < n_length_src; n_index++)
    {
        vsip_cvput_f(p_vector_dst, n_index, vsip_cvget_f(p_vector_src, n_index));
    }

    for (vsip_index n_index = n_length_src; n_index < n_length_dst; n_index++)
    {
        vsip_cvput_f(p_vector_dst, n_index, vsip_cmplx_f(0.0f, 0.0f));
    }
}