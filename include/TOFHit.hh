#ifndef _MyTOFHit_HH_
#define _MyTOFHit_HH_ 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"



class G4AttDef;
class G4AttValue;

/// Hadron Calorimeter hit
///
/// It records:
/// - the cell column ID and row ID
/// - the energy deposit
/// - the cell position and rotation

class TOFHit : public G4VHit
{
public:
  TOFHit() = default;
  TOFHit(G4int iRow, G4int iCol, G4int iLay);
  TOFHit(const TOFHit &right) = default;
  ~TOFHit() override = default;

  TOFHit &operator=(const TOFHit &right) = default;
  G4bool operator==(const TOFHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);

  void Draw() override;
  const std::map<G4String, G4AttDef> *GetAttDefs() const override;
  std::vector<G4AttValue> *CreateAttValues() const override;
  void Print() override;

  void SetColumnID(G4int z) { fColumnID = z; }
  G4int GetColumnID() const { return fColumnID; }

  void SetRowID(G4int z) { fRowID = z; }
  G4int GetRowID() const { return fRowID; }

  void SetLayerID(G4int z) { fLayerId = z; }
  G4int GetLayerID() const { return fLayerId; }

  void SetDetectorID(G4int val) { fDetID = val; }
  G4int GetDetectorID() const { return fDetID; }

  void SetEdep(const G4double &de) { fEdep = de; }
  void AddEdep(const G4double &de) { fEdep += de; }
  G4double GetEdep() const { return fEdep; }
  void SetTrackLength(const double &length) { fTrackLength = length; }
  void AddTrackLength(const G4double &length) { fTrackLength += length; }
  G4double GetTrackLength() const { return fTrackLength; }

  void SetPos(G4ThreeVector xyz) { fPos = xyz; }
  G4ThreeVector GetPos() const { return fPos; }

  void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
  G4RotationMatrix GetRot() const { return fRot; }

private:
  G4int fColumnID = -1;
  G4int fRowID = -1;
  G4int fLayerId = -1;
  G4int fDetID = -1;
  G4double fEdep = 0.;
  G4double fTime = 0.;
  G4ThreeVector fPos;
  G4RotationMatrix fRot;
  G4double fTrackLength = 0.;
};

using TOFHitsCollection = G4THitsCollection<TOFHit>;

extern G4ThreadLocal G4Allocator<TOFHit> *TOFHitAllocator;

inline void *TOFHit::operator new(size_t)
{
  if (!TOFHitAllocator)
  {
    TOFHitAllocator = new G4Allocator<TOFHit>;
  }
  return (void *)TOFHitAllocator->MallocSingle();
}

inline void TOFHit::operator delete(void *aHit)
{
  TOFHitAllocator->FreeSingle((TOFHit *)aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif
