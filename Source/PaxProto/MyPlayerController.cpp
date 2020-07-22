// Copyright of Night Owls 2020 - inclusive ©


#include "MyPlayerController.h"
#include "Engine/World.h"
#include "Pax.h"
#include "Seat.h"

#define PAX ECollisionChannel::ECC_GameTraceChannel1 
#define LOCATION ECollisionChannel::ECC_GameTraceChannel2 


//set up settings
AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//input settings
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}


// Called to bind functionality to input
void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Input Bindings 
	InputComponent->BindAction(FName("LeftClick"), EInputEvent::IE_Pressed, this, &AMyPlayerController::SetClicked);
	InputComponent->BindAction(FName("LeftClick"), EInputEvent::IE_Released, this, &AMyPlayerController::SetReleased);
	
	
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//clicked on some sort of actor, eg pax or seat
	if (ClickedOn)
	{
		//confirm we have the actor eg not nullptr
		if (PaxHandle)
		{
			//Update Position to match mouse position
			PaxHandle->SetActorLocation(GetCursorHooverPosition());
			PaxHandle->SetViewStatInfo(false);
			SearchForActorAtCursor(LOCATION);
			SendTriggerToTarget();
		}
	}
	else
	{
		CheckAndRefreshPaxUI();
		
	}

}

//Called when bind action left mouse button is pressed
void AMyPlayerController::SetClicked()
{
	ClickedOn = true;

	SearchForActorAtCursor(PAX);
	
	if (PaxHandle)
	{
		PaxHandle->Clicked();
	}

}

//Called when bind action left mouse button is released
void AMyPlayerController::SetReleased()
{
	ClickedOn = false;

	if (PaxHandle && !(ClickedOn))
	{
		PaxHandle->Released();
		if (Target) {PaxHandle->ManageTarget(Target);}
	}
}

/*Line traces for objects from camera, stores AActors in header file for use in tick()
Contains a cast IF argument is PAX, then do not place in tick*/
void AMyPlayerController::SearchForActorAtCursor(ECollisionChannel Trace)
{
	//Get Pointers
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APlayerCameraManager* PCAM = PC->PlayerCameraManager;

	//Declare outparameters
	FVector WorldLocation;
	FVector WorldDirection;
	FVector StartLocation = PCAM->GetCameraLocation();
	FHitResult HitResult;

	//Convert mouse to world xyz
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	FVector EndLocation = StartLocation + (WorldDirection * LineTraceRange);

	//always check for nulls
	if (PC && PCAM)
	{
		//line trace
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			Trace);
	}

	//cast result to appropriate pointer
	if (Trace == PAX) { PaxHandle = Cast<APax>(HitResult.GetActor()); }
	if (Trace == LOCATION) { Target = HitResult.GetActor(); }

}

//Returns position for floating objects, will offset for camera angle
FVector AMyPlayerController::GetCursorHooverPosition()
{
	//Get pointers
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APlayerCameraManager* PCAM = PC->PlayerCameraManager;

	//Declare outparameters
	FVector WorldLocation;
	FVector WorldDirection;
	FVector StartLocation = PCAM->GetCameraLocation();

	//Convert mouse position to world xyz
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	
	//return coordinate in xyz back along path (illusion of floating)
	return StartLocation + (WorldDirection * (LineTraceRange - LiftedElevationHeight));
}

//Activates player UI (refreshes)
void AMyPlayerController::CheckAndRefreshPaxUI()
{
	if (PaxHandle) { PaxHandle->SetViewStatInfo(false); }
	SearchForActorAtCursor(PAX);
	if (PaxHandle) { PaxHandle->SetViewStatInfo(true); }
}

//Send trigger to target actor, eg a seat to activate UI overhead
void AMyPlayerController::SendTriggerToTarget()
{
	//confirm target is not null
	if (Target)
	{
		//check its a seat
		if (ASeat* Seat = Cast<ASeat>(Target))
		{
			Seat->SetUIEnabled(true);
		}
	}

	
}