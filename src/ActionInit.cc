#include "ActionInit.hh"
// #include "RunAction.hh"
#include "EventAction.hh"
#include "PrimaryGenerator.hh"
void ActionInit::BuildForMaster() const
{
    EventAction *eventAction = new EventAction;
    // SetUserAction(new RunAction(eventAction));
}
void ActionInit::Build() const
{
    PrimaryGenerator *generator = new PrimaryGenerator();
    SetUserAction(generator);
    EventAction *eventAction = new EventAction();
    SetUserAction(eventAction);
    // RunAction *runAction = new RunAction(eventAction);
    // SetUserAction(runAction);
}
