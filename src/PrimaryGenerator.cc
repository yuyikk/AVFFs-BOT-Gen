#include "PrimaryGenerator.hh"
#include "Constants.hh"
#include "G4Event.hh"
#include "GlobalVars.hh"
#include "TRandom3.h"
#include "GeneratorTree.hh"
#include <time.h>
G4ThreeVector GenPrimaryVertex();
PrimaryGenerator::PrimaryGenerator() : fParticleGun(nullptr)
{
    fParticleGun = new G4ParticleGun(1);
}
PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}
void PrimaryGenerator::GeneratePrimaries(G4Event *myEvt)
{
    gGeneratorTree->GetEvent(myEvt->GetEventID());
    G4int nPart = gGeneratorTree->GetNumberOfParticle();
    G4ParticleTable *partTable = G4ParticleTable::GetParticleTable();
    // G4cout << "#particle: " << nPart << G4endl;
    for (int i = 0; i < nPart; ++i)
    {
        G4int pdgid = gGeneratorTree->GetPdgID(i);
        G4ParticleDefinition *partDef = partTable->FindParticle(pdgid);

        // Kinematics
        G4double Px = gGeneratorTree->GetPx(i) * GeV;
        G4double Py = gGeneratorTree->GetPy(i) * GeV;
        G4double Pz = gGeneratorTree->GetPz(i) * GeV;
        G4ThreeVector momentum(Px, Py, Pz);
        G4double mass = partDef->GetPDGMass();
        G4double Etot = std::sqrt(momentum.mag2() + mass * mass);
        G4double Ek = Etot - mass;
        // G4cout << "Part Info: " << pdgid << "\t" << Px << "\t" << Py << "\t" << Pz << "\t" << Etot << "\t" << mass << "\t" << Ek << G4endl;
        
        G4ThreeVector vertex = GenPrimaryVertex();
        // if (vertex.x() * vertex.x() + vertex.y() * vertex.y() > 1 * cm * 1 * cm)
        // G4cout << "Vertex Info: " << vertex.x() / cm << "\t" << vertex.y() / cm << "\t" << vertex.z() / cm << G4endl;
        fParticleGun->SetParticleDefinition(partDef);
        fParticleGun->SetParticleMomentumDirection(momentum.unit());
        fParticleGun->SetParticleEnergy(Ek);
        fParticleGun->SetParticlePosition(vertex);
        // Generate the particle in the event
        fParticleGun->GeneratePrimaryVertex(myEvt);
    }
    // G4cout << "-- -- -- -- -- -- -- -- -- --" << G4endl;
}
G4ThreeVector GenPrimaryVertex()
{
    G4double z_max = MyTarget::kLengthContainer / 2;
    G4double r_max = 1.0 * cm;
    TRandom3 rnd;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long randseed = ts.tv_sec * 1000000000L + ts.tv_nsec;
    rnd.SetSeed(randseed);
    double x, y, z;
    double r = 100 * cm;
    while (r > r_max)
    {
        x = rnd.Uniform(-r_max, r_max);
        y = rnd.Uniform(-r_max, r_max);
        r = std::sqrt(x * x + y * y);
    }
    z = rnd.Uniform(-z_max, z_max);
    return G4ThreeVector(x, y, z);
}