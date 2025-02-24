// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cart.generated.h"

//Forward Declarations
class ACarttrack;
class ACabinManager;

UCLASS()
class PAXPROTO_API ACart : public AActor
{
	GENERATED_BODY()
	
public:	
		ACart();

		virtual void Tick(float DeltaTime) override;

	//Utility Functions
	UFUNCTION(BlueprintNativeEvent)
		void RunTimeLine(bool Direction);
	UFUNCTION(BlueprintCallable)
		void UpdateTimeLine(float Alpha);
	UFUNCTION(BlueprintCallable)
		void FinishedTimeLine();
	UFUNCTION(BlueprintCallable)
		void RightClicked();
	

	//Component Handles
	UPROPERTY(EditAnywhere, Category = "Cart")
		UStaticMeshComponent* CartMesh = nullptr;
	UPROPERTY(EditAnywhere, Category = "Cart")
		ACarttrack* cTrack = nullptr;

protected:
		virtual void BeginPlay() override;

private:
	ACabinManager* Manager = nullptr;
	bool ForwardDir;
};
