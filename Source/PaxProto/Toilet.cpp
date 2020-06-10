// Copyright of Night Owls 2020 - inclusive ©


#include "Toilet.h"
#include "Components/BoxComponent.h"

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

	//Configure Seat Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Click Box"));
	CollisionBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionBox->SetBoxExtent(FVector(75.0f, 32.0f, 70.0f));
	CollisionBox->SetCollisionProfileName("UI");
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

