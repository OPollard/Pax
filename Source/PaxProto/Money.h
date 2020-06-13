// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Money.generated.h"


class UPaxState;
class ACabinManager;

UCLASS()
class PAXPROTO_API AMoney : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AMoney();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void HooveredOver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UActorComponent* OwningActorComponent = nullptr;
	UPaxState* PaxState = nullptr;
	AActor* ResultActor = nullptr;
	ACabinManager* CabinManager = nullptr;
};
