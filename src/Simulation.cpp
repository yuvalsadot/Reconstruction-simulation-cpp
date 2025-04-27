#include <fstream>   // For std::ifstream
#include <iostream>  // For std::cerr
#include <vector>    // For std::vector
#include <string>    // For std::string
#include <sstream>   // For std::istringstream
#include "Simulation.h" 
#include "Auxiliary.h"
#include "Action.h"
Simulation* backup = nullptr;

// constructor
Simulation::Simulation(const string &configFilePath) :
isRunning(false),
isCurrActLogOrCls(false),
planCounter(0),
actionsLog(),
plans(),
settlements(),
facilitiesOptions()
{
    // read config file
    ifstream configFile(configFilePath); 
    string line;
    while (getline(configFile, line)) 
    {
        if (line.empty() || line[0] == '#') 
            continue; // Skip comments and empty lines
        vector<string> args = Auxiliary::parseArguments(line); 
        string command = args[0];
        if (command == "settlement")
            settlements.emplace_back(new Settlement(args[1], SettlementType(stoi(args[2]))));
        else if (command == "facility")
            facilitiesOptions.emplace_back(FacilityType(args[1], FacilityCategory(stoi(args[2])), stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6])));
        else if (command == "plan") 
        {
            string currSetName = args[1]; 
            int currSet;
            bool isFound = false;
            for (size_t i = 0; (i < settlements.size()) & (!isFound); i++)
            {
                if (currSetName == settlements[i]->getName())
                {
                    isFound = true;
                    currSet = i;
                }
            }
            SelectionPolicy *currPolicy = nullptr;
            if (args[2] == "nve")
                currPolicy = new NaiveSelection();
            else if (args[2] == "bal")
                currPolicy = new BalancedSelection(0, 0, 0);
            else if (args[2] == "eco")
                currPolicy = new EconomySelection();
            else if (args[2] == "env")
                currPolicy = new SustainabilitySelection();
            Plan p = Plan(planCounter, *settlements[currSet], currPolicy, facilitiesOptions);
            plans.emplace_back(p);
            planCounter++;  
        }
    }
}

// copy constructor
Simulation::Simulation(const Simulation &other) :
isRunning(other.isRunning),
isCurrActLogOrCls(other.isCurrActLogOrCls),
planCounter(other.planCounter),
actionsLog(),
plans(other.plans), // no need to deep copy, Plan implements RO5
settlements(),
facilitiesOptions(other.facilitiesOptions)
{
    // deep copy actionsLog and settlements
    for (BaseAction* action : other.actionsLog)
        actionsLog.emplace_back(action->clone());
    for (Settlement* settlement : other.settlements)
        settlements.emplace_back(new Settlement(*settlement));
}

// destructor
Simulation::~Simulation() 
{
    for (Settlement* settlement : settlements)
        if (settlement != nullptr)
            delete settlement;
    settlements.clear(); // Clear the vector after deleting objects
    for (BaseAction* action : actionsLog)
        delete action;
    actionsLog.clear();
}

// copy assignment operator
Simulation& Simulation::operator =(const Simulation& other)
{
    // case of self-assignment
    if (this == &other)
        return *this;
    // free resources
    for (size_t i = 0; i < settlements.size() ; i++)
    {
        bool setFound = false;
        for (size_t j = 0; (j < other.settlements.size()) & (!setFound); j++)
            if (settlements[i]->getName() == other.settlements[j]->getName())
                setFound = true;
        if (!setFound)
        {
            delete settlements[i];
            settlements[i] = nullptr;
        }
    }
    for (BaseAction* action : actionsLog)
        delete action;
    actionsLog.clear();
    plans.clear();
    facilitiesOptions.clear();
    // copy primitive fields
    isRunning = other.isRunning;
    isCurrActLogOrCls = other.isCurrActLogOrCls;
    planCounter = other.planCounter;
    // deep copy plans
    for (const Plan& plan : other.plans)
        plans.emplace_back(plan); // use Plan's copy constructor
    // deep copy facilitiesOptions
    for (const FacilityType& facility : other.facilitiesOptions)
        facilitiesOptions.emplace_back(FacilityType(facility.getName(), facility.getCategory(), facility.getCost(), facility.getLifeQualityScore(), facility.getEconomyScore(), facility.getEnvironmentScore()));
    // deep copy actionsLog
    for (const BaseAction* action : other.actionsLog)
        actionsLog.emplace_back(action->clone());
    return *this;
}

// move constructor
Simulation::Simulation(Simulation&& other) :
isRunning(other.isRunning),
isCurrActLogOrCls(other.isCurrActLogOrCls),
planCounter(other.planCounter),
actionsLog(move(other.actionsLog)),
plans(move(other.plans)), // no need to deep copy, Plan implements RO5
settlements(move(other.settlements)),
facilitiesOptions(move(other.facilitiesOptions))
{
    other.isRunning = false;
    other.isCurrActLogOrCls = false;
    other.planCounter = 0;
}

// move assignment opertor
Simulation &Simulation::operator=(Simulation &&other)
{
    // case of self-assignment
    if (this == & other)
        return *this;
    // free resources
    for (size_t i = 0; i < settlements.size() ; i++)
    {
        bool setFound = false;
        for (size_t j = 0; (j < other.settlements.size()) & (!setFound); j++)
            if (settlements[i]->getName() == other.settlements[j]->getName())
                setFound = true;
        if (!setFound)
            delete settlements[i];
    }
    for (BaseAction* action : actionsLog)
        delete action;
    actionsLog.clear();
    plans.clear();
    facilitiesOptions.clear();
    // move fields
    isRunning = other.isRunning;
    isCurrActLogOrCls = other.isCurrActLogOrCls;
    planCounter = other.planCounter;
    plans = move(other.plans);
    facilitiesOptions = move(other.facilitiesOptions);
    actionsLog = move(other.actionsLog);
    settlements = move(other.settlements);
    // reset other
    other.isRunning = false;
    other.isCurrActLogOrCls = false;
    other.planCounter = 0;
    return *this;
}

// simulate the program
void Simulation::start()
{ 
    open();
    while (isRunning)
    {
        cout << "Enter command:" << endl;
        string input;
        getline(std::cin, input);
        vector<string> userInput = Auxiliary::parseArguments(input);        BaseAction *currAction = checkAction(userInput);
        currAction->act(*this);
        if(!isCurrActLogOrCls) // don't log 'print' and 'close' actions
            actionsLog.emplace_back(currAction);
        else
        {
            isCurrActLogOrCls = false;
            delete currAction;
        }
    }
}

BaseAction* Simulation::checkAction(vector<string> userInput)
{
    string firstWord = userInput[0];
    if (firstWord == "step")
    {
        return new SimulateStep(stoi(userInput[1]));
    }
    else if (firstWord == "plan")
    {
        return new AddPlan(userInput[1], userInput[2]);
    }
    else if (firstWord == "settlement")
    {
        SettlementType currType = string2settType(userInput[2]);
        return new AddSettlement(userInput[1],currType); 
    }
    else if (firstWord == "facility")
    {
        FacilityCategory currCategory = string2facCategory(userInput[2]);
        return new AddFacility(userInput[1], currCategory, stoi(userInput[3]), stoi(userInput[4]), stoi(userInput[5]), stoi(userInput[6]));
    }
    else if (firstWord == "planStatus")
    {
        return new PrintPlanStatus(stoi(userInput[1]));
    }
    else if (firstWord == "changePolicy")
    {
        return new ChangePlanPolicy(stoi(userInput[1]), userInput[2]);
    }
    else if (firstWord == "log")
    {
        isCurrActLogOrCls = true;
        return new PrintActionsLog();
    }
    else if (firstWord == "close")
    {
        isCurrActLogOrCls = true;
        return new Close();
    }
    else if (firstWord == "backup")
    {
        return new BackupSimulation();
    }
    else if (firstWord == "restore")
    {
        return new RestoreSimulation();
    }
    else 
    {
        return nullptr;
    }
}

// adders
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    Plan p = Plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.emplace_back(p);
    planCounter++;
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.emplace_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{
    if (!isSettlementExists(settlement->getName()))
    {
        settlements.emplace_back(settlement);
        return true;        
    }
    else
        return false;
}

bool Simulation::addFacility(FacilityType facility)
{
    if (!isFacilityExists(facility.getName()))
    {
        facilitiesOptions.emplace_back(facility);
        return true;
    }
    else
        return false;
}

// auxiliary functions
SettlementType Simulation::string2settType (string input)
{
    if (stoi(input) == 0)
        return SettlementType::VILLAGE;
    else if (stoi(input) == 1)
        return SettlementType::CITY;
    else
        return SettlementType::METROPOLIS;
}

FacilityCategory Simulation::string2facCategory (string input)
{
    if (stoi(input) == 0)
        return FacilityCategory::LIFE_QUALITY;
    else if (stoi(input) == 1)
        return FacilityCategory::ECONOMY;
    else
        return FacilityCategory::ENVIRONMENT;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    bool isExist = false; 
    for (size_t i = 0; (i < settlements.size()) & (!isExist); i++)
        if (settlementName == settlements[i]->getName())
            isExist = true;
    return isExist;
}

bool Simulation::isFacilityExists(const string &facilityName)
{
    bool isExist = false; 
        for (size_t i = 0; (i < facilitiesOptions.size()) & (!isExist); i++)
            if (facilityName == facilitiesOptions[i].getName())
                isExist = true;
    return isExist;
}

bool Simulation::isPlanExists(const int planId)
{
    if ((planId >= planCounter) | (planId < 0))
        return false;
    else
        return true;
}

// getters
Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (Settlement *settlement : settlements)
        if (settlementName == settlement->getName())
            return *settlement;
    static Settlement defualtSet("noSet", SettlementType::VILLAGE);
    return defualtSet;
}

Plan &Simulation::getPlan(const int planID)
{
    return plans[planID];
}

const int Simulation::getPlanCounter()
{
    return planCounter;
}

const vector<BaseAction *> &Simulation::getActionsLog()
{
    return actionsLog;
}

// step
void Simulation::step()
{
    for (Plan &plan : plans)
        plan.step();
}

void Simulation::close()
{
    isRunning = false;
    cout << "The simulation has ended." << endl;
}

void Simulation::open()
{
    cout << "The simulation has started" << endl;
    isRunning = true;
}

void Simulation::restore()
{
    *this = *backup; // copy assignment operator of Simulation
}