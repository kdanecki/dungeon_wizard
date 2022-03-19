// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

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
	/*
	UPROPERTY(BlueprintReadWrite)
		float XSize;
	UPROPERTY(BlueprintReadWrite)
		float YSize;
	UPROPERTY(BlueprintReadWrite)
		float ZSize;
		*/
	/*UFUNCTION(BlueprintCallable)
		void CreateRoom(FRoomSpawnInfo RoomSpawnInfo);
	/*UPROPERTY(EditAnywhere)
		//UStaticMesh* WallMesh;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* WallMaterial;
	UPROPERTY(EditAnywhere)
		UStaticMesh* FloorMesh;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* FloorMaterial;*/
	//UFUNCTION()
		//void CreateRoom();
	UPROPERTY(EditAnywhere)
		TArray<FDoorInfo> Doors;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;
	//UPROPERTY(EditAnywhere)
		//UStaticMesh* RoomMesh;
	//UPROPERTY(EditAnywhere)
		//UMaterialInterface* RoomMaterial;

};
