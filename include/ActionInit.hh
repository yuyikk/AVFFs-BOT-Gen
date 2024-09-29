#ifndef _ACTIONINIT_HH_
#define _ACTIONINIT_HH_
#include "G4VUserActionInitialization.hh"
class ActionInit : public G4VUserActionInitialization
{
    public:
        ActionInit() = default;
        ~ActionInit() override = default;
        void Build() const override;
        void BuildForMaster() const override;
        

};
#endif