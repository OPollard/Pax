// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

//Overall Player Title Rank
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
class PAXPROTO_API USaveData final : public USaveGame
{
	GENERATED_BODY()

public:

		USaveData();
	
	//Utility Functions
	UFUNCTION(BlueprintCallable)
		void UpdateRank();
	UFUNCTION(BlueprintCallable)
		void Reset();
	
	//Properties to SAVE
	UPROPERTY(BlueprintReadWrite)
		FString PlayerName;
	UPROPERTY(BlueprintReadWrite)
		ERank PlayerRank;
	UPROPERTY(BlueprintReadWrite)
		int32 LifeTimeEarnings;
	UPROPERTY(BlueprintReadWrite)
		int32 PaxFlown;


	
};
