#include "GeneratorTree.hh"
#include "TTree.h"
#include "TFile.h"
GeneratorTree::GeneratorTree(TFile *f)
{
    fTree = (TTree *)f->Get("T");
    for (int i = 0; i < 5; ++i)
    {
        fPdgID[i] = -1;
        fPx[i] = 0;
        fPy[i] = 0;
        fPz[i] = 0;
    }
}
GeneratorTree::GeneratorTree(const char *filename)
{
    fFile = TFile::Open(filename);
    fTree = (TTree *)fFile->Get("T");
}
GeneratorTree::~GeneratorTree()
{
    fFile->Close();
}
void GeneratorTree::InitTree()
{
    fTree->SetBranchAddress("NParticle", &fNparticle);
    fTree->SetBranchAddress("Weight", &fWeight);
    fTree->SetBranchAddress("PID", fPdgID);
    fTree->SetBranchAddress("Px", fPx);
    fTree->SetBranchAddress("Py", fPy);
    fTree->SetBranchAddress("Pz", fPz);
}
int GeneratorTree::GetNumberOfEvent() const
{
    return fTree->GetEntries();
}
void GeneratorTree::GetEvent(const int &i) const
{
    fTree->GetEntry(i);
}
int GeneratorTree::GetPdgID(const int &idx) const
{
    if (idx > fMaxParticles)
    {
        return -999;
    }
    return fPdgID[idx]; 
}
double GeneratorTree::GetPx(const int &idx) const
{ 
    if (idx > fMaxParticles)
    {
        return -999;
    }
    return fPx[idx]; 
}
double GeneratorTree::GetPy(const int &idx) const
{
    if (idx > fMaxParticles)
    {
        return -999;
    } 
    return fPy[idx]; 
}
double GeneratorTree::GetPz(const int &idx) const
{
    if (idx > fMaxParticles)
    {
        return -999;
    } 
    return fPz[idx]; 
}
