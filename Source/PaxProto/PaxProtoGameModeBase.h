// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PaxProtoGameModeBase.generated.h"

/**
 * 
 */

UCLASS()
class PAXPROTO_API APaxProtoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	APaxProtoGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	~APaxProtoGameModeBase();

private:
	
};
