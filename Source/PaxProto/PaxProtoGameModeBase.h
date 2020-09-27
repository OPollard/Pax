// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PaxProtoGameModeBase.generated.h"

UENUM()
enum ELevel
{
	MENU, VENUS, EARTH, MARS, NUMOFLEVELS
};

UCLASS()
class PAXPROTO_API APaxProtoGameModeBase final : public AGameModeBase
{
	GENERATED_BODY()

public:

	APaxProtoGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	~APaxProtoGameModeBase();

	UFUNCTION(BlueprintCallable)
		ELevel GetCurrentLevel();

	UFUNCTION(BlueprintCallable)
		ELevel ChooseRandomLevel();

private:

	static ELevel CurrentLevel;
	
}; ELevel APaxProtoGameModeBase::CurrentLevel = ELevel::MENU;
