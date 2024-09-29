#ifndef _TOFSD_HH_
#define _TOFSD_HH_ 1

#include "G4VSensitiveDetector.hh"
#include "StandardDetectorSD.hh"
#include "Math/Interpolator.h"

#include "TOFHit.hh"
#include "G4String.hh"
#include "Constants.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class TTree;

/// Hadron calorimeter sensitive detector

class TOFSD : public StandardDetectorSD
{
public:
  TOFSD(G4String name, G4String abbrev);
  ~TOFSD() override;

  virtual void Initialize(G4HCofThisEvent *) override;
  virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;
  virtual void EndOfEvent(G4HCofThisEvent *) override;
  // inline void SetAttenuationLG(const G4double &val);
  // inline void SetReflectanceLG(const G4double &val);
protected:
  virtual void RegisterTree(TTree *) override;
  virtual void Clear() override;
  TOFHitsCollection *fTOFHitsCollection = nullptr;
  float fTotalEdep;
  float fTotalTrackLength;
  std::vector<int> fModuleID;
  std::vector<float> fModuleEdep;
  std::vector<float> fModuleTrackLength;
};
// inline void TOFSD::SetReflectanceLG(const G4double &val)
// {
//   fReflectanceLG = val;
// }

// inline void TOFSD::SetAttenuationLG(const G4double &val)
// {
//   fAttenuationLG = val;
// }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
