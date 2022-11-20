// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Item.h"
#include "DungeonGenerator.h"
#include "RoomSave.h"

#include "dungeon_wizardGameModeBase.generated.h"

/**
 * 
 */


UCLASS()
class DUNGEON_WIZARD_API Adungeon_wizardGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString&ErrorMessage) override;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AItem>> MyResources;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ADungeonGenerator> GeneratorType;
	ADungeonGenerator* Generator;
	UPROPERTY(EditAnywhere)
		int LoadDistance;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
	
	void RoomLoader();
	UPROPERTY(VisibleAnywhere)
	TArray<int> LoadedRooms;
	TArray<int> RoomsToLoad;
	TArray<int> NeedToBeLoaded;
	void AddRooms(int Index, int Depth);
//	TArray<ARoom*> SpawnedRooms;
	ARoom* SpawnRoomFromSave(URoomSave* RoomSave);
	UFUNCTION(BlueprintCallable)
	void UpdateRooms();
	void LoadRoom(int Index, int Depth);
	void SpawnLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame, int Index);
	void UnloadRoom(int Index);
	void SaveRoom(int Index);
	void SaveAllRooms();
	void SavePassages();
	UFUNCTION(BlueprintCallable)
		void SaveGame();
	UPROPERTY()
		FString GameName;
};
