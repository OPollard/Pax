// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Toilet.generated.h"

//Forward Declarations
class UBoxComponent;
class URectLightComponent;

UCLASS()
class PAXPROTO_API AToilet final : public AActor
{
	GENERATED_BODY()
	
public:	

		AToilet();

		virtual void Tick(float DeltaTime) override;

	//Getters/Setters
	UFUNCTION(BlueprintCallable)
		void SetOccupied(bool X);
	UFUNCTION(BlueprintCallable)
		bool GetOccupied()const;
		void SetOccupier(AActor* Pax);
		AActor* GetOccupier() const;
	
	//Component Handles
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Toilet")
		UBoxComponent* CollisionBox = nullptr;
	UPROPERTY(EditAnywhere, Category = "Toilet")
		URectLightComponent* IndicatorLight = nullptr;

protected:

		virtual void BeginPlay() override;

private:

	AActor* Occupier = nullptr;

	bool FrontToiletOccupied;
	bool RearToiletOccupied;
	

};
