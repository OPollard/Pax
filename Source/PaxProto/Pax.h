// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pax.generated.h"

//Declarations
class ASeat;
class AToilet;
class UPaxState;
class ACabinManager;

UCLASS()
class PAXPROTO_API APax : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Sets default values for this character's properties
	APax();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Cursor Interactions	////////////////////////////
	void Clicked();
	void Released();

	UFUNCTION(BlueprintCallable)
	void SetDeployLocation(FVector Location);
	UFUNCTION(BlueprintCallable)
	FVector GetDeployLocation();

	void SetPreMoveLocation(FTransform PaxOrientation);
	FTransform GetPreMoveLocation();

	void SetViewStatInfo(bool x);
	UFUNCTION(BlueprintCallable)
	bool GetViewStatInfo();

	void UpdateState();

	// Utility Functions	/////////////////////////////

	void ManageTarget(AActor* Target);
	void AdaptSpeeds();

	UFUNCTION(BlueprintCallable)
		void SetInfluence(const TArray<AActor*>& NearbyActors, bool foundActors);
		
	// Utility TICK Functions //////////////////////

	void TargetAcquiring();
	void CheckIsMoving();

private:
	
	//Variables
	bool EnableStatInfo;

	//Locations and offset targets (to avoid Pax targeting middle of a mesh)
	FVector DeployLocation;
	FVector SeatDeployLocationOffset;
	FVector ToiletDeployLocationOffset;

	//Cached orientation
	FTransform PreMoveTransform;

	//Pointers
	ASeat* TargetSeat = nullptr;
	ASeat* CurrentSeat = nullptr;
	AToilet* Toilet = nullptr;
	UPaxState* State = nullptr;
	ACabinManager* Manager = nullptr;

	FTimerHandle PaxTimerHandle;

};
