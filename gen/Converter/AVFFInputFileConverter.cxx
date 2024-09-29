#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "RootFileFormat.h"

using namespace std;

//input the file name as parameter (usually a .dat or .txt file), and then
//the program convert it into a root file with the same name, but root file type

//second parameter indicated input generator type
// 0 : esepp
// 1 : MCEGENpiN_radcorr
// 2 : genKYandOnePion

const double eMass = 0.51099893e-3;
const double pMass = 938.272046e-3;

struct ParEntry{
    int ParInfo[6];
    double KinInfo[8];
};

//used for sorting purpose
int GetRank(int val);
void PrintEntry(const ParEntry& entry);
void FillOutputArray();
void ClearEntry();
void AVFFInputFileConverter(const string & filename, int type = 0);
int main(int argc, char **argv)
{
    if (argc == 2)
    {
        AVFFInputFileConverter(argv[1]);
    }
    else
    {
        AVFFInputFileConverter(argv[1], atoi(argv[2]));
    }
    return 0;
}
void AVFFInputFileConverter(const string & filename, int type)
{
    //open the input file and make sure it is found
    ifstream inFile;
    inFile.open(filename);
    
    if (!inFile.is_open()){
        cout<<"input file "<<filename<<" not found, exit the program"<<endl;
        exit(0);
    }
    
    //remove everything after . and then replace it with the .root
    string rootFileName = filename;
    
    size_t pos = filename.find_last_of(".");
    
    if (pos != std::string::npos){
        rootFileName = rootFileName.substr(0, pos) + "_AVFF.root";
    }
    else{
        cout<<"something wrong with the input file name, exit the program"<<endl;
        exit(0);
    }
    
    //create the root file and init the tree
    TFile* RootFile = new TFile(rootFileName.c_str(), "RECREATE");
    TTree* tree = new TTree("T", "T");
    InitTree(tree);
    
    if (type == 0){
        //read the esepp format file
        double tmp[9];
        int ThisPID[3] = {11, 2212, 22};
        double ThisMass[3] = {eMass, pMass, 0};
        while (inFile >> tmp[0]){
            for (int i=1; i<9; i++) inFile >> tmp[i];
            if (inFile.eof()) break;
            
            NParticle = 0;
            for (int i = 0; i<3; i++){
                double E     = tmp[i*3 + 0] / 1000.; //convert from MeV to GeV
                double theta = tmp[i*3 + 1];
                double phi   = tmp[i*3 + 2];
                
                ///skip empty slots
                if (fabs(E) < 1e-6 && fabs(theta) < 1e-6 && fabs(phi) < 1e-6) continue;
                
                double mom     = sqrt(E*E - ThisMass[i]*ThisMass[i]);
                Pz[NParticle]  = mom*cos(theta);
                Px[NParticle]  = mom*sin(theta)*cos(phi);
                Py[NParticle]  = mom*sin(theta)*sin(phi);
                PID[NParticle] = ThisPID[i];
                
                Weight = 1.;
                NParticle++;
            }
            if (NParticle != 0) tree->Fill();
        }
    }
    else{
        //read the Hall B gen format file
        vector<ParEntry> AllEntry;
        string FileLine;
        while(getline(inFile, FileLine)){
            istringstream HeaderStream(FileLine);
            
            vector<string> HeaderInfo;
            string tmp;
            while (HeaderStream >> tmp) HeaderInfo.push_back(tmp);
            
            NParticle = 0;
            AllEntry.clear();
            NParticle = stoi(HeaderInfo[0]);
            
            if (type == 1) Weight = stof(HeaderInfo[9]);
            else if (type == 2) Weight = 1.;
            
            for (int i=0; i<NParticle; i++){
                getline(inFile, FileLine);
                istringstream ParInfoStream(FileLine);
                
                ParEntry ThisEntry;
                for (int j=0; j<6; j++) ParInfoStream>>ThisEntry.ParInfo[j];
                for (int j=0; j<8; j++) ParInfoStream>>ThisEntry.KinInfo[j];
                
                AllEntry.push_back(ThisEntry);
            }
            
            //sort the entries in a particular order
            sort(AllEntry.begin(), AllEntry.end(), 
                 [](const ParEntry& a, const ParEntry& b)
                 {return GetRank(a.ParInfo[3]) < GetRank(b.ParInfo[3]);}  
                );
                
            //Saving into the ROOT tree
            for (int i=0; i<NParticle; i++){
                PID[i] = AllEntry[i].ParInfo[3];
                Px[i]  = AllEntry[i].KinInfo[0];
                Py[i]  = AllEntry[i].KinInfo[1];
                Pz[i]  = AllEntry[i].KinInfo[2];
            }
            
            if (NParticle != 0) tree->Fill();
        }
    }
    
    //saving the tree into the root file
    RootFile->cd();
    tree->Write();
    RootFile->Close();
}

int GetRank(int val)
{
    if      (val == 11)   return 1; //electron
    else if (val == 2112) return 2; //neutron
    else if (val == 2212) return 3; //proton
    else if (val == 211)  return 4; //pi+
    else if (val == -211) return 5; //pi-
    else if (val == 22)   return 6; //photon
    else return 7;
}

void PrintEntry(const ParEntry& entry)
{
    for (int j=0; j<6; j++) cout<<entry.ParInfo[j]<<" ";
    for (int j=0; j<8; j++) cout<<entry.KinInfo[j]<<" ";
    cout<<endl;
}
