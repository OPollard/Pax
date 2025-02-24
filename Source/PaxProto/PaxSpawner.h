// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaxSpawner.generated.h"

//Forward Declarations
class UBoxComponent;
class APax;

UCLASS()
class PAXPROTO_API APaxSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	
		APaxSpawner();
	
		virtual void Tick(float DeltaTime) override;
		
	//Utility Functions
		void GetSpawnPoint(const int32 NumOfPaxToSpawn, FVector& Location);
		bool CheckWithinBounds(const FVector& Point);

	//Component Handles
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* SpawnArea = nullptr;
	
protected:
		virtual void BeginPlay() override;

private:
		bool Q; //represents turn phase
		FVector SpawnPoint;
};
