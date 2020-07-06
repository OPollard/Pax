// Copyright of Night Owls 2020 - inclusive ©

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "PaxState.generated.h"

//Forward Declarations
class AMoney;

// Characterises the age group
UENUM()
enum EGeneration
{
	YOUNG,
	ADULT,
	OLD,

	NUMOFGENERATIONS
};

UENUM()
enum EPoliteness
{
	UNSOCIAL,
	NEUTRAL,
	SOCIAL,

	NUMOFPOLITENESS
};

UENUM()
enum EWealth
{
	POOR,
	AVERAGE,
	WEALTHY,
	RICH,

	NUMOFWEALTH
};

//Preprocessor Directive to construct a MACRO DELEGATE where this name is found
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPaxDelegate);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PAXPROTO_API UPaxState : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UPaxState();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Delegate to be broadcasted across the game
	FPaxDelegate OnDeath;

	void Initialise();

	void UpdateCores();

	bool SpawnMoney();

	UFUNCTION(BlueprintPure)
		EGeneration GetAgeGroup();
	
	UFUNCTION(BlueprintPure)
		EPoliteness GetPoliteness();

	UFUNCTION(BlueprintPure)
		int32 GetAge();

	UFUNCTION(BlueprintPure)
		FString GetName();

	UFUNCTION(BlueprintPure)
		float GetNutrition();

	UFUNCTION(BlueprintPure)
		float GetExcrement();

	UFUNCTION(BlueprintPure)
		float GetSocietal();

	void SetSocialBias(float x);

	UFUNCTION(BlueprintPure)
		float GetEnergy();

	UFUNCTION(BlueprintPure)
		float GetAnimPlaySpeed();

	void SetAnimPlaySpeed(float WalkSpeed);

	// Status Functions	///
	void ResetStates();

	void SetOnboard(bool x);
	bool GetOnboard();

	void SetSitting(bool x);
	bool GetSitting();

	void SetFloating(bool x);
	bool GetFloating();

	void SetInToilet(bool x);
	bool GetInToilet();

	void SetMoving(bool x);

	UFUNCTION(BlueprintPure)
	bool GetMoving();

	UFUNCTION(BlueprintPure)
	FText GetMoneyAsText();
	float GetMoney();
	void SetMoney(float x);

	void ResetDeltaSum();
	int GetDeltaSum();

	void SetAwaitingPickUp(bool x);
	bool GetAwaitingPickup();


	bool IsAlive();
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AMoney> MoneyActor;

private:

	int32 Age{ 0 };
	EGeneration AgeGroup;
	EPoliteness Politeness;
	EWealth Wealthiness;
	float Money{ 0.0f };

	bool Onboard{ false };
	bool Sitting;
	bool Floating;
	bool Moving;
	bool InToilet;
	
	//Core Values
	float Nutrition{ 100.00f };
	float Energy{ 75.00f };
	float Excrement{ 0.00f };
	float Societal{ 50.00f };
	float SocialBias{ 0.0f };

	int Delta{ 0 };
	int WealthAdjustment{ 0 };
	int DeltaSum{ 0 };
	int MoneyDropLimit{ 0 };
	float DrainDatum{ 0.0f };

	
	
	
	bool AwaitingPickup{ false };
	bool Alive{ true };
	bool AnnouncedDeath{ false };
	float AnimationPlaySpeed{ };

	FString Name = "Nobody";
	TArray<FString> Names { "Ben Dover", "Wang Liqin", "Stef Crabs", "Dixie Normous", "Jack Goff", "Justin Sider", "Thomas Fister", "Wendy Wacko", "Brownie Shytles", "Richard Pound", "Peter Bunerz", "Heath Cuckburn", "Thea Beaver", //13
		"Mike Wiener", "Woody Held", "Mike Litoris", "Chew Weedman", "Jack Kanoff", "William Stroker", "Peter Fletcher", "Liam Walker", "Ashton Davidson", "Bailey Hunter", "Alfie Ball", "Kyson Davidson", "Myles Mathis", //13
		"Brayan Gentry", "Marcelo Barry", "Devon Rojas", "Jamie Mills", "Blair Williamson", "Kris Hart", "Jordan Doyle", "Aiden West", "Jordan Adkins", "Willy Mosley", "Drew Reynolds", "Avery Hodges", "Nicky Cline", //13
		"Glen Byrne", "Ali Harvey", "Phoneix Fletcher", "Reed Shaw", "Lane Russel", "Jesse Day", "Alex Wilkerson", "Glen Shafter", "Marley Fitzpatrick", "Jody Mercado", "Brook Palmer", "Danni Price", "Jess Burke", "Kit Pearce", //14
		"Chris Clark", "Jaden Meyers", "Gene Hopper", "Reed Madden", "Bailey Douglas", "Charlie Mayo" }; //6
};
