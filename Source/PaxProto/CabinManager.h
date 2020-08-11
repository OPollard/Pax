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
class PAXPROTO_API ACabinManager final : public AActor
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
	float GetRemainingTime()const;
	UFUNCTION(BlueprintPure)
	float GetWarningTime()const;
	UFUNCTION(BlueprintPure)
	float GetTotalEarnableMoney()const;
	UFUNCTION(BlueprintPure)
	FText GetTotalEarnableMoneyAsText()const;

	void SetTotalEarnableMoney();
	void SetTotalEarnableMoney(float X);

	void SetTempPickUpAmount(float X);
	UFUNCTION(BlueprintPure)
	float GetTempPickUpAmount()const ;
	UFUNCTION(BlueprintPure)
	FText GetTempPickUpAmountAsText()const;

	void RunPickUpUI();
	void ResetPickUpUI();

	UFUNCTION(BlueprintPure)
	bool GetPickUpUIStatus()const;
	
	UFUNCTION()
	void RegisterNewPax(AActor* NewActor);

	UFUNCTION(BlueprintPure)
	int32 GetNumPaxOnBoard()const;
	UFUNCTION(BlueprintPure)
	int32 GetNumSeatsInLevel()const;
	UFUNCTION(BlueprintPure)
	int32 GetRemainingSeats()const;
	UFUNCTION(BlueprintPure)
	float GetLoadFactor()const;

	UFUNCTION(BlueprintCallable)
	void GetUnseatedPax(TArray<AActor*>& UnseatedPax);

	UFUNCTION(BlueprintCallable)
	void SetCurrentPhase(EPhase NewPhase);
	UFUNCTION(BlueprintPure)
	EPhase GetCurrentPhase()const;

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

	//assigned in constructor/begin play using constants
	float LevelBoardingTime{ 0.0f };
	float LevelFlightTime{ 0.0f };

	EPhase FlightPhase;

	FTimerHandle ManagerHandle;
	FTimerHandle LevelTimeHandle;
	
	TArray<AActor*> Passengers;
	TArray<UPaxState*> PassengerStates;
	TArray<AActor*> Doors;
	TArray<AActor*> Seats;
	TArray<AActor*> Toilets;
	TArray<AActor*> WaitingAreas;
	TArray<AActor*> UnseatedPassengers;

	int32 PaxOnboard{ 0 };
	int32 NumSeatsInLevel{ 0 };

	float TotalEarnableMoney{ 0.0f };
	float TempPickUpAmount{ 0.0f };

	bool EnablePickUpUI{ false };
	bool GoForLaunch{ true };

};
