#ifndef _StandardDetectorSD_hh_
#define _StandardDetectorSD_hh_

#include "G4VSensitiveDetector.hh"
#include "StandardHit.hh"
#include "G4String.hh"
#include <vector>
class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class TTree;

class StandardDetectorSD : public G4VSensitiveDetector
{
    public:
        StandardDetectorSD(G4String name, G4String abbrev);
        virtual ~StandardDetectorSD();
        virtual void Initialize(G4HCofThisEvent *);
        virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
        virtual void EndOfEvent(G4HCofThisEvent *);
        // static int GetNhits() { return fNhits; }
        static G4bool hasHits;

    protected:
        virtual void RegisterTree(TTree *);
        virtual void Clear();
        G4int fID;
        G4String fAbbrev;
        StandardHitsCollection *fHitsCollection;
        G4bool fRegistered;
        short fN;
        std::vector<short> fPID;
        std::vector<short> fTID;
        std::vector<short> fPTID;
        std::vector<short> fDID;
        std::vector<short> fProcessID;
        std::vector<float> fX;
        std::vector<float> fY;
        std::vector<float> fZ;
        std::vector<float> fPx;
        std::vector<float> fPy;
        std::vector<float> fPz;
        std::vector<float> fTime;
        std::vector<float> fEdep;
        std::vector<float> fTrackL;
        std::vector<float> fVx;
        std::vector<float> fVy;
        std::vector<float> fVz;
        std::vector<float> fOutx;
        std::vector<float> fOuty;
        std::vector<float> fOutz;

};
#endif