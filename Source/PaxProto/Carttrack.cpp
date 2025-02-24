// Copyright of Codebird Ltd 2020 - inclusive © 

#include "Carttrack.h"
#include "Components/SplineComponent.h"

ACarttrack::ACarttrack()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//Root guard
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	}

	//Configure cart track component
	CartTrack = CreateDefaultSubobject<USplineComponent>(TEXT("Track"));
	CartTrack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACarttrack::BeginPlay()
{
	Super::BeginPlay();
}

void ACarttrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Getter reference
USplineComponent* ACarttrack::GetSplineTrack()
{
	return CartTrack;
}
