#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &facilityName);
        bool isPlanExists(const int planId);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        const int getPlanCounter();
        const vector<BaseAction*> &getActionsLog();
        void step();
        void close();
        void open();
        void restore();
        // RULE OF 5
        Simulation(const Simulation& other); // copy constructor
        ~Simulation(); // destructor
        Simulation& operator=(const Simulation& other); //copy assignment operator
        Simulation(Simulation&& other); // move constructor
        Simulation& operator=(Simulation&& other); // move assignment opertor

    private:
        SettlementType string2settType (string input);
        FacilityCategory string2facCategory (string input);
        BaseAction* checkAction(vector<string> userInput);
        bool isRunning;
        bool isCurrActLogOrCls;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};

extern Simulation* backup;