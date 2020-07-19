// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CabinManager.generated.h"

class UPaxState;

UENUM()
enum EPhase
{
	BOARDING,
	TAKEOFF,
	CRUISE,
	LANDING,
	DISEMBARKING,

	NUMOFPHASES
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FManagerDelegate);

UCLASS()
class PAXPROTO_API ACabinManager : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(BlueprintAssignable)
	FManagerDelegate TakeOffPulse;

	ACabinManager();

	virtual void Tick(float DeltaTime) override;
	
	void Initialise();

	//Game timing
	UFUNCTION(BlueprintPure)
	float GetRemainingTime(); 
	UFUNCTION(BlueprintPure)
	float GetTotalEarnableMoney();
	UFUNCTION(BlueprintPure)
	FText GetTotalEarnableMoneyAsText();

	void SetTotalEarnableMoney();
	void SetTotalEarnableMoney(float x);

	void SetTempPickUpAmount(float x);
	UFUNCTION(BlueprintPure)
	float GetTempPickUpAmount();
	UFUNCTION(BlueprintPure)
	FText GetTempPickUpAmountAsText();

	void RunPickUpUI();
	void ResetPickUpUI();

	UFUNCTION(BlueprintPure)
	bool GetPickUpUIStatus();
	
	UFUNCTION()
	void RegisterNewPax(AActor* NewActor);

	UFUNCTION(BlueprintPure)
	int32 GetNumPaxOnboard();
	UFUNCTION(BlueprintPure)
	int32 GetNumSeatsInLevel();
	UFUNCTION(BlueprintPure)
	int32 GetRemainingSeats();
	UFUNCTION(BlueprintPure)
	float GetLoadFactor();

	UFUNCTION(BlueprintCallable)
	void SetCurrentPhase(EPhase NewPhase);
	UFUNCTION(BlueprintPure)
	EPhase GetCurrentPhase();

protected:
	
	virtual void BeginPlay() override;

private:	

	UFUNCTION()
	void PaxHasDied();

	UFUNCTION()
	void CheckDoorStatus();
	//Game Timings - maybe reconstruct a bit
	void TimerTick();
	void Launch();

	//temporary
	float LevelBoardingTime{ 0.0f };
	float LevelFlightTime{ 0.0f };

	EPhase FlightPhase;

	FTimerHandle ManagerHandle;
	FTimerHandle LevelTimeHandle;
	
	TArray<AActor*> Passengers;
	TArray<UPaxState*> PassengerStates;
	TArray<AActor*> Doors;
	TArray<AActor*> Seats;

	int32 PaxOnboard{ 0 };
	int32 NumSeatsInLevel{ 0 };

	float TotalEarnableMoney{ 0.0f };
	float TempPickUpAmount{ 0.0f };

	bool EnablePickUpUI{ false };
	bool GoForLaunch{ true };

};
