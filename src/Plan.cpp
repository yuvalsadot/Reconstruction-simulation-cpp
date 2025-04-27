#include "Plan.h"
#include <iostream>
using namespace std;

// constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) : 
plan_id(planId),
settlement(settlement),
selectionPolicy(selectionPolicy),
status(PlanStatus::AVALIABLE),
facilities(),
underConstruction(),
facilityOptions(facilityOptions),
life_quality_score(0),
economy_score(0),
environment_score(0){}

// .........Rule Of 3.........

// copy constructor
Plan::Plan(const Plan& other) :
plan_id(other.plan_id),
settlement(other.settlement),
selectionPolicy(other.selectionPolicy->clone()),
status(other.status),
facilities(),
underConstruction(),
facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score),
economy_score(other.economy_score),
environment_score(other.environment_score)
{
    for (Facility* facility : other.facilities)
    {
        Facility* fac = new Facility(*facility);
        facilities.emplace_back(fac);
    }
    for (Facility* facility : other.underConstruction)
    {
        Facility* fac = new Facility(*facility);
        underConstruction.emplace_back(fac);
    }    
}

// destructor
Plan::~Plan() {
    // Clean up facilities
    for (Facility* facility : facilities) 
        delete facility;
    facilities.clear();
    // Clean up underConstruction
    for (Facility* facility : underConstruction) 
        delete facility;
    underConstruction.clear();
    // Clean up pointer    
    delete selectionPolicy;
}

// move constructor
Plan::Plan(Plan &&other) :
plan_id(other.plan_id),
settlement(other.settlement),
selectionPolicy(other.selectionPolicy),
status(other.status),
facilities(move(other.facilities)),
underConstruction(move(other.underConstruction)),
facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score),
economy_score(other.economy_score),
environment_score(other.environment_score)
{
    other.selectionPolicy = nullptr;
    other.facilities.clear();
    other.underConstruction.clear();
}

// getters
const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

const string Plan::getSelectionPolicy() const
{
    // get the last 3 chars of the selection policy
    return selectionPolicy->toString().substr(selectionPolicy->toString().length() - 3);
}

// setter
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    SelectionPolicy* prev = this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
    delete prev;
}

// step
void Plan::step()
{ 
    // stage 1
    if (status == PlanStatus::AVALIABLE) 
    {
        // stage 2
        while (underConstruction.size() < static_cast<size_t>(settlement.getType()) + 1)
        {
            Facility *fac = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
            addFacility (fac);
        }
    }
    // stage 3
    for (size_t i = 0; i < underConstruction.size(); /* no increment here */)
    {
        // preform step and use it's returned value to decide what to do 
        if (underConstruction[i]->step() == FacilityStatus::OPERATIONAL) 
        {
            addFacility(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
            //  Do not increment i, as the next element has shifted into the current position
        }
        else
        {
            i++; // Increment only if no element was erased
        }
    }
    // stage 4
    if (underConstruction.size() == static_cast<size_t>(settlement.getType()) + 1)
        status = PlanStatus::BUSY;
    else 
        status = PlanStatus::AVALIABLE;
}

// other methods
void Plan::printStatus()
{
    cout << "PlanStatus: " << static_cast<int>(status) << endl;
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

void Plan::addFacility(Facility *facility)
{
    // add facility to the right vector
    if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) 
    {
        underConstruction.emplace_back(facility);
    }
    else 
    {
        facilities.emplace_back(facility);
        // update score fields
        life_quality_score += facility->getLifeQualityScore();
        economy_score += facility->getEconomyScore();
        environment_score += facility->getEnvironmentScore(); 
    }
}

// toString
const string Plan::toString1() const
{
    string str2ret = "PlanID: " + to_string(plan_id) + "\nSettlementName: " + settlement.getName() + "\nStatus: ";
    if (status == PlanStatus::AVALIABLE)
        str2ret += "AVALIABLE";
    else
        str2ret +="BUSY";
    str2ret += "\n" + selectionPolicy->toString();
    str2ret += "\nLifeQualityScore: " + to_string(life_quality_score) + "\nEconomyScore: " + to_string(economy_score) + "\nEnvrionmentScore: " + to_string(environment_score) + "\n";
    for (Facility* facility : underConstruction) 
    {
        str2ret += facility->toString() + "\n";
    }
    for (Facility* facility : facilities) 
    {
        str2ret += facility->toString() + "\n";
    }
    return str2ret;
}

const string Plan::toString2() const
{
    return "PlanID: " + to_string(plan_id) + "\nSettlementName: " + settlement.getName() + "\nLifeQualityScore: " + to_string(life_quality_score) + "\nEconomyScore: " + to_string(economy_score) + "\nEnvrionmentScore: " + to_string(environment_score) + "\n"; 
}