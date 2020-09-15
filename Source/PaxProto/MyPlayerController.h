// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

//Forward Declarations
class APax;

UCLASS()
class PAXPROTO_API AMyPlayerController final : public APlayerController
{
	GENERATED_BODY()

public:

		AMyPlayerController();

		virtual void BeginPlay() override;

		virtual void Tick(float DeltaTime) override;

		virtual void SetupInputComponent() override;
	
	//Getters/Setters
		void SetClicked();
		void SetReleased();

	//Functions
		void SearchForActorAtCursor(ECollisionChannel Trace);
		FVector GetCursorHooverPosition() const;
		void CheckAndRefreshPaxUI();
		void SendTriggerToTarget() const;

private:

	//Pointers
	APax* PaxHandle = nullptr;
	AActor* Target = nullptr;

	//Variables
	bool ClickedOn;
	float LineTraceRange = 1400.0f; //Distance from Camera source to trace.... CHECK CAMERA Z value per level
	float LiftedElevationHeight = 450.0f; //Height to hoover at when clicked ... CHECK PER LEVEL
	
};
