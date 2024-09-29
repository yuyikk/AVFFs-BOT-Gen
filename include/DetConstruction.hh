#ifndef _DETCONSTRUCTION_HH_
#define _DETCONSTRUCTION_HH_ 
#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh" 
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"

class DetConstruction : public G4VUserDetectorConstruction
{
    public:
        DetConstruction();
        ~DetConstruction() override;

        G4VPhysicalVolume *Construct() override;
        void ConstructSDandField() override;
        void BuildMaterial();
    private:
        G4LogicalVolume *logicTOFModule = nullptr;
        G4LogicalVolume *logicBField = nullptr;
        G4double fArmHadAngle = 0;
        G4RotationMatrix *fArmHadRotation = nullptr;
        G4FieldManager *fFieldMgr = nullptr;
        G4UniformMagField *fBField = nullptr;
};
#endif
