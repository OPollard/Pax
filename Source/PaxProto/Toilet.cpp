// Copyright of Codebird Ltd 2020 - inclusive © 


#include "Toilet.h"
#include "Components/BoxComponent.h"
#include "Components/RectLightComponent.h"

AToilet::AToilet()
{
	PrimaryActorTick.bCanEverTick = true;

	//Guard against having no root
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	}

	//Configure Toilet Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Click Box"));
	CollisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetBoxExtent(FVector(55.0f, 25.0f, 70.0f));
	CollisionBox->SetCollisionProfileName("UI");

	//Configure Indicator Light
	IndicatorLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Indicator Light"));
	IndicatorLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	IndicatorLight->SetRelativeLocation(FVector(21.0f, 0.0f, 68.0f));
	IndicatorLight->SetRelativeRotation(FRotator(-90.0f,0.0f,180.0f));
	IndicatorLight->SetIntensity(100.0f);
	IndicatorLight->SetAttenuationRadius(75.0f);
	IndicatorLight->SetBarnDoorAngle(90.0f);
	IndicatorLight->SetBarnDoorLength(5.0f);
	IndicatorLight->SetCastShadows(false);

	FrontToiletOccupied = false;
	RearToiletOccupied = false;
}

void AToilet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AToilet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Set appropriate toilet has occupied/unoccupied depending on which one is being challenged
void AToilet::SetOccupied(const bool X)
{
	//Get world name
	const FString ID = this->GetName();
	//Check if its forward or rear door
	if (ID.Contains("d"))
	{
		//Set it accordingly
		FrontToiletOccupied = X;
	}
	else
	{
		RearToiletOccupied = X;
	}
	//Increase light brightness to signal that its occupied
	IndicatorLight->SetIntensity(((X) ? 500.0f : 100.0f));
}

//Get appropriate toilet occupancy, depending on which is being challenged
bool AToilet::GetOccupied()const
{
	//Get world name
	const FString ID = this->GetName();
	//Return according door
	if (ID.Contains("d"))
	{
		return FrontToiletOccupied;
	}
	else
	{
		return RearToiletOccupied;
	}
}

void AToilet::SetOccupier(AActor* Pax)
{
	Occupier = Pax;
}

AActor* AToilet::GetOccupier() const
{
	return Occupier;
}
