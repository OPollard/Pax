// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

//Door delegate to signal door has moved 
DECLARE_DYNAMIC_DELEGATE(FDoorDelegate);

UCLASS()
class PAXPROTO_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	

	//Delegates
	FDoorDelegate DoorMoved;

	// Sets default values for this actor's properties
	ADoor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetDoorStatus(bool x);
	UFUNCTION(BlueprintPure)
		bool GetDoorStatus();
	
	UFUNCTION(BlueprintCallable)
		void SetLocked(bool x);
	UFUNCTION(BlueprintCallable)
		bool GetLocked();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	bool isOpen{ true };
	bool Locked{ false };
};
