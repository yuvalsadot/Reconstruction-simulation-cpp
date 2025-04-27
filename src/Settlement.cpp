#include "Settlement.h"


// constructor
Settlement::Settlement(const string &name, SettlementType type) : 
name(name),
type(type) {}

// getters
const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

// toString
string Settlement::settlementTypeToString(SettlementType type)
{
    switch (type)
    {
        case SettlementType::VILLAGE:
            return "village";
        case SettlementType::CITY:
            return "city";
        case SettlementType::METROPOLIS:
            return "metropolis";
        default: return "";
    }
}

const string Settlement::toString() const
{
    return name + " is a " + settlementTypeToString(type);
}