// Copyright Epic Games, Inc. All Rights Reserved.

//#pragma optimize("", off)
#pragma once
#include "dungeon_wizardGameModeBase.h"
#include "alchemik/mixtures/resources.h"
#include "alchemik/mixtures/mixture.h"
#include "DungeonSave.h"

#include "Kismet/GameplayStatics.h"
//#include "alchemik/player.h"
//#include "Item.h"

/*
class Uclass
{
public:
	static int class_cnt;
	Uclass();
	int class_id;
};*/

//int Uclass::class_cnt;
int game_started = 0;


void Adungeon_wizardGameModeBase::InitGame(const FString& MapName, const FString& Options, FString&ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	//Uclass::class_cnt = 0;
	game_started = 1;
	/*int count = 0;
	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(AItem::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
		{
			count++;
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, It->GetName());
			}
		}
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), count));
	}*/
}



void Adungeon_wizardGameModeBase::BeginPlay()
{
	Super::BeginPlay();
//	create_elements();
	elements = new Elements();
	mixtures = new Mixtures();

	create_mixtures();
	create_skills();

	resources = new Resources();
	for (int i = 0; i < MyResources.Num(); i++)
	{
		AItem * Item = Cast<AItem>(MyResources[i]->GetDefaultObject());
		Resource *r = new Resource((Element_type)(Item->ResourceId), -1);
		r->ue = MyResources[i];
		resources->add(r);
	}
	FActorSpawnParameters SpawnParams;
	//FString TestString = FString::Printf(TEXT("Game"));
	GameName = TEXT("Game");
	Generator = GetWorld()->SpawnActor<ADungeonGenerator>(GeneratorType, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	Generator->GameName = GameName;
	if (UDungeonSave* LoadedGame = Cast<UDungeonSave>(UGameplayStatics::LoadGameFromSlot(GameName, 0)))
	{
		Generator->RoomCount = LoadedGame->RoomCount;
		LoadRoom(LoadedGame->CurrentRoom, 3);
		/*if (URoomSave* LoadedRoom = Cast<URoomSave>(UGameplayStatics::LoadGameFromSlot(GameName + FString::FromInt(LoadedGame->CurrentRoom), 0)))
		{
			SpawnRoomFromSave(LoadedRoom, 3);
		}*/
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		//UE_LOG(LogTemp, Warning, TEXT("LOADED: %s"), *LoadedGame->PlayerName);
	}
	else
	{
		Generator->Generate();
		Generator->RoomCount = 1;
		LoadRoom(0, 3);
	}
	int a = 10;
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), a));
	}
}

ARoom* Adungeon_wizardGameModeBase::SpawnRoomFromSave(URoomSave* RoomSave)
{
	ARoom * Room = GetWorld()->SpawnActor<ARoom>(RoomSave->RoomType, RoomSave->Transform);
	if (RoomSave->IsFinished)
	{
		for (FResourceSave Info : RoomSave->ResourcesInfo)
		{
			ANaturalResource* Resource = GetWorld()->SpawnActor<ANaturalResource>(Info.ResourceType, Info.Transform);
			Resource->ResourcesLeft = Info.ResourcesLeft;
			Resource->UpdateMesh(FVector(0, 0, 0), Info.ResourcesLeft);
		}
		for (FItemSave Info : RoomSave->ItemsInfo)
		{
			AItem* Item = GetWorld()->SpawnActor<AItem>(Info.ItemType, Info.Transform);

		}
		Room->Mesh->SetMaterial(0, Generator->Biomes[RoomSave->BiomeIndex].RoomMaterial);
	}
	else
	{
		Generator->FinishRoom(Room, RoomSave);
	}
	return Room;
}

void Adungeon_wizardGameModeBase::LoadRoom(int Index, int Depth)
{
	if (!LoadedRooms.Contains(Index))
	{
		if (URoomSave* LoadedRoom = Cast<URoomSave>(UGameplayStatics::LoadGameFromSlot(GameName + FString::FromInt(Index), 0)))
		{
			ARoom* Room = SpawnRoomFromSave(LoadedRoom);
			LoadedRooms.Add(Index);
			if (Depth > 0)
			{
				for (int i : Room->NeighborsIndex)
				{
					LoadRoom(i, Depth - 1);
				}
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, FString::Printf(TEXT("ERROR: room save doesn't exist")));
			}
		}

	}
}
