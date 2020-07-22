// Copyright of Night Owls 2020 - inclusive ©

#include "Seat.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"
#include "Pax.h"
#include "PaxState.h"


// Sets default values
ASeat::ASeat()
{
 	//Turned off tick due to many seats
	PrimaryActorTick.bCanEverTick = false;
	

	//Guard against having no root
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	}

	//Configure Seat Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Click Box"));
	CollisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetBoxExtent(FVector(22.0f, 30.0f, 40.0f));
	CollisionBox->SetCollisionProfileName("UI");

	//Configure Indicator Light
	IndicatorLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Indicator Light"));
	IndicatorLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	IndicatorLight->SetIntensity(100.0f);
	IndicatorLight->SetAttenuationRadius(5.0f);
	IndicatorLight->SetRelativeLocation(FVector(0.0f, -22.0f, 31.0f));
	IndicatorLight->SetCastShadows(false);

}

// Called when the game starts or when spawned
void ASeat::BeginPlay()
{
	Super::BeginPlay();	

}

// Called every frame
void ASeat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isUIEnabled = false;

}

void ASeat::SetUIEnabled(bool x)
{
	isUIEnabled = x;
}

bool ASeat::GetUIEnabled()
{
	return isUIEnabled;
}

FString ASeat::GetSeatID()
{
	return (this->GetName()).Mid(4,2);
}

void ASeat::SetOccupied(bool x)
{
	isOccupied = x;
	(isOccupied) ? IndicatorLight->SetVisibility(false) : IndicatorLight->SetVisibility(true);
}

bool ASeat::GetOccupied()
{
	return isOccupied;
}