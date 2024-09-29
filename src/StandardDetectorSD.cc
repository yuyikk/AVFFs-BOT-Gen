#include "StandardDetectorSD.hh"
#include "GlobalVars.hh"
#include "RootTree.hh"
#include "StandardHit.hh"
#include "TrackInformation.hh"

#include "TROOT.h"
#include "TError.h"
#include "TObject.h"
#include "TTree.h"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHandle.hh"
#include "G4Track.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include <string>
#include "Constants.hh"
G4bool StandardDetectorSD::hasHits = false;
StandardDetectorSD::StandardDetectorSD(G4String name, G4String abbrev) : G4VSensitiveDetector(name), fAbbrev(abbrev), fHitsCollection(NULL), fRegistered(false)
{
    std::string n = name;
    std::hash<std::string> hash;

    fID = hash(n) % 1000000;
    G4String cname = "StandardSDColl_" + name;
    cname = fAbbrev + cname;
    collectionName.insert(cname);
    fN = 0;
}
StandardDetectorSD::~StandardDetectorSD()
{
}

void StandardDetectorSD::Initialize(G4HCofThisEvent *HCE)
{
    if (!fRegistered)
    {
        RegisterTree(gRootTree->GetTree());
        fRegistered = true;
    }
    fHitsCollection = new StandardHitsCollection(SensitiveDetectorName, collectionName[0]);

    G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    HCE->AddHitsCollection(HCID, fHitsCollection);
    hasHits = false;
    // for (int i = 0; i < MyTOF::kNofModules; ++i)
    // {
    //     fHitsCollection->insert(new StandardHit());
    // }
    Clear();
}

G4bool StandardDetectorSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
    if (!fHitsCollection)
    {
        return false;
    }
    G4Track *trk = step->GetTrack();
    // TrackInformation *trkInfo = (TrackInformation *)(trk->GetUserInformation());
    G4double Edep = step->GetTotalEnergyDeposit();
    G4int AncestorID = -1; // trkInfo->GetAncestor(fID);

    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4StepPoint *postStepPoint = step->GetPostStepPoint();
    G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
    G4VPhysicalVolume *thePhysVol = theTouchable->GetVolume();

    G4int PID = trk->GetParticleDefinition()->GetPDGEncoding();
    G4int TrackID = trk->GetTrackID();
    G4int ParentTrackID = trk->GetParentID();
    G4ThreeVector InPos = preStepPoint->GetPosition();
    G4ThreeVector InMomentum = preStepPoint->GetMomentum();

    G4ThreeVector OutPos = postStepPoint->GetPosition();
    G4ThreeVector OutMomentum = postStepPoint->GetMomentum();

    G4ThreeVector VertexPos = trk->GetVertexPosition();

    G4double Time = preStepPoint->GetGlobalTime();

    G4double StepLength = 0.;
    if (trk->GetParticleDefinition()->GetPDGCharge() != 0)
    {
        StepLength = step->GetStepLength();
    }
    G4int CopyNo = theTouchable->GetCopyNumber();

    if (AncestorID < 0)
    {
        AncestorID = TrackID;
    }

    const G4VProcess *creatorProcess = trk->GetCreatorProcess();

    G4int ProcessID = -9999;
    if (creatorProcess)
    {
        ProcessID = creatorProcess->GetProcessSubType();
    }
    StandardHit *hit = NULL;

    for (G4int i = fHitsCollection->entries() - 1; i >= 0; --i)
    {
	StandardHit *savedHit = (*fHitsCollection)[i];
        if (savedHit->GetTrackID() == AncestorID && savedHit->GetCopyNo() == CopyNo)
        {
            hit = (*fHitsCollection)[i];
            break;
        }
    }
    if (hit)
    {
        hit->AddEdep(Edep);
        hit->AddTrackLength(StepLength);
        if (hit->GetTrackID() == TrackID)
        {
            if (hit->GetTime() > Time)
            {
                hit->SetTime(Time);
            }
            hit->SetOutPosition(OutPos);
            hit->SetOutMomentum(OutMomentum);
        }
    }
    else
    {
        hit = new StandardHit();
        hit->SetPID(PID);
        hit->SetTrackID(TrackID);
        hit->SetParentTrackID(ParentTrackID);
        hit->SetDetectorID(CopyNo);
        hit->SetProcessID(ProcessID);
        hit->SetInPosition(InPos);
        hit->SetInMomentum(InMomentum);
        hit->SetVertexPosition(VertexPos);
        hit->SetOutPosition(OutPos);
        hit->SetOutMomentum(OutMomentum);
        hit->SetTime(Time);
        hit->SetEdep(Edep);
        hit->SetTrackLength(StepLength);
        hit->SetPhysV(thePhysVol);
        hit->SetCopyNo(CopyNo);
        hit->SetProcessID(ProcessID);
        fHitsCollection->insert(hit);
    }
    // G4int nSencondaries = step->GetNumberOfSecondariesInCurrentStep();
    // if (nSencondaries > 0 && AncestorID >= 0)
    // {
    //     for (auto &secondary : *(step->GetSecondaryInCurrentStep()))
    //     {
    //         if (secondary->GetUserInformation() != 0)
    //         {
    //             continue;
    //         }
    //         TrackInformation *newTrkInfo = new TrackInformation(trkInfo);
    //         newTrkInfo->SetAncestor(fID, AncestorID);
    //         G4Track *secTrk = (G4Track *)secondary;
    //         secTrk->SetUserInformation(newTrkInfo);
    //     }
    // }
    return true;
}
void StandardDetectorSD::EndOfEvent(G4HCofThisEvent *HCE)
{
    if (!HCE)
    {
        return;
    }
    G4int nHits = fHitsCollection->GetSize();

    if (nHits <= 0)
    {
        return;
    }
    fN = nHits;
    hasHits = (fN > 0);
    // G4cout << "Haha SdSD nHits: " << fN << G4endl;
    for (int i = 0; i < fN; ++i)
    {
        StandardHit *hit = (*fHitsCollection)[i];
        fPID.push_back(hit->GetPID());
        fTID.push_back(hit->GetTrackID());
        fPTID.push_back(hit->GetParentTrackID());
        fDID.push_back(hit->GetDetectorID());
        fProcessID.push_back(hit->GetProcessID());
        fX.push_back(hit->GetInPosition().x());
        fY.push_back(hit->GetInPosition().y());
        fZ.push_back(hit->GetInPosition().z());
        fPx.push_back(hit->GetInMomentum().x());
        fPy.push_back(hit->GetInMomentum().y());
        fPz.push_back(hit->GetInMomentum().z());
        fTime.push_back(hit->GetTime());
        fEdep.push_back(hit->GetEdep());
        fTrackL.push_back(hit->GetTrackLength());
        fVx.push_back(hit->GetVertexPosition().x());
        fVy.push_back(hit->GetVertexPosition().y());
        fVz.push_back(hit->GetVertexPosition().z());
    }
}
void StandardDetectorSD::RegisterTree(TTree *tree)
{
    tree->Branch("PDGID", &fPID);
    tree->Branch("TrackID", &fTID);
    tree->Branch("ParentTrackID", &fPTID);
    tree->Branch("DetID", &fDID);
    tree->Branch("ProcessID", &fProcessID);
    tree->Branch("HitInPosition.X", &fX);
    tree->Branch("HitInPosition.Y", &fY);
    tree->Branch("HitInPosition.Z", &fZ);
    tree->Branch("HitInMomentum.X", &fPx);
    tree->Branch("HitInMomentum.Y", &fPy);
    tree->Branch("HitInMomentum.Z", &fPz);
    tree->Branch("VertexPosition.X", &fVx);
    tree->Branch("VertexPosition.Y", &fVy);
    tree->Branch("VertexPosition.Z", &fVz);
    tree->Branch("HitOutPosition.X", &fOutx);
    tree->Branch("HitOutPosition.Y", &fOuty);
    tree->Branch("HitOutPosition.Z", &fOutz);
    tree->Branch("HitGlobalTime", &fTime);
    tree->Branch("HitEdep", &fEdep);
    tree->Branch("TrackLength", &fTrackL);
}
void StandardDetectorSD::Clear()
{
    fN = 0;
    fPID.clear();
    fTID.clear();
    fPTID.clear();
    fDID.clear();
    fProcessID.clear();
    fX.clear();
    fY.clear();
    fZ.clear();
    fPx.clear();
    fPy.clear();
    fPz.clear();
    fTime.clear();
    fEdep.clear();
    fTrackL.clear();
    fVx.clear();
    fVy.clear();
    fVz.clear();
    fOutx.clear();
    fOuty.clear();
    fOutz.clear();
}
