// Copyright of Night Owls 2020 - inclusive ©

#include "Pax.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "Seat.h"
#include "Toilet.h"
#include "PaxState.h"
#include "Constants.h"
#include "Door.h"
#include "CabinManager.h"

// Sets default values
APax::APax()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	State = CreateDefaultSubobject<UPaxState>(TEXT("State"));
}

// Called when the game starts or when spawned
void APax::BeginPlay()
{
	Super::BeginPlay();

	//Targets walk location to current spawned spot
	DeployLocation = this->GetActorLocation();

	//Change walk speed to age group buff/nerf
	AdaptSpeeds();

	//Timer on repeat to enable attributes to change with time
	GetWorldTimerManager().SetTimer(PaxTimerHandle, this, &APax::UpdateState, 1.0f, true, 0.0f);
	
}

// Called every frame - refactor in future
void APax::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//To make sure walking animation is accurate and updated
	CheckIsMoving();

	//Controls deployment target, corrects snapping and enables/disables seat hopping
	TargetAcquiring();
}

// Called to bind functionality to input
void APax::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


//called from playercontroller
void APax::Clicked()
{
	SetPreMoveLocation(this->GetActorTransform());
	State->ResetStates();
	State->SetFloating(true);
	
}
void APax::Released()
{
	State->SetFloating(false);
	SetActorTransform(this->GetPreMoveLocation());
}

//Commanded go to location
void APax::SetDeployLocation(FVector Location)
{

	if (TargetSeat)
	{
		DeployLocation = Location + SeatDeployLocationOffset;
	}
	else
	{
		DeployLocation = Location + ToiletDeployLocationOffset;
	}
	
}
FVector APax::GetDeployLocation()
{
	return DeployLocation;
}

//Cached/Stored location before going to deploy location
void APax::SetPreMoveLocation(FTransform PaxOrientation)
{
	PreMoveTransform = PaxOrientation;
}
FTransform APax::GetPreMoveLocation()
{
	return PreMoveTransform;
}

//Enable Pax Info Stat UI
void APax::SetViewStatInfo(bool x)
{
	EnableStatInfo = x;
}
bool APax::GetViewStatInfo()
{
	return EnableStatInfo;
}


//Redirects function to state so it updates, called repeatable from timer in begin play
void APax::UpdateState()
{
	State->UpdateCores();
}

//Checks what the target is, eg toilet, seat
void APax::ManageTarget(AActor* Target)
{	
	//Set Casts
	TargetSeat = Cast<ASeat>(Target);
	Toilet = Cast<AToilet>(Target);

	//if target is seat
	if (TargetSeat)
	{
		//empty seat
		if(!TargetSeat->GetOccupied())
		{
			//do additional checks to avoid it occupying a seat but never making it due to path

			//stands pax up off seat if seat is further away then next to it and not on initial placement from a non-seat like an airbridge
			if (FVector::Dist(TargetSeat->GetActorLocation(), this->GetActorLocation()) > 60.0f)
			{
				if (CurrentSeat)
				{
					this->SetActorLocation(this->GetActorLocation() + FVector(0.0f, 40.0f, 0.0f));
				}
			}

			SetDeployLocation(TargetSeat->GetActorLocation());

			//Cabin Manager needs to know about the new pax ONBOARD
			if (!State->GetOnboard())
			{
				Manager = Cast<ACabinManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACabinManager::StaticClass()));
				if (Manager) Manager->RegisterNewPax(this);
				if (State) State->SetOnboard(true);
			}

		}
	}
	
}

//Changes Pax walk speed depending on age
void APax::AdaptSpeeds()
{
	UCharacterMovementComponent* Pax_Movement = GetCharacterMovement();

	if (State->GetAgeGroup() == EGeneration::YOUNG)
	{
		Pax_Movement->MaxWalkSpeed = YOUNG_WALK_SPEED;
	}
	else if (State->GetAgeGroup() == EGeneration::OLD)
	{
		Pax_Movement->MaxWalkSpeed = OLD_WALK_SPEED;
	}
	else 
	{ 
		Pax_Movement->MaxWalkSpeed = ADULT_WALK_SPEED; 
	}

	State->SetAnimPlaySpeed(Pax_Movement->MaxWalkSpeed);
}

// PER TICK :Receives overlap actors, calculates social bias
void APax::SetInfluence(const TArray<AActor*>& NearbyActors, bool FoundActors)
{
	if (FoundActors)
	{
		float GroupBias{ 0 };
		//Get length for to iterate for loop
		int NumOfActors = NearbyActors.Num();
		for (int i{ 0 }; i < NumOfActors; ++i)
		{
			float Bias{ 0 };
			//Get its pax state handle
			if (UPaxState* ActorState = NearbyActors[i]->FindComponentByClass<UPaxState>())
			{
				//AGE GROUP COMPARISON
				//if they are the same, we add zero, so only do something if different
				if (this->State->GetAgeGroup() != ActorState->GetAgeGroup())
				{
					if (this->State->GetAgeGroup() == EGeneration::YOUNG)
					{
						(ActorState->GetAgeGroup() == EGeneration::ADULT) ? --Bias : ++Bias;
					}
					else if (this->State->GetAgeGroup() == EGeneration::ADULT)
					{
						(ActorState->GetAgeGroup() == EGeneration::YOUNG) ? ++Bias : --Bias;
					}
					else //assumes this is old
					{
						(ActorState->GetAgeGroup() == EGeneration::YOUNG) ? --Bias : ++Bias;
					}
				}
				//POLITENESS CHECK
				if (ActorState->GetPoliteness() != EPoliteness::NEUTRAL)
				{
					(ActorState->GetPoliteness() == SOCIAL) ? ++Bias : --Bias; //assumes unsocial if false
				}
			}
			//DISTANCE MODIFIER
			Bias = Bias - (Bias * (FMath::Clamp((GetDistanceTo(NearbyActors[i])), 0.0f, 90.0f) / INFLUENCE_RANGE));
			//Sum Up All Nearby Actors Influence
			GroupBias += Bias;
		}
		//Create Average Bias over all overlapping actors
		GroupBias /= NumOfActors;
		//send to pax state	
		State->SetSocialBias(GroupBias);
	}
}

//NEEDS FINE TUNING :<(
//called every tick, senses seat occupancy,distance,snapping
void APax::TargetAcquiring()
{
	//If we have a target seat
	if (TargetSeat && (GetDeployLocation() == TargetSeat->GetActorLocation()))
	{
			//are we close to it?
			if ((this->GetDistanceTo(TargetSeat) < 50.0f))
			{
				State->SetMoving(false);
				State->SetSitting(true);

				//2 new lines
				TargetSeat->SetOccupied(true);
				this->SetCanAffectNavigationGeneration(false);

				//snap to seat
				this->SetActorLocation(TargetSeat->GetActorLocation());
				this->SetActorRotation(FRotator(0, -90, 0));

				//this new seat is now our current, will reroute this function on next tick
				CurrentSeat = TargetSeat;
			}
			else
			{
				State->SetSitting(false);

				//2 new lines
				TargetSeat->SetOccupied(false);
				this->SetCanAffectNavigationGeneration(true);
			}

	}

	else if (CurrentSeat)
	{
		if ((this->GetDistanceTo(CurrentSeat) < 50.0f))
		{	
			State->SetMoving(false); // maybe set moving
			State->SetSitting(true);

			//occupancy
			CurrentSeat->SetOccupied(true);

			//snap to seat
			this->SetActorLocation(CurrentSeat->GetActorLocation());
			this->SetActorRotation(FRotator(0, -90, 0));
		}
		else
		{
			State->SetSitting(false);

			//occupancy
			CurrentSeat->SetOccupied(false);
		}
	}

	//If we have a target and its a toilet
	else if (Toilet && (GetDeployLocation() == Toilet->GetActorLocation())) //TODO
	{
		//are we close to the door
		if (((this->GetActorLocation().X - DeployLocation.X) < 35.0f)
			&& ((this->GetActorLocation().Y - DeployLocation.Y) < 35.0f))
		{
			//snap actor to middle of seat
			this->SetActorLocation(Toilet->GetActorLocation());
			//rotate to front of seat/level/aircraft
			this->SetActorRotation(FRotator(0, -90, 0));
		}
	}
	else
	{
		return;
	}
}

//Checks if Pax is moving
void APax::CheckIsMoving()
{
	State->SetMoving((this->GetVelocity().IsZero()) ? false : true);
}

