// Copyright of Night Owls 2020 - inclusive © 


#include "Money.h"
#include "PaxState.h"

// Sets default values
AMoney::AMoney()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}

// Called when the game starts or when spawned
void AMoney::BeginPlay()
{
	Super::BeginPlay();
	OwningActorComponent = (GetOwner()->FindComponentByClass(UPaxState::StaticClass()));
	PaxState = Cast<UPaxState>(OwningActorComponent);
}

// Called every frame
void AMoney::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//called from blueprint when cursor is overhead
void AMoney::HooveredOver()
{
	Destroy();
	float NewMoney = PaxState->GetMoney() - (float)(PaxState->GetDeltaSum());
	PaxState->SetMoney(NewMoney);
}

