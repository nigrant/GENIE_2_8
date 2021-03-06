//____________________________________________________________________________
/*
 Copyright (c) 2003-2013, GENIE Neutrino MC Generator Collaboration
 For the full text of the license visit http://copyright.genie-mc.org
 or see $GENIE/LICENSE

 Author: Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
         STFC, Rutherford Appleton Laboratory - October 01, 2004

 For the class documentation see the corresponding header file.

 Important revisions after version 2.0.0 :

*/
//____________________________________________________________________________

#include <string>
#include <sstream>
#include <vector>

#include <TROOT.h>
#include <TSystem.h>
#include <TFolder.h>
#include <TObjString.h>

#include "Conventions/EnvSnapshot.h"
#include "Messenger/Messenger.h"
#include "Ntuple/NtpMCJobEnv.h"

using std::string;
using std::ostringstream;
using std::vector;
using namespace genie;
using namespace genie::controls;

//____________________________________________________________________________
NtpMCJobEnv::NtpMCJobEnv()
{
  fEnv = 0;
}
//____________________________________________________________________________
NtpMCJobEnv::~NtpMCJobEnv()
{

}
//____________________________________________________________________________
TFolder * NtpMCJobEnv::TakeSnapshot(void)
{
  if (fEnv) delete fEnv;
  fEnv = 0;

  LOG("Ntp", pNOTICE)
      << "Taking environment snapshot and saving it in a TFolder";

  fEnv = gROOT->GetRootFolder()->AddFolder("genv","GENIE user environment");
  gROOT->GetListOfBrowsables()->Add(fEnv,"genv");

  fEnv->SetOwner(true);

  vector<string> envvars;

  unsigned int ivar=0;
  while(kMCEnv[ivar]) {
    envvars.push_back(string(kMCEnv[ivar]));
    ivar++;
  }

  LOG("Ntp", pINFO) << "** MC Job Environment:";
  vector<string>::iterator variter;
  for(variter = envvars.begin(); variter != envvars.end(); ++variter) {

     ostringstream entry;
     string var   = *variter;
     string value = (gSystem->Getenv(var.c_str()) ?
                               gSystem->Getenv(var.c_str()) : "UNDEFINED");

     LOG("Ntp", pINFO) << "$" << var << " ---> " << value;

     entry << "envv:" << var << ";value:" << value;
     fEnv->Add(new TObjString(entry.str().c_str()));
  }
  return fEnv;
}
//____________________________________________________________________________
