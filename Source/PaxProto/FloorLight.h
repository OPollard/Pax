// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorLight.generated.h"

UCLASS()
class PAXPROTO_API AFloorLight : public AActor
{
	GENERATED_BODY()
	
public:	
	
		AFloorLight();
	
		virtual void Tick(float DeltaTime) override;

protected:
	
		virtual void BeginPlay() override;

};
