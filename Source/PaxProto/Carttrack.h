// Copyright of Codebird Ltd 2020 - inclusive © 
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Carttrack.generated.h"

//Forward Declarations
class USplineComponent;

UCLASS()
class PAXPROTO_API ACarttrack : public AActor
{
	GENERATED_BODY()
	
public:	
		ACarttrack();
		virtual void Tick(float DeltaTime) override;

	//Component Handles
	UPROPERTY(EditAnywhere, Category = "Track")
		USplineComponent* CartTrack = nullptr;

	//Getters/Setters
	USplineComponent* GetSplineTrack();

protected:

	// Called when the game starts or when spawned
		virtual void BeginPlay() override;

};
