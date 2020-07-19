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
	//Allows richer pax to drop more money, by manipulating all future delta numbers
	WealthAdjustment = Money / START_MONEY_POOR;
	//Maximum amount possible to drop to allow PAX to hit 0 money on landing if all stats are 0
	DrainDatum = Money / FREEFLIGHT_CRUISETIME;
	//Adjust drop limit so that it takes longer for richer people to drop as their delta increases faster
	MoneyDropLimit = CASH_DROP * WealthAdjustment; 
}

// Called every second from Pax.cpp
void UPaxState::UpdateCores()
{

	//Temporary Setting of Indicators
	NutritionChx = NUTRITION_CHX_AVG;
	EnergyChx = ENERGY_CHX_AVG;
	ExcrementChx = EXCREMENT_CHX_AVG;

	//Update Attributes
	Nutrition += NutritionChx;
	Energy += EnergyChx;
	Excrement += ExcrementChx;
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
			//higher stats the better the delta, excrement is best at 0, represents a ratio of maximum best stats and makes that a ratio of maximum earnable per tick
			Delta = (int)(((Nutrition + Energy + (FMath::Abs(Excrement - 100.0f) + Societal)) / 400) * DrainDatum);
			DeltaSum += Delta;
			if (DeltaSum > MoneyDropLimit) { DeltaSum = MoneyDropLimit; }
		}
		else //must be over spawn threshold
		{
			//if( !awaiting a pickup)
			if (!AwaitingPickup)
			{
				//if spawn successful, set awaiting pickup true to avoid more spawns
				AwaitingPickup = (SpawnMoney()) ? true : false;	
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("Delta: %i"), DeltaSum);
	}
	
}

//spawn money overhead PAX, and await cursor ahead. Functionality in money.cpp
bool UPaxState::SpawnMoney()
{
	if (MoneyActor)
	{
		FVector SpawnLocation = (GetOwner()->GetActorLocation()) + FVector(0, 5, 100);
		FRotator SpawnRotation = FRotator(45, 0,0);
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

int32 UPaxState::GetNutritionIndicator()
{
	if (NutritionChx > 0.0f) return SMALLINCREASE;
	else if (NutritionChx < 0.0f) return SMALLDECREASE;
	else { return NOCHANGE; }
}

float UPaxState::GetExcrement()
{
	return Excrement;
}
int32 UPaxState::GetExcrementIndicator()
{
	if (ExcrementChx > 0.0f) return SMALLINCREASE;
	else if (ExcrementChx < 0.0f) return SMALLDECREASE;
	else { return NOCHANGE; }
}


float UPaxState::GetSocietal()
{
	return Societal;
}
int32 UPaxState::GetSocialIndicator()
{
	if (SocialBias > 0.0f) return SMALLINCREASE;
	else if (SocialBias < 0.0f) return SMALLDECREASE;
	else { return NOCHANGE; }
}


void UPaxState::SetSocialBias(float x)
{
	SocialBias = x;
}

float UPaxState::GetEnergy()
{
	return Energy;
}
int32 UPaxState::GetEnergyIndicator()
{
	if (EnergyChx > 0.0f) return SMALLINCREASE;
	else if (EnergyChx < 0.0f) return SMALLDECREASE;
	else { return NOCHANGE; }
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

//Is pax onboard or off
void UPaxState::SetOnboard(bool x)
{
	Onboard = x;
}

bool UPaxState::GetOnboard()
{
	return Onboard;
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

void UPaxState::ResetDeltaSum()
{
	DeltaSum = 0;
}

int UPaxState::GetDeltaSum()
{
	return DeltaSum;
}

void UPaxState::SetAwaitingPickUp(bool x)
{
	AwaitingPickup = x;
}

bool UPaxState::GetAwaitingPickup()
{
	return AwaitingPickup;
}

bool UPaxState::IsAlive()
{
	Alive = (Nutrition < 1) ? false : true;
	return Alive;
}