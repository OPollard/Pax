// Copyright of Night Owls 2020 - inclusive © 


#include "SaveData.h"
#include "Constants.h"


USaveData::USaveData()
{
	PlayerName = "";
	PlayerRank = ERank::BEGINNER;
	LifeTimeEarnings = 0;
	PaxFlown = 0;
}


// UPDATE : Change PlayerRank Based on Amount of Earnings/Pax etc
void USaveData::UpdateRank()
{
	if (LifeTimeEarnings < BEGINNER_CAP) PlayerRank = ERank::BEGINNER;
	else if (LifeTimeEarnings < INTERMEDIATE_CAP) PlayerRank = ERank::BEGINNER;
	else if (LifeTimeEarnings < EXPERT_CAP)  PlayerRank = ERank::INTERMEDIATE;
	else if (LifeTimeEarnings < SUPERHERO_CAP) PlayerRank = ERank::EXPERT;
	else PlayerRank = ERank::SUPERHERO;
}

// Reset all saved data to zero
void USaveData::Reset()
{
	LifeTimeEarnings = 0;
	PaxFlown = 0;
}