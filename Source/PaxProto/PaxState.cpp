// Copyright of Night Owls 2020 - inclusive ©

#include "PaxState.h"
#include "Constants.h"
#include "Kismet/KismetTextLibrary.h"
#include "Money.h"

UPaxState::UPaxState()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Find money BP in content and make it a subclass in the variable stored in .h
	static ConstructorHelpers::FObjectFinder<UClass> MoneyActorClassFinder(TEXT("Blueprint'/Game/Blueprints/Money_BP.Money_BP_C'"));
	MoneyActor = MoneyActorClassFinder.Object;
}


void UPaxState::BeginPlay()
{
	Super::BeginPlay();

	//set up stats on spawn
	Initialise();
	
}

void UPaxState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


//Calibrate Stats on startup, called from Pax
void UPaxState::Initialise()
{

	Name = Names[FMath::RandRange(0, (Names.Num()-1))];
	Age = FMath::RandRange(1, 80);

	Politeness = static_cast<EPoliteness>(FMath::RandRange(0, EPoliteness::NUMOFPOLITENESS-1));
	Wealthiness = static_cast<EWealth>(FMath::RandRange(0, EWealth::NUMOFWEALTH-1));

	if (Age >= 0 && Age < 12) { AgeGroup = EGeneration::YOUNG; }
	if (Age >= 12 && Age < 65) { AgeGroup = EGeneration::ADULT; }
	if (Age >= 65 && Age < 81) { AgeGroup = EGeneration::OLD; }

	switch (Wealthiness)
	{
	case POOR: Money = START_MONEY_POOR; break;
	case AVERAGE: Money = START_MONEY_AVG; break;
	case WEALTHY: Money = START_MONEY_WEALTHY; break;
	case RICH: Money = START_MONEY_RICH; break;
	default: Money = START_MONEY_AVG;
	}
	
}

// Called every second from Pax.cpp
void UPaxState::UpdateCores()
{
	//Update Attributes
	Nutrition += NUTRITION_CHX_AVG;
	Energy += ENERGY_CHX_AVG;
	Excrement += EXCREMENT_CHX_AVG;
	Societal += SocialBias;

	//If not alive
	if (!IsAlive())
	{
		if (!AnnouncedDeath) //and not announced
		{
			OnDeath.Broadcast();
			AnnouncedDeath = true;
			Moving = false;
		}

	}
	else //assumes alive
	{
		//if DeltaSum is below spawn threshold
		if (DeltaSum < MoneyDropLimit)
		{
			//higher stats the better the delta, excrement is best at 0, represents a ratio of maximum best stats and makes that a ratio of money drain
			Delta = (int)(((Nutrition + Energy + (FMath::Abs(Excrement - 100.0f) + Societal)) / 400) * MONEY_DRAIN);
			DeltaSum += Delta;
			if (DeltaSum > MoneyDropLimit) { DeltaSum = MoneyDropLimit; }
		}
		else //must be over spawn threshold
		{
			//if( !awaiting a pickup)
			if (!AwaitingPickup)
			{
				//FPaxDelegate.MoneyDrop(broadcast)
				//if spawn successful, set awaiting pickup true to avoid more spawns
				AwaitingPickup = (SpawnMoney()) ? true : false;
				//CURSOR OVERHEAD ACTOR WILL TRIGGER EVENT TO CABIN MANAGER TO SUM TOTAL LEVEL EARNINGS, WILL ALSO REDUCE PAX MONEY
				//get overhead action to re-trigger this for not awaiting pickup back to false
				//set end awaiting a pickup as true;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Delta: %i"), DeltaSum);
	}
	
}

bool UPaxState::SpawnMoney()
{
	if (MoneyActor)
	{
		FVector SpawnLocation = (GetOwner()->GetActorLocation()) + FVector(0, 0, 100);
		FRotator SpawnRotation = FRotator(0, 0, 0);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		AMoney* SpawnedMoney = GetWorld()->SpawnActor<AMoney>(MoneyActor, SpawnLocation, SpawnRotation, SpawnParameters);
		SpawnedMoney->AttachToActor(GetOwner(),FAttachmentTransformRules::KeepWorldTransform);
		return true;
	}
	return false;
}

//Returns Age of Pax
int32 UPaxState::GetAge()
{
	return Age;
}

//Return Generation
EGeneration UPaxState::GetAgeGroup()
{
	return AgeGroup;
}

EPoliteness UPaxState::GetPoliteness()
{
	return Politeness;
}

FString UPaxState::GetName()
{
	return Name;
}

float UPaxState::GetNutrition()
{
	return Nutrition;
}

float UPaxState::GetExcrement()
{
	return Excrement;
}

float UPaxState::GetSocietal()
{
	return Societal;
}

void UPaxState::SetSocialBias(float x)
{
	SocialBias = x;
}

float UPaxState::GetEnergy()
{
	return Energy;
}

float UPaxState::GetAnimPlaySpeed()
{
	return AnimationPlaySpeed;
}

//Set anim walk play speed as a ratio of movement speed
void UPaxState::SetAnimPlaySpeed(float WalkSpeed)
{
	AnimationPlaySpeed = WalkSpeed / ADULT_WALK_SPEED;
}

void UPaxState::ResetStates()
{
	Sitting = false;
	Floating = false;
	Moving = false;
	InToilet = false;
}

//Is pax sitting in a seat
void UPaxState::SetSitting(bool x)
{
	Sitting = x;
}
bool UPaxState::GetSitting()
{
	return Sitting;
}

//Is pax elevated by mouse
void UPaxState::SetFloating(bool x)
{
	Floating = x;
}
bool UPaxState::GetFloating()
{
	return Floating;
}

//Is pax using the loo
void UPaxState::SetInToilet(bool x)
{
	InToilet = x;
}
bool UPaxState::GetInToilet()
{
	return InToilet;
}

//Is pax moving
void UPaxState::SetMoving(bool x)
{
	Moving = x;
}
bool UPaxState::GetMoving()
{
	return Moving;
}

// Returns money stat as FText format for the UI
FText UPaxState::GetMoneyAsText()
{
	FString p_Money{ FString::SanitizeFloat(Money, 0) };
	FString p_Display{ "$" };
	p_Display.Append(p_Money);
	return UKismetTextLibrary::Conv_StringToText(p_Display);
}

float UPaxState::GetMoney()
{
	return Money;
}

void UPaxState::SetMoney(float x)
{
	Money = x;
}

int UPaxState::GetDeltaSum()
{
	return DeltaSum;
}

bool UPaxState::IsAlive()
{
	Alive = (Nutrition < 1) ? false : true;
	return Alive;
}