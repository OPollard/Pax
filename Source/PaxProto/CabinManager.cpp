// Copyright of Night Owls 2020 - inclusive © 


#include "CabinManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "AIController.h"
#include "Kismet/KismetTextLibrary.h"

#include "Pax.h"
#include "PaxState.h"
#include "Door.h"
#include "Seat.h"

// Sets default values
ACabinManager::ACabinManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACabinManager::BeginPlay()
{
	Super::BeginPlay();

	//Redirects equivalent begin play function to enable other actors to spawn first
	GetWorldTimerManager().SetTimer(ManagerHandle, this, &ACabinManager::Initialise, 1.0f, false, 0.0f);
}

// Called every frame
void ACabinManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Decrease Level Timer
void ACabinManager::TimerTick()
{
	if (LevelBoardingTime) {LevelBoardingTime--;}
	else 
	{ 
		if (GoForLaunch)
		{
			Launch();
			GoForLaunch = false;
		}
	}
}

//Fetch Remaining SegmentTime
float ACabinManager::GetRemainingTime()
{
	return LevelBoardingTime;
}



//Return total possible money earnable on level
float ACabinManager::GetTotalEarnableMoney()
{
	return TotalEarnableMoney;
}

// Returns money stat as FText format for the UI
FText ACabinManager::GetTotalEarnableMoneyAsText()
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
void ACabinManager::SetTotalEarnableMoney(float x)
{
	TotalEarnableMoney = x;
}

void ACabinManager::SetTempPickUpAmount(float x)
{
	TempPickUpAmount = x;
}

float ACabinManager::GetTempPickUpAmount()
{
	return TempPickUpAmount;
}

FText ACabinManager::GetTempPickUpAmountAsText()
{
	FString PU_Money{ FString::SanitizeFloat(TempPickUpAmount, 0) };
	FString PU_Display{};
	(TempPickUpAmount >= 0) ? PU_Display.Append("+") : PU_Display.Append("-");
	PU_Display.Append("$");
	PU_Display.Append(PU_Money);
	return UKismetTextLibrary::Conv_StringToText(PU_Display);
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

bool ACabinManager::GetPickUpUIStatus()
{
	return EnablePickUpUI;
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
			DPtr->DoorMoved.BindDynamic(this, &ACabinManager::CheckDoorStatus); //single delegate binding from door
		}
	}

	//Get Seat Actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASeat::StaticClass(), Seats);
	NumSeatsInLevel = Seats.Num();
	UE_LOG(LogTemp, Warning, TEXT("%i"), NumSeatsInLevel);

	//Fetch so the GAME UI works
	SetTotalEarnableMoney(0);

	//Gameplay Timer Tracker
	GetWorldTimerManager().SetTimer(LevelTimeHandle, this, &ACabinManager::TimerTick, 1.0f, true, 1.0f);

}


//Called when event dispatched from pax state
void ACabinManager::PaxHasDied()
{
	//cycle through pax states on record
	for (const auto& PState : PassengerStates)
	{
		if (!PState->IsAlive()) //Check any particular pax is Alive
		{
			if (APax* P = Cast<APax>(PState->GetOwner())) //find its AActor
			{
				if (AController* PaxAICon = P->GetController()) //Get Its Controller (AI in this case)
				{
					PaxAICon->UnPossess();	//Remove any activation of that PAX
				}
			}
		}
	}
}

// called once from door event delegate
void ACabinManager::CheckDoorStatus()
{
	size_t counter{ 0 };
	for (const auto& d : Doors)
	{
		if (ADoor* DPtr = Cast<ADoor>(d))
		{
			if (!DPtr->GetDoorStatus()) { ++counter; } // is this door closed then add to number open
		}
	}
	if (counter == Doors.Num()) //we want all closed
	{
		Launch();
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
	
		NewPax->SetOnboard(true);
	}
}

//Get number of pax onboard
int32 ACabinManager::GetNumPaxOnboard()
{
	return PaxOnboard;
}

//Get number of seats in level
int32 ACabinManager::GetNumSeatsInLevel()
{
	return NumSeatsInLevel;
}

//Get remaining seats
int32 ACabinManager::GetRemainingSeats()
{
	return NumSeatsInLevel - PaxOnboard;
}

//Get percent of airplane seats occupied
float ACabinManager::GetLoadFactor()
{

	if (NumSeatsInLevel)
	{
		float pax = PaxOnboard;
		float seats = NumSeatsInLevel;
		float ratio = (pax / seats) * 100.0f;
		
		return (pax / seats) * 100.0f;
	}

	return 0.0f;
}


//Pulse Launch Protocol
void ACabinManager::Launch()
{
	if(GoForLaunch)	TakeOffPulse.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("LAUNCH"));

	//Search all actors, prior to checking if onboard
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APax::StaticClass(), Passengers);
	for (auto p : Passengers)
	{
		if (UPaxState* ps = Cast<APax>(p)->FindComponentByClass<UPaxState>())
		{
			//if not onboard, get owning actor, and destroy
			if (!ps->GetOnboard()) ps->GetOwner()->Destroy();
		}
	}


}
