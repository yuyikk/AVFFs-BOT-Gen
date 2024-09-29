#ifndef _PRIMARYGENERATOR_HH_
#define _PRIMARYGENERATOR_HH_
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "RootTree.hh"
#include <vector>
class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGenerator();
        ~PrimaryGenerator();
        virtual void GeneratePrimaries(G4Event *);
        void Clear();
        void RegisterTree(TTree *t);

    private:
        G4bool fRegistered = false;
        G4ParticleGun *fParticleGun;
        std::vector<short> fPDGID;
        std::vector<float> fPx;
        std::vector<float> fPy;
        std::vector<float> fPz;
        std::vector<float> fX;
        std::vector<float> fY;
        std::vector<float> fZ;
};
#endif