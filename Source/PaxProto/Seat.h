// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Seat.generated.h"


class UBoxComponent;
class UPointLightComponent;

UCLASS()
class PAXPROTO_API ASeat final : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASeat();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetUIEnabled(bool X);
	UFUNCTION(BlueprintCallable)
	bool GetUIEnabled()const;
	void UITimerExpired();

	UFUNCTION(BlueprintPure)
	FString GetSeatID()const;

	void SetOccupied(bool X);
	UFUNCTION(BlueprintPure)
	bool GetOccupied()const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FTimerHandle UITimer;

	UPROPERTY(EditAnywhere, Category = "Seat")
	UBoxComponent* CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Seat")
	UPointLightComponent* IndicatorLight = nullptr;

	bool IsOccupied{ false };
	bool IsUIEnabled{ false };
};
