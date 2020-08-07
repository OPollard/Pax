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

//Pax desired location
UENUM()
enum ETarget
{
	SELF, TARGETSEAT, CURRENTSEAT, TOILET, NUMOFTARGETS
};

UCLASS()
class PAXPROTO_API APax final : public ACharacter
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
	FVector GetDeployLocation() const;

	void SetPreMoveLocation(FTransform PaxOrientation);
	FTransform GetPreMoveLocation()const;

	void SetViewStatInfo(bool X);
	UFUNCTION(BlueprintCallable)
	bool GetViewStatInfo()const;

	void UpdateState()const;
	void SetEnableStateUpdate(bool X);
	bool GetEnableStateUpdate()const;

	// Utility Functions	/////////////////////////////

	void ManageTarget(AActor* Target);
	void ManageTargetPost();
	void AdaptSpeeds()const;

	UFUNCTION(BlueprintCallable)
	void SetInfluence(const TArray<AActor*>& NearbyActors, bool FoundActors);
	UFUNCTION(BlueprintCallable)
		ETarget GetTargetPlace()const;
		
	// Utility TICK Functions //////////////////////

	void TargetAcquiring();
	void SelfAcquiring();
	void TargetSeatAcquiring();
	void CurrentSeatAcquiring();
	void ToiletAcquiring()const;
	void CheckIsMoving()const;

	
private:
	
	//Variables
	bool EnableStateUpdate;
	bool EnableStatInfo;
	ETarget TargetPlace;

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
	AToilet* CurrentToilet = nullptr;
	UPaxState* State = nullptr;
	ACabinManager* Manager = nullptr;

	//Timers
	FTimerHandle PaxTimerHandle;
};
