/******************************************************************************
 * class to support interaction with the FSL tools
 *
 * WARNING! platform specific code in here....
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-09-23
 *****************************************************************************/

#include"RtFSLInterface.h"
#include"RtExperiment.h"

#include"ace/Task.h"
#include"ace/Mutex.h"

// support class for farming jobs to threads
class SystemCommand : public ACE_Task_Base {
 public:

  SystemCommand(string _command) { 
    command = _command; 
    started = false;
    running = false;
    success = false;
  }

  // start the thread to run the command
  int svc() {
    started = true;

    // here is the system call!!
    if(system(NULL)) { // check for availability
      running = true;
      success = (system(command.c_str()) == 0);    
      running = false;
    }
    else {
      success = false;
      running = false;
    }

    return (success ? 0 : 1);
  }

  string getCommand() { return command; }
  bool   getStarted() { return started; }
  bool   getRunning() { return running; }
  bool   getSuccess() { return success; }

 private:  
  string command;
  bool   started;
  bool   running;
  bool   success;
};

// static declaration
map<unsigned int, SystemCommand*> RtFSLInterface::cmdMap;

// execute a command string in the system environment
FslJobID RtFSLInterface::runCommand(string command) {

  FslJobID ret = FSL_NO_SUCH_JOB;

  // if NOT windoze
#ifndef WIN32 
  
    // start the job

    ACE_Mutex mut;
    mut.acquire();

    FslJobID newID = RtFSLInterface::getNextJobID();
    SystemCommand *syscmd = new SystemCommand(command);    
    
    cmdMap[newID] = syscmd;
    syscmd->activate();
    ret = newID;

    mut.release();

#else
  
  cout << "RtFSLInterface::runCommand: fsl system calls are unavailable on windows" << endl;

#endif

  return ret;

}

// execute a command string in the system environment and block till finished
FslJobStatus RtFSLInterface::runCommandBlocking(string command) {

  FslJobStatus ret = FSL_JOB_ERROR;

  // if NOT windoze
#ifndef WIN32 
  
    // start the job
    SystemCommand *syscmd = new SystemCommand(command);    
    syscmd->svc();
    ret = (syscmd->getSuccess() ? FSL_JOB_FINISHED : FSL_JOB_ERROR);
#else
  
  cout << "RtFSLInterface::runCommand: fsl system calls are unavailable on windows" << endl;

#endif

  return ret;

}

// perform a same-subject registration of two EPI volumes
// in:
//  target  image that will be registered to
//  movable image that will be registered
//  xfmFile file to save the transformation in (if empty a temp file
//          is created) 
FslJobID RtFSLInterface::registerSameSubjEPI(string target, string movable, 
					     string xfmFile, bool block) {
  string command = getConfig().get("study:softwareDir"); 
    command 
      += string("/scripts/fsl_reg_subj_epi.sh ")
      + " -t " + target
      + " -m " + movable
      + " -s " + xfmFile;

  return (block ? 
	  RtFSLInterface::runCommandBlocking(command) :
	  RtFSLInterface::runCommand(command)
	  );
}

// apply a pre-computed transformation to a file
// in:
//  target  image that was registered to
//  movable image that was registered
//  input   image to be transformed
//  output  image filename to save to
//  xfmFile file to save the transformation in (if empty a temp file
//          is created) 
FslJobID RtFSLInterface::applyTransform(string target, string movable, 
					string input,  string output,
					string xfmFile, bool block) {
  string command = getConfig().get("study:softwareDir"); 
    
  command 
    += string("/scripts/fsl_reg_subj_epi.sh ")
    + " -t " + target
    + " -m " + movable
    + " -i " + input
    + " -o " + output
    + " -r " + xfmFile;

  return (block ? 
	  RtFSLInterface::runCommandBlocking(command) :
	  RtFSLInterface::runCommand(command)
	  );
}

// make a brain mask out of an epi volume
// in:
//  filename of the volume to base the mask off of
FslJobID RtFSLInterface::makeBrainMask(string brainVolume, 
				       string maskFilename,
				       string betOptions, 
				       bool block) {
  string command = "../scripts/make_bet_mask.sh "
    + brainVolume + " " 
    + maskFilename + " " 
    + betOptions;

  return (block ? 
	  RtFSLInterface::runCommandBlocking(command) :
	  RtFSLInterface::runCommand(command)
	  );
}

// run a block design analysis
// in:
//  filePrefix    full path plus file prefix of volumes to include
//  runNum        series number of the images to include
//  blockLength   number of seconds per block
//  numConditions number of stimulus conditions (including rest)
FslJobID RtFSLInterface::runAnalysis(string filePrefix, unsigned int runNum, 
				     float blockLength, 
				     unsigned int numConditions) {
  cout << "RtFSLInterface::runAnalysis() is not yet implemented" << endl;
  return cmdMap.size();
}

// get the next job id
FslJobID RtFSLInterface::getNextJobID() {
  return cmdMap.size();
}

// get a job status
FslJobStatus RtFSLInterface::getJobStatus(FslJobID jobID) {
  // check that the job exists
  if(jobID >= cmdMap.size()) {
    return FSL_NO_SUCH_JOB;
  }

  // get the job
  SystemCommand *cmd = cmdMap[jobID];
  if(cmd == NULL) {
    return FSL_NO_SUCH_JOB;
  }

  // find its status
  if(!cmd->getStarted() || cmd->getRunning()) {
    return FSL_JOB_RUNNING;
  }
  else if(cmd->getStarted() && !cmd->getRunning() && cmd->getSuccess()) {
    return FSL_JOB_FINISHED;
  }
  else if(cmd->getStarted() && !cmd->getRunning() && !cmd->getSuccess()) {
    return FSL_JOB_ERROR;
  }
  
  return FSL_JOB_UNKNOWN_STATE;
}


