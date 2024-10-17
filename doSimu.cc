#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4StepLimiterPhysics.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BIC_HP.hh"
#include "G4OpticalPhysics.hh"
#include <iostream>
#include "DetConstruction.hh"
// #include "MyPhysicsList.hh"
#include "ActionInit.hh"
#include "RootTree.hh"
#include "TFile.h"
#include "TTree.h"
#include "GeneratorTree.hh"
#include "Randomize.hh"
#include "TString.h"
#include <time.h>
#include "TH1I.h"
// #include "GlobalVars.hh"

using namespace std;
const bool GUIStatus = false;
RootTree *gRootTree = 0;
GeneratorTree *gGeneratorTree = 0;
int main(int argc, char *argv[])
{
    TString ofilename = Form("%s.BOT.root", argv[1]);
    gRootTree = new RootTree(ofilename);
    TH1I *h_event = new TH1I("h_event", "Number of events generated in the simulation", 1, 0, 1);

    // load generator data
    // TFile *generatorFile = TFile::Open(argv[3]);
    gGeneratorTree = new GeneratorTree(argv[2]);
    gGeneratorTree->InitTree();
    G4int nEvent = gGeneratorTree->GetNumberOfEvent();
    h_event->SetBinContent(1, nEvent);

    G4Random::setTheEngine(new CLHEP::Ranlux64Engine);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long randseed = ts.tv_sec * 1000000000L + ts.tv_nsec;
    std::cout << "Using random seed " << randseed << std::endl;
    G4Random::setTheSeed(randseed);
    G4RunManager *runManager = new G4RunManager();
    auto physicslist = new FTFP_BERT;
    // physicslist->RegisterPhysics(new G4OpticalPhysics());
    // auto physicslist = new QGSP_BIC_HP;
    runManager->SetUserInitialization(new DetConstruction());
    runManager->SetUserInitialization(physicslist);
    physicslist->RegisterPhysics(new G4StepLimiterPhysics());
    runManager->SetUserInitialization(physicslist);
    runManager->SetUserInitialization(new ActionInit());
    runManager->Initialize();
    G4UIExecutive *ui = 0;
    if (GUIStatus == true)
    {
        G4VisManager *visManager = new G4VisExecutive();
        visManager->Initialise();
        ui = new G4UIExecutive(argc, argv);
        G4UImanager *UImanager = G4UImanager::GetUIpointer();
        UImanager->ApplyCommand("/vis/open OGL");
        
        // UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
        
        // UImanager->ApplyCommand("/vis/scene/add/volume");
        // UImanager->ApplyCommand("/vis/viewer/flush");
        // UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
        UImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 90 90");
        UImanager->ApplyCommand("/vis/drawVolume");
        UImanager->ApplyCommand("/vis/viewer/setStyle wireframe");
        // UImanager->ApplyCommand("/vis/viewer/zoom 1.2");
        UImanager->ApplyCommand("/vis/scene/add/axes 0 0 0 1 m");
        // UImanager->ApplyCommand("/vis/viewer/set/style surface");
        // UImanager->ApplyCommand("/vis/viewer/set/aa 4");
        UImanager->ApplyCommand("/vis/ogl/export high_res_image.png 2560 1440");
    }

    // UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
    //  UImanager->ApplyCommand("/run/beamOn 1");

    runManager->SetPrintProgress(min(nEvent / 20, 100000));
    runManager->BeamOn(nEvent);

    if (GUIStatus == true)
    {
        ui->SessionStart();
    }

    // h_event->SetDirectory(ofilename);

    // delete ui;
    // delete visManager;
    delete runManager;
    delete gRootTree;
    delete gGeneratorTree;

    return 0;
}
