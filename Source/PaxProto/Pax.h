// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pax.generated.h"

//Forward Declarations
class ASeat;
class AToilet;
class UPaxState;
class ACabinManager;
class AWaitingArea;
class UAudioComponent;
class ACart;

//Pax desired location
UENUM()
enum ETarget
{
	SELF, TARGETSEAT, CURRENTSEAT, TOILET, WAITINGAREA, NUMOFTARGETS
};
//Pax Influence Affect
UENUM()
enum EInfluenceAffect
{
	NEGATIVE, NONE, POSITIVE, NUMOFAFFECTS
};

UCLASS()
class PAXPROTO_API APax final : public ACharacter
{
	GENERATED_BODY()

protected:
		virtual void BeginPlay() override;

public:

		APax();
	
		virtual void Tick(float DeltaTime) override;

		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Utility Functions
		void Clicked();
		void Released();
		void UpdateState()const;
		void ManageTarget(AActor* Target);
		void ManageTargetPost();
		void AdaptSpeeds()const;
	UFUNCTION(BlueprintNativeEvent)
		void FoodOrder();
	UFUNCTION(BlueprintCallable)
		void FoodOrderReset();

	//Getters/Setters
	UFUNCTION(BlueprintCallable)
		void SetDeployLocation(FVector Location);
	UFUNCTION(BlueprintCallable)
		FVector GetDeployLocation() const;
		void SetPreMoveLocation(FTransform PaxOrientation);
		FTransform GetPreMoveLocation()const;
		void SetViewStatInfo(bool X);
	UFUNCTION(BlueprintCallable)
		bool GetViewStatInfo()const;
		void SetEnableStateUpdate(bool X);
		bool GetEnableStateUpdate()const;
		void SetSphereSpawnLocation(FVector Location);
	UFUNCTION(BlueprintCallable)
		FVector GetSphereSpawnLocation();
	UFUNCTION(BlueprintCallable)
		void SetInfluence(const TArray<AActor*>& NearbyActors, bool FoundActors);
	UFUNCTION(BlueprintCallable)
		ETarget GetTargetPlace()const;
	UFUNCTION(BlueprintCallable)
		EInfluenceAffect GetInfluenceAffect();
		void SetInfluenceAffect(EInfluenceAffect Affect);
		void SetEnableTextureOverlay(bool X);//TODO confirm
	UFUNCTION(BlueprintCallable)
		bool GetEnableTextureOverlay();  //TODO confirm
		
	// Utility TICK Functions //////////////////////

		void TargetAcquiring();
		void SelfAcquiring();
		void TargetSeatAcquiring();
		void CurrentSeatAcquiring();
		void ToiletAcquiring()const;
		void CheckIsMoving()const;	

	//Component Handles
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* DeathScream = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* BlockedHuff = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* Accept = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAudioComponent* StatFillUp = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPaxState* State = nullptr;

private:
	
	//Variables
	bool EnableTextureOverlay;
	bool EnableStateUpdate;
	bool EnableStatInfo;
	bool FoodRequest;

	ETarget TargetPlace;
	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<EInfluenceAffect> InfluenceAffect;

	//Locations and offset targets (to avoid Pax targeting middle of a mesh)
	FVector DeployLocation;
	FVector SeatDeployLocationOffset;
	FVector ToiletDeployLocationOffset;
	FVector FloatingPaxSphereLocation;

	//Cached orientation
	FTransform PreMoveTransform;

	//Pointers
	ASeat* TargetSeat = nullptr;
	ASeat* CurrentSeat = nullptr;
	AToilet* Toilet = nullptr;
	AToilet* CurrentToilet = nullptr;
	ACabinManager* Manager = nullptr;
	AWaitingArea* WaitingArea = nullptr;
	ACart* Cart = nullptr;

	//Timers
	FTimerHandle PaxTimerHandle;
	FTimerHandle PaxFoodRequestHandle;
};
