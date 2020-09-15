// Copyright of Codebird Ltd 2020 - inclusive © 

#include "Seat.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/UnrealString.h"


ASeat::ASeat()
{
 	//Turned off tick due to many seats
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	this->SetActorTickEnabled(false);

	//Guard against having no root
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	}

	//Configure Seat Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Click Box"));
	CollisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetBoxExtent(FVector(22.0f, 30.0f, 40.0f));	

	//Configure Indicator Light
	IndicatorLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Indicator Light"));
	IndicatorLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	IndicatorLight->SetIntensity(100.0f);
	IndicatorLight->SetAttenuationRadius(5.0f);
	IndicatorLight->SetRelativeLocation(FVector(0.0f, -22.0f, 31.0f));
	IndicatorLight->SetCastShadows(false);

}

void ASeat::BeginPlay()
{
	Super::BeginPlay();
	IsUIEnabled = false;
}

void ASeat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Called from function in class
void ASeat::UITimerExpired()
{
	//turn off UI
	SetUIEnabled(false);
}

//Return UI state
bool ASeat::GetUIEnabled()const
{
	return IsUIEnabled;
}

//Called from player controller when hoovered over
void ASeat::SetUIEnabled(const bool X)
{
	//Enable flag for Widget to make UI visible
	IsUIEnabled = X;
	
	if (IsUIEnabled)
	{
		//start timer for lag for its disappearance, snake effect
		GetWorldTimerManager().SetTimer(UITimer, this, &ASeat::UITimerExpired,0.25,false);
	}
	else
	{
		//Reset Timer
		GetWorldTimerManager().ClearTimer(UITimer);
	}
	
}

//Get Seat ID from world outliner (last 2 digits)
FString ASeat::GetSeatID()const
{
	return (this->GetName()).Mid(4,2);
}

//Set occupancy and ALSO light status
void ASeat::SetOccupied(const bool X)
{
	IsOccupied = X;
	(IsOccupied) ? IndicatorLight->SetVisibility(false) : IndicatorLight->SetVisibility(true);
}
//Return occupancy
bool ASeat::GetOccupied()const
{
	return IsOccupied;
}