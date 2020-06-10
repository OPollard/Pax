// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Toilet.generated.h"

class UBoxComponent;

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

	UPROPERTY(EditAnywhere, Category = "Seat")
		UBoxComponent* CollisionBox = nullptr;

};
