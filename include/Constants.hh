//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B5/include/Constants.hh
/// \brief Definition of B5 example constants.

#ifndef B5Constants_h
#define B5Constants_h 1
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <cmath>


constexpr G4int kNofHodoscopes1 = 15;
constexpr G4int kNofHodoscopes2 = 25;
constexpr G4int kNofChambers = 5;
constexpr G4int kNofEmColumns = 20;
constexpr G4int kNofEmRows = 4;
constexpr G4int kNofEmCells = kNofEmColumns * kNofEmRows;
constexpr G4int kNofHadColumns = 1;
constexpr G4int kNofHadRows = 140;
constexpr G4int kNofHadCells = kNofHadColumns * kNofHadRows;

// The size of experiment hall
constexpr G4double kHallSizeX = 30 * m;
constexpr G4double kHallSizeY = 10 * m;
constexpr G4double kHallSizeZ = 19 * m;
namespace MyTOF
{
    // relative to the center of MyAarmHad

    constexpr G4double kLengthToTarget = 15 * m;
    constexpr G4int kNofLayers = 7;
    constexpr G4int kNofRows = 140;
    constexpr G4int kNofColumns = 1;
    constexpr G4int kNofModules = kNofRows * kNofColumns * kNofLayers;
    constexpr G4double kLayerShift = 0 * cm;

    constexpr G4double kModSizeX = 200 * cm;
    constexpr G4double kModSizeY = 6 * cm;
    constexpr G4double kModSizeZ = 6 * cm;

    constexpr G4double kIronSizeX = kModSizeX;
    constexpr G4double kIronSizeY = kModSizeY;
    constexpr G4double kIronSizeZ = 2 * cm;

    constexpr G4double kSizeX = 200 * cm;
    constexpr G4double kSizeY = 840 * cm;
    constexpr G4double kSizeZ = kModSizeZ * kNofLayers;

    constexpr G4double kPosX = 0;
    constexpr G4double kPosY = 0;
    constexpr G4double kPosZ = kLengthToTarget - 8.5 * m + kSizeZ / 2;

}

namespace MyTarget
{
    constexpr G4double kRadiusContainer = 2.54 * cm; // 2 inches in diameter
    constexpr G4double kRadiusTarget = kRadiusContainer - 0.25 * mm;
    constexpr G4double kLengthContainer = 20 * cm + 0.3 * mm;
    constexpr G4double kLengthTarget = 20 * cm;
    constexpr G4double kPosX = 0;
    constexpr G4double kPosY = 0;
    constexpr G4double kPosZ = -kHallSizeZ / 2 + 100 * cm;
}
namespace MyArmHad // the arm for hadron detection
{
    constexpr G4double kSizeX = 2.5 * m;
    constexpr G4double kSizeY = 8.5 * m;
    constexpr G4double kSizeZ = 14.05 * m;
    constexpr G4double kPosX = 0;
    constexpr G4double kPosY = 0;
    constexpr G4double kPosZ = 0;
    constexpr G4double kAngle = -45 * degree;
}
namespace MyMagnet
{
    constexpr G4double kLengthToTarget = 1.5 * m;
    constexpr G4double kFieldSizeX = 34 * cm;
    constexpr G4double kFieldSizeY = 142 * cm;
    constexpr G4double kFieldSizeZ = 100 * cm;
    constexpr G4double kShellThickness = 100 * cm;
    constexpr G4double kSizeX = kFieldSizeX + 2 * kShellThickness;
    constexpr G4double kSizeY = kFieldSizeY + 2 * kShellThickness;
    constexpr G4double kSizeZ = kFieldSizeZ;
    constexpr G4double kPosX = 0;
    constexpr G4double kPosY = 0;
    constexpr G4double kPosZ = -650 * cm; 
    constexpr G4double kFieldDirectionX = 0;
    constexpr G4double kFieldDirectionY = 1;
    constexpr G4double kFieldDirectionZ = 0;
    constexpr G4double kBx = 0;
    constexpr G4double kBy = 0.5 * tesla;
    constexpr G4double kBz = 0;
}
namespace MyArmLep // the arm for lepton detection
{
    constexpr G4double kSizeX = 4 * m;
    constexpr G4double kSizeY = 10 * m;
    constexpr G4double kSizeZ = 36 * m;

    constexpr G4double kPosX = 0 * m;
    constexpr G4double kPosY = 0 * m;
    constexpr G4double kPosZ = 0 * m;
}


namespace MyParticleGun
{
    constexpr G4double kEtot = 2.2 * GeV; // total energy
    constexpr G4double kPosX = MyTarget::kPosX;
    constexpr G4double kPosY = MyTarget::kPosY;
    constexpr G4double kPosZ = MyTarget::kPosZ - MyTarget::kLengthContainer / 2; // shoot in from the surface of HCal
    constexpr G4double kDirectionX = 0;
    constexpr G4double kDirectionY = 0;
    constexpr G4double kDirectionZ = 1;
}
#endif
