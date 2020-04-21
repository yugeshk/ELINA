/*
 *
 *  This source file is part of ELINA (ETH LIbrary for Numerical Analysis).
 *  ELINA is Copyright © 2020 Department of Computer Science, ETH Zurich
 *  This software is distributed under GNU Lesser General Public License Version 3.0.
 *  For more information, see the ELINA project website at:
 *  http://elina.ethz.ch
 *
 *  THE SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER
 *  EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO ANY WARRANTY
 *  THAT THE SOFTWARE WILL CONFORM TO SPECIFICATIONS OR BE ERROR-FREE AND ANY
 *  IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
 *  TITLE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL ETH ZURICH BE LIABLE FOR ANY
 *  DAMAGES, INCLUDING BUT NOT LIMITED TO DIRECT, INDIRECT,
 *  SPECIAL OR CONSEQUENTIAL DAMAGES, ARISING OUT OF, RESULTING FROM, OR IN
 *  ANY WAY CONNECTED WITH THIS SOFTWARE (WHETHER OR NOT BASED UPON WARRANTY,
 *  CONTRACT, TORT OR OTHERWISE).
 *
 *  @file relu_approx.h
 *  @author Christoph Müller
 *  @brief Provides the kernels for the relu approximation.
 */

#ifndef __RELU_APPROX_H_INCLUDED__
#define __RELU_APPROX_H_INCLUDED__

#include "interval.h"


__device__
void lcoeff_replace_relu_bounds(float_type& inf_coeff, float_type& sup_coeff, float_type& res_inf_cst, float_type& res_sup_cst, const float_type lb, const float_type ub, const bool use_area_heuristic)
{
    const float_type width_inf = add_rd(ub, -lb);
    const float_type width_sup = add_ru(ub, -lb);
    const float_type lambda_inf = div_rd(ub, width_sup);
    const float_type lambda_sup = div_ru(ub, width_inf);

    const float_type old_inf_coeff = inf_coeff;
    const float_type old_sup_coeff = sup_coeff;

    if((old_sup_coeff == 0) && (old_inf_coeff == 0))
    {
        inf_coeff = 0.0;
        sup_coeff = 0.0;
    }
    else if(ub <= 0)
    {
        inf_coeff = 0.0;
        sup_coeff = 0.0;
    }
    else if(lb > 0)
    {
        inf_coeff = old_inf_coeff;
        sup_coeff = old_sup_coeff;
    }
    else if(old_sup_coeff < 0)
    {
        const float_type mu_inf = -mul_rd(lambda_inf, lb);
        const float_type mu_sup = -mul_ru(lambda_sup, lb);
        interval_mul_expr_coeff(inf_coeff, sup_coeff, lambda_inf, lambda_sup, old_inf_coeff, old_sup_coeff);
        float_type tmp1, tmp2;
        interval_mul_cst_coeff(tmp1, tmp2, mu_inf, mu_sup, old_inf_coeff, old_sup_coeff);

        res_inf_cst = add_rd(res_inf_cst, add_rd(tmp1, -min_denormal));
        res_sup_cst = add_ru(res_sup_cst, add_ru(tmp2,  min_denormal));
    }
    else if (old_inf_coeff > 0)
    {
        if(use_area_heuristic)
        {
            const float_type area1 = 0.5*ub*width_sup;
            const float_type area2 = -0.5*lb*width_sup;

            if(area1 < area2)
            {
                inf_coeff = 0.0;
                sup_coeff = 0.0;
            }
            else
            {
                inf_coeff = old_inf_coeff;
                sup_coeff = old_sup_coeff;
            }
        }
        else
        {
            inf_coeff = 0.0;
            sup_coeff = 0.0;
        }
    }
    else
    {
        inf_coeff = 0.0;
        sup_coeff = 0.0;
        float_type tmp1, tmp2;
        interval_mul(tmp1, tmp2, old_inf_coeff, old_sup_coeff, 0, ub);

        res_inf_cst = add_rd(res_inf_cst, tmp1);
        res_sup_cst = add_ru(res_sup_cst, tmp1);
    }
}


__device__
void ucoeff_replace_relu_bounds(float_type& inf_coeff, float_type& sup_coeff, float_type& res_inf_cst, float_type& res_sup_cst, const float_type lb, const float_type ub, const bool use_area_heuristic)
{
    const float_type width_inf = add_rd(ub, -lb);
    const float_type width_sup = add_ru(ub, -lb);
    const float_type lambda_inf = div_rd(ub, width_sup);
    const float_type lambda_sup = div_ru(ub, width_inf);

    const float_type old_inf_coeff = inf_coeff;
    const float_type old_sup_coeff = sup_coeff;

    if((old_sup_coeff == 0) && (old_inf_coeff == 0))
    {
        inf_coeff = 0.0;
        sup_coeff = 0.0;
    }
    else if(ub <= 0)
    {
        inf_coeff = 0.0;
        sup_coeff = 0.0;
    }
    else if(lb > 0)
    {
        inf_coeff = old_inf_coeff;
        sup_coeff = old_sup_coeff;
    }
    else if(old_inf_coeff > 0)
    {
        const float_type mu_inf = -mul_rd(lambda_inf, lb);
        const float_type mu_sup = -mul_ru(lambda_sup, lb);
        interval_mul_expr_coeff(inf_coeff, sup_coeff, lambda_inf, lambda_sup, old_inf_coeff, old_sup_coeff);
        float_type tmp1, tmp2;
        interval_mul_cst_coeff(tmp1, tmp2, mu_inf, mu_sup, old_inf_coeff, old_sup_coeff);

        res_inf_cst = add_rd(res_inf_cst, add_rd(tmp1, -min_denormal));
        res_sup_cst = add_ru(res_sup_cst, add_ru(tmp2,  min_denormal));
    }
    else if(old_sup_coeff < 0)
    {
        if(use_area_heuristic)
        {
            const float_type area1 = 0.5*ub*width_sup;
            const float_type area2 = -0.5*lb*width_sup;

            if(area1 < area2)
            {
                inf_coeff = 0.0;
                sup_coeff = 0.0;
            }
            else
            {
                inf_coeff = old_inf_coeff;
                sup_coeff = old_sup_coeff;
            }
        }
        else
        {
            inf_coeff = 0.0;
            sup_coeff = 0.0;
        }
    }
    else
    {
        inf_coeff = 0.0;
        sup_coeff = 0.0;
        float_type tmp1, tmp2;
        interval_mul(tmp1, tmp2, old_inf_coeff, old_sup_coeff, 0, ub);

        res_inf_cst = add_rd(res_inf_cst, tmp2);
        res_sup_cst = add_ru(res_sup_cst, tmp2);
    }
}

#endif //__RELU_APPROX_H_INCLUDED__
