/***********************************************************************/
/*                                                                     */
/*   svm_struct_learn_custom.c (instantiated for SVM-perform)          */
/*                                                                     */
/*   Allows implementing a custom/alternate algorithm for solving      */
/*   the structual SVM optimization problem. The algorithm can use     */ 
/*   full access to the SVM-struct API and to SVM-light.               */
/*                                                                     */
/*   Author: Thorsten Joachims                                         */
/*   Date: 09.01.08                                                    */
/*                                                                     */
/*   Copyright (c) 2008  Thorsten Joachims - All rights reserved       */
/*                                                                     */
/*   This software is available for non-commercial use only. It must   */
/*   not be modified and distributed without prior permission of the   */
/*   author. The author is not responsible for implications from the   */
/*   use of this software.                                             */
/*                                                                     */
/***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svm_struct_api.h"
#include "svm_light/svm_common.h"
#include "svm_struct/svm_struct_common.h"
#include "svm_struct/svm_struct_learn.h"

#define MAX(x,y)      ((x) < (y) ? (y) : (x))
#define MIN(x,y)      ((x) > (y) ? (y) : (x))
#define SIGN(x)       ((x) > (0) ? (1) : (((x) < (0) ? (-1) : (0))))


void svm_learn_struct_joint_custom(SAMPLE sample, STRUCT_LEARN_PARM *sparm,
				   LEARN_PARM *lparm, KERNEL_PARM *kparm, 
				   STRUCTMODEL *sm)
{
  int         i,j;
  int         numIt=0;
  long        argmax_count=0;
  long        totconstraints=0;
  double      epsilon,epsilon_shrink;
  double      lossval,factor,dist;
  double      margin=0;
  double      slack, slacksum, slacksum_full, ceps=99999;
  double      dualitygap,modellength,alphasum;
  long        sizePsi;
  double      *alpha=NULL;
  long        *alphahist=NULL,optcount=0;
  CONSTSET    cset;
  SVECTOR     *diff=NULL;
  double      *diff_n=NULL;
  SVECTOR     *lhs;
  MODEL       *svmModel=NULL;
  DOC         *doc;

  EXAMPLE     *ex=sample.examples;
  double      rt_total=0,rt_opt=0,rt_init=0,rt_psi=0,rt_viol=0,rt_kernel=0;
  double      rt1=0,rt2=0;

  double      score,diffscore,class,*class_old=NULL,*margin_old=NULL;
  double      *diffval=NULL,*diffvar=NULL,*diffmean=NULL,*diffconf=NULL;
  long        fullround=0,checkcount,flipcount;

  DOC         **x=(ex[0].x.doc);
  double      *y=(ex[0].y.class);
  long        n=ex[0].y.totdoc;

  rt1=get_runtime();

  init_struct_model(sample,sm,sparm,lparm,kparm); 
  sizePsi=sm->sizePsi+1;          /* sm must contain size of psi on return */

  if(sparm->slack_norm == 1) {
    lparm->svm_c=sparm->C;          /* set upper bound C */
    lparm->sharedslack=1;
  }
  else if(sparm->slack_norm == 2) {
    printf("ERROR: The joint algorithm does not apply to L2 slack norm!"); 
    fflush(stdout);
    exit(0); 
  }
  else {
    printf("ERROR: Slack norm must be L1 or L2!"); fflush(stdout);
    exit(0);
  }
  if(sparm->loss_function != ERRORRATE) {
    printf("ERROR: The custom algorithm can only optimize errorrate as the loss\n"); 
    printf("       function. Please use algorithm '-w 3' instead.\n"); 
    fflush(stdout);
    exit(1);
  }
  if(kparm->kernel_type != LINEAR) {
    printf("ERROR: Non-linear kernels are not supported in this algorithm!\n"); 
    printf("       Please use algorithm '-w 3' instead.\n"); 
    fflush(stdout);
    exit(1);
  }

  lparm->biased_hyperplane=0;     /* set threshold to zero */
  epsilon=100.0;                  /* start with low precision and
				     increase later */
  epsilon_shrink=epsilon;

  cset=init_struct_constraints(sample, sm, sparm);
  if(cset.m > 0) {
    alpha=(double *)realloc(alpha,sizeof(double)*cset.m);
    alphahist=(long *)realloc(alphahist,sizeof(long)*cset.m);
    for(i=0; i<cset.m; i++) {
      alpha[i]=0;
      alphahist[i]=-1; /* -1 makes sure these constraints are never removed */
    }
  }
  kparm->gram_matrix=init_kernel_matrix(&cset,kparm);

  /* set initial model and slack variables */
  svmModel=(MODEL *)my_malloc(sizeof(MODEL));
  lparm->epsilon_crit=epsilon;
  svm_learn_optimization(cset.lhs,cset.rhs,cset.m,sizePsi+n,
			 lparm,kparm,NULL,svmModel,alpha);
  add_weight_vector_to_linear_model(svmModel);
  sm->svm_model=svmModel;
  sm->w=svmModel->lin_weights; /* short cut to weight vector */

  diff_n=create_nvector(sm->sizePsi);
  clear_nvector(diff_n,sm->sizePsi);
  class_old=(double *)my_malloc(sizeof(double)*(n));
  margin_old=(double *)my_malloc(sizeof(double)*(n));
  diffval=(double *)my_malloc(sizeof(double)*(n));
  diffvar=(double *)my_malloc(sizeof(double)*(n));
  diffmean=(double *)my_malloc(sizeof(double)*(n));
  diffconf=(double *)my_malloc(sizeof(double)*(n));
  for(i=0; i<n; i++) {
    class_old[i]=y[i];
    margin_old[i]=0;
    diffval[i]=0;
    diffvar[i]=0;
    diffmean[i]=100.0/n;
    diffconf[i]=-1;
  }
  margin=0;
  flipcount=n;

  rt_init+=MAX(get_runtime()-rt1,0);
  rt_total+=rt_init;

    /*****************/
   /*** main loop ***/
  /*****************/
  do { /* iteratively find and add constraints to working set */

      if(struct_verbosity>=1) { 
	printf("Iter %i: ",++numIt); 
	fflush(stdout);
      }
      
      rt1=get_runtime();

      /* Should the algorithm test all examples in this iteration? */
      fullround=(epsilon_shrink < (epsilon/5.0))
	         || (numIt < 5) 
	         || (ceps < sparm->epsilon)
	         || (!sparm->shrinking);

      /**** compute current slack ****/
      alphasum=0;
      for(j=0;(j<cset.m);j++) 
	  alphasum+=alpha[j];
      for(j=0,slack=0;(j<cset.m) && (slack==0);j++)  
	if(alpha[j] > alphasum/cset.m)
	  slack=MAX(0,cset.rhs[j]-classify_example(svmModel,cset.lhs[j]));
      
      /**** find a violated joint constraint ****/
      lhs=NULL;
      dist=0;
      flipcount=0;
      checkcount=0;

      if(struct_verbosity >= 2) rt2=get_runtime();
	
      /**** find most violated joint constraint ***/
      for(i=0; i<n; i++) {
	  	
	if(fullround || (diffconf[i] < 0)) {
	  argmax_count++;
	  checkcount++;
	  score=classify_example(svmModel,x[i]);
	  diffscore=100.0/n-2.0*y[i]*score;
	  if(diffscore > 0) 
	    class=-y[i];
	  else 
	    class=y[i];
	  
	  /**** scale feature vector for margin rescaling ****/
	  factor=-(y[i]-class_old[i])+(y[i]-class);
	  
	  /**** add current fy-fybar to constraint ****/
	  if(factor != 0) {
	    flipcount++;
	    add_vector_ns(diff_n,x[i]->fvec,factor); 
	  }

	  /**** add loss to rhs ****/
	  if(y[i] == class)                         
	    lossval=0;
	  else
	    lossval=100;
	  margin+=lossval/n-margin_old[i];           
	  class_old[i]=class;
	  margin_old[i]=lossval/n;

	  /**** keep statistics for mini-batches ****/
	  diffvar[i]=0.5*diffvar[i]+0.5*pow(diffmean[i]-diffscore,2.0);
	  diffmean[i]=0.5*diffmean[i]+0.5*diffscore;
	  diffconf[i]=fabs(diffmean[i])-1.0*sqrt(diffvar[i]);
	  diffval[i]=diffscore;
	}

      } /* end of example loop */

      if(struct_verbosity>=2) {
	printf("%5.1f%% flipped / %5.1f%% checked ",100.0*flipcount/n,
	                                            100.0*checkcount/n);
	rt_viol+=MAX(get_runtime()-rt2,0);
	rt2=get_runtime();
      }

      /* create sparse vector from dense sum and ignore rounding errors*/
      for(i=0;i<=sm->sizePsi;i++) 
	if(fabs(diff_n[i]) < 0.00000000000000001) 
	  diff_n[i]=0;
      diff=create_svector_n(diff_n,sm->sizePsi,"",1.0);
      
      /**** if `error', then add constraint and recompute QP ****/
      doc=create_example(cset.m,0,1,1,diff);
      dist=classify_example(svmModel,doc);
      free_example(doc,0);
      ceps=MAX(0,margin-dist-slack);
      if(struct_verbosity>=2) rt_psi+=MAX(get_runtime()-rt2,0);	
      if(ceps > sparm->epsilon) { 
	/**** resize constraint matrix and add new constraint ****/
	if(struct_verbosity>=2) rt2=get_runtime();
	cset.lhs=(DOC **)realloc(cset.lhs,sizeof(DOC *)*(cset.m+1));
	cset.lhs[cset.m]=create_example(cset.m,0,1,1,diff);
	cset.rhs=(double *)realloc(cset.rhs,sizeof(double)*(cset.m+1));
	cset.rhs[cset.m]=margin;
	alpha=(double *)realloc(alpha,sizeof(double)*(cset.m+1));
	alpha[cset.m]=0;
	alphahist=(long *)realloc(alphahist,sizeof(long)*(cset.m+1));
	alphahist[cset.m]=optcount;
	cset.m++;
	totconstraints++;
	if(struct_verbosity>=2) rt_psi+=MAX(get_runtime()-rt2,0);	

	if(struct_verbosity>=1) {
	  printf(":");fflush(stdout);
	}
	if(struct_verbosity>=2) rt2=get_runtime();
	kparm->gram_matrix=update_kernel_matrix(kparm->gram_matrix,cset.m-1,
						&cset,kparm);
	if(struct_verbosity>=2) rt_kernel+=MAX(get_runtime()-rt2,0);
	
	/**** get new QP solution ****/
	if(struct_verbosity>=1) {
	  printf("*");fflush(stdout);
	}
	if(struct_verbosity>=2) rt2=get_runtime();
	/* set svm precision so that higher than eps of most violated constr */
	if(!fullround) {
	  epsilon_shrink=MIN(epsilon_shrink,MAX(ceps,sparm->epsilon)); 
	  lparm->epsilon_crit=epsilon_shrink/2.0; 
	}
	else {
	  epsilon=MIN(epsilon,MAX(ceps,sparm->epsilon)); /* best eps so far */
	  lparm->epsilon_crit=epsilon/2.0; 
	  epsilon_shrink=epsilon;
	}
	/* Run the QP solver on cset. */
	free_model(svmModel,0);
	svmModel=(MODEL *)my_malloc(sizeof(MODEL));
	kparm->kernel_type=GRAM; /* use kernel stored in kparm */
	svm_learn_optimization(cset.lhs,cset.rhs,cset.m,sizePsi,
			       lparm,kparm,NULL,svmModel,alpha);
	kparm->kernel_type=LINEAR; 
	svmModel->kernel_parm.kernel_type=LINEAR;
	add_weight_vector_to_linear_model(svmModel);
	sm->svm_model=svmModel;
	sm->w=svmModel->lin_weights; /* short cut to weight vector */
	optcount++;
	/* keep track of when each constraint was last
	   active. constraints marked with -1 are not updated */
	for(j=0;j<cset.m;j++) 
	  if((alphahist[j]>-1) && (alpha[j] != 0))  
	    alphahist[j]=optcount;
	if(struct_verbosity>=2) rt_opt+=MAX(get_runtime()-rt2,0);
	
	/* Check if some of the linear constraints have not been
	   active in a while. Those constraints are then removed to
	   avoid bloating the working set beyond necessity. */
	if(struct_verbosity>=2)
	  printf("-");fflush(stdout);
	remove_inactive_constraints(&cset,alpha,optcount,alphahist,50);
      }
      else {
	free_svector(diff);
      }

      if(struct_verbosity>=1)
	printf("(NumConst=%d, SV=%ld, CEps=%.4f, QPEps=%.4f)\n",cset.m,
	       svmModel->sv_num-1,ceps,svmModel->maxdiff);
	
      rt_total+=MAX(get_runtime()-rt1,0);

  } while((ceps > sparm->epsilon) || (!fullround) ||
	  finalize_iteration(ceps,0,sample,sm,cset,alpha,sparm)
	 );
  

  if(struct_verbosity>=1) {
    printf("Final epsilon on KKT-Conditions: %.5f\n",
	   MAX(svmModel->maxdiff,ceps));

    /**** compute sum of slacks ****/
    /**** WARNING: If positivity constraints are used, then the
	  maximum slack id is larger than what is allocated
	  below ****/
    slacksum=0;
    for(j=0;j<cset.m;j++) 
      slacksum=MAX(slacksum,
		   cset.rhs[j]-classify_example(svmModel,cset.lhs[j]));
    slacksum_full=0;
    for(j=0;j<n;j++) 
      slacksum_full+=MAX(0,diffval[j]);

    alphasum=0;
    for(i=0; i<cset.m; i++)  
      alphasum+=alpha[i]*cset.rhs[i];
    modellength=model_length_s(svmModel);
    dualitygap=(0.5*modellength*modellength+sparm->C*(slacksum+ceps))
               -(alphasum-0.5*modellength*modellength);
    
    printf("Upper bound on duality gap: %.5f\n", dualitygap);
    printf("Dual objective value: dval=%.5f\n",
	    alphasum-0.5*modellength*modellength);
    printf("Primal objective value: pval=%.5f\n",
	    0.5*modellength*modellength+sparm->C*slacksum_full);
    printf("Total number of constraints in final working set: %i (of %i)\n",(int)cset.m,(int)totconstraints);
    printf("Number of iterations: %d\n",numIt);
    printf("Number of calls to 'find_most_violated_constraint': %ld\n",argmax_count);
    if(sparm->slack_norm == 1) {
      printf("Number of SV: %ld \n",svmModel->sv_num-1);
      printf("Norm of weight vector: |w|=%.5f\n",
	     model_length_s(svmModel));
    }
    else if(sparm->slack_norm == 2){ 
      printf("Number of SV: %ld (including %ld at upper bound)\n",
	     svmModel->sv_num-1,svmModel->at_upper_bound);
      printf("Norm of weight vector (including L2-loss): |w|=%.5f\n",
	     model_length_s(svmModel));
    }
    printf("Value of slack variable (on working set): xi=%.5f\n",slacksum);
    printf("Norm of longest difference vector: ||Psi(x,y)-Psi(x,ybar)||=%.5f\n",
	   length_of_longest_document_vector(cset.lhs,cset.m,kparm));
    if(struct_verbosity>=2) 
      printf("Runtime in cpu-seconds: %.2f (%.2f%% for QP, %.2f%% for kernel, %.2f%% for Argmax, %.2f%% for Psi, %.2f%% for init)\n",
	   rt_total/100.0, (100.0*rt_opt)/rt_total, (100.0*rt_kernel)/rt_total,
	   (100.0*rt_viol)/rt_total, (100.0*rt_psi)/rt_total, 
	   (100.0*rt_init)/rt_total);
    else if(struct_verbosity==1) 
      printf("Runtime in cpu-seconds: %.2f\n",rt_total/100.0);
  }
  if(struct_verbosity>=4)
    printW(sm->w,sizePsi,n,lparm->svm_c);

  if(svmModel) {
    sm->svm_model=copy_model(svmModel);
    sm->w=sm->svm_model->lin_weights; /* short cut to weight vector */
  }

  print_struct_learning_stats(sample,sm,cset,alpha,sparm);

  free(diff_n);
  free(class_old);
  free(margin_old);
  free(diffval);
  free(diffvar);
  free(diffmean);
  free(diffconf);
  if(svmModel)
    free_model(svmModel,0);
  free(alpha); 
  free(alphahist); 
  free(cset.rhs); 
  for(i=0;i<cset.m;i++) 
    free_example(cset.lhs[i],1);
  free(cset.lhs);
  if(kparm->gram_matrix)
    free_matrix(kparm->gram_matrix);
}

