// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CabinManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FManagerDelegate);

class UPaxState;
UCLASS()
class PAXPROTO_API ACabinManager : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintAssignable)
	FManagerDelegate TakeOffPulse;

	ACabinManager();

	virtual void Tick(float DeltaTime) override;


protected:
	
	virtual void BeginPlay() override;

private:	

	void Initialise();

	UFUNCTION()
	void PaxHasDied();

	UFUNCTION()
	void CheckDoorStatus();

	FTimerHandle ManagerHandle;
	
	TArray<AActor*> Passengers;
	TArray<UPaxState*> PassengerStates;
	TArray<AActor*> Doors;

};
