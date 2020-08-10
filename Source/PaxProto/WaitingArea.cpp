// Copyright of Night Owls 2020 - inclusive © 

#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Pax.h"
#include "WaitingArea.h"

// Sets default values
AWaitingArea::AWaitingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WaitingAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WaitingAreaBox"));
	WaitingAreaBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	WaitingAreaBox->SetBoxExtent(FVector(40.0f,40.0f,40.0f));

	LightOne = CreateDefaultSubobject<UPointLightComponent>(TEXT("TopLeftlight"));
	LightTwo = CreateDefaultSubobject<UPointLightComponent>(TEXT("TopRightlight"));
	LightThree = CreateDefaultSubobject<UPointLightComponent>(TEXT("BottomLeftlight"));
	LightFour = CreateDefaultSubobject<UPointLightComponent>(TEXT("BottomRightlight"));

	LightOne->AttachToComponent(WaitingAreaBox, FAttachmentTransformRules::KeepRelativeTransform);
	LightTwo->AttachToComponent(WaitingAreaBox, FAttachmentTransformRules::KeepRelativeTransform);
	LightThree->AttachToComponent(WaitingAreaBox, FAttachmentTransformRules::KeepRelativeTransform);
	LightFour->AttachToComponent(WaitingAreaBox, FAttachmentTransformRules::KeepRelativeTransform);

	LightOne->SetAutoActivate(true);
	LightTwo->SetAutoActivate(true);
	LightThree->SetAutoActivate(true);
	LightFour->SetAutoActivate(true);

	Occupied = false;

}

// Called when the game starts or when spawned
void AWaitingArea::BeginPlay()
{
	Super::BeginPlay();
	WaitingAreaBox->OnComponentBeginOverlap.AddDynamic(this, &AWaitingArea::CustomOnComponentBeginOverlap);
}

// Called every frame
void AWaitingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaitingArea::CustomOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	if(OtherActor->ActorHasTag(TEXT("Person")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Person Waiting"));
	}
}

void AWaitingArea::SetOccupied(bool X)
{
	Occupied = X;
}

bool AWaitingArea::GetOccupied()const
{
	return Occupied;
}

