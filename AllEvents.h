//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 21 16:48:59 2014 by ROOT version 4.00/08
// from TTree h101/NeutrinoNtp
// found on file: fluxmodification.root
//////////////////////////////////////////////////////////

#ifndef AllEvents_h
#define AllEvents_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class AllEvents {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Float_t         beamwgt;
   Int_t           ntp;
   Int_t           npart;
   Int_t           id[20];   //[npart]
   Float_t         ini_pos[20][3];   //[npart]
   Float_t         ini_mom[20][3];   //[npart]
   Float_t         ini_eng[20];   //[npart]
   Float_t         ini_t[20];   //[npart]
   Float_t         fin_mom[20][3];   //[npart]
   Float_t         fin_pol[20][3];   //[npart]

   // List of branches
   TBranch        *b_beamwgt;   //!
   TBranch        *b_ntp;   //!
   TBranch        *b_npart;   //!
   TBranch        *b_id;   //!
   TBranch        *b_ini_pos;   //!
   TBranch        *b_ini_mom;   //!
   TBranch        *b_ini_eng;   //!
   TBranch        *b_ini_t;   //!
   TBranch        *b_fin_mom;   //!
   TBranch        *b_fin_pol;   //!

   AllEvents(TTree *tree=0);
   ~AllEvents();
   Int_t  Cut(Int_t entry);
   Int_t  GetEntry(Int_t entry);
   Int_t  LoadTree(Int_t entry);
   void   Init(TTree *tree);
   void   Loop();
   Bool_t Notify();
   void   Show(Int_t entry = -1);
};

#endif

#ifdef AllEvents_cxx
AllEvents::AllEvents(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("fluxmodification.root");
      if (!f) {
         f = new TFile("fluxmodification.root");
      }
      tree = (TTree*)gDirectory->Get("h101");

   }
   Init(tree);
}

AllEvents::~AllEvents()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t AllEvents::GetEntry(Int_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Int_t AllEvents::LoadTree(Int_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Int_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void AllEvents::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses of the tree
   // will be set. It is normaly not necessary to make changes to the
   // generated code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running with PROOF.

   // Set branch addresses
   if (tree == 0) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("beamwgt",&beamwgt);
   fChain->SetBranchAddress("ntp",&ntp);
   fChain->SetBranchAddress("npart",&npart);
   fChain->SetBranchAddress("id",id);
   fChain->SetBranchAddress("ini_pos",ini_pos);
   fChain->SetBranchAddress("ini_mom",ini_mom);
   fChain->SetBranchAddress("ini_eng",ini_eng);
   fChain->SetBranchAddress("ini_t",ini_t);
   fChain->SetBranchAddress("fin_mom",fin_mom);
   fChain->SetBranchAddress("fin_pol",fin_pol);
   Notify();
}

Bool_t AllEvents::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. Typically here the branch pointers
   // will be retrieved. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed.

   // Get branch pointers
   b_beamwgt = fChain->GetBranch("beamwgt");
   b_ntp = fChain->GetBranch("ntp");
   b_npart = fChain->GetBranch("npart");
   b_id = fChain->GetBranch("id");
   b_ini_pos = fChain->GetBranch("ini_pos");
   b_ini_mom = fChain->GetBranch("ini_mom");
   b_ini_eng = fChain->GetBranch("ini_eng");
   b_ini_t = fChain->GetBranch("ini_t");
   b_fin_mom = fChain->GetBranch("fin_mom");
   b_fin_pol = fChain->GetBranch("fin_pol");

   return kTRUE;
}

void AllEvents::Show(Int_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AllEvents::Cut(Int_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef AllEvents_cxx
