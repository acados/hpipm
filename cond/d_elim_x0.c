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

#define GEMV_T_LIBSTR blasfeo_dgemv_t
#define ELIM_X0_WORKSPACE d_elim_x0_workspace
#define OCP_QP d_ocp_qp
#define ROWIN_LIBSTR blasfeo_drowin
#define SIZE_STRMAT blasfeo_memsize_dmat
#define SIZE_STRVEC blasfeo_memsize_dvec
#define STRMAT blasfeo_dmat
#define STRVEC blasfeo_dvec

#define MEMSIZE_ELIM_X0 d_memsize_elim_x0
#define CREATE_ELIM_X0 d_create_elim_x0
#define ELIM_X0 d_elim_x0


struct d_elim_x0_workspace{
	struct blasfeo_dmat *BAbt0_i;
	struct blasfeo_dvec *b0;
	struct blasfeo_dvec *ux0;
	int nx0;
	int memsize;
	};



void MEMSIZE_ELIM_X0(struct OCP_QP *qp)
	{

	int N = qp->N;
	int *nx = qp->nx;
	int *nu = qp->nu;

	int size = 0;

	size += 1*sizeof(struct STRMAT); // BAbt0_i
	size += 2*sizeof(struct STRVEC); // b0 x0

	size += SIZE_STRMAT(nu[0]+nx[0]+1, nx[1]); // BAbt0_i
	size += SIZE_STRVEC(nx[1]); // b0
	size += SIZE_STRVEC(nx[0]); // x0

	size = (size+63)/64*64; // make multiple of typical cache line size
	size += 1*64; // align once to typical cache line size

	return size;

	}



void CREATE_ELIM_X0(struct OCP_QP *qp, struct ELIM_X0_WORKSPACE *ws, void *mem)
	{

	int N = qp->N;
	int *nx = qp->nx;
	int *nu = qp->nu;

	// matrix struct
	struct STRMAT *sm_ptr = (struct STRMAT *) mem;

	//
	ws->BAbt0_i = sm_ptr;
	sm_ptr += 1;

	// vector struct
	struct STRVEC *sv_ptr = (struct STRVEC *) sm_ptr;

	//
	ws->b0 = sv_ptr;
	sv_ptr += 1;
	//
	ws->x0 = sv_ptr;
	sv_ptr += 1;

	// align to typicl cache line size
	size_t s_ptr = (size_t) sv_ptr;
	s_ptr = (s_ptr+63)/64*64;

	// void stuff
	char *c_ptr = (char *) s_ptr;

	//
	CREATE_STRMAT(nu[0]+nx[0]+1, nx[1], ws->BAbt0_i, c_ptr);
	c_ptr += ws->BAbt0_i->memsize;
	//
	CREATE_STRVEC(nx[1], ws->b0, c_ptr);
	c_ptr += ws->b0->memsize;
	//
	CREATE_STRVEC(nx[0], ws->x0, c_ptr);
	c_ptr += ws->x0->memsize;

	//
	ws->memsize = MEMSIZE_ELIM_X0(qp);

	return;

	}



void ELIM_X0(struct OCP_QP *qp, struct ELIM_X0_WORKSPACE *ws)
	{

	int ii;

	int N = qp->N;
	int *nx = qp->nx;

	struct STRMAT *BAbt0_i = ws->BAbt0_i;
	struct STRVEC *b0 = ws->b0;
	struct STRVEC *x0 = ws->x0;

	int idx_e;
	int idx_i;
	int nx0_tmp;

	// extract x0 from bounds // TODO soft constraint on initial state
	idx_e = 0;
	idx_i = 0;
	nx0_tmp = 0;
	for(ii=0; ii<nb[0]; ii++)
		{
		if(qp->d[0][ii]==qp->d[0][nb[0]+ng[0]+ii]) // equality constraint
			{
			GECP_LIBSTR(1, nx[1], qp->BAbt+0, qp->idxb[ii], 0, BAt0_e, idx_e, 0);
			x0[idx_e] = qp->d[0][ii];
			// TODO idxb, d, nb
			idx_e += 1;
			}
		else // inequality constraint
			{
			GECP_LIBSTR(1, nx[1], qp->BAbt+0, qp->idxb[ii], 0, BAbt0_i, idx_i, 0);
			// TODO idxb, d, nb
			idx_i += 1;
			}
		}

	GEMV_T_LIBSTR(idx_e, nx[1], 1.0, BAt0_e, 0, 0, x0, 0, 1.0, qp->b+0, 0, b0, 0);
	ROWIN_LIBSTR(nx[1], 1.0, b0, 0, BAbt0_i, idx_i, 0);

	// update nx TODO nu too !!!!!!!!!!!!!!
	ws->nx0 = nx[0];
	nx[0] = 0;

	return;

	}



void RECOV_X0(struct OCP_QP *qp, struct ELIM_X0_WORKSPACE *ws)
	{

	int N = qp->N;
	int *nx = qp->nx;

	nx[0] = ws->nx0;

	return;

	}
