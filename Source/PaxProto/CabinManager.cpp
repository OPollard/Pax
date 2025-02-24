// Copyright of Codebird Ltd 2020 - inclusive © 

#include "CabinManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Kismet/KismetTextLibrary.h"
#include "Constants.h"
#include "Pax.h"
#include "PaxState.h"
#include "Door.h"
#include "Seat.h"
#include "Toilet.h"
#include "WaitingArea.h"
#include "PaxSpawner.h"
#include "Pax_AIController.h"

ACabinManager::ACabinManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACabinManager::BeginPlay()
{
	Super::BeginPlay();

	//Redirects equivalent begin play function to enable other actors to spawn first
	GetWorldTimerManager().SetTimer(ManagerHandle, this, &ACabinManager::Initialise, 1.0f, false, 0.0f);
}

void ACabinManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Initialise function, delayed to ensure other actors are spawned
void ACabinManager::Initialise()
{
	//Get Door Actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), Doors);
	for (auto& D : Doors)
	{
		if (ADoor* DPtr = Cast<ADoor>(D))
		{	
			//single delegate binding from door
			DPtr->DoorMoved.BindDynamic(this, &ACabinManager::CheckDoorStatus); 
		}
	}

	//Get Seat Actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASeat::StaticClass(), Seats);
	NumSeatsInLevel = Seats.Num();

	//Get Toilet Actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AToilet::StaticClass(), Toilets);

	//Get Waiting Area Actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaitingArea::StaticClass(), WaitingAreas);

	//Game play Timer Tracker
	GetWorldTimerManager().SetTimer(LevelTimeHandle, this, &ACabinManager::TimerTick, 1.0f, true, 1.0f);

	//Variables
	FlightPhase = EPhase::BOARDING;
	LevelBoardingTime = FREEFLIGHT_BOARDINGTIME;
	LevelFlightTime = FREEFLIGHT_CRUISETIME;
	RechargeTime = 0.0f;
	
	//Assign pointer to pax spawner in level
	PaxSpawner = Cast<APaxSpawner>(UGameplayStatics::GetActorOfClass(this, APaxSpawner::StaticClass()));

	//Spawn Actors in SpawnArea Based on Number of Seats
	for (int32 i = 0; i < NumSeatsInLevel; i++)
	{
		//Make sure the line above worked
		if (PaxSpawner)
		{
			//Gets a random spawn vector location in the bounding box on the pax spawner.
			FVector Location;
			PaxSpawner->GetSpawnPoint(NumSeatsInLevel, Location);
			FRotator Rotator;
			FActorSpawnParameters SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; //ensure player always has the correct number of pax to play the leve

			//Spawn a pax from the subclass selected in blueprints, the variable is in the Public .h part Tsubclassof etc
			Passengers.Push(GetWorld()->SpawnActor<APax>(Pax, Location, Rotator, SpawnParam));
		}
	}
	
	//Accummulate the total available money to be earned in this game. Display.
	for (auto p : Passengers)
	{
		if (p)
		{
			SetTotalTargetMoney(this->GetTotalTargetMoney() + p->FindComponentByClass<UPaxState>()->GetMoney());
		}
	}

	//Set level targets
	FirstClassTarget = GetTotalTargetMoney() * FIRSTCLASS_RATIO;
	BusinessTarget = GetTotalTargetMoney() * BUSINESS_RATIO;
	EconomyTarget = GetTotalTargetMoney() * ECONOMY_RATIO;
	//Fetch so the GAME UI works
	SetTotalEarnableMoney(0);

}

//Fetch Remaining SegmentTime
float ACabinManager::GetRemainingTime()const
{
	if (FlightPhase == EPhase::BOARDING)
		return LevelBoardingTime;
	else if (FlightPhase == EPhase::CRUISE)
		return LevelFlightTime;
	else return 1901.0f; //duff error number
}

//non blueprint pure version
float ACabinManager::GetWarningTime()const
{
	return FREEFLIGHT_WARNINGTIME;
}

//Get amount of recharge time remaining
float ACabinManager::GetCurrentRechargeTime()const
{
	return RechargeTime;
}

//Return recharge time, getter of from constants
float ACabinManager::GetRechargeTime()const
{
	return SERVICE_RECHARGE_TIME;
}

//Reset recharge time
void ACabinManager::ResetRechargeTime()
{
	RechargeTime = 0.0f;
}

//Set maximum amount of money achievable in the level
void ACabinManager::SetTotalTargetMoney(const float Value)
{
	TotalTargetMoney = Value;
}
//Getters for targets
float ACabinManager::GetFirstClassTarget()const
{
	return FirstClassTarget;
}
float ACabinManager::GetBusinessTarget()const
{
	return BusinessTarget;
}
float ACabinManager::GetEconomyTarget()const
{
	return EconomyTarget;
}

//Return maximum amount of money achievable
float ACabinManager::GetTotalTargetMoney()const
{
	return TotalTargetMoney;
}

//Return total possible money earned on level
float ACabinManager::GetTotalEarnableMoney()const
{
	return TotalEarnableMoney;
}

// Returns money stat as FText format for the UI
FText ACabinManager::GetTotalEarnableMoneyAsText()const
{
	FString T_Money{ FString::SanitizeFloat(TotalEarnableMoney, 0) };
	FString T_Display{ "$" };
	T_Display.Append(T_Money);
	return UKismetTextLibrary::Conv_StringToText(T_Display);
}

//Set Total earnable money based on all pax personal money combined
void ACabinManager::SetTotalEarnableMoney()
{
	for (const auto& p : PassengerStates)
	{
		TotalEarnableMoney += p->GetMoney();
	}
}

//Set total earnable money to specific parameter amount
void ACabinManager::SetTotalEarnableMoney(const float X)
{
	TotalEarnableMoney = X;
}

void ACabinManager::SetTempPickUpAmount(const float X)
{
	TempPickUpAmount = X;
}

float ACabinManager::GetTempPickUpAmount()const
{
	return TempPickUpAmount;
}

//Used for UI, convert pickup amount to returnable FText type
FText ACabinManager::GetTempPickUpAmountAsText()const
{
	FString Pu_Money{ FString::SanitizeFloat(TempPickUpAmount, 0) };
	FString Pu_Display{};
	(TempPickUpAmount >= 0) ? Pu_Display.Append("+") : Pu_Display.Append("-");
	Pu_Display.Append("$");
	Pu_Display.Append(Pu_Money);
	return UKismetTextLibrary::Conv_StringToText(Pu_Display);
}


//set pickup UI for timer amount of 2.25 seconds
void ACabinManager::RunPickUpUI()
{
	EnablePickUpUI = true;
	GetWorldTimerManager().SetTimer(ManagerHandle, this, &ACabinManager::ResetPickUpUI, 1.0f, false, 2.25f);
}

void ACabinManager::ResetPickUpUI()
{
	EnablePickUpUI = false;
	TempPickUpAmount = 0;
}

bool ACabinManager::GetPickUpUIStatus()const
{
	return EnablePickUpUI;
}

//Called when event dispatched from pax state
void ACabinManager::PaxHasDied()
{
	//cycle through pax states on record
	for (const auto& PState : PassengerStates)
	{
		if (PState)
		{
			if (!PState->IsAlive()) //Check any particular pax is Alive
			{
				if (APax* P = Cast<APax>(PState->GetOwner())) //find its AActor
				{
					if (AController* PaxAICon = P->GetController()) //Get Its Controller (AI in this case)
					{
						PaxAICon->UnPossess();	//Remove any activation of that PAX

						//Punish player with death penalty
						TotalEarnableMoney += DEATH_PENALTY;
					}
				}
			}
		}
	}
}

//Called when pax starts sleeping
void ACabinManager::PaxIsSleeping()
{
	//cycle through pax to find the sleeping on or ones
	for (const auto& PState : PassengerStates)
	{
		if (PState)
		{
			//If any of the passenger states are tired
			if (PState->IsTired())
			{
				//get owning pawn to find controller 
				if (APax* P = Cast<APax>(PState->GetOwner()))
				{
					//find controller and cast to custom ai controller class
					if (APax_AIController* PaxAICon = Cast<APax_AIController>(P->GetController()))
					{
						//push onto the array of controllers
						TempAIControllers.Push(PaxAICon);
						PaxAICon->UnPossess(); //unpossess sleeping pax until timer is up
						//begin sleeping timer, then call wakepaxup
						GetWorldTimerManager().SetTimer(SleepTimeHandle, this, &ACabinManager::WakePaxUp, SLEEP_TIME, false);
					}
				}
			}
		}
	}
}

//called from paxissleeping to wake up pax and repossess
void ACabinManager::WakePaxUp()
{
	//clear timer
	GetWorldTimerManager().ClearTimer(SleepTimeHandle);
	//check for sleeping pax
	for (const auto& PState : PassengerStates)
	{
		if (PState)
		{
			//find sleeping pax
			if (PState->IsTired())
			{
				//reset state data
				PState->SetEnergy(100.0f);
				PState->SetAnnouncedSleep(false);

				//get owning pawn
				if (APax* P = Cast<APax>(PState->GetOwner()))
				{
					//ensure not null call
					if (TempAIControllers.Num() > 0)
					{
						//get next ai controller and GIVE it to the pax
						TempAIControllers.Pop()->Possess(P);
					}
				}
			}
		}
	}
}

//Register the new pax on manifest
void ACabinManager::RegisterNewPax(AActor* NewActor)
{
	if (UPaxState* NewPax = Cast<APax>(NewActor)->FindComponentByClass<UPaxState>())
	{
		//Add to array on manifest
		PassengerStates.Add(NewPax);
		++PaxOnboard; //add to the number of pax onboard, array.num() not working

		//Register to await for death
		NewPax->OnDeath.AddUniqueDynamic(this, &ACabinManager::PaxHasDied);

		//Register to await for sleep
		NewPax->Sleep.AddUniqueDynamic(this, &ACabinManager::PaxIsSleeping);
	
		NewPax->SetOnboard(true);
	}
}

//Get number of pax onboard
int32 ACabinManager::GetNumPaxOnBoard()const
{
	return PaxOnboard;
}

//Get number of seats in level
int32 ACabinManager::GetNumSeatsInLevel()const
{
	return NumSeatsInLevel;
}

//Get remaining seats
int32 ACabinManager::GetRemainingSeats()const
{
	return NumSeatsInLevel - PaxOnboard;
}

//Get percent of airplane seats occupied
float ACabinManager::GetLoadFactor() const
{

	if (NumSeatsInLevel)
	{
		const float LocalPax = PaxOnboard;
		const float LocalSeats = NumSeatsInLevel;
		const float LocalRatio = (LocalPax / LocalSeats) * 100.0f;
		
		return LocalRatio;
	}

	return 0.0f;
}

//get all the pax not secure, so liable to being thrown around
void ACabinManager::GetUnseatedPax(TArray<AActor*>& UnseatedPax)
{
	//search all states onboard
	for (const auto& ps : PassengerStates)
	{
		//find those not seated
		if (!ps->GetSitting())
		{
			//add to unseated list
			UnseatedPax.Push(ps->GetOwner());
			ps->GetOwner()->SetActorEnableCollision(false);
		}
	}
	//Punish score by the number of deaths
	TotalEarnableMoney += (DEATH_PENALTY * static_cast<float>(UnseatedPax.Num()));

	
}

//set a new current flight phase
void ACabinManager::SetCurrentPhase(const EPhase NewPhase)
{
	FlightPhase = NewPhase;
}

//return the current flight phase eg cruise
EPhase ACabinManager::GetCurrentPhase()const
{
	return FlightPhase;
}


// called once from door event delegate
void ACabinManager::CheckDoorStatus()
{
	//keep track of number of doors
	size_t Counter{ 0 };
	for (const auto& d : Doors)
	{
		if (ADoor* DPtr = Cast<ADoor>(d))
		{
			if (!DPtr->GetDoorStatus()) { ++Counter; } // is this door closed then add to number open
		}
	}
	if (Counter == Doors.Num()) //we want all closed
	{
		Launch(); //send to launch protocol
	}
}

//Decrease Level Timers
void ACabinManager::TimerTick()
{
	//if in boarding phase, tick boarding timer down
	//if timer = 0, launch
	if (FlightPhase == EPhase::BOARDING)
	{
		//If any time is left, decrease when called
		if (LevelBoardingTime) { LevelBoardingTime--; }
		else
		{
			//then check if we haven't launched already
			if (GoForLaunch)
			{
				Launch(); //send to launch protocol
				//GetWorldTimerManager().ClearTimer(LevelTimeHandle); //clear any timer on this
			}
		}
	}
	//if in cruising phase, tick cruising timer down
	//if timer = 0, land
	else if (FlightPhase == EPhase::CRUISE)
	{
		if (LevelFlightTime) { LevelFlightTime--; }
		else
		{
			Land();
		}
	}	
	else 
	{
		//left empty on purpose 
	}
	if (RechargeTime < SERVICE_RECHARGE_TIME)
	{
		RechargeTime++;
	}
}

//Pulse Launch Protocol
void ACabinManager::Launch()
{
	//Check not been launched already
	if(GoForLaunch)	TakeOffPulse.Broadcast();
	GoForLaunch = false; //prevent more launches

	for (auto p : Passengers)
	{
		//check p is not empty
		if (p)
		{
			//enable update state before checking if onboard, otherwise nullptr;
			p->SetEnableStateUpdate(true);
			if (UPaxState* Ps = p->FindComponentByClass<UPaxState>())
			{
				//if not OnBoard, get owning actor, and destroy
				if (!Ps->GetOnboard()) Ps->GetOwner()->Destroy();
			}
		}
	}
	//Set all areas which a pax is thrown from, to unoccupied
	for (auto t : Toilets)
	{
		Cast<AToilet>(t)->SetOccupied(false);
	}
	for (auto wa : WaitingAreas)
	{
		Cast<AWaitingArea>(wa)->SetOccupied(false);
	}
}

//Pulse Landing Protocol
void ACabinManager::Land()
{
	if (GoForLand) LandingPulse.Broadcast();
	GoForLand = false;
	GetWorldTimerManager().ClearTimer(LevelTimeHandle);//clear timer

	//stop stats sticking for all pax
	for (auto p : Passengers)
	{
		if (p)
		{
			p->SetEnableStateUpdate(false);
		}
	}
}
