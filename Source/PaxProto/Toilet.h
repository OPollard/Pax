// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Toilet.generated.h"

class UBoxComponent;
class URectLightComponent;

UCLASS()
class PAXPROTO_API AToilet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToilet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Toilet")
		UBoxComponent* CollisionBox = nullptr;
	UPROPERTY(EditAnywhere, Category = "Toilet")
		URectLightComponent* IndicatorLight = nullptr;

	void SetOccupied(bool x);
	bool GetOccupied();

private:
	
	bool isOccupied;

};
