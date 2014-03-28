#include "SignalsManager.hpp"
#include "Logger.hpp"

void SignalsManager::send(Signals signal)
{
    //Log("send:");
    //Log(static_cast<int>(signal));
    signals.insert(signal);
}


bool SignalsManager::receive(Signals signal)
{
    if(signals.count(signal) > 0){
//        Log("receive:");
//        Log(static_cast<int>(signal));
        signals.erase(signal);
        return true;
    }
    return false;
}

