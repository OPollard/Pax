// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Money.generated.h"

//Forward Declarations
class UPaxState;
class ACabinManager;

UCLASS()
class PAXPROTO_API AMoney final : public AActor
{
	GENERATED_BODY()
	
public:	
		AMoney();
	
		virtual void Tick(float DeltaTime) override;
	
	//Utility Functions
	UFUNCTION(BlueprintCallable)
		void HooveredOver();

protected:
		virtual void BeginPlay() override;

private:

	//Pointers
	UActorComponent* OwningActorComponent = nullptr;
	UPaxState* PaxState = nullptr;
	AActor* ResultActor = nullptr;
	ACabinManager* CabinManager = nullptr;
};
