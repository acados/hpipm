/**************************************************************************************************
*                                                                                                 *
* This file is part of HPIPM.                                                                     *
*                                                                                                 *
* HPIPM -- High Performance Interior Point Method.                                                *
* Copyright (C) 2017 by Gianluca Frison.                                                          *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* HPIPM is free software; you can redistribute it and/or                                          *
* modify it under the terms of the GNU Lesser General Public                                      *
* License as published by the Free Software Foundation; either                                    *
* version 2.1 of the License, or (at your option) any later version.                              *
*                                                                                                 *
* HPIPM is distributed in the hope that it will be useful,                                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                                  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                                            *
* See the GNU Lesser General Public License for more details.                                     *
*                                                                                                 *
* You should have received a copy of the GNU Lesser General Public                                *
* License along with HPIPM; if not, write to the Free Software                                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA                  *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/

#include "../include/hpipm_tree.h"



int MEMSIZE_TREE_OCP_QP_IPM_ARG(struct TREE_OCP_QP *qp)
	{

	return 0;

	}



void CREATE_TREE_OCP_QP_IPM_ARG(struct TREE_OCP_QP *qp, struct TREE_OCP_QP_IPM_ARG *arg, void *mem)
	{

	arg->memsize = 0;

	return;

	}



void SET_DEFAULT_TREE_OCP_QP_IPM_ARG(struct TREE_OCP_QP_IPM_ARG *arg)
	{

	arg->mu0 = 10;
	arg->alpha_min = 1e-12;
	arg->res_g_max = 1e-6;
	arg->res_b_max = 1e-8;
	arg->res_d_max = 1e-8;
	arg->res_m_max = 1e-8;
	arg->iter_max = 20;
	arg->stat_max = 20;
	arg->pred_corr = 1;
	arg->cond_pred_corr = 1;
	arg->warm_start = 0;

	return;

	}



int MEMSIZE_TREE_OCP_QP_IPM(struct TREE_OCP_QP *qp, struct TREE_OCP_QP_IPM_ARG *arg)
	{

	// loop index
	int ii;

	// extract ocp qp size
	int Nn = qp->dim->Nn;
	int *nx = qp->dim->nx;
	int *nu = qp->dim->nu;
	int *nb = qp->dim->nb;
	int *ng = qp->dim->ng;
	int *ns = qp->dim->ns;

	// compute core qp size and max size
	int nvt = 0;
	int net = 0;
	int nct = 0;
	int nxM = 0;
	int nuM = 0;
	int nbM = 0;
	int ngM = 0;
	int nsM = 0;
	for(ii=0; ii<Nn-1; ii++)
		{
		nvt += nx[ii]+nu[ii]+2*ns[ii];
		net += nx[ii+1];
		nct += 2*nb[ii]+2*ng[ii]+2*ns[ii];
		nxM = nx[ii]>nxM ? nx[ii] : nxM;
		nuM = nu[ii]>nuM ? nu[ii] : nuM;
		nbM = nb[ii]>nbM ? nb[ii] : nbM;
		ngM = ng[ii]>ngM ? ng[ii] : ngM;
		nsM = ns[ii]>nsM ? ns[ii] : nsM;
		}
	nvt += nx[ii]+nu[ii]+2*ns[ii];
	nct += 2*nb[ii]+2*ng[ii]+2*ns[ii];
	nxM = nx[ii]>nxM ? nx[ii] : nxM;
	nuM = nu[ii]>nuM ? nu[ii] : nuM;
	nbM = nb[ii]>nbM ? nb[ii] : nbM;
	ngM = ng[ii]>ngM ? ng[ii] : ngM;
	nsM = ns[ii]>nsM ? ns[ii] : nsM;

	int size = 0;

	size += 8*Nn*sizeof(struct STRVEC); // dux dt res_g res_d res_m Gamma gamma Zs_inv
	size += 3*(Nn-1)*sizeof(struct STRVEC); // dpi res_b Pb
	size += 6*sizeof(struct STRVEC); // tmp_nxM 4*tmp_nbgM

	size += 1*Nn*sizeof(struct STRMAT); // L
	size += 2*sizeof(struct STRMAT); // AL

	size += 1*SIZE_STRVEC(nxM); // tmp_nxM
	size += 4*SIZE_STRVEC(nbM+ngM); // tmp_nbgM
	size += 1*SIZE_STRVEC(nsM); // tmp_nsM
	for(ii=0; ii<Nn-1; ii++) size += 1*SIZE_STRVEC(nx[ii+1]); // Pb
	for(ii=0; ii<Nn; ii++) size += 1*SIZE_STRVEC(2*ns[ii]); // Zs_inv
	for(ii=0; ii<Nn; ii++) size += 1*SIZE_STRMAT(nu[ii]+nx[ii]+1, nu[ii]+nx[ii]); // L
	size += 2*SIZE_STRMAT(nuM+nxM+1, nxM+ngM); // AL

	size += 1*sizeof(struct CORE_QP_IPM_WORKSPACE);
	size += 1*MEMSIZE_CORE_QP_IPM(nvt, net, nct);

	size += 5*arg->stat_max*sizeof(REAL);

	size = (size+63)/64*64; // make multiple of typical cache line size
	size += 1*64; // align once to typical cache line size

	return size;

	}



void CREATE_TREE_OCP_QP_IPM(struct TREE_OCP_QP *qp, struct TREE_OCP_QP_IPM_ARG *arg, struct TREE_OCP_QP_IPM_WORKSPACE *workspace, void *mem)
	{

	// loop index
	int ii;

	// extract ocp qp size
	int Nn = qp->dim->Nn;
	int *nx = qp->dim->nx;
	int *nu = qp->dim->nu;
	int *nb = qp->dim->nb;
	int *ng = qp->dim->ng;
	int *ns = qp->dim->ns;


	// compute core qp size and max size
	int nvt = 0;
	int net = 0;
	int nct = 0;
	int nxM = 0;
	int nuM = 0;
	int nbM = 0;
	int ngM = 0;
	int nsM = 0;
	for(ii=0; ii<Nn-1; ii++)
		{
		nvt += nx[ii]+nu[ii]+2*ns[ii];
		net += nx[ii+1];
		nct += 2*nb[ii]+2*ng[ii]+2*ns[ii];
		nxM = nx[ii]>nxM ? nx[ii] : nxM;
		nuM = nu[ii]>nuM ? nu[ii] : nuM;
		nbM = nb[ii]>nbM ? nb[ii] : nbM;
		ngM = ng[ii]>ngM ? ng[ii] : ngM;
		nsM = ns[ii]>nsM ? ns[ii] : nsM;
		}
	nvt += nx[ii]+nu[ii]+2*ns[ii];
	nct += 2*nb[ii]+2*ng[ii]+2*ns[ii];
	nxM = nx[ii]>nxM ? nx[ii] : nxM;
	nuM = nu[ii]>nuM ? nu[ii] : nuM;
	nbM = nb[ii]>nbM ? nb[ii] : nbM;
	ngM = ng[ii]>ngM ? ng[ii] : ngM;
	nsM = ns[ii]>nsM ? ns[ii] : nsM;


	// core struct
	struct CORE_QP_IPM_WORKSPACE *sr_ptr = mem;

	// core workspace
	workspace->core_workspace = sr_ptr;
	sr_ptr += 1;
	struct CORE_QP_IPM_WORKSPACE *cws = workspace->core_workspace;


	// matrix struct
	struct STRMAT *sm_ptr = (struct STRMAT *) sr_ptr;

	workspace->L = sm_ptr;
	sm_ptr += Nn;
	workspace->AL = sm_ptr;
	sm_ptr += 2;


	// vector struct
	struct STRVEC *sv_ptr = (struct STRVEC *) sm_ptr;

	workspace->dux = sv_ptr;
	sv_ptr += Nn;
	workspace->dpi = sv_ptr;
	sv_ptr += Nn;
	workspace->dt = sv_ptr;
	sv_ptr += Nn;
	workspace->res_g = sv_ptr;
	sv_ptr += Nn;
	workspace->res_b = sv_ptr;
	sv_ptr += Nn;
	workspace->res_d = sv_ptr;
	sv_ptr += Nn;
	workspace->res_m = sv_ptr;
	sv_ptr += Nn;
	workspace->Gamma = sv_ptr;
	sv_ptr += Nn;
	workspace->gamma = sv_ptr;
	sv_ptr += Nn;
	workspace->Pb = sv_ptr;
	sv_ptr += Nn;
	workspace->Zs_inv = sv_ptr;
	sv_ptr += Nn;
	workspace->tmp_nxM = sv_ptr;
	sv_ptr += 1;
	workspace->tmp_nbgM = sv_ptr;
	sv_ptr += 4;
	workspace->tmp_nsM = sv_ptr;
	sv_ptr += 1;


	// double/float stuff
	REAL *d_ptr = (REAL *) sv_ptr;
	
	workspace->stat = d_ptr;
	d_ptr += 5*arg->stat_max;

	workspace->stat_max = arg->stat_max;
	workspace->warm_start = arg->warm_start;


	// align to typicl cache line size
	size_t s_ptr = (size_t) d_ptr;
	s_ptr = (s_ptr+63)/64*64;


	// void stuf
	char *c_ptr = (char *) s_ptr;

	for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRMAT(nu[ii]+nx[ii]+1, nu[ii]+nx[ii], workspace->L+ii, c_ptr);
		c_ptr += (workspace->L+ii)->memsize;
		}

	CREATE_STRMAT(nuM+nxM+1, nxM+ngM, workspace->AL+0, c_ptr);
	c_ptr += (workspace->AL+0)->memsize;

	CREATE_STRMAT(nuM+nxM+1, nxM+ngM, workspace->AL+1, c_ptr);
	c_ptr += (workspace->AL+1)->memsize;

	for(ii=0; ii<Nn-1; ii++)
		{
		CREATE_STRVEC(nx[ii+1], workspace->Pb+ii, c_ptr);
		c_ptr += (workspace->Pb+ii)->memsize;
		}

		for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRVEC(2*ns[ii], workspace->Zs_inv+ii, c_ptr);
		c_ptr += (workspace->Zs_inv+ii)->memsize;
		}

	CREATE_STRVEC(nxM, workspace->tmp_nxM, c_ptr);
	c_ptr += workspace->tmp_nxM->memsize;

	CREATE_STRVEC(nbM+ngM, workspace->tmp_nbgM+0, c_ptr);
	c_ptr += (workspace->tmp_nbgM+0)->memsize;

	CREATE_STRVEC(nbM+ngM, workspace->tmp_nbgM+1, c_ptr);
	c_ptr += (workspace->tmp_nbgM+1)->memsize;

	CREATE_STRVEC(nbM+ngM, workspace->tmp_nbgM+2, c_ptr);
	c_ptr += (workspace->tmp_nbgM+2)->memsize;

	CREATE_STRVEC(nbM+ngM, workspace->tmp_nbgM+3, c_ptr);
	c_ptr += (workspace->tmp_nbgM+3)->memsize;

	CREATE_STRVEC(nsM, workspace->tmp_nsM+0, c_ptr);
	c_ptr += (workspace->tmp_nsM+0)->memsize;

	CREATE_CORE_QP_IPM(nvt, net, nct, cws, c_ptr);
	c_ptr += workspace->core_workspace->memsize;


	// alias members of workspace and core_workspace
	//
	c_ptr = (char *) cws->dv;
	for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRVEC(nu[ii]+nx[ii], workspace->dux+ii, c_ptr);
		c_ptr += (nu[ii]+nx[ii])*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		}
	//
	c_ptr = (char *) cws->dpi;
	for(ii=0; ii<Nn-1; ii++)
		{
		CREATE_STRVEC(nx[ii+1], workspace->dpi+ii, c_ptr);
		c_ptr += (nx[ii+1])*sizeof(REAL);
		}
	//
	c_ptr = (char *) cws->dt;
	for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRVEC(2*nb[ii]+2*ng[ii]+2*ns[ii], workspace->dt+ii, c_ptr);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		}
	//
	c_ptr = (char *) cws->res_g;
	for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRVEC(nu[ii]+nx[ii], workspace->res_g+ii, c_ptr);
		c_ptr += (nu[ii]+nx[ii])*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		}
	//
	c_ptr = (char *) cws->res_b;
	for(ii=0; ii<Nn-1; ii++)
		{
		CREATE_STRVEC(nx[ii+1], workspace->res_b+ii, c_ptr);
		c_ptr += (nx[ii+1])*sizeof(REAL);
		}
	//
	c_ptr = (char *) cws->res_d;
	for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRVEC(2*nb[ii]+2*ng[ii]+2*ns[ii], workspace->res_d+ii, c_ptr);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		}
	//
	c_ptr = (char *) cws->res_m;
	for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRVEC(2*nb[ii]+2*ng[ii]+2*ns[ii], workspace->res_m+ii, c_ptr);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		}
	//
	c_ptr = (char *) cws->Gamma;
	for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRVEC(2*nb[ii]+2*ng[ii]+2*ns[ii], workspace->Gamma+ii, c_ptr);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		}
	//
	c_ptr = (char *) cws->gamma;
	for(ii=0; ii<Nn; ii++)
		{
		CREATE_STRVEC(2*nb[ii]+2*ng[ii]+2*ns[ii], workspace->gamma+ii, c_ptr);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += nb[ii]*sizeof(REAL);
		c_ptr += ng[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		c_ptr += ns[ii]*sizeof(REAL);
		}


	workspace->memsize = MEMSIZE_TREE_OCP_QP_IPM(qp, arg);


#if defined(RUNTIME_CHECKS)
	if(c_ptr > ((char *) mem) + workspace->memsize)
		{
		printf("\nCreate_tree_ocp_qp_ipm: outsize memory bounds!\n\n");
		exit(1);
		}
#endif


	return;

	}



int SOLVE_TREE_OCP_QP_IPM(struct TREE_OCP_QP *qp, struct TREE_OCP_QP_SOL *qp_sol, struct TREE_OCP_QP_IPM_ARG *arg, struct TREE_OCP_QP_IPM_WORKSPACE *ws)
	{

	struct CORE_QP_IPM_WORKSPACE *cws = ws->core_workspace;

	// alias qp vectors into qp_sol
	cws->v = qp_sol->ux->pa;
	cws->pi = qp_sol->pi->pa;
	cws->lam = qp_sol->lam->pa;
	cws->t = qp_sol->t->pa;

	// no constraints
	if(cws->nc==0)
		{
		FACT_SOLVE_KKT_UNCONSTR_TREE_OCP_QP(qp, qp_sol, ws);
		COMPUTE_RES_TREE_OCP_QP(qp, qp_sol, ws);
		cws->mu = ws->res_mu;
		ws->iter = 0;
		return 0;
		}

	// blasfeo alias for residuals
	struct STRVEC str_res_g;
	struct STRVEC str_res_b;
	struct STRVEC str_res_d;
	struct STRVEC str_res_m;
	str_res_g.m = cws->nv;
	str_res_b.m = cws->ne;
	str_res_d.m = cws->nc;
	str_res_m.m = cws->nc;
	str_res_g.pa = cws->res_g;
	str_res_b.pa = cws->res_b;
	str_res_d.pa = cws->res_d;
	str_res_m.pa = cws->res_m;

	REAL *qp_res = ws->qp_res;
	qp_res[0] = 0;
	qp_res[1] = 0;
	qp_res[2] = 0;
	qp_res[3] = 0;

	ws->mu0 = arg->mu0;

	int kk;
	REAL tmp;
	REAL mu_aff0;

	// init solver
	INIT_VAR_TREE_OCP_QP(qp, qp_sol, ws);

	// compute residuals
	COMPUTE_RES_TREE_OCP_QP(qp, qp_sol, ws);
	BACKUP_RES_M(cws);
	cws->mu = ws->res_mu;

	cws->alpha = 1.0;

	// compute infinity norm of residuals
	VECNRM_INF_LIBSTR(cws->nv, &str_res_g, 0, &qp_res[0]);
	VECNRM_INF_LIBSTR(cws->ne, &str_res_b, 0, &qp_res[1]);
	VECNRM_INF_LIBSTR(cws->nc, &str_res_d, 0, &qp_res[2]);
	VECNRM_INF_LIBSTR(cws->nc, &str_res_m, 0, &qp_res[3]);

	for(kk=0; \
			kk<arg->iter_max & \
			cws->alpha>arg->alpha_min & \
			(qp_res[0]>arg->res_g_max | \
			qp_res[1]>arg->res_b_max | \
			qp_res[2]>arg->res_d_max | \
			qp_res[3]>arg->res_m_max); kk++)
		{

		// fact and solve kkt
		FACT_SOLVE_KKT_STEP_TREE_OCP_QP(qp, ws);

		// alpha
		COMPUTE_ALPHA_QP(cws);
		if(kk<ws->stat_max)
			ws->stat[5*kk+0] = cws->alpha;

		// Mehrotra's predictor-corrector
		if(arg->pred_corr==1)
			{
			// mu_aff
			COMPUTE_MU_AFF_QP(cws);
			if(kk<ws->stat_max)
				ws->stat[5*kk+1] = cws->mu_aff;

			tmp = cws->mu_aff/cws->mu;
			cws->sigma = tmp*tmp*tmp;
			if(kk<ws->stat_max)
				ws->stat[5*kk+2] = cws->sigma;

			COMPUTE_CENTERING_CORRECTION_QP(cws);

			// fact and solve kkt
			SOLVE_KKT_STEP_TREE_OCP_QP(qp, ws);

			// alpha
			COMPUTE_ALPHA_QP(cws);
			if(kk<ws->stat_max)
				ws->stat[5*kk+3] = cws->alpha;

			// conditional Mehrotra's predictor-corrector
			if(arg->cond_pred_corr==1)
				{

				// save mu_aff (from prediction step)
				mu_aff0 = cws->mu_aff;

				// compute mu for predictor-corrector-centering
				COMPUTE_MU_AFF_QP(cws);

//				if(cws->mu_aff > 2.0*cws->mu)
				if(cws->mu_aff > 2.0*mu_aff0)
					{

					// centering direction
					COMPUTE_CENTERING_QP(cws);

					// solve kkt
					SOLVE_KKT_STEP_TREE_OCP_QP(qp, ws);

					// alpha
					COMPUTE_ALPHA_QP(cws);
					if(kk<ws->stat_max)
						ws->stat[5*kk+3] = cws->alpha;

					}

				}

			}

		//
		UPDATE_VAR_QP(cws);

		// compute residuals
		COMPUTE_RES_TREE_OCP_QP(qp, qp_sol, ws);
		BACKUP_RES_M(cws);
		cws->mu = ws->res_mu;
		if(kk<ws->stat_max)
			ws->stat[5*kk+4] = ws->res_mu;

		// compute infinity norm of residuals
		VECNRM_INF_LIBSTR(cws->nv, &str_res_g, 0, &qp_res[0]);
		VECNRM_INF_LIBSTR(cws->ne, &str_res_b, 0, &qp_res[1]);
		VECNRM_INF_LIBSTR(cws->nc, &str_res_d, 0, &qp_res[2]);
		VECNRM_INF_LIBSTR(cws->nc, &str_res_m, 0, &qp_res[3]);

		}
	
	ws->iter = kk;
	
	// max iteration number reached
	if(kk == arg->iter_max)
		return 1;

	// min step lenght
	if(cws->alpha <= arg->alpha_min)
		return 2;

	// NaN in the solution
#ifdef USE_C99_MATH
	if(isnan(cws->mu))
		return 3;
#else
	if(cws->mu != cws->mu)
		return 3;
#endif

	// normal return
	return 0;

	}


