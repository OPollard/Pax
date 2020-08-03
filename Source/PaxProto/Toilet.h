// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Toilet.generated.h"

class UBoxComponent;
class URectLightComponent;

UCLASS()
class PAXPROTO_API AToilet final : public AActor
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

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Toilet")
		UBoxComponent* CollisionBox = nullptr;
	UPROPERTY(EditAnywhere, Category = "Toilet")
		URectLightComponent* IndicatorLight = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetOccupied(bool X);
	UFUNCTION(BlueprintCallable)
	bool GetOccupied()const;

	void SetOccupier(AActor* Pax);
	AActor* GetOccupier() const;

private:
	
	bool FrontToiletOccupied;
	bool RearToiletOccupied;
	AActor* Occupier = nullptr;

};
