// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Seat.generated.h"

//Forward Declarations
class UBoxComponent;
class UPointLightComponent;

UCLASS()
class PAXPROTO_API ASeat final : public AActor
{
	GENERATED_BODY()
	
public:	
	
		ASeat();

		virtual void Tick(float DeltaTime) override;

	//Utility Functions
		void UITimerExpired();

	//Getters/Setters
	UFUNCTION(BlueprintCallable)
		void SetUIEnabled(bool X);
	UFUNCTION(BlueprintCallable)
		bool GetUIEnabled()const;
	UFUNCTION(BlueprintPure)
		FString GetSeatID()const;
	UFUNCTION(BlueprintCallable)
		void SetOccupied(bool X);
	UFUNCTION(BlueprintPure)
		bool GetOccupied()const;
	
	//Component Handles
	UPROPERTY(EditAnywhere, Category = "Seat")
		UBoxComponent* CollisionBox = nullptr;
	UPROPERTY(EditAnywhere, Category = "Seat")
		UPointLightComponent* IndicatorLight = nullptr;

protected:

		virtual void BeginPlay() override;

private:

	FTimerHandle UITimer;

	bool IsOccupied{ false };
	bool IsUIEnabled{ false };
};
