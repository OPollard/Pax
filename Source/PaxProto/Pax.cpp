// Copyright of Night Owls 2020 - inclusive ©

#include "Pax.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Seat.h"
#include "Toilet.h"
#include "PaxState.h"
#include "Constants.h"
#include "CabinManager.h"
#include "WaitingArea.h"
#include "Components/AudioComponent.h"

// Sets default values
APax::APax()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	State = CreateDefaultSubobject<UPaxState>(TEXT("State"));
	DeathScream = CreateDefaultSubobject<UAudioComponent>(TEXT("Death Scream"));
}

// Called when the game starts or when spawned
void APax::BeginPlay()
{
	Super::BeginPlay();

	//Targets walk location to current spawned spot
	DeployLocation = this->GetActorLocation();

	EnableStateUpdate = false;
	//Toggles the overlay to change head colour green/red depending on positive/negative bias on sphere overlap
	EnableTextureOverlay = false;

	//Calibrate Offsets
	SeatDeployLocationOffset = FVector(0.0f, 40.0f, 0.0f);
	ToiletDeployLocationOffset = FVector(120.0f, 0.0f, 0.0f);

	//Change walk/rotation speed to age group buff/nerf
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

	//if not floating, make sure the overlay colour is reset
	if(!State->GetFloating()) SetEnableTextureOverlay(false);


}

// Called to bind functionality to input
void APax::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

//called from player controller
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
void APax::SetDeployLocation(const FVector Location)
{
	DeployLocation = Location;	
}
FVector APax::GetDeployLocation() const
{
	return DeployLocation;
}

//Cached/Stored location before going to deploy location
void APax::SetPreMoveLocation(const FTransform PaxOrientation)
{
	PreMoveTransform = PaxOrientation;
}
FTransform APax::GetPreMoveLocation()const
{
	return PreMoveTransform;
}

//Enable Pax Info Stat UI
void APax::SetViewStatInfo(const bool X)
{
	EnableStatInfo = X;
}
bool APax::GetViewStatInfo()const
{
	return EnableStatInfo;
}


//Redirects function to state so it updates, called repeatable from timer in begin play
void APax::UpdateState()const
{
	if(EnableStateUpdate) State->UpdateCores();
}

void APax::SetEnableStateUpdate(bool X)
{
	EnableStateUpdate = X;
}

bool APax::GetEnableStateUpdate()const
{
	return EnableStateUpdate;
}
//Checks what the target is, eg toilet, seat
void APax::ManageTarget(AActor* Target)
{	

		//Set Casts
		TargetSeat = Cast<ASeat>(Target);
		Toilet = Cast<AToilet>(Target);
		WaitingArea = Cast<AWaitingArea>(Target);

		//if target is seat
		if (TargetSeat && State->IsAlive())
		{
			//empty seat
			if (!TargetSeat->GetOccupied())
			{
				//do additional checks to avoid it occupying a seat but never making it due to path

				//stands pax up off seat if seat is further away then next to it and not on initial placement from a non-seat like an airbridge
				if (FVector::Dist(TargetSeat->GetActorLocation(), this->GetActorLocation()) > 60.0f)
				{
					//gets off chair to go to target
					if (TargetPlace == ETarget::CURRENTSEAT)
					{
						this->SetActorLocation(this->GetActorLocation() + SeatDeployLocationOffset);
					}
					//gets out toilet to go to target
					if (TargetPlace == ETarget::TOILET)
					{
						//set offset in blueprint toilet_bp when wanting to leave
					}
				}
				ManageTargetPost();
			}
		}

		//if target is a toilet
		if (Toilet && State->IsAlive())
		{
	
			//empty toilet
			if (!Toilet->GetOccupied())
			{
				//do additional checks to avoid it occupying a seat but never making it due to path

				//stands pax up off seat if seat is further away then next to it and not on initial placement from a non-seat like an airbridge
				if (FVector::Dist(Toilet->GetActorLocation(), this->GetActorLocation()) > 60.0f)
				{
					if (CurrentSeat)
					{
						this->SetActorLocation(this->GetActorLocation() + SeatDeployLocationOffset);
					}
				}
				ManageTargetPost();
			}
		}

		//if target is waiting area
		if (WaitingArea && State->IsAlive())
		{
			if(!WaitingArea->GetOccupied())
			{
				ManageTargetPost();
			}
		}
	
	
}
//Dirty post delay activations, after pax thinking time from seat
void APax::ManageTargetPost()
{
	if (TargetSeat)
	{
		//Set deploy location and target so we can make this a nullptr
		SetDeployLocation(TargetSeat->GetActorLocation());
		//Remove Occupancy of previous seat
		if (CurrentSeat) CurrentSeat->SetOccupied(false);
		//Important for correct path, turned to false when sat down
		this->SetCanAffectNavigationGeneration(true);
		//Redirect path for tick
		TargetPlace = ETarget::TARGETSEAT;

		//Cabin Manager needs to know about the new pax OnBoard
		if (!State->GetOnboard())
		{
			//Get the cabin manager
			Manager = Cast<ACabinManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACabinManager::StaticClass()));
			if (Manager) Manager->RegisterNewPax(this);
			if (State) State->SetOnboard(true);
		}
	}
	else if (Toilet)
	{
		//Set deploy location and target so we can make this a nullptr
		SetDeployLocation(Toilet->GetActorLocation() + ToiletDeployLocationOffset);
		//	Toilet->SetOccupied(false);
		//Remove Occupancy of previous seat
		if (CurrentSeat) CurrentSeat->SetOccupied(false);
		TargetPlace = ETarget::TOILET;

		//Cabin manager will need to know who is in the toilet TODO
		if (!State->GetOnboard())
		{
			//Get Cabin Manager
			Manager = Cast<ACabinManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACabinManager::StaticClass()));
			if (Manager) Manager->RegisterNewPax(this);
			if (State) State->SetOnboard(true);
		}
	}

	else if(WaitingArea)
	{
		//Set deploy location and target so we can make this a nullptr
		SetDeployLocation(WaitingArea->GetActorLocation());
		TargetPlace = ETarget::WAITINGAREA;
		//Remove previous occupancy
		if (CurrentSeat) CurrentSeat->SetOccupied(false);
		//New occupancy triggered on overlap

		//Cabin manager will need to know who is in the waiting area TODO
		if (!State->GetOnboard())
		{
			//Get Cabin Manager
			Manager = Cast<ACabinManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACabinManager::StaticClass()));
			if (Manager) Manager->RegisterNewPax(this);
			if (State) State->SetOnboard(true);
		}
	}
	else
	{
		return;
	}
}


//Changes Pax walk speed depending on age
void APax::AdaptSpeeds()const
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
	//set anim play speed for legs
	State->SetAnimPlaySpeed(Pax_Movement->MaxWalkSpeed);
	//set rotation rate to give lethargic style
	Pax_Movement->RotationRate.Yaw = (Pax_Movement->MaxWalkSpeed / ADULT_WALK_SPEED) * STANDARD_ROTATION_SPEED;
}

//Set Seat Location for overlap location
void APax::SetSphereSpawnLocation(FVector Location)
{
	FloatingPaxSphereLocation = Location;
}

//Get Seat Location for overlap location
FVector APax::GetSphereSpawnLocation()
{
	return FloatingPaxSphereLocation;
}

// PER TICK :Receives overlap actors, calculates social bias
void APax::SetInfluence(const TArray<AActor*>& NearbyActors,const bool FoundActors)
{
	if (FoundActors)
	{
		float GroupBias{ 0 };
		//Get length to iterate for loop
		const int NumOfActors = NearbyActors.Num();
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
			//Get reference to found actor
			APax* NearbyPax = Cast<APax>(NearbyActors[i]);
			if (NearbyPax)
			{
				//Set influence state appropriately, to trigger correct overlay
				if (Bias > 0.0f) NearbyPax->SetInfluenceAffect(EInfluenceAffect::POSITIVE);
				else if (Bias < 0.0f) NearbyPax->SetInfluenceAffect(EInfluenceAffect::NEGATIVE);
				else NearbyPax->SetInfluenceAffect(EInfluenceAffect::NONE);
				//if the found actor is sitting down, enable overlay
				if(State->GetFloating() || EnableStatInfo) NearbyPax->SetEnableTextureOverlay(true);
			}
			//DISTANCE MODIFIER
			Bias = Bias - (Bias * (FMath::Clamp((GetDistanceTo(NearbyActors[i])), 0.0f, INFLUENCE_RANGE) / INFLUENCE_RANGE));
			//Sum Up All Nearby Actors Influence
			GroupBias += Bias;
			//Create Average Bias over all overlapping actors
			GroupBias /= NumOfActors;
			//send to pax state	
			State->SetSocialBias(GroupBias);
		}
	}
}


//set texture overlay toggle
void APax::SetEnableTextureOverlay(bool X)
{
	EnableTextureOverlay = X;
}

//fetch overlay toggle, used in BP
bool APax::GetEnableTextureOverlay()
{
	return EnableTextureOverlay;
}

//called every tick, senses seat occupancy,distance,snapping
void APax::TargetAcquiring()
{
	//Redirect Scripts to appropriate action
	switch (TargetPlace)
	{
		case SELF:
			SelfAcquiring();
			 break;
		case TARGETSEAT:
			TargetSeatAcquiring();
			 break;
		case CURRENTSEAT:
			CurrentSeatAcquiring();
		case TOILET:
			ToiletAcquiring();
			 break;
		default:
			 break;
	}
}

//Lock onto current location ignore ReSharper Static
void APax::SelfAcquiring()
{
	//blank at the moment
}
//Lock onto seat target DO NOT TOUCH WITHOUT READING THOROUGHLY FIRST
void APax::TargetSeatAcquiring()
{
	if ((this->GetDistanceTo(TargetSeat) < SEAT_SNAP_RANGE)) //snap when close, ran once
	{
		TargetSeat->SetOccupied(true);						 
		State->SetMoving(false);							 
		State->SetSitting(true);							 
		this->SetActorLocation(TargetSeat->GetActorLocation());
		this->SetActorRotation(FRotator(0, -90, 0));
		this->SetCanAffectNavigationGeneration(false); //set true in manage target
		CurrentSeat = TargetSeat;
		TargetSeat = nullptr; //danger
		TargetPlace = ETarget::CURRENTSEAT; //redirect for next tick
	}
}

//Lock onto current seat
void APax::CurrentSeatAcquiring()
{
	if (!State->GetFloating()) //ran all the time while not floating and sat down
	{
		this->SetActorLocation(CurrentSeat->GetActorLocation());
		this->SetActorRotation(FRotator(0, -90, 0));
		State->SetMoving(false); //for animations
		State->SetSitting(true);
	}
}
//Lock onto toilet target
void APax::ToiletAcquiring()const
{
	if (Toilet && Toilet->GetOccupied())
	{
		State->SetExcrement(0.0f);
	}
}

//Checks if Pax is moving
void APax::CheckIsMoving()const
{
	State->SetMoving((this->GetVelocity().IsZero()) ? false : true);
	if(State->GetMoving())State->SetSitting(false);
}

//Return target place
ETarget APax::GetTargetPlace()const
{
	return TargetPlace;
}

//Get influence affect state enum
EInfluenceAffect APax::GetInfluenceAffect()
{
	return InfluenceAffect;
}

//Set influence affect state
void APax::SetInfluenceAffect(EInfluenceAffect Affect)
{
	InfluenceAffect = Affect;
}