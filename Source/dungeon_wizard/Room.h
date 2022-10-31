// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ItemSpawner.h"
#include "NaturalResource.h"
#include "Item.h"

#include "Room.generated.h"

/*
USTRUCT(BlueprintType)
struct FDoorInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int WallNumber;
	UPROPERTY(BlueprintReadWrite)
	float Location;
	UPROPERTY(BlueprintReadWrite)
	FVector WorldLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector WorldDirection;
	UPROPERTY(BlueprintReadWrite)
	float XSize;
	UPROPERTY(BlueprintReadWrite)
	float YSize;
public:
	FDoorInfo()
	{
		WallNumber = 0;
		Location = 0;
		WorldLocation = FVector();
		WorldDirection = FVector();
		XSize = 0;
		YSize = 0;
	}
	FDoorInfo(int WallNumber, float Location, float XSize, float YSize, FVector RoomLocation)
	{
		this->WallNumber = WallNumber;
		this->Location = Location;
		FRotator Rot(0, 90 * (WallNumber - 1), 0);
		FVector Location = Rot.RotateVector(FVector(1, 0, 0)) * (float)(WallDistance / 0.02) + FVector(0, 0, Z / 0.02 - 5.0);
		WorldLocation = FVector();
		WorldDirection = FVector();
		this->XSize = XSize;
		this->YSize = YSize;
	}

	bool operator<(const FDoorInfo & Other ) const
	{
		return (Location < Other.Location);
	}
};
*//*
USTRUCT(BlueprintType)
struct FRoomSpawnInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		float XSize;
	UPROPERTY(BlueprintReadWrite)
		float YSize;
	UPROPERTY(BlueprintReadWrite)
		float ZSize;
	UPROPERTY(BlueprintReadWrite)
		TArray<FDoorInfo> DoorsInfo;
};*/

USTRUCT(BlueprintType)
struct FDoorInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Direction;
	UPROPERTY(EditAnywhere)
		FVector2D Size;
};

UCLASS()
class DUNGEON_WIZARD_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		FVector Dimensions;
	UPROPERTY(EditAnywhere)
		TArray<FDoorInfo> Doors;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int Index;
	UPROPERTY(VisibleAnywhere)
		TArray<int> NeighborsIndex;
	UPROPERTY(EditAnywhere)
		float Size;
	UPROPERTY(VisibleAnywhere)
		int BiomeIndex;
	UPROPERTY(EditAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* Collision;
	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
		int number;
	UPROPERTY(BlueprintReadWrite)
		TArray<UItemSpawner*> ItemsToSpawn;
	TArray<AItem*> Items;
	UPROPERTY(BlueprintReadOnly)
	TArray<ANaturalResource*> NaturalResources;

	//UPROPERTY(EditAnywhere)
		//UStaticMesh* RoomMesh;
	//UPROPERTY(EditAnywhere)
		//UMaterialInterface* RoomMaterial;

};
