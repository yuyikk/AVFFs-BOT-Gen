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
2. `./doSimu generator_root_file runnumber`. 

    The `doSimu` accepts two arguements, the first is the root file from generator and the second is the runnumber that you can set freely.
    
    You will get a root file `runnumber.BOT.root` in the current directory, in which a tree is stored and also a histogram storing number of generated enents. 

## About Generator
You can find two directories under directory `gen`: `Converter` and `epGen`
### 1. For Converter
It convert the dat file produced from generator to root files used as input in the G4 simulation. You can find examples of dat files in the directory `Converter/data`.
#### How to use
1. cd Converter
2. `./compile`
3. `./AVFFInputFileConverter.exe dat_file type`

    dat_file: The output from generator
    
    type: it can be 0, 1 or 2

    0: convert output from `epGen`

    1: CONVERT output dat file `data/MCEGENpi_radcoor.dat`

    2: convert output dat file `data/OnePion...`
    
    The output root file with same name of dat_file can be found under the directory of dat_file

### 2. For ep generator

#### How to compile
1. cd `gen/epGen`
2. `make`
