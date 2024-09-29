#include "TFile.h"
#include "TTree.h"

const unsigned short MAXPAR = 100;

UShort_t NParticle;
Double_t Weight;
Int_t    PID[MAXPAR];
Double_t Px[MAXPAR];
Double_t Py[MAXPAR];
Double_t Pz[MAXPAR];

void InitTree(TTree* t)
{
    t->Branch("NParticle", &NParticle);
    t->Branch("Weight",    &Weight);
    
    t->Branch("PID",       &PID[0],  "PID[NParticle]/I");
    t->Branch("Px",        &Px[0],   "Px[NParticle]/D" );
    t->Branch("Py",        &Py[0],   "Py[NParticle]/D" );
    t->Branch("Pz",        &Pz[0],   "Pz[NParticle]/D" );
}


