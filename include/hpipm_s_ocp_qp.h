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

#ifndef HPIPM_S_OCP_QP_H_
#define HPIPM_S_OCP_QP_H_



#include <blasfeo_target.h>
#include <blasfeo_common.h>

#include "hpipm_s_ocp_qp_dim.h"



#ifdef __cplusplus
extern "C" {
#endif



struct s_ocp_qp
	{
	struct s_ocp_qp_dim *dim;
	struct blasfeo_smat *BAbt; // dynamics matrix & vector work space
	struct blasfeo_smat *RSQrq; // hessian of cost & vector work space
	struct blasfeo_smat *DCt; // inequality constraints matrix
	struct blasfeo_svec *b; // dynamics vector
	struct blasfeo_svec *rqz; // gradient of cost & gradient of slacks
	struct blasfeo_svec *d; // inequality constraints vector
	struct blasfeo_svec *m; // rhs of complementarity condition
	struct blasfeo_svec *Z; // (diagonal) hessian of slacks
	int **idxb; // index of box constraints
	int **idxs; // index of soft constraints
	int memsize; // memory size in bytes
	};



//
int s_sizeof_ocp_qp();
//
int s_memsize_ocp_qp(struct s_ocp_qp_dim *dim);
//
void s_create_ocp_qp(struct s_ocp_qp_dim *dim, struct s_ocp_qp *qp, void *memory);
//
void s_change_bounds_dimensions_ocp_qp(int *nbu, int *nbx, struct s_ocp_qp *qp);
//
void s_cvt_colmaj_to_ocp_qp(float **A, float **B, float **b, float **Q, float **S, float **R, float **q, float **r, int **idxb, float **lb, float **ub, float **C, float **D, float **lg, float **ug, float **Zl, float **Zu, float **zl, float **zu, int **idxs, float **ls, float **us, struct s_ocp_qp *qp);
//
void s_cvt_rowmaj_to_ocp_qp(float **A, float **B, float **b, float **Q, float **S, float **R, float **q, float **r, int **idxb, float **lb, float **ub, float **C, float **D, float **lg, float **ug, float **Zl, float **Zu, float **zl, float **zu, int **idxs, float **ls, float **us, struct s_ocp_qp *qp);
//
void s_cvt_colmaj_to_ocp_qp_A(int stage, float *mat, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_A(int stage, struct s_ocp_qp *qp, float *mat);
//
void s_cvt_colmaj_to_ocp_qp_B(int stage, float *mat, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_B(int stage, struct s_ocp_qp *qp, float *mat);
//
void s_cvt_colmaj_to_ocp_qp_b(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_b(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_Q(int stage, float *mat, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_Q(int stage, struct s_ocp_qp *qp, float *mat);
//
void s_cvt_colmaj_to_ocp_qp_S(int stage, float *mat, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_S(int stage, struct s_ocp_qp *qp, float *mat);
//
void s_cvt_colmaj_to_ocp_qp_R(int stage, float *mat, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_R(int stage, struct s_ocp_qp *qp, float *mat);
//
void s_cvt_colmaj_to_ocp_qp_q(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_q(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_r(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_r(int stage, struct s_ocp_qp *qp, float *vec);
// TODO remove !!!
void s_cvt_colmaj_to_ocp_qp_lbx(int stage, float *vec, struct s_ocp_qp *qp);
// TODO remove !!!
void s_cvt_ocp_qp_to_colmaj_lbx(int stage, struct s_ocp_qp *qp, float *vec);
// TODO remove !!!
void s_cvt_colmaj_to_ocp_qp_ubx(int stage, float *vec, struct s_ocp_qp *qp);
// TODO remove !!!
void s_cvt_ocp_qp_to_colmaj_ubx(int stage, struct s_ocp_qp *qp, float *vec);
// TODO remove !!!
void s_cvt_colmaj_to_ocp_qp_lbu(int stage, float *vec, struct s_ocp_qp *qp);
// TODO remove !!!
void s_cvt_ocp_qp_to_colmaj_lbu(int stage, struct s_ocp_qp *qp, float *vec);
// TODO remove !!!
void s_cvt_colmaj_to_ocp_qp_ubu(int stage, float *vec, struct s_ocp_qp *qp);
// TODO remove !!!
void s_cvt_ocp_qp_to_colmaj_ubu(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_idxb(int stage, int *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_idxb(int stage, struct s_ocp_qp *qp, int *vec);
//
void s_cvt_colmaj_to_ocp_qp_lb(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_lb(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_ub(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_ub(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_C(int stage, float *mat, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_C(int stage, struct s_ocp_qp *qp, float *mat);
//
void s_cvt_colmaj_to_ocp_qp_D(int stage, float *mat, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_D(int stage, struct s_ocp_qp *qp, float *mat);
//
void s_cvt_colmaj_to_ocp_qp_lg(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_lg(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_ug(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_ug(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_ocp_qp_to_colmaj_Zl(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_ocp_qp_to_colmaj_zu(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_ocp_qp_to_colmaj_zl(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_ocp_qp_to_colmaj_Zu(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_idxs(int stage, int *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_idxs(int stage, struct s_ocp_qp *qp, int *vec);
//
void s_cvt_colmaj_to_ocp_qp_ls(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_ls(int stage, struct s_ocp_qp *qp, float *vec);
//
void s_cvt_colmaj_to_ocp_qp_us(int stage, float *vec, struct s_ocp_qp *qp);
//
void s_cvt_ocp_qp_to_colmaj_us(int stage, struct s_ocp_qp *qp, float *vec);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif // HPIPM_S_OCP_QP_H_
