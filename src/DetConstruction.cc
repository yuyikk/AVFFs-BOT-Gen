#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4PVReplica.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "TOFSD.hh"
#include "DetConstruction.hh"
#include "Constants.hh"
#include "G4UserLimits.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"

DetConstruction::DetConstruction()
{
    logicTOFModule = 0;
    fArmHadAngle = MyArmHad::kAngle;
    // fArmHadAngle = 0;
    fArmHadRotation = new G4RotationMatrix();
    // Around the X-axis (rotateX):
    //  A positive angle rotates counterclockwise when looking in the positive X direction (looking towards +X).
    // Around the Y-axis (rotateY):
    //  A positive angle rotates counterclockwise when looking in the positive Y direction (looking towards +Y).
    // Around the Z-axis (rotateZ):
    //  A positive angle rotates counterclockwise when looking in the positive Z direction (looking towards +Z).
    // fArmHadRotation->rotateY(fArmHadAngle); // rotate around y axis by an angle
}
DetConstruction::~DetConstruction()
{
    delete fArmHadRotation;
    delete logicTOFModule;
}
void DetConstruction::BuildMaterial()
{
    G4NistManager *nist = G4NistManager::Instance();

    // Vacuum
    G4Material *Vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    // Build world material
    const G4int NofEnergy = 12;
    G4double rindexWorld[NofEnergy] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
                                       1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; // Air
    G4double photonE[NofEnergy] = {2.08 * eV, 2.38 * eV, 2.58 * eV,
                                   2.70 * eV, 2.76 * eV, 2.82 * eV,
                                   2.92 * eV, 2.95 * eV, 3.02 * eV,
                                   3.10 * eV, 3.26 * eV, 3.44 * eV};
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", photonE, rindexWorld, NofEnergy);
    G4Material *Air = nist->FindOrBuildMaterial("G4_AIR");
    Air->SetMaterialPropertiesTable(mptWorld);

    G4Material *Iron = nist->FindOrBuildMaterial("G4_Fe");
    // G4cout << "Iron density: " << Iron->GetDensity() / (g / cm3) << G4endl;

    // Build scintillator BC408
    G4Element *C = nist->FindOrBuildElement("C");
    G4Element *H = nist->FindOrBuildElement("H");

    G4Material *BC408 = new G4Material("BC408", 1.023 * g / cm3, 2);
    BC408->AddElement(C, 1000);
    BC408->AddElement(H, 1104);

    G4double RIndexBC408[NofEnergy] = {1.58, 1.58, 1.58, 1.58, 1.58, 1.58,
                                       1.58, 1.58, 1.58, 1.58, 1.58, 1.58};
    G4double AbsorbLength[NofEnergy] = {380 * cm, 380 * cm, 380 * cm, 380 * cm,
                                        380 * cm, 380 * cm, 380 * cm, 380 * cm,
                                        380 * cm, 380 * cm, 380 * cm, 380 * cm};
    G4double scintilFast[NofEnergy] = {0.00, 0.03, 0.17, 0.40,
                                       0.55, 0.83, 1.00, 0.84,
                                       0.49, 0.20, 0.07, 0.04};

    G4MaterialPropertiesTable *mptBC408 = new G4MaterialPropertiesTable();
    mptBC408->AddProperty("RINDEX", photonE, RIndexBC408, NofEnergy);
    mptBC408->AddProperty("ABSLENGTH", photonE, AbsorbLength, NofEnergy);
    mptBC408->AddProperty("FASTCOMPONENT", photonE, scintilFast, NofEnergy, true);
    mptBC408->AddConstProperty("SCINTILLATIONYIELD", 10000. / MeV);
    mptBC408->AddConstProperty("RESOLUTIONSCALE", 1.0);
    mptBC408->AddConstProperty("FASTTIMECONSTANT", 2.1 * ns, true);
    mptBC408->AddConstProperty("YIELDRATIO", 1, true);
    mptBC408->DumpTable();
    BC408->SetMaterialPropertiesTable(mptBC408);
    BC408->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

    G4Material *LH2 = nist->FindOrBuildMaterial("G4_lH2"); // liquid hydrogen for target

    // aluminium for target container
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al");
    G4cout << "Material name: " << Al->GetName() << G4endl;
    // G4Material *Aluminum = new G4Material()

    G4cout << "== == == == Summary of materials definitions == == == ==" << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
G4VPhysicalVolume *DetConstruction::Construct()
{
    BuildMaterial();
    auto Vacuum = G4Material::GetMaterial("G4_Galactic");
    //auto Air = G4Material::GetMaterial("G4_Galactic");
    auto Air = G4Material::GetMaterial("G4_AIR");
    auto BC408 = G4Material::GetMaterial("BC408");
    auto Iron = G4Material::GetMaterial("G4_Fe");
    auto Al = G4Material::GetMaterial("G4_Al");
    auto LH2 = G4Material::GetMaterial("G4_lH2");
    // define the experiment hall
    G4Box *solidWorld = new G4Box("solidWorld", kHallSizeX / 2, kHallSizeY / 2, kHallSizeZ / 2);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");
    G4Tubs *solidBeamPipe = new G4Tubs("solidBeamPipe", 0, 2 * MyTarget::kRadiusContainer, kHallSizeZ / 2, 0, CLHEP::twopi);
    G4LogicalVolume *logicBeamPipe = new G4LogicalVolume(solidBeamPipe, Vacuum, "logicBeamPipe");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(
        0,
        G4ThreeVector(0, 0, 0), // positon where it is placed
        logicWorld,
        "physWorld",
        0,
        false,
        0,
        true);
    G4VPhysicalVolume *physBeamPipe = new G4PVPlacement(
        0,
        G4ThreeVector(0, MyTarget::kPosY, 0), // positon where it is placed
        logicBeamPipe,
        "physBeamPipe",
        logicWorld,
        false,
        0,
        true);

    // Target
    // The targer material is liquid hydrogen and
    // the container is made of Aluminium
    G4Tubs *tub1 = new G4Tubs("tub1", 0, MyTarget::kRadiusContainer, MyTarget::kLengthContainer / 2, 0, CLHEP::twopi);
    G4Tubs *solidTarget = new G4Tubs("solidTarget", 0, MyTarget::kRadiusTarget, MyTarget::kLengthTarget / 2, 0, CLHEP::twopi);
    G4SubtractionSolid *solidContainer = new G4SubtractionSolid("solidContainer", tub1, solidTarget);
    G4LogicalVolume *logicContainer = new G4LogicalVolume(solidContainer, Al, "logicContainer");
    G4LogicalVolume *logicTarget = new G4LogicalVolume(solidTarget, LH2, "logicTarget");
    G4ThreeVector posTarget(MyTarget::kPosX, MyTarget::kPosY, MyTarget::kPosZ);
    new G4PVPlacement(0, posTarget, logicContainer, "physContainer", logicBeamPipe, false, 0, true);
    new G4PVPlacement(0, posTarget, logicTarget, "physTarget", logicBeamPipe, false, 0, true);
    // world of ArmHad
    G4Box *solidArmHad = new G4Box("solidArmHad", MyArmHad::kSizeX / 2, MyArmHad::kSizeY / 2, MyArmHad::kSizeZ / 2);
    G4LogicalVolume *logicArmHad = new G4LogicalVolume(solidArmHad, Air, "logicArmHad");
    G4ThreeVector ArmHadPos(MyArmHad::kPosX, MyArmHad::kPosY, MyArmHad::kPosZ);
    G4ThreeVector axisPos(MyTarget::kPosX, MyTarget::kPosY, MyTarget::kPosZ);
    G4ThreeVector preShift = ArmHadPos - axisPos;
    fArmHadRotation->rotateY(fArmHadAngle);
    G4ThreeVector postShift = (fArmHadRotation->inverse()).operator()(preShift);
    //G4cout << "==The Rotation Matrix==" << G4endl;
    //G4cout << fArmHadRotation->xx() << "\t" << fArmHadRotation->xy() << "\t" << fArmHadRotation->xz() << G4endl;
    //G4cout << fArmHadRotation->yx() << "\t" << fArmHadRotation->yy() << "\t" << fArmHadRotation->yz() << G4endl;
    //G4cout << fArmHadRotation->zx() << "\t" << fArmHadRotation->zy() << "\t" << fArmHadRotation->zz() << G4endl;
    //G4ThreeVector z(0, 0, 1);
    // G4ThreeVector zprime = fArmHadRotation->operator()(z);
    // G4cout << "====axis vector====\t" << axisPos << G4endl;
    // G4cout << "====TOF vector====\t" << ArmHadPos << G4endl;
    // G4cout << "====Original vector====\t" << preShift << G4endl;
    // G4cout << "====Rotated vector====\t" << postShift << G4endl;
    G4ThreeVector finalPosition = axisPos + postShift;
    // fArmHadRotation->rotateY(-2*fArmHadAngle);
    G4VPhysicalVolume *physArmHad = new G4PVPlacement(
        fArmHadRotation,
        finalPosition,
        logicArmHad,
        "physArmHad",
        logicWorld,
        false,
        0,
        true);

    // Magnet
    G4Box *box1 = new G4Box("box1", MyMagnet::kSizeX / 2, MyMagnet::kSizeY / 2, MyMagnet::kSizeZ / 2);
    G4Box *solidBField = new G4Box("solidBField", MyMagnet::kFieldSizeX / 2, MyMagnet::kFieldSizeY / 2, MyMagnet::kFieldSizeZ / 2);
    G4SubtractionSolid *solidMagnetShell = new G4SubtractionSolid("solidMagnetShell", box1, solidBField);
    G4LogicalVolume *logicMagnetShell = new G4LogicalVolume(solidMagnetShell, Iron, "logicMagnetShell");
    logicBField = new G4LogicalVolume(solidBField, Air, "logicBField");
    auto userLimits = new G4UserLimits(0.01 * m);
    logicBField->SetUserLimits(userLimits);
    G4ThreeVector MagnetPos = G4ThreeVector(MyMagnet::kPosX, MyMagnet::kPosY, MyMagnet::kPosZ);
    new G4PVPlacement(0, MagnetPos, logicMagnetShell, "physMagnetShell", logicArmHad, false, 0, true);
    new G4PVPlacement(0, MagnetPos, logicBField, "physBField", logicArmHad, false, 0, true);

    // Place a virtual detector in front of the magnet
    G4Box *solidVrt1 = new G4Box("solidVrt1", MyMagnet::kFieldSizeX / 2, MyMagnet::kFieldSizeY / 2, 0.5 * um);
    G4LogicalVolume *logicVrt1 = new G4LogicalVolume(solidVrt1, Vacuum, "logicVrt1");
    G4ThreeVector Vrt1Pos = G4ThreeVector(MyMagnet::kPosX, MyMagnet::kPosY, MyMagnet::kPosZ - MyMagnet::kSizeZ / 2 - 0.5 * um);
    new G4PVPlacement(0, Vrt1Pos, logicVrt1, "physVrt1", logicArmHad, false, 10001, true);
    // Define the TOF and place it into ArmHad
    G4Box *solidTOF = new G4Box("solidTOF",
                                MyTOF::kSizeX / 2,
                                MyTOF::kSizeY / 2,
                                MyTOF::kSizeZ / 2);
    G4LogicalVolume *logicTOF = new G4LogicalVolume(solidTOF, Air, "logicTOF");
    G4ThreeVector TOFPos(MyTOF::kPosX, MyTOF::kPosY, MyTOF::kPosZ);
    G4VPhysicalVolume *physTOF = new G4PVPlacement(
        0, TOFPos, logicTOF, "physTOF", logicArmHad, false, 0, true);

    G4Box *solidTOFModule = new G4Box("solidTOFModule",
                                      MyTOF::kModSizeX / 2,
                                      MyTOF::kModSizeY / 2,
                                      MyTOF::kModSizeZ / 2);
    logicTOFModule = new G4LogicalVolume(solidTOFModule, BC408, "logicTOFModule");
    G4Box *solidIronPlate = new G4Box("solidIronPlate", MyTOF::kIronSizeX / 2, MyTOF::kIronSizeY / 2, MyTOF::kIronSizeZ / 2);
    G4LogicalVolume *logicIronPlate = new G4LogicalVolume(solidIronPlate, Iron, "logicIronPlate");
    G4int copyN = 0;
    G4int nIron = 0;
    G4double x0 = 0;
    G4double y0 = -MyTOF::kSizeY / 2 + MyTOF::kModSizeY / 2;
    G4double z0 = -MyTOF::kSizeZ / 2 + MyTOF::kModSizeZ / 2;
    G4double dx = MyTOF::kModSizeX;
    G4double dy = MyTOF::kModSizeY;
    // G4double dz = MyTOF::kIronSizeZ + MyTOF::kModSizeZ;
    G4double dz = MyTOF::kModSizeZ;
    // G4bool hasIron[MyTOF::kNofLayers] = {true, true, true, false, false, false, false};
    // // Place iron in front of scintillators
    // for (int ilay = 0; ilay < MyTOF::kNofLayers; ++ilay)
    // {
    //     for (int icol = 0; icol < MyTOF::kNofColumns; ++icol)
    //     {
    //         for (int irow = 0; irow < MyTOF::kNofRows; ++irow)
    //         {
    //             if (!hasIron[ilay])
    //             {
    //                 continue;
    //             }
    //             G4double x = x0 + icol * dx;
    //             G4double y = y0 + irow * dy;
    //             G4double z = z0 + ilay * dz;
    //             G4ThreeVector pos(x, y, z);
    //             new G4PVPlacement(0, pos, logicIronPlate, "physIronPlate", logicTOF, false, 1000 + copyN);
    //         }
    //     }
    // }
    // z0 += dz / 2;
    for (int ilay = 0; ilay < MyTOF::kNofLayers; ++ilay)
    {
        for (int icol = 0; icol < MyTOF::kNofColumns; ++icol)
        {
            for (int irow = 0; irow < MyTOF::kNofRows; ++irow)
            {
                G4double x = x0 + icol * dx;
                G4double y = y0 + irow * dy;
                G4double z = z0 + ilay * dz;
                G4ThreeVector pos(x, y, z);
                new G4PVPlacement(0, pos, logicTOFModule, "physTOFModule", logicTOF, false, copyN, true);
                copyN++;
            }
        }
    }
    // Place a virtual detector in front of the TOF
    G4Box *solidVrt2 = new G4Box("solidTOF",
                                 MyTOF::kSizeX / 2,
                                 MyTOF::kSizeY / 2,
                                 0.5 * um);
    G4LogicalVolume *logicVrt2 = new G4LogicalVolume(solidVrt2, Vacuum, "logicVrt2");
    G4ThreeVector Vrt2Pos(MyTOF::kPosX, MyTOF::kPosY, MyTOF::kPosZ - MyTOF::kSizeZ / 2 - 0.5 * um);
    new G4PVPlacement(0, Vrt2Pos, logicVrt2, "physVrt2", logicArmHad, false, 10002, true);
    G4VisAttributes *TOFVisAtt = new G4VisAttributes(G4Colour(0.19, 0.67, 1));
    TOFVisAtt->SetVisibility(false);
    logicTOF->SetVisAttributes(TOFVisAtt);
    logicArmHad->SetVisAttributes(TOFVisAtt);
    G4VisAttributes *TOFModuleVisAtt = new G4VisAttributes(G4Colour(0.19, 0.67, 1));
    TOFModuleVisAtt->SetVisibility(true);
    // TOFModuleVisAtt->SetForceSolid(true);
    logicTOFModule->SetVisAttributes(TOFModuleVisAtt);
    G4VisAttributes *TOFIronPlateVisAtt = new G4VisAttributes(G4Color::Red());
    logicIronPlate->SetVisAttributes(TOFIronPlateVisAtt);

    G4Color col = G4Color::Gray();
    // col.SetAlpha(0.8);
    G4VisAttributes *ContainerVisAtt = new G4VisAttributes(col);
    // ContainerVisAtt->SetForceSolid();
    logicContainer->SetVisAttributes(ContainerVisAtt);

    // ContainerVisAtt->Set
    G4VisAttributes *TargetVisAtt = new G4VisAttributes(G4Color::Cyan());
    TargetVisAtt->SetForceSolid(true);
    logicTarget->SetVisAttributes(TargetVisAtt);

    G4VisAttributes *MagnetVisAtt = new G4VisAttributes(G4Color::Brown());
    // MagnetVisAtt->SetForceSolid(true);
    logicMagnetShell->SetVisAttributes(MagnetVisAtt);
    G4VisAttributes *BeamPipeVisAtt = new G4VisAttributes(G4Color::Gray());
    // BeamPipeVisAtt->SetForceSolid();
    BeamPipeVisAtt->SetVisibility(true);
    BeamPipeVisAtt->SetForceWireframe(true);

    logicBeamPipe->SetVisAttributes(BeamPipeVisAtt);

    G4VisAttributes *VrtAtt = new G4VisAttributes(G4Color::Gray());
    VrtAtt->SetForceSolid();
    logicVrt1->SetVisAttributes(VrtAtt);
    logicVrt2->SetVisAttributes(VrtAtt);

    // G4cout << "Hahahaha=========1" << G4endl;
    return physWorld;
}
void DetConstruction::ConstructSDandField()
{
    G4SDManager *sdMan = G4SDManager::GetSDMpointer();
    G4String sdName = "TOF";
    StandardDetectorSD *sdDet1 = new StandardDetectorSD("Virtual", "");
    TOFSD *sdTOF = new TOFSD(sdName, "");
    sdMan->AddNewDetector(sdDet1);
    sdMan->AddNewDetector(sdTOF);
    SetSensitiveDetector("logicVrt1", sdDet1);
    SetSensitiveDetector("logicVrt2", sdDet1);
    SetSensitiveDetector("logicTOFModule", sdTOF);
    // SetSensitiveDetector("logicTOFModule", sdTOF);

    // Define the magnetic field
    G4ThreeVector fieldValue = G4ThreeVector(MyMagnet::kBx, MyMagnet::kBy, MyMagnet::kBz); // Uniform field along z-axis
    fBField = new G4UniformMagField(fieldValue);
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(fBField);
    G4Mag_UsualEqRhs *equationOfMotion = new G4Mag_UsualEqRhs(fBField);
    G4MagIntegratorStepper *stepper = new G4ClassicalRK4(equationOfMotion); // Runge-Kutta stepper
    G4ChordFinder *chordFinder = new G4ChordFinder(fBField, 1e-2 * mm, stepper);
    fFieldMgr->SetChordFinder(chordFinder);
    logicBField->SetFieldManager(fFieldMgr, true);
    // MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
    // logicDetector->SetSensitiveDetector(sensDet);
}
