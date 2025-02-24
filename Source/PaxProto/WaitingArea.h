// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaitingArea.generated.h"

//Forward Declarations
class UBoxComponent;
class URectLightComponent;
class APax;

UCLASS()
class PAXPROTO_API AWaitingArea : public AActor
{
	GENERATED_BODY()
	
public:	
	
		AWaitingArea();
	
		virtual void Tick(float DeltaTime) override;

	//Custom Trigger Events
	UFUNCTION()
		void CustomOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void CustomOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//Getters/Setters
		void SetOccupied(bool X);
		bool GetOccupied()const;
	
	//Utility Functions
	UFUNCTION(BlueprintNativeEvent)
		void ChangeSourceTexture(bool IsOccupied);

	//Component Handles
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		URectLightComponent* IndicatorLight = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		URectLightComponent* IndicatorHand = nullptr;
	UPROPERTY(EditAnywhere)
		UBoxComponent* WaitingAreaBox = nullptr;
	
protected:
	
		virtual void BeginPlay() override;

private:

	APax* Passenger = nullptr;

	bool Occupied;

};
