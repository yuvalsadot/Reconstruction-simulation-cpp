#include "Action.h"

BaseAction::BaseAction() :
errorMsg(),
status(ActionStatus::ERROR) {}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    cout << "Error: " + errorMsg << endl;
}

const string BaseAction::status2String() const
{
    if (status == ActionStatus::ERROR)
        return " ERROR";
    else 
        return " COMPLETED";
}

// getters
ActionStatus BaseAction::getStatus() const
{
    return status;
}

const string& BaseAction::getErrorMsg() const
{
    return errorMsg;
}

// .....................SimulateStep.....................
SimulateStep::SimulateStep(const int numOfSteps): 
BaseAction(),
numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; i++)
        simulation.step();
    complete();
}

const string SimulateStep::toString() const
{
    return "step " + to_string(numOfSteps) + status2String() + "\n";
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this); // uses default copy consructor 
}

// .....................AddPlan.....................
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : 
BaseAction(),
settlementName(settlementName),
selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation)
{
    if (!simulation.isSettlementExists(settlementName))
        error("The settlement doesn't exist");
    else {
        SelectionPolicy *currPolicy;
        if (selectionPolicy == "nve")
            currPolicy = new NaiveSelection();
        else if (selectionPolicy == "bal")
            currPolicy = new BalancedSelection(0, 0, 0);
        else if (selectionPolicy == "eco")
            currPolicy = new EconomySelection();
        else if (selectionPolicy == "env")
            currPolicy = new SustainabilitySelection();
        else
        {
            error("The selection policy doesn't exist");
            return;
        }
        simulation.addPlan(simulation.getSettlement(settlementName),currPolicy);
        complete();
    }
}

const string AddPlan::toString() const
{
    return "plan " + settlementName + " " + selectionPolicy + status2String() + "\n";
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this); // uses default copy consructor
}

// .....................AddSettlement.....................
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : 
BaseAction(),
settlementName(settlementName),
settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation)
{
    if (simulation.isSettlementExists(settlementName))
        error("The settlement is already exists");
    else
    {
        simulation.addSettlement(new Settlement(settlementName, settlementType));
        complete();
    }
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this); // uses default copy consructor
}

const string AddSettlement::toString() const
{
    string str2ret = "settlemnet " + settlementName;
    if (settlementType == SettlementType::VILLAGE)
        str2ret += " 0";
    else if (settlementType == SettlementType::CITY)
        str2ret += " 1";
    else if (settlementType == SettlementType::METROPOLIS)
        str2ret += " 2";
    return str2ret + status2String() + "\n";
}

// .....................AddFacility.....................
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) :
BaseAction(),
facilityName(facilityName),
facilityCategory(facilityCategory),
price(price),
lifeQualityScore(lifeQualityScore),
economyScore(economyScore),
environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation)
{
    if (!simulation.addFacility(FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore)))
        error("The facility is already exists");
    else
        complete();
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this); // uses default copy consructor
}

const string AddFacility::toString() const
{
    string str2ret = "facility " + facilityName;
    if (facilityCategory == FacilityCategory::LIFE_QUALITY)
        str2ret += " 0";
    else if (facilityCategory == FacilityCategory::ECONOMY)
        str2ret += " 1";
    else if (facilityCategory == FacilityCategory::ENVIRONMENT)
        str2ret += " 2";
    str2ret += " " + to_string(price) + " " + to_string(lifeQualityScore) + " " + to_string(economyScore) + " " + to_string(environmentScore) + status2String() + "\n";
    return str2ret;
}

// .....................PrintPlanStatus.....................
PrintPlanStatus::PrintPlanStatus(int planId) :
BaseAction(),
planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation)
{
    if (!simulation.isPlanExists(planId))
        error("The plan doesn't exist");
    else
    {
        cout << simulation.getPlan(planId).toString1() << endl;
        complete();
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this); // uses default copy consructor
}

const string PrintPlanStatus::toString() const
{
    return "planStatus " + to_string(planId) + (status2String()) + "\n";
}

// .....................ChangePlanPolicy.....................
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) :
BaseAction(),
planId(planId),
newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation)
{
    string currPolicy = simulation.getPlan(planId).getSelectionPolicy();
    if (!simulation.isPlanExists(planId) || newPolicy == currPolicy)
        error("Cannot change selection policy");
    else
    {
        
        SelectionPolicy *newSelPolicy = nullptr;
        if (newPolicy == "nve")
            newSelPolicy = new NaiveSelection();
        else if (newPolicy == "bal")
            newSelPolicy = new BalancedSelection(simulation.getPlan(planId).getlifeQualityScore(), simulation.getPlan(planId).getEconomyScore(), simulation.getPlan(planId).getEnvironmentScore());
        else if (newPolicy == "eco")
            newSelPolicy = new EconomySelection();
        else if (newPolicy == "env")
            newSelPolicy = new SustainabilitySelection(); 
        simulation.getPlan(planId).setSelectionPolicy(newSelPolicy);
        cout << "planID: " + to_string(planId) + "\npreviousPolicy: " + currPolicy + "\nnewPolicy: " + newPolicy << endl;        
        complete();
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this); // uses default copy consructor
}

const string ChangePlanPolicy::toString() const
{
    return "changePolicy " + to_string(planId) + " " + newPolicy + status2String() + "\n";
}

// .....................PrintActionsLog.....................
PrintActionsLog::PrintActionsLog() :
BaseAction() {}

void PrintActionsLog::act(Simulation &simulation)
{
    for (BaseAction* currAct : simulation.getActionsLog())
    {
        cout << currAct->toString() << endl;
    }
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this); // uses default copy consructor
}

const string PrintActionsLog::toString() const
{
    return "log";
}

// .....................Close.....................
Close::Close() :
BaseAction() {}

void Close::act(Simulation &simulation)
{
    for(int i = 0; i < simulation.getPlanCounter(); i++)
        cout << simulation.getPlan(i).toString2() << endl;
    simulation.close();
}

Close *Close::clone() const
{
    return new Close(*this); // uses default copy consructor
}

const string Close::toString() const
{
    return "The simulation has finished";
}

// .....................BackupSimulation.....................
BackupSimulation::BackupSimulation() :
BaseAction() {}

void BackupSimulation::act(Simulation &simulation)
{
    if(backup != nullptr)
        delete backup;    
    backup = new Simulation(simulation);
    complete();
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this); // uses default copy consructor
}

const string BackupSimulation::toString() const
{
    return "backup " + status2String() + "\n";
}

// .....................RestoreSimulation.....................
RestoreSimulation::RestoreSimulation() :
BaseAction() {}

void RestoreSimulation::act(Simulation &simulation)
{  
    if(backup == nullptr)
        error("No backup available");
    else
        simulation.restore();
    complete();
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this); // uses default copy consructor
}

const string RestoreSimulation::toString() const
{
    return "restore " + status2String() + "\n";
}