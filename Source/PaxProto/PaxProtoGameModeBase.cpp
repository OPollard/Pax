// Copyright of Codebird Ltd 2020 - inclusive © 

#include "PaxProtoGameModeBase.h"
#include "MyPlayerController.h"



APaxProtoGameModeBase::APaxProtoGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	//Enables the game to use our own custom player controller
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