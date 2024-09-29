#ifndef _EVENTACTION_HH_
#define _EVENTACTION_HH_
#include "Constants.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include <array>

class TTree;
class EventAction : public G4UserEventAction
{
    public:
        EventAction();
        ~EventAction() override = default;

        void BeginOfEventAction(const G4Event *) override;
        void EndOfEventAction(const G4Event *) override;
        // G4double GetTOFEdep() { return fTOFEdep; }
        // std::vector<G4int> &GetTOFRowIdVector() { return fTOFRowId; }
        // std::vector<G4int> &GetTOFColumnIdVector() { return fTOFColumnId; }
        // std::vector<G4int> &GetTOFLayerIdVector() { return fTOFLayerId; }
        // std::vector<G4double> &GetTOFHitEdep() { return fTOFHitEdep; }

    private:
        // std::vector<G4double> fTOFEdep;
        void RegisterTree(TTree *);
        // G4double fTOFEdep = 0.0;
        // G4int fTOFId = 1;
        G4int fEventID;
        G4String fCollName;
        // std::vector<G4double> fTOFHitEdep;
        // std::vector<G4int> fTOFRowId;
        // std::vector<G4int> fTOFColumnId;
        // std::vector<G4int> fTOFLayerId;
};
#endif