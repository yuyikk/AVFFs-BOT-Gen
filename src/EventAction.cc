#include "EventAction.hh"
#include "StandardHit.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"

#include "TROOT.h"
#include "TError.h"
#include "TObject.h"
#include "TTree.h"

#include "GlobalVars.hh"
#include "RootTree.hh"
#include "G4String.hh"
#include "G4Abla.hh"
#include "StandardDetectorSD.hh"

using std::array;
using std::vector;

EventAction::EventAction() : G4UserEventAction(), fEventID(0)
{
    // G4RunManager::GetRunManager()->SetPrintProgress(100);
    RegisterTree(gRootTree->GetTree());
}
void EventAction::BeginOfEventAction(const G4Event *event)
{
    fEventID = event->GetEventID();
    // G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    // G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    // G4String TOFName = "TOF/TOFCollection";
    // G4cout << "TOFName " << TOFName << G4endl;
    // fTOFId = sdManager->GetCollectionID(TOFName);
    // fTOFRowId.clear();
    // fTOFColumnId.clear();
    // fTOFLayerId.clear();
    // fTOFHitEdep.clear();
}
void EventAction::EndOfEventAction(const G4Event *event)
{
    // G4cout << "=>> End of Events!!!" << G4endl;
    // G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    // G4int totalHit = 0;
    // G4double totalEdep = 0.0;

    // G4HCofThisEvent *hitCollEvt = event->GetHCofThisEvent();
    // G4VHitsCollection *hitColl = hitCollEvt->GetHC(fTOFId);
    // size_t nhit = hitColl->GetSize();
    // // G4cout << "nHits: " << nhit << G4endl;
    // for (size_t i = 0; i < nhit; ++i)
    // {
    //     G4double Edep = 0.0;
    //     TOFHit *hit = static_cast<TOFHit *>(hitColl->GetHit(i));
    //     Edep = hit->GetEdep();
    //     // if (Edep <= 0.0)
    //     //     continue;
    //     if (Edep > 0.0)
    //     {
    //         fTOFRowId.push_back(hit->GetRowID());
    //         fTOFColumnId.push_back(hit->GetColumnID());
    //         fTOFLayerId.push_back(hit->GetLayerID());
    //         fTOFHitEdep.push_back(Edep);
    //     }
    //     totalHit++;
    //     totalEdep += Edep;
    // }
    // fTOFEdep = totalEdep;
    // G4int evtId = event->GetEventID();
    // if (nhit > 0)
    // {
    //     analysisManager->FillNtupleIColumn(0, evtId);
    //     analysisManager->FillNtupleDColumn(5, fTOFEdep);
    //     analysisManager->AddNtupleRow();
    //     // G4cout << "evtId: " << evtId << G4endl;
    //     // G4cout << "Edep/MeV: " << fTOFEdep / MeV << "\t" << fTOFEdep << G4endl;
    // }
    if (!StandardDetectorSD::hasHits)
    {
        return;
    }
    gRootTree->FillTree();
}
void EventAction::RegisterTree(TTree *tree)
{
    tree->Branch("EventID", &fEventID, "EventID/I");
}