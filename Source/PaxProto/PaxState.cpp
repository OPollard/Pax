// Copyright of Codebird Ltd 2020 - inclusive © 

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
	//Randomise Name and Age on Spawn
	Name = Names[FMath::RandRange(0, (Names.Num()-1))];
	Age = FMath::RandRange(1, 80);
	//Randomise Wealth and Politeness on Spawn
	Politeness = static_cast<EPoliteness>(FMath::RandRange(0, EPoliteness::NUMOFPOLITENESS-1));
	Wealthiness = static_cast<EWealth>(FMath::RandRange(0, EWealth::NUMOFWEALTH-1));
	//Set Enum Depending on Age Result
	if (Age >= 0 && Age < 12) { AgeGroup = EGeneration::YOUNG; }
	if (Age >= 12 && Age < 65) { AgeGroup = EGeneration::ADULT; }
	if (Age >= 65 && Age < 81) { AgeGroup = EGeneration::OLD; }

	//Allocate an amount of potential money from the pax depending on wealthiness
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

	//Add slight variance to core starting values
	Nutrition = (FMath::RandRange(80.0f, 100.0f));
	Energy = (FMath::RandRange(20.0f, 80.0f));
	Excrement = (FMath::RandRange(0.0f, 15.0f));
	Societal = (FMath::RandRange(45.0f, 55.0f));

	//Change constants based on age
	switch (AgeGroup)
	{
		case YOUNG:
			NutritionChx = NUTRITION_CHX_YNG;
			EnergyChx = ENERGY_CHX_YNG;
			ExcrementChx = EXCREMENT_CHX_YNG;
			break;

		case ADULT:
			NutritionChx = NUTRITION_CHX_AVG;
			EnergyChx = ENERGY_CHX_AVG;
			ExcrementChx = EXCREMENT_CHX_AVG;
			break;
		case OLD:
			NutritionChx = NUTRITION_CHX_OLD;
			EnergyChx = ENERGY_CHX_OLD;
			ExcrementChx = EXCREMENT_CHX_OLD;
			break;
		default:
			break;
	}
}

// Called every second from Pax.cpp
void UPaxState::UpdateCores()
{
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
		//if needs to sleep
		if (IsTired() && !GetMoving())
		{
			if (!AnnouncedSleep)
			{
				Sleep.Broadcast();
				AnnouncedSleep = true;
				Moving = false;
			}
		}
		else //assumes alive
		{
			//Update Attributes
			Nutrition += NutritionChx;
			Energy += EnergyChx;
			Excrement += ExcrementChx;
			Societal += SocialBias;

			//if DeltaSum is below spawn threshold
			if (DeltaSum < MoneyDropLimit)
			{
				//higher stats the better the delta, excrement is best at 0, represents a ratio of maximum best stats and makes that a ratio of maximum earned per tick
				Delta = static_cast<int>(((Nutrition + Energy + (FMath::Abs(Excrement - 100.0f) + Societal)) / 400) * DrainDatum);
				DeltaSum += Delta;
				//If the delta goes over our limit, cap it and prevent further growth
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
		}
}

//spawn money overhead PAX, and await cursor ahead. Functionality in money.cpp
bool UPaxState::SpawnMoney()const
{
	if (MoneyActor)
	{
		//Get Variables for function below
		const FVector SpawnLocation = (GetOwner()->GetActorLocation()) + FVector(0, 5, 100);
		const FRotator SpawnRotation = FRotator(45, 0,0);
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		//Spawn Money Actor
		AMoney* SpawnedMoney = GetWorld()->SpawnActor<AMoney>(MoneyActor, SpawnLocation, SpawnRotation, SpawnParameters);
		//Attach to pax so it moves with them, like tilting on takeoff etc
		SpawnedMoney->AttachToActor(GetOwner(),FAttachmentTransformRules::KeepWorldTransform);
		return true;
	}
	return false;
}

int32 UPaxState::GetAge()const
{
	return Age;
}

EGeneration UPaxState::GetAgeGroup()const
{
	return AgeGroup;
}

EPoliteness UPaxState::GetPoliteness()const
{
	return Politeness;
}

FString UPaxState::GetName()const
{
	return Name;
}

void UPaxState::SetNutrition(float Value)
{
	Nutrition = Value;
}

float UPaxState::GetNutrition()const
{
	return Nutrition;
}
//Represents in state form, the level of change
int32 UPaxState::GetNutritionIndicator()const
{
	if (NutritionChx < -0.5f) return LARGEDECREASE;
	else if (NutritionChx < 0.0f) return SMALLDECREASE;
	else if (NutritionChx > 0.5f) return LARGEINCREASE;
	else if (NutritionChx > 0.0f) return SMALLINCREASE;
	else return NOCHANGE;
}

void UPaxState::SetExcrement(const float Value)
{
	Excrement = Value;
}

float UPaxState::GetExcrement()const
{
	return Excrement;
}
//Represents in state form, the level of change
int32 UPaxState::GetExcrementIndicator()const
{
	if (ExcrementChx < -0.5f) return LARGEDECREASE;
	else if (ExcrementChx < 0.0f) return SMALLDECREASE;
	else if (ExcrementChx > 0.5f) return LARGEINCREASE;
	else if (ExcrementChx > 0.0f) return SMALLINCREASE;
	else return NOCHANGE;
}
float UPaxState::GetSocietal()const
{
	return Societal;
}
//Represents in state form, the level of change
int32 UPaxState::GetSocialIndicator()const
{
	if (SocialBias < -0.25f) return LARGEDECREASE;
	else if (SocialBias < 0.0f) return SMALLDECREASE;
	else if (SocialBias > 0.25f) return LARGEINCREASE;
	else if (SocialBias > 0.0f) return SMALLINCREASE;
	else return NOCHANGE;
}


void UPaxState::SetSocialBias(const float Value)
{
	SocialBias = Value;
}

void UPaxState::SetEnergy(float Value)
{
	Energy = Value;
}

float UPaxState::GetEnergy()const
{
	return Energy;
}
//Represents in state form, the level of change
int32 UPaxState::GetEnergyIndicator()const
{
	if (EnergyChx < -0.5f) return LARGEDECREASE;
	else if (EnergyChx < 0.0f) return SMALLDECREASE;
	else if (EnergyChx > 0.5f) return LARGEINCREASE;
	else if (EnergyChx > 0.0f) return SMALLINCREASE;
	else return NOCHANGE;
}

void UPaxState::SetAnnouncedSleep(bool X)
{
	AnnouncedSleep = X;
}


float UPaxState::GetAnimPlaySpeed()const
{
	return AnimationPlaySpeed;
}

//Set anim walk play speed as a ratio of movement speed
void UPaxState::SetAnimPlaySpeed(const float WalkSpeed)
{
	AnimationPlaySpeed = WalkSpeed / ADULT_WALK_SPEED;
}
//Set States to Default of false
void UPaxState::ResetStates()
{
	Sitting = false;
	Floating = false;
	Moving = false;
	InToilet = false;
}

//Is pax OnBoard or off
void UPaxState::SetOnboard(const bool X)
{
	Onboard = X;
}

bool UPaxState::GetOnboard()const
{
	return Onboard;
}

//Is pax sitting in a seat
void UPaxState::SetSitting(const bool X)
{
	Sitting = X;
}
bool UPaxState::GetSitting()const
{
	return Sitting;
}

//Is pax elevated by mouse
void UPaxState::SetFloating(const bool X)
{
	Floating = X;
}
bool UPaxState::GetFloating()const
{
	return Floating;
}

//Is pax using the loo
void UPaxState::SetInToilet(const bool X)
{
	InToilet = X;
}
bool UPaxState::GetInToilet() const
{
	return InToilet;
}

//Is pax moving
void UPaxState::SetMoving(const bool X)
{
	Moving = X;
}
bool UPaxState::GetMoving()const
{
	return Moving;
}

void UPaxState::SetWaitArea(bool X)
{
	WaitArea = X;
}

bool UPaxState::GetWaitArea()const
{
	return WaitArea;
}

// Returns money stat as FText format for the UI
FText UPaxState::GetMoneyAsText()const
{
	FString p_Money{ FString::SanitizeFloat(Money, 0) };
	FString p_Display{ "$" };
	p_Display.Append(p_Money);
	return UKismetTextLibrary::Conv_StringToText(p_Display);
}

float UPaxState::GetMoney()const
{
	return Money;
}

void UPaxState::SetMoney(const float X)
{
	Money = X;
}

void UPaxState::ResetDeltaSum()
{
	DeltaSum = 0;
}

int UPaxState::GetDeltaSum()const
{
	return DeltaSum;
}

void UPaxState::SetAwaitingPickUp(const bool X)
{
	AwaitingPickup = X;
}

bool UPaxState::GetAwaitingPickup()const
{
	return AwaitingPickup;
}
//Alive is currently dependant on amount of nutrition, may change or add late
bool UPaxState::IsAlive()
{
	if (Nutrition < 1)
	{
		Alive = false;
	}
	return Alive;
}

//Tiredness is currently dependant on amount of energy
bool UPaxState::IsTired()
{
	return (Energy < 1) ? true : false;
}

//Uncomfortable status currently depends on Toilet or Social
bool UPaxState::IsUncomfortable()
{
	return ((Excrement > 99) || (Societal < 1)) ? true : false;
}