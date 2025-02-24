// Copyright of Codebird Ltd 2020 - inclusive © 


#include "Door.h"


ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// set from blueprint version
void ADoor::SetDoorStatus(const bool X)
{
	IsOpen = X;
	//sends to cabin manager to signal its moved, singular event delegate
	DoorMoved.ExecuteIfBound(); 
}
// called from blueprint
bool ADoor::GetDoorStatus() const
{
	return IsOpen;
}

void ADoor::SetLocked(const bool X)
{
	Locked = X;
}

bool ADoor::GetLocked() const
{
	return Locked;
}
