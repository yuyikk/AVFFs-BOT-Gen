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
        int fN;
        std::vector<int> fPID;
        std::vector<int> fTID;
        std::vector<int> fPTID;
        std::vector<int> fDID;
        std::vector<int> fProcessID;
        std::vector<double> fX;
        std::vector<double> fY;
        std::vector<double> fZ;
        std::vector<double> fPx;
        std::vector<double> fPy;
        std::vector<double> fPz;
        std::vector<double> fTime;
        std::vector<double> fEdep;
        std::vector<double> fTrackL;
        std::vector<double> fVx;
        std::vector<double> fVy;
        std::vector<double> fVz;
        std::vector<double> fOutx;
        std::vector<double> fOuty;
        std::vector<double> fOutz;

};
#endif