#include "windows.h" /* for threading */
#include <tchar.h>
//#include <strsafe.h>

//#include "unistd.h" /* for sleep */
#include "mex.h"
#include "matrix.h"

DWORD WINAPI do_thread( LPVOID lpParam ) 
{
  int tid;
  tid = (int)lpParam;
  mexPrintf("In thread: just started thread #%d\n", tid);
  sleep(4000); /* pretend that the code is busy for a few seconds */
  mexPrintf("In thread: about to exit from thread #%d\n", tid);
  return 0;
}


void mexFunction (int nlhs, mxArray * plhs[], int nrhs, const mxArray * prhs[])
{
  #define num  10

//  if (nrhs<1)
//    mexErrMsgTxt("not enough input arguments");
//  else
//    num = mxGetScalar(prhs[0]);

  void *threads[num];
  void *ids[num];
  int t;
  for(t=0; t < num ; t++){
    mexPrintf("In main: creating thread %d\n", t);
     threads[t] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            do_thread,       // thread function name
            (void*)t,          // argument to thread function 
            0,                      // use default creation flags 
            &ids[t]);   // returns the thread identifier 
    
    //rc = pthread_create(&threads[t], NULL, do_thread, (void *)t);
    if (!threads[t]){
      mexErrMsgTxt("problem with return code from pthread_create()");
    }
    sleep(1); /* wait some time before making the next thread */
  }
  return;
}
