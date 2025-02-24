// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gate.generated.h"


//Forward Declarations
class ACabinManager;
class UBoxComponent;
class USpotLightComponent;

UCLASS()
class PAXPROTO_API AGate : public AActor
{
	GENERATED_BODY()
	
public:	
	
		AGate();
	
		virtual void Tick(float DeltaTime) override;
	
	//Custom Trigger Events
	UFUNCTION()
		void CustomOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Utility Functions
	UFUNCTION(BlueprintCallable)
		void MovePax();
	UFUNCTION()
		void TakeOffRecieve();

	//Component Handles
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* GateBox = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpotLightComponent* SpotLight = nullptr;

protected:
	
		virtual void BeginPlay() override;

private:
	
	ACabinManager* Manager = nullptr;
	
	//Timer designed to create a slight gap between old pax moving away and the second pax spawning in and creating a collision
	FTimerHandle PaxSpawnInterval;

	bool EnableSpawn{ true };
	
	//Static reference to the current position in the queue to be spawned, initialised below
	static int32 PaxSelector;

}; int32 AGate::PaxSelector = 0;
