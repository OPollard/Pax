// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PaxProtoGameModeBase.h"
#include "MyPlayerController.h"



APaxProtoGameModeBase::APaxProtoGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PlayerControllerClass = AMyPlayerController::StaticClass();	
		
}

void APaxProtoGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void APaxProtoGameModeBase::Tick(float DeltaTime)
{

}

APaxProtoGameModeBase::~APaxProtoGameModeBase()
{
;
}