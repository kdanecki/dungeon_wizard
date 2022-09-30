// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.h"
#include "Passage.h"
#include "Item.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NaturalResource.h"
#include "Faction.h"
#include "HumanBase.h"
#include "AICharacterController.h"

#include "DungeonGenerator.generated.h"

USTRUCT(BlueprintType)
struct FResourceInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<ANaturalResource> ResourceType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector ConeDirection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ConeAngle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Rarity;
};

USTRUCT(BlueprintType)
struct FBiome
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FString Name;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* RoomMaterial;
	UPROPERTY(EditAnywhere)
		TArray<FResourceInfo> PossibleResources;

};



UCLASS()
class DUNGEON_WIZARD_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		TArray<FBiome> Biomes;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AFaction> FactionType;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AHumanBase>> Races;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<ARoom>> RoomTypes;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<ARoom>> CrossingTypes;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<APassage>> PassageTypes;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AActor>> BlockadeTypes;
	UPROPERTY(EditAnywhere)
		int RoomsLeft;
	UPROPERTY(EditAnywhere)
		int StartingRoom;
	UPROPERTY(EditAnywhere)
		int StartingBiome;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* RedMaterial;
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AItem>> ItemsToSpawn;


	UFUNCTION(BlueprintImplementableEvent)
		float RandomFloat(float Min, float Max);
	UFUNCTION(BlueprintImplementableEvent)
		int RandomInt(int Min, int Max);
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnResource(FResourceInfo ResourceInfo, FVector Center, float Length, ARoom* Room);
	UFUNCTION(BlueprintImplementableEvent)
		AAICharacterController* SpawnCharacter(TSubclassOf<AHumanBase> Race, FVector Location);
	void Generate();
	void FinishRoom(ARoom* Starting);
	void EndRoom(ARoom* Starting);
	void SpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent, FVector2D StartSize, FVector2D EndSize, TArray<AActor*> IgnoreActor, UMaterialInterface* Material);
	APassage* ForceSpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent, FVector2D StartSize, FVector2D EndSize, UMaterialInterface* Material);
	TArray<ARoom*> UnfinishedRooms;
	//TArray<ARoom*> SpawnedRooms;


	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> TraceVisibility;
	
};
