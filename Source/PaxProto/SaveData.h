// Copyright of Night Owls 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

/**
 * 
 */
UENUM()
enum class ERank : uint8
{
	BEGINNER,
	INTERMEDIATE,
	EXPERT,
	SUPERHERO,

	NUMOFRANKS
};

UCLASS()
class PAXPROTO_API USaveData : public USaveGame
{
	GENERATED_BODY()

public:

	USaveData();
	
	UFUNCTION(BlueprintCallable)
	void UpdateRank();
	UFUNCTION(BlueprintCallable)
	void Reset();
	
	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;
	UPROPERTY(BlueprintReadWrite)
	ERank PlayerRank;
	UPROPERTY(BlueprintReadWrite)
	int32 LifeTimeEarnings;
	UPROPERTY(BlueprintReadWrite)
	int32 PaxFlown;


	
};
