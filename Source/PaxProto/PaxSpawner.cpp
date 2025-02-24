// Copyright of Codebird Ltd 2020 - inclusive © 
#include "PaxSpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pax.h"

APaxSpawner::APaxSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//Root Guard
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	}

	//Dimensions of Spawn Area Box
	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Area"));
	SpawnArea->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 150.0f));
	SpawnArea->SetCanEverAffectNavigation(false);

}

void APaxSpawner::BeginPlay()
{
	Super::BeginPlay();

	//Get center of spawn point
	SpawnPoint = this->GetActorLocation();
	//Set turn direction
	Q = true;
}

void APaxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Create a spawn queue in a snail formation, equally spaced
void APaxSpawner::GetSpawnPoint(const int32 NumOfPaxToSpawn, FVector& Location)
{
	//Get dimension to work area
	float BoxEdgeX = SpawnArea->GetScaledBoxExtent().X;
	float BoxEdgeY = SpawnArea->GetScaledBoxExtent().Y;
	float BoxFloor = this->GetActorLocation().Z - (SpawnArea->GetScaledBoxExtent().Z / 2);
	//Set SpawnPoint to the floor of the box to ensure no falling or noises from landing happen
	SpawnPoint.Z = BoxFloor;
	//Split each side of the square into 4 lengths
	int32 GridSideSquareLength = (NumOfPaxToSpawn / 4) + 1;
	//Get a vector to represent a 'notch' movement around the grid, like a tab in coding
	FVector CursorMoveX = FVector((BoxEdgeX / GridSideSquareLength), 0.0f, 0.0f);
	FVector CursorMoveY = FVector(0.0f, (BoxEdgeY / GridSideSquareLength), 0.0f);

	if (Q) //up and right
	{
		//try up, check in bounds
		SpawnPoint += CursorMoveY;
		if (CheckWithinBounds(SpawnPoint))
		{
			Location = SpawnPoint;
		}
		else
		{
			//if not in bounds, reverse action, try right, check in bounds
			SpawnPoint -= CursorMoveY;
			SpawnPoint += CursorMoveX;
			if (CheckWithinBounds(SpawnPoint))
			{
				Location = SpawnPoint;
			}
			else
			{
				//if not in bounds, reverse action, try new orientation (down and left, eg q is false)
				SpawnPoint -= CursorMoveX;
				Q = false;
				GetSpawnPoint(NumOfPaxToSpawn, Location); //recall method to filter to Q false path
			}
		}
	}
	else //down and left
	{
		SpawnPoint -= CursorMoveY;
		if (CheckWithinBounds(SpawnPoint))
		{
			Location = SpawnPoint;
		}
		else
		{
			SpawnPoint += CursorMoveY;
			SpawnPoint -= CursorMoveX;
			if (CheckWithinBounds(SpawnPoint))
			{
				Location = SpawnPoint;
			}
			else
			{
				SpawnPoint += CursorMoveX;
				Q = true;
				GetSpawnPoint(NumOfPaxToSpawn, Location);
			}
		}
	}
}

//Check spawnpoint is within spawn area bounds
bool APaxSpawner::CheckWithinBounds(const FVector& Point)
{
	float UpperLimitX = this->GetActorLocation().X + (SpawnArea->GetScaledBoxExtent().X);
	float LowerLimitX = this->GetActorLocation().X - (SpawnArea->GetScaledBoxExtent().X);
	float UpperLimitY = this->GetActorLocation().Y + (SpawnArea->GetScaledBoxExtent().Y);
	float LowerLimitY = this->GetActorLocation().Y - (SpawnArea->GetScaledBoxExtent().Y);

	if ((Point.X < UpperLimitX) && (Point.X > LowerLimitX) && (Point.Y < UpperLimitY) && (Point.Y > LowerLimitY))
		return true;
	else
		return false;
}
