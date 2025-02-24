// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

//Door delegate to signal if the door has moved 
DECLARE_DYNAMIC_DELEGATE(FDoorDelegate);

UCLASS()
class PAXPROTO_API ADoor final : public AActor
{
	GENERATED_BODY()
	
public:	

		ADoor();
	
		virtual void Tick(float DeltaTime) override;

	//Getters/Setters
	UFUNCTION(BlueprintCallable)
		void SetLocked(bool X);
	UFUNCTION(BlueprintCallable)
		bool GetLocked() const;
	UFUNCTION(BlueprintCallable)
		void SetDoorStatus(bool X);
	UFUNCTION(BlueprintPure)
		bool GetDoorStatus() const;

	//Delegates
	FDoorDelegate DoorMoved;

protected:
	
		virtual void BeginPlay() override;

private:
	
	bool IsOpen{ true };
	bool Locked{ false };
};
