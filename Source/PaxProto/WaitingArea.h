// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaitingArea.generated.h"

class UBoxComponent;
class URectLightComponent;
class APax;

UCLASS()
class PAXPROTO_API AWaitingArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaitingArea();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	UFUNCTION()
	void CustomOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void CustomOnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetOccupied(bool X);
	bool GetOccupied()const;
	
	UFUNCTION(BlueprintNativeEvent)
	void ChangeSourceTexture(bool IsOccupied);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URectLightComponent* IndicatorLight = nullptr;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	APax* Passenger = nullptr;

	bool Occupied;
	UPROPERTY(EditAnywhere)
	UBoxComponent* WaitingAreaBox = nullptr;
	


};
