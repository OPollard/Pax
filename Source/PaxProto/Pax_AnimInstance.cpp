// Copyright of Night Owls 2020 - inclusive ©


#include "Pax_AnimInstance.h"
#include "Pax.h"
#include "PaxState.h"


void UPax_AnimInstance::NativeInitializeAnimation()
{
	//Very Important Line
	Super::NativeInitializeAnimation();

	//Cache the owning pawn for use in Tick
	OwningPawn = TryGetPawnOwner();
	PaxPawn = Cast<APax>(OwningPawn);
	if (PaxPawn) { PaxState = PaxPawn->FindComponentByClass<UPaxState>(); }
}

//Tick
void UPax_AnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaTimeX);

	//Always Check Pointers
	if (!OwningPawn || !PaxPawn || !PaxState)
	{
		return;
	}

	//Check for animation triggers
	IsFloating = (PaxState->GetFloating());

	IsSitting = (PaxState->GetSitting());

	IsMoving = (PaxState->GetMoving());

	IsAlive = (PaxState->IsAlive());

}

// Fetch anim play speed from pax state
void UPax_AnimInstance::SetPaxWalkSpeed()
{
	Pax_WalkSpeed = (PaxState) ? PaxState->GetAnimPlaySpeed() : 1.0f;
}