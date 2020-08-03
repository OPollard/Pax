// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PaxProtoGameModeBase.generated.h"

UCLASS()
class PAXPROTO_API APaxProtoGameModeBase final : public AGameModeBase
{
	GENERATED_BODY()

public:

	APaxProtoGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	~APaxProtoGameModeBase();

private:
	
};
