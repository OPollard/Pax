// Copyright of Night Owls 2020 - inclusive © 


#include "Money.h"
#include "Kismet/GameplayStatics.h"
#include "PaxState.h"
#include "CabinManager.h"

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
	ResultActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACabinManager::StaticClass());
	CabinManager = Cast<ACabinManager>(ResultActor);
}

// Called every frame
void AMoney::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//called from blueprint when cursor is overhead
void AMoney::HooveredOver()
{
	if (PaxState)
	{
		//destroy coin actor
		Destroy();
		float DroppedMoney = PaxState->GetDeltaSum();
		float NewMoney = PaxState->GetMoney() - DroppedMoney;
		PaxState->SetMoney(NewMoney);

		PaxState->SetAwaitingPickUp(false);

		if (CabinManager)
		{
			//send dropped money to cabin manager to keep track of
			float NewTemp = CabinManager->GetTempPickUpAmount() + DroppedMoney;
			CabinManager->SetTempPickUpAmount(NewTemp);
			//enable UI to show collected amount
			CabinManager->RunPickUpUI();
			//increase game earnings
			CabinManager->SetTotalEarnableMoney((CabinManager->GetTotalEarnableMoney()) + DroppedMoney);
		}
		
		PaxState->ResetDeltaSum();
	}
}

