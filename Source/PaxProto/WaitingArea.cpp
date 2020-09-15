// Copyright of Night Owls 2020 - inclusive © 

#include "WaitingArea.h"
#include "Components/BoxComponent.h"
#include "Components/RectLightComponent.h"
#include "Pax.h"
#include "PaxState.h"


// Sets default values
AWaitingArea::AWaitingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Guard against having no root
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	}


	WaitingAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WaitingAreaBox"));
	WaitingAreaBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	WaitingAreaBox->SetBoxExtent(FVector(80.0f,80.0f,50.0f));

	IndicatorLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Indicator"));
	IndicatorLight->SetAutoActivate(true);
	IndicatorLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	IndicatorHand = CreateDefaultSubobject<URectLightComponent>(TEXT("IndicatorHand"));
	IndicatorHand->SetAutoActivate(true);
	IndicatorHand->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Occupied = false;

}

// Called when the game starts or when spawned
void AWaitingArea::BeginPlay()
{
	Super::BeginPlay();
	
	
	
	WaitingAreaBox->OnComponentBeginOverlap.AddDynamic(this, &AWaitingArea::CustomOnComponentBeginOverlap);
	WaitingAreaBox->OnComponentEndOverlap.AddDynamic(this, &AWaitingArea::CustomOnComponentEndOverlap);
	
	
	IndicatorLight->SetIntensity(50.0f);
	IndicatorHand->SetIntensity(10.0f);

}

// Called every frame
void AWaitingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//when the pax enters the wait area, NOT triggered passing by
void AWaitingArea::CustomOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Passenger = Cast<APax>(OtherActor);
	//check passenger end destination is wait area
	if ((Passenger) && (Passenger->GetTargetPlace() == ETarget::WAITINGAREA) && !this->GetOccupied())
	{
		//log it on as in the wait area
		if (UPaxState* PS = Passenger->FindComponentByClass<UPaxState>())
		{
			SetOccupied(true);
			PS->SetWaitArea(true);
		}
		
	}
}

//when the pax leaves the wait area
void AWaitingArea::CustomOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//check its leaving the wait area
	if ((Passenger))
	{
		//get state
		if (UPaxState* PS = Passenger->FindComponentByClass<UPaxState>())
		{
			//ensure it was previously in wait area
			if (PS->GetWaitArea())
			{
				PS->SetWaitArea(false);
				SetOccupied(false);
				

			}
			
		}
		
	}
}

//Change Light Pattern 
void AWaitingArea::ChangeSourceTexture_Implementation(bool IsOccupied)
{
	//dummy function, check BP
}

void AWaitingArea::SetOccupied(bool X)
{
	Occupied = X;
	
	if (X)
	{
		IndicatorLight->SetIntensity(500.0f);
		IndicatorHand->SetIntensity(60.0f);
	}
	else
	{
		IndicatorLight->SetIntensity(50.0f);
		IndicatorHand->SetIntensity(10.0f);
	}
	
}

bool AWaitingArea::GetOccupied()const
{
	return Occupied;
}

