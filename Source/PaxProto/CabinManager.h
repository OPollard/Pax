// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CabinManager.generated.h"

//Forward Declarations
class UPaxState;
class APaxSpawner;
class APax;
class APax_AIController;

//Flight Phase
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

//Delegate Declaration
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FManagerDelegate);

UCLASS()
class PAXPROTO_API ACabinManager final : public AActor
{
	GENERATED_BODY()
	
public:	
		ACabinManager();

		virtual void Tick(float DeltaTime) override;
	
	//Utility Functions
		void Initialise();
	UFUNCTION()
		void RegisterNewPax(AActor* NewActor);
	UFUNCTION(BlueprintCallable)
		void ResetRechargeTime();
		void RunPickUpUI();
		void ResetPickUpUI();
	UFUNCTION()
		void PaxHasDied();
	UFUNCTION()
		void PaxIsSleeping();
		void WakePaxUp();
	UFUNCTION()
		void CheckDoorStatus();
		void TimerTick();
		void Launch();
		void Land();

	//Getters/Setters
	UFUNCTION(BlueprintPure)
		float GetRemainingTime()const;
	UFUNCTION(BlueprintPure)
		float GetWarningTime()const;
	UFUNCTION(BlueprintPure)
		float GetRechargeTime()const;
	UFUNCTION(BlueprintPure)
		float GetCurrentRechargeTime()const;
	UFUNCTION(BlueprintPure)
		float GetTotalEarnableMoney()const;
	UFUNCTION(BlueprintPure)
		FText GetTotalEarnableMoneyAsText()const;
	UFUNCTION(BlueprintPure)
		float GetTotalTargetMoney()const;
	UFUNCTION(BlueprintPure)
		float GetFirstClassTarget()const;
	UFUNCTION(BlueprintPure)
		float GetBusinessTarget()const;
	UFUNCTION(BlueprintPure)
		float GetEconomyTarget()const;
	UFUNCTION(BlueprintPure)
		float GetTempPickUpAmount()const ;
	UFUNCTION(BlueprintPure)
		FText GetTempPickUpAmountAsText()const;
		void SetTempPickUpAmount(float X);
		void SetTotalTargetMoney(float Value);
		void SetTotalEarnableMoney();
		void SetTotalEarnableMoney(float X);
	UFUNCTION(BlueprintPure)
		bool GetPickUpUIStatus()const;
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
	
	//Types
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APax> Pax;
	UPROPERTY(EditAnywhere)
		TArray<APax*> Passengers;
	
	//Delegates
	UPROPERTY(BlueprintAssignable)
		FManagerDelegate TakeOffPulse;
	UPROPERTY(BlueprintAssignable)
		FManagerDelegate LandingPulse;

protected:
	
		virtual void BeginPlay() override;

private:	

	//assigned in constructor/begin play using constants
	float LevelBoardingTime{ 0.0f };
	float LevelFlightTime{ 0.0f };
	float RechargeTime{ 0.0f };

	EPhase FlightPhase;

	FTimerHandle ManagerHandle;
	FTimerHandle LevelTimeHandle;
	FTimerHandle SleepTimeHandle;

	//Reference so we can find the bounds of the paxspawner in the level
	APaxSpawner* PaxSpawner = nullptr;

	//store sleeping pax controllers
	UPROPERTY()TArray<APax_AIController*> TempAIControllers;
	UPROPERTY()TArray<UPaxState*> PassengerStates;
	UPROPERTY()TArray<AActor*> Doors;
	UPROPERTY()TArray<AActor*> Seats;
	UPROPERTY()TArray<AActor*> Toilets;
	UPROPERTY()TArray<AActor*> WaitingAreas;
	UPROPERTY()TArray<AActor*> UnseatedPassengers;

	int32 PaxOnboard{ 0 };
	int32 NumSeatsInLevel{ 0 };

	float TotalTargetMoney{ 0.0f };
	float FirstClassTarget{ 0.0f };
	float BusinessTarget{ 0.0f };
	float EconomyTarget{ 0.0f };
	float TotalEarnableMoney{ 0.0f };
	float TempPickUpAmount{ 0.0f };

	bool EnablePickUpUI{ false };
	bool GoForLaunch{ true };
	bool GoForLand{ true };

};
