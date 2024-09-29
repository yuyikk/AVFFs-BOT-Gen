#include "TOFHit.hh"
#include "DetConstruction.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<TOFHit>* TOFHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TOFHit::TOFHit(G4int rowId,G4int columnId, G4int layerId)
: fColumnID(columnId), fRowID(rowId), fLayerId(layerId)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TOFHit::operator==(const TOFHit &right) const
{
  return ( fColumnID==right.fColumnID && fRowID==right.fRowID && fLayerId == right.fLayerId);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TOFHit::Draw()
{
  auto visManager = G4VVisManager::GetConcreteInstance();
  if (! visManager || (fEdep==0.)) return;

  // Draw a calorimeter cell with depth propotional to the energy deposition
  G4Transform3D trans(fRot.inverse(),fPos);
  G4VisAttributes attribs;
  attribs.SetColour(G4Colour::Red());
  attribs.SetForceSolid(true);
  G4Box box("dummy",15.*cm,15.*cm,1.*m*fEdep/(0.1*GeV));
  visManager->Draw(box,attribs,trans);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* TOFHit::GetAttDefs() const
{
  G4bool isNew;
  auto store = G4AttDefStore::GetInstance("TOFHit",isNew);

  if (isNew) {
    (*store)["HitType"]
      = G4AttDef("HitType","Hit Type","Physics","","G4String");

    (*store)["Column"]
      = G4AttDef("Column","Column ID","Physics","","G4int");

    (*store)["Row"]
      = G4AttDef("Row","Row ID","Physics","","G4int");

    (*store)["Energy"]
      = G4AttDef("Energy","Energy Deposited","Physics","G4BestUnit",
                 "G4double");

    (*store)["Pos"]
      = G4AttDef("Pos", "Position", "Physics","G4BestUnit",
                 "G4ThreeVector");
  }
  return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* TOFHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;

  values
    ->push_back(G4AttValue("HitType","TOFHit",""));
  values
    ->push_back(G4AttValue("Column",G4UIcommand::ConvertToString(fColumnID),
                           ""));
  values
    ->push_back(G4AttValue("Row",G4UIcommand::ConvertToString(fRowID),""));
  values
    ->push_back(G4AttValue("Energy",G4BestUnit(fEdep,"Energy"),""));
  values
    ->push_back(G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));

  return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TOFHit::Print()
{
  G4cout << "  Cell[" << fRowID << ", " << fColumnID << ", " << fLayerId << "] "
    << fEdep/MeV << " (MeV) " << fPos << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

