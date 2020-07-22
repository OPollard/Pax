// Copyright of Night Owls 2020 - inclusive ©


#include "Toilet.h"
#include "Components/BoxComponent.h"
#include "Components/RectLightComponent.h"

// Sets default values
AToilet::AToilet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Guard against having no root
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	}

	//Configure Toilet Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Click Box"));
	CollisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetBoxExtent(FVector(75.0f, 32.0f, 70.0f));
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

	isOccupied = false;
}

// Called when the game starts or when spawned
void AToilet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToilet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Set occupancy status
void AToilet::SetOccupied(bool x)
{
	isOccupied = x;
	IndicatorLight->SetIntensity(((x)? 1500.0f : 100.0f));
}

//Get occupancy status
bool AToilet::GetOccupied()
{
	return isOccupied;
}


