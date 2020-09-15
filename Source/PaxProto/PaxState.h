// Copyright of Codebird Ltd 2020 - inclusive © 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "PaxState.generated.h"

//Forward Declarations
class AMoney;

//Age group
UENUM()
enum EGeneration
{	YOUNG, ADULT, OLD, NUMOFGENERATIONS	};
//Level of politeness
UENUM()
enum EPoliteness
{	UNSOCIAL, NEUTRAL, SOCIAL,NUMOFPOLITENESS };
//Wealth category
UENUM()
enum EWealth
{	POOR, AVERAGE, WEALTHY, RICH, NUMOFWEALTH };
//UI indicator
UENUM()
enum EIndicator
{	LARGEDECREASE = -2, SMALLDECREASE = -1, NOCHANGE = 0,
	SMALLINCREASE = 1, LARGEINCREASE = 2, NUMOFINDICATORS };

//Delegate Declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPaxDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PAXPROTO_API UPaxState final : public UActorComponent
{
	GENERATED_BODY()

protected:
		virtual void BeginPlay() override;

public:
		UPaxState();

		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Utility Functions
		void Initialise();
		void UpdateCores();
		bool SpawnMoney()const;
		void ResetStates();
		bool IsAlive();
		bool IsTired();
		bool IsUncomfortable();
		void ResetDeltaSum();

	//Core Getters/Setters
	UFUNCTION(BlueprintPure)
		EGeneration GetAgeGroup()const;
	UFUNCTION(BlueprintPure)
		EPoliteness GetPoliteness()const;
	UFUNCTION(BlueprintPure)
		int32 GetAge()const;
	UFUNCTION(BlueprintPure)
		FString GetName()const;
		void SetNutrition(float Value);
	UFUNCTION(BlueprintPure)
		float GetNutrition()const;
	UFUNCTION(BlueprintPure)
		int32 GetNutritionIndicator()const;
		void SetExcrement(float Value);
	UFUNCTION(BlueprintPure)
		float GetExcrement()const;
	UFUNCTION(BlueprintPure)
		int32 GetExcrementIndicator()const;
	UFUNCTION(BlueprintPure)	
		float GetSocietal()const;	
	UFUNCTION(BlueprintPure)
		int32 GetSocialIndicator()const;
		void SetSocialBias(float Value);
		void SetEnergy(float Value);
	UFUNCTION(BlueprintPure)
		float GetEnergy()const;
	UFUNCTION(BlueprintPure)
		int32 GetEnergyIndicator() const;
		void SetAnnouncedSleep(bool X);
	UFUNCTION(BlueprintPure)
		float GetAnimPlaySpeed()const;
		void SetAnimPlaySpeed(float WalkSpeed);

	//Status Getters/Setters
		void SetOnboard(bool X);
		bool GetOnboard()const;
		void SetSitting(bool X);
	UFUNCTION(BlueprintPure)
		bool GetSitting()const;
		void SetFloating(bool X);
	UFUNCTION(BlueprintPure)
		bool GetFloating()const;
		void SetInToilet(bool X);
		bool GetInToilet()const;
	UFUNCTION(BlueprintCallable)
		void SetMoving(bool X);
	UFUNCTION(BlueprintPure)
		bool GetMoving()const;
		void SetWaitArea(bool X);
		bool GetWaitArea()const;

	//Money Getters/Setters
	UFUNCTION(BlueprintPure)
		FText GetMoneyAsText()const;
		float GetMoney()const;
		void SetMoney(float X);
		int GetDeltaSum()const;
		void SetAwaitingPickUp(bool X);
		bool GetAwaitingPickup()const;

	//Delegates to be broadcast across the game
	FPaxDelegate OnDeath;
	FPaxDelegate Sleep;

	//Types
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AMoney> MoneyActor;

private:

	int32 Age{ 0 };
	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<EGeneration> AgeGroup;
	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<EPoliteness> Politeness;
	UPROPERTY(VisibleAnywhere)
		TEnumAsByte<EWealth> Wealthiness;
	UPROPERTY(VisibleAnywhere)
	float Money{ 0.0f };

	bool Onboard{ false };
	bool Sitting;
	bool Floating;
	bool Moving;
	bool InToilet;
	bool WaitArea{ false };
	
	//Core Values
	float Nutrition{ 100.00f };
	float Energy{ 75.00f };
	float Excrement{ 0.00f };
	float Societal{ 50.00f };
	
	//Core Indicators
	float NutritionChx{ 0.0f };
	float EnergyChx{ 0.0f };
	float ExcrementChx{ 0.0f };
	float SocialBias{ 0.0f };

	int Delta{ 0 };
	int WealthAdjustment{ 0 };
	int DeltaSum{ 0 };
	int MoneyDropLimit{ 0 };
	float DrainDatum{ 0.0f };
	
	bool AwaitingPickup{ false };
	bool Alive{ true };
	bool AnnouncedDeath{ false };
	bool AnnouncedSleep{ false };
	float AnimationPlaySpeed{ };

	FString Name = "Nobody";
	TArray<FString> Names { "Ben Dover", "Wang Liqin", "Stef Crabs", "Dixie Normous", "Jack Goff", "Justin Sider", "Thomas Fister", "Wendy Wacko", "Brownie Shytles", "Richard Pound", "Peter Bunerz", "Heath Cuckburn", "Thea Beaver", //13
		"Mike Wiener", "Woody Held", "Mike Litoris", "Chew Weedman", "Jack Kanoff", "William Stroker", "Peter Fletcher", "Liam Walker", "Ashton Davidson", "Bailey Hunter", "Alfie Ball", "Kyson Davidson", "Myles Mathis", //13
		"Brayan Gentry", "Marcelo Barry", "Devon Rojas", "Jamie Mills", "Blair Williamson", "Kris Hart", "Jordan Doyle", "Aiden West", "Jordan Adkins", "Willy Mosley", "Drew Reynolds", "Avery Hodges", "Nicky Cline", //13
		"Glen Byrne", "Ali Harvey", "Phoneix Fletcher", "Reed Shaw", "Lane Russel", "Jesse Day", "Alex Wilkerson", "Glen Shafter", "Marley Fitzpatrick", "Jody Mercado", "Brook Palmer", "Danni Price", "Jess Burke", "Kit Pearce", //14
		"Chris Clark", "Jaden Meyers", "Gene Hopper", "Reed Madden", "Bailey Douglas", "Charlie Mayo" }; //6
};
