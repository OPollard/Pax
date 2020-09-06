// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Pax_AnimInstance.generated.h"

/**
 * 
 */
class APax;
class UPaxState;

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class PAXPROTO_API UPax_AnimInstance final : public UAnimInstance
{
	GENERATED_BODY()
public:
	/** Is Floating */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaxPosition)
		bool IsFloating;
	
	/** Is Sitting In Seat*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaxPosition)
		bool IsSitting;

	// Is pax moving like walking, eg NOT zero velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaxPosition)
		bool IsMoving;

	// Trigger for death animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaxState)
		bool IsAlive;

	// Trigger for sleeping animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaxState)
		bool IsTired;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaxState)
		bool IsUncomfortable;

	// Handle to hold speed, accessible through anim_bp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PaxState)
		float Pax_WalkSpeed;

	// Fetches new anim walk speed from pax state
	UFUNCTION(BlueprintCallable, Category = PaxState)
		void SetPaxWalkSpeed();

public:

	//Find owner of this animation as a pawn
	APawn* OwningPawn = nullptr;
	
	//Pax Handle and ready for cast
	APax* PaxPawn = nullptr;

	//Pax State Handle
	UPaxState* PaxState = nullptr;
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

};
