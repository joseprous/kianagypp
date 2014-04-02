#include "SignalsManager.hpp"
#include "Logger.hpp"

void SignalsManager::send(Signals signal)
{
    signals.insert(signal);
}


bool SignalsManager::receive(Signals signal)
{
    if(signals.count(signal) > 0){
        signals.erase(signal);
        return true;
    }
    return false;
}

void SignalsManager::sendf(Signals signal, float value)
{
    signals.insert(signal);
    valuesf[signal] = value;
}

void SignalsManager::sendb(Signals signal, bool value)
{
    signals.insert(signal);
    valuesb[signal] = value;
}

boost::optional<float> SignalsManager::receivef(Signals signal)
{
    if(signals.count(signal) > 0){
        signals.erase(signal);
        return boost::optional<float>(valuesf[signal]);
    }
    return boost::optional<float>();    
}

boost::optional<bool> SignalsManager::receiveb(Signals signal)
{
    if(signals.count(signal) > 0){
        signals.erase(signal);
        return boost::optional<bool>(valuesb[signal]);
    }
    return boost::optional<bool>();    
}
