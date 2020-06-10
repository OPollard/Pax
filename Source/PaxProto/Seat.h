// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Seat.generated.h"


class UBoxComponent;

UCLASS()
class PAXPROTO_API ASeat : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASeat();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetUIEnabled(bool x);
	UFUNCTION(BlueprintCallable)
	bool GetUIEnabled();
	UFUNCTION(BlueprintPure)
		FString GetSeatID();

	void SetOccupied(bool x);
	UFUNCTION(BlueprintPure)
	bool GetOccupied();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Seat")
	UBoxComponent* CollisionBox = nullptr;

	bool isOccupied{ false };
	bool isUIEnabled{ false };
};
