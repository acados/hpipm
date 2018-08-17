/**************************************************************************************************
*                                                                                                 *
* This file is part of HPIPM.                                                                     *
*                                                                                                 *
* HPIPM -- High-Performance Interior Point Method.                                                *
* Copyright (C) 2017-2018 by Gianluca Frison.                                                     *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* This program is free software: you can redistribute it and/or modify                            *
* it under the terms of the GNU General Public License as published by                            *
* the Free Software Foundation, either version 3 of the License, or                               *
* (at your option) any later version                                                              *.
*                                                                                                 *
* This program is distributed in the hope that it will be useful,                                 *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                   *
* GNU General Public License for more details.                                                    *
*                                                                                                 *
* You should have received a copy of the GNU General Public License                               *
* along with this program.  If not, see <https://www.gnu.org/licenses/>.                          *
*                                                                                                 *
* The authors designate this particular file as subject to the "Classpath" exception              *
* as provided by the authors in the LICENSE file that accompained this code.                      *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/

#ifndef HPIPM_S_OCP_QP_SOL_H_
#define HPIPM_S_OCP_QP_SOL_H_



#include <blasfeo_target.h>
#include <blasfeo_common.h>

#include "hpipm_s_ocp_qp_dim.h"



#ifdef __cplusplus
extern "C" {
#endif



struct s_ocp_qp_sol
	{
	struct s_ocp_qp_dim *dim;
	struct blasfeo_svec *ux;
	struct blasfeo_svec *pi;
	struct blasfeo_svec *lam;
	struct blasfeo_svec *t;
	void *misc;
	int memsize; // memory size in bytes
	};



//
int s_sizeof_ocp_qp_sol();
//
int s_memsize_ocp_qp_sol(struct s_ocp_qp_dim *dim);
//
void s_create_ocp_qp_sol(struct s_ocp_qp_dim *dim, struct s_ocp_qp_sol *qp_sol, void *memory);
//
void s_cvt_ocp_qp_sol_to_colmaj(struct s_ocp_qp_sol *qp_sol, float **u, float **x, float **ls, float **us, float **pi, float **lam_lb, float **lam_ub, float **lam_lg, float **lam_ug, float **lam_ls, float **lam_us);
//
void s_cvt_colmaj_to_ocp_qp_sol(float **u, float **x, float **ls, float **us, float **pi, float **lam_lb, float **lam_ub, float **lam_lg, float **lam_ug, float **lam_ls, float **lam_us, struct s_ocp_qp_sol *qp_sol);
//
void s_cvt_ocp_qp_sol_to_rowmaj(struct s_ocp_qp_sol *qp_sol, float **u, float **x, float **ls, float **us, float **pi, float **lam_lb, float **lam_ub, float **lam_lg, float **lam_ug, float **lam_ls, float **lam_us);
//
void s_cvt_ocp_qp_sol_to_libstr(struct s_ocp_qp_sol *qp_sol, struct blasfeo_svec *u, struct blasfeo_svec *ls, struct blasfeo_svec *us, struct blasfeo_svec *x, struct blasfeo_svec *pi, struct blasfeo_svec *lam_lb, struct blasfeo_svec *lam_ub, struct blasfeo_svec *lam_lg, struct blasfeo_svec *lam_ug, struct blasfeo_svec *lam_ls, struct blasfeo_svec *lam_us);
//
void s_cvt_ocp_qp_sol_to_colmaj_u(int stage, struct s_ocp_qp_sol *qp_sol, float *vec);
//
void s_cvt_ocp_qp_sol_to_colmaj_x(int stage, struct s_ocp_qp_sol *qp_sol, float *vec);
//
void s_cvt_ocp_qp_sol_to_colmaj_pi(int stage, struct s_ocp_qp_sol *qp_sol, float *vec);
//
void s_cvt_ocp_qp_sol_to_colmaj_lam_lb(int stage, struct s_ocp_qp_sol *qp_sol, float *vec);
//
void s_cvt_ocp_qp_sol_to_colmaj_lam_ub(int stage, struct s_ocp_qp_sol *qp_sol, float *vec);
//
void s_cvt_ocp_qp_sol_to_colmaj_lam_lg(int stage, struct s_ocp_qp_sol *qp_sol, float *vec);
//
void s_cvt_ocp_qp_sol_to_colmaj_lam_ug(int stage, struct s_ocp_qp_sol *qp_sol, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_sol_u(int stage, float *vec, struct s_ocp_qp_sol *qp_sol);
//
void s_cvt_colmaj_to_ocp_qp_sol_x(int stage, float *vec, struct s_ocp_qp_sol *qp_sol);
//
void s_cvt_colmaj_to_ocp_qp_sol_sl(int stage, float *vec, struct s_ocp_qp_sol *qp_sol);
//
void s_cvt_colmaj_to_ocp_qp_sol_su(int stage, float *vec, struct s_ocp_qp_sol *qp_sol);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif // HPIPM_S_OCP_QP_SOL_H_
