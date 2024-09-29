#ifndef _GENERATORTREE_HH_
#define _GENERATORTREE_HH_
#include "TFile.h"
#include "TTree.h"
class GeneratorTree
{
    public:
        GeneratorTree(TFile *f);
        GeneratorTree(const char *filename);
        virtual ~GeneratorTree();
        void InitTree();
        inline TTree *GetTree() const { return fTree; }
        inline int GetNumberOfParticle() const {return fNparticle;}
        int GetNumberOfEvent() const;
        void GetEvent(const int &i) const;

        inline double GetWeight() const { return fWeight; }
        int GetPdgID(const int &idx) const;
        double GetPx(const int &idx) const;
        double GetPy(const int &idx) const;
        double GetPz(const int &idx) const;

    private:
        TFile *fFile = nullptr;
        TTree *fTree = nullptr;
        const UShort_t fMaxParticles = 100;
        UShort_t fNparticle = 0;
        Double_t fWeight = 1.0;
        Int_t fPdgID[100];
        Double_t fPx[100];
        Double_t fPy[100];
        Double_t fPz[100];
};
#endif