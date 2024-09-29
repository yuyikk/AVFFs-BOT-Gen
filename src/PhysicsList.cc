#include "PhysicsList.hh"
PhysicsList::PhysicsList()
{
    RegisterPhysics(new G4EmStandardPhysics()); // electromagnetic interaction
    RegisterPhysics(new G4OpticalPhysics()); // 
}
PhysicsList::~PhysicsList()
{

}