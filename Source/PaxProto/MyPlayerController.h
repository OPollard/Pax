// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
class APax;

UCLASS()
class PAXPROTO_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMyPlayerController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;
	
	void SetClicked();
	void SetReleased();

private:

	//Pointers
	APax* PaxHandle = nullptr;
	AActor* Target = nullptr;

	//Variables
	bool ClickedOn;
	float LineTraceRange = 1400.0f; //Distance from Camera source to trace.... CHECK CAMERA Z value per level
	float LiftedElevationHeight = 450.0f; //Height to hoover at when clicked ... CHECK PER LEVEL
	
	//Functions
	void SearchForActorAtCursor(ECollisionChannel Trace);
	FVector GetCursorHooverPosition();
	void CheckAndRefreshPaxUI();
	void SendTriggerToTarget();
	

};
