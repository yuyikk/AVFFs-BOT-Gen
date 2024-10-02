# AVFFs-BOT-GEN
The Beam-on-target simulations on neutron arm.
The codes read in particle information from generators.
The codes are almost the same as https://github.com/yuyikk/AVFFs-BOT, except an interface of reading particles from generators. 
## How to build
1. change to the directory where `src` and `include` located.
2. `mkdir -p build`
3. `cd build`
4. `cmake ..`
5. `make`

    You will get a executable file `doSimu` in the current directory.

## How to run
1. `cd build`
2. `./doSimu runnumber generator_root_file`. 

    The `doSimu` accepts two arguements, the first is the runnumber that you can set freely and the second is the converted root file from generator.
    
    You will get a root file `runnumber.BOT.root` in the current directory, in which a tree is stored and also a histogram storing number of generated enents. 

## Structure of the output TTree
|Branch name|data format|Description|
|:-|:-|:-|
|EventID|EventID/I|Event id|                                        
|GenPart.PDGID | vector\<int>|pdg id of particles from generator|                        
|GenPart.Px | vector\<float>|x component of particle momentum from generator, the unit is MeV |                         
|GenPart.Py | vector\<float>|y component of particle momentum from generator, the unit is MeV|                         
|GenPart.Pz | vector\<float>|z component of particle momentum from generator, the unit is MeV|                       
|GenPart.X | vector\<float> | primary vertex x, the unit is cm|                         
|GenPart.Y | vector\<float> | primary vertex y, the unit is cm|                         
|GenPart.Z | vector\<float> |primary vertex y, the unit is cm|                         
|PDGID     | vector\<int> | pdg id of particles hitting on virtual detectors|                            
|TrackID   | vector\<int> | track id|                           
|ParentTrackID | vector\<int> | parent track id of the track|                        
|DetID     | vector\<int>   | detector id, 10001 for virtual detector at magnet and 10002 for at TOF|                         
|ProcessID | vector\<int>  | physical processes id in geant4|                           
|HitInPosition.X | vector\<float> |particle going-in position x on virtual detextors, the unit is mm|                    
|HitInPosition.Y | vector\<float> |particle going-in position y on virtual detextors, the unit is mm|                    
|HitInPosition.Z | vector\<float> |particle going-in position z on virtual detextors, the unit is mm|                   
|HitInMomentum.X | vector\<float> |particle going-in momentum x on virtual detectors, the unit is MeV|                     
|HitInMomentum.Y | vector\<float> |particle going-in momentum y on virtual detectors, the unit is MeV|                   
|HitInMomentum.Z | vector\<float> |particle going-in momentum z on virtual detectors, the unit is MeV|                   
|VertexPosition.X | vector\<float> |vertex (where particle is generated) x of particles hitting on virtual detectors, the unit is mm|                   
|VertexPosition.Y | vector\<float> |vertex (where particle is generated) y of particles hitting on virtual detectors, the unit is mm|
|VertexPosition.Z | vector\<float> |vertex (where particle is generated) y of particles hitting on virtual detectors, the unit is mm|                  
|HitOutPosition.X | vector\<float> |particle going-out position x on virtual detextors, the unit is mm|                  
|HitOutPosition.Y | vector\<float> |particle going-out position y on virtual detextors, the unit is mm|                    
|HitOutPosition.Z | vector\<float> |particle going-out position z on virtual detextors, the unit is mm|                    
|HitGlobalTime | vector\<float>    |global time of particles hitting on virtual detectors, the unit is ns|                 
|HitEdep   | vector\<float>        |Energy depositon of particles on virtual detectors, the unit is MeV|                  
|TrackLength | vector\<float>|track length of particles in virtual detectors|                        
|TOF.PDGID | vector\<int>    |pdg id of particles going-in TOF|                        
|TOF.TrackID | vector\<int>  | particle track id|                        
|TOF.ParentTrackID | vector\<int>| track id of parent particles|                    
|TOF.DetID | vector\<int> | detector id of TOF modules that particles hitted, ranges 0 ~ 979|                            
|TOF.ProcessID | vector\<int> | physical processes id in geant4|                      
|TOF.HitInPosition.X | vector\<float> |particle going-in position x on TOF, the unit is mm|                 
|TOF.HitInPosition.Y | vector\<float> |particle going-in position y on TOF, the unit is mm|               
|TOF.HitInPosition.Z | vector\<float> |particle going-in position z on TOF, the unit is mm|               
|TOF.HitInMomentum.X | vector\<float> |particle going-in momentum x on TOF, the unit is MeV|               
|TOF.HitInMomentum.Y | vector\<float> |particle going-in momentum y on TOF, the unit is MeV|               
|TOF.HitInMomentum.Z | vector\<float> |particle going-in momentum z on TOF, the unit is MeV|               
|TOF.VertexPosition.X | vector\<float> |vertex (where particle is generated) x of particles hitting on TOF, the unit is mm|              
|TOF.VertexPosition.Y | vector\<float>|vertex (where particle is generated) y of particles hitting on TOF, the unit is mm|               
|TOF.VertexPosition.Z | vector\<float>|vertex (where particle is generated) z of particles hitting on TOF, the unit is mm|               
|TOF.HitOutPosition.X | vector\<float>|particle going-out position x on TOF, the unit is mm|               
|TOF.HitOutPosition.Y | vector\<float>|particle going-out position y on TOF, the unit is mm|               
|TOF.HitOutPosition.Z | vector\<float>|particle going-out position z on TOF, the unit is mm|               
|TOF.HitGlobalTime | vector\<float>   |global time of particles hitting on TOF, the unit is ns|               
|TOF.HitEdep | vector\<float>         |energy deposition of particles hittong on TOF, the unit is MeV|               
|TOF.TrackLength | vector\<float>     |track length of particles in TOF|               
|TOF.ModuleID | vector\<int>          |detector id of TOF modules|               
|TOF.TotalEdep | TOF.TotalEdep/F      |total energy deposition on TOF in an event, the unit is MeV|              
|TOF.TotalTrackLength | TOF.TotalTrackLength/F | total track length on TOF in an event, the unit is mm|      
|TOF.ModuleEdep | vector\<float> | total energy depostion on a single TOF module in an event, the unit is MeV|                    
|TOF.ModuleTrackLength | vector\<float>|total track length on a single TOF module in an event, the unit is mm|              

## About Generator
You can find two directories under directory `gen`| `Converter` and `epGen`
### 1. For Converter
It convert the dat file produced from generator to root files used as input in the G4 simulation. You can find examples of dat files in the directory `Converter/data`.
#### How to use
1. cd Converter
2. `./compile`
3. `./AVFFInputFileConverter.exe dat_file type`

    dat_file| The output from generator
    
    type| it can be 0, 1 or 2

    0| convert output from `epGen`

    1| CONVERT output dat file `data/MCEGENpi_radcoor.dat`

    2| convert output dat file `data/OnePion...`
    
    The output root file with same name of dat_file can be found under the directory of dat_file

### 2. For ep generator

#### How to compile
1. cd `gen/epGen`
2. `make`

