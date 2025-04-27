#include "Facility.h"

// construcrtor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) : 
name(name),
category(category),
price(price),
lifeQuality_score(lifeQuality_score),
economy_score(economy_score),
environment_score(environment_score) {}

// getters
const string &FacilityType::getName() const
{
    return name;
}

int FacilityType::getCost() const
{
    return price;
}

int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const
{
    return environment_score;
}

int FacilityType::getEconomyScore() const
{
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const
{
    return category;
}

// .....................Facility.....................

// construcrtors
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) :
FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
settlementName(settlementName),
status(FacilityStatus::UNDER_CONSTRUCTIONS),
timeLeft(price) {}

Facility::Facility(const FacilityType &type, const string &settlementName):
FacilityType(type),
settlementName(settlementName),
status(FacilityStatus::UNDER_CONSTRUCTIONS),
timeLeft(type.getCost()) {}

// getters
const string &Facility::getSettlementName() const
{
    return settlementName;
}

const int Facility::getTimeLeft() const
{
    return timeLeft;
}

const FacilityStatus &Facility::getStatus() const
{
    return status;
}

// setter
void Facility::setStatus(FacilityStatus status)
{
    this->status = status;
}

// step
FacilityStatus Facility::step()
{
    timeLeft--;
    if (timeLeft == 0)
    {
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return status;
}

// toString
string Facility::categoryToString(FacilityCategory category)
{
    switch (category)
    {
        case FacilityCategory::LIFE_QUALITY:
            return "life quality";
        case FacilityCategory::ECONOMY:
            return "economy";
        case FacilityCategory::ENVIRONMENT:
            return "environment";
        default: return "";
    }
}

const string Facility::toString() const
{
    string str2ret = "FacilityName: " + name + "\nFacilityStatus: ";
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS)
        str2ret += "UNDER_CONSTRUCTIONS";
    else 
        str2ret += "OPERATIONAL";
    return str2ret;
}