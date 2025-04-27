#include "SelectionPolicy.h"
#include <cmath>
#include <limits>

//.........................NaiveSelection.........................
// Constructor
NaiveSelection::NaiveSelection() : 
lastSelectedIndex(0) {}

// selectFacility
const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int currIndex = lastSelectedIndex;
    // update class field
    if(static_cast<size_t>(lastSelectedIndex) == facilitiesOptions.size())
        lastSelectedIndex = 0;
    else
        lastSelectedIndex++;
    return facilitiesOptions[currIndex];
}

// toString
const string NaiveSelection::toString() const
{
    return "SelectionPolicy: nve"; 
}

// clone
NaiveSelection *NaiveSelection::clone() const 
{
    return new NaiveSelection(*this); // uses default copy consructor 
}

//.........................BalancedSelection.........................
// Constructor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) :
LifeQualityScore(LifeQualityScore),
EconomyScore(EconomyScore),
EnvironmentScore(EnvironmentScore) {}

// Calculate max and min scores
static int MinScore(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
{
    return std::min(std::min(LifeQualityScore, EconomyScore), EnvironmentScore);
}

static int MaxScore(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
{
    return std::max(std::max(LifeQualityScore, EconomyScore), EnvironmentScore);
}        

// selectFacility
const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int minDistance = std::numeric_limits<int>::max(); // MAX_INTEGER
    int minFacIndex = 0;
    for (size_t i = 0; i < facilitiesOptions.size(); i++)
    {
        // Calculate max and min scores
        int maxScore = MaxScore(LifeQualityScore + facilitiesOptions[i].getLifeQualityScore(), EconomyScore + facilitiesOptions[i].getEconomyScore(), EnvironmentScore + facilitiesOptions[i].getEnvironmentScore());
        int minScore = MinScore(LifeQualityScore + facilitiesOptions[i].getLifeQualityScore(), EconomyScore + facilitiesOptions[i].getEconomyScore(), EnvironmentScore + facilitiesOptions[i].getEnvironmentScore());
        int currDistance = maxScore - minScore;
        // Check for the current minimum distance and update the index
        if (currDistance < minDistance)
        {
            minFacIndex = i;
            minDistance = currDistance;
        }
    }
    // update class fields
    LifeQualityScore = LifeQualityScore + facilitiesOptions[minFacIndex].getLifeQualityScore();
    EconomyScore = EconomyScore + facilitiesOptions[minFacIndex].getEconomyScore();
    EnvironmentScore = EnvironmentScore + facilitiesOptions[minFacIndex].getEnvironmentScore();
    // Return the facility with the minimum distance
    return facilitiesOptions[minFacIndex];
}

// toString
const string BalancedSelection::toString() const
{
    return "SelectionPolicy: bal";
}

// clone
BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(*this); // uses default copy consructor 
}

//.........................EconomySelection.........................
// Constructor
EconomySelection::EconomySelection() :
lastSelectedIndex(-1) {}

// selectFacility
const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    size_t i = (static_cast<size_t>(lastSelectedIndex) + 1) % facilitiesOptions.size();
    // Search for the next ECO facility 
    while (facilitiesOptions[i].getCategory() != FacilityCategory::ECONOMY) 
    {
        if (i == facilitiesOptions.size())
            i = 0;
        else 
            i = (i + 1) % facilitiesOptions.size();    
    }
    lastSelectedIndex = i;
    return facilitiesOptions[i];
}

// toString
const string EconomySelection::toString() const
{
    return "SelectionPolicy: eco";
}

// clone
EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this); // uses default copy consructor 
}

//.........................SustainabilitySelection.........................
// Constructor
SustainabilitySelection::SustainabilitySelection() :
lastSelectedIndex(-1) {}

// selectFacility
const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    size_t i = (static_cast<size_t>(lastSelectedIndex) + 1) % facilitiesOptions.size();
    // Search for the next ENV facility 
    while (facilitiesOptions[i].getCategory() != FacilityCategory::ENVIRONMENT)
    {
        if (i == facilitiesOptions.size())
            i = 0;
        else 
            i = (i + 1) % facilitiesOptions.size();    
    }
    lastSelectedIndex = i;
    return facilitiesOptions[i];
}

// toString
const string SustainabilitySelection::toString() const
{
    return "SelectionPolicy: env";
}

// clone
SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this); // uses default copy consructor 
}