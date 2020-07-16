// Copyright of Night Owls 2020 - inclusive © 


#include "Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// set from blueprint
void ADoor::SetDoorStatus(bool x)
{
	isOpen = x;
	DoorMoved.ExecuteIfBound(); //sends to cabin manager to signal its moved
}

bool ADoor::GetDoorStatus()
{
	return isOpen;
}

//set if it is locked
void ADoor::SetLocked(bool x)
{
	Locked = x;
}

bool ADoor::GetLocked()
{
	return Locked;
}
