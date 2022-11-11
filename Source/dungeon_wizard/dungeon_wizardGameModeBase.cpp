// Copyright Epic Games, Inc. All Rights Reserved.

//#pragma optimize("", off)
#include "dungeon_wizardGameModeBase.h"
#include "DungeonSave.h"
#include "dungeon_wizardGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"
#include "PlayerControllerBase.h"

void Adungeon_wizardGameModeBase::InitGame(const FString& MapName, const FString& Options, FString&ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}



void Adungeon_wizardGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParams;
	GameName = TEXT("Game");
	Generator = GetWorld()->SpawnActor<ADungeonGenerator>(GeneratorType, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	Generator->GameName = GameName;
	if (UDungeonSave* LoadedGame = Cast<UDungeonSave>(UGameplayStatics::LoadGameFromSlot(GameName, 0)))
	{
		Generator->SpawnedRooms.SetNum(LoadedGame->RoomCount);
		Generator->RoomCount = LoadedGame->RoomCount;
		for (int i = 0; i < LoadedGame->RoomCount; i++)
		{
			if (URoomSave* LoadedRoom = Cast<URoomSave>(UGameplayStatics::LoadGameFromSlot(GameName + FString::FromInt(i), 0)))
			{
				Generator->SpawnedRooms[i] = SpawnRoomFromSave(LoadedRoom);
				Generator->SpawnedRooms[i]->NeighborsIndex = LoadedRoom->NextRooms;
				Generator->SpawnedRooms[i]->Index = i;
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, FString::Printf(TEXT("save doen't exist")));
				}
			}
		}
		if (UPassagesSave* PassagesSave = Cast<UPassagesSave>(UGameplayStatics::LoadGameFromSlot(GameName + FString("Passages"), 0)))
		{
			Generator->PassagesSave = PassagesSave;
			for (FPassageSave Info : PassagesSave->PassagesInfo)
			{
				APassage* Passage = GetWorld()->SpawnActor<APassage>(Info.PassageType, Info.Transform, SpawnParams);
				Passage->Mesh->SetMaterial(0, Info.Material);
				Passage->MeshParams = Info.MeshParams;
				Passage->OnRep_MeshParams();
			}
		}
	}
	else
	{
		Generator->Generate();
		Generator->RoomCount = 1;
	}
	FTimerHandle UnusedHandle;
	FTimerDelegate MyDelegate = FTimerDelegate::CreateUObject(this, &Adungeon_wizardGameModeBase::RoomLoader);
	GetWorldTimerManager().SetTimer(UnusedHandle, MyDelegate, 1, true, 0.5);
	FTimerHandle UnusedHandle2;
	FTimerDelegate MyDelegate2 = FTimerDelegate::CreateUObject(this, &Adungeon_wizardGameModeBase::UpdateRooms);
	GetWorldTimerManager().SetTimer(UnusedHandle2, MyDelegate2, 1, true);
}

void Adungeon_wizardGameModeBase::Tick(float DeltaTime)
{

}

void Adungeon_wizardGameModeBase::RoomLoader()
{
	if (NeedToBeLoaded.Num() > 0 && Generator->LoadingStatus == Idle)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("loaded %d"), NeedToBeLoaded[0]));
		}
		LoadRoom(NeedToBeLoaded[0], 0);
		NeedToBeLoaded.RemoveAt(0);
	}
	
}

void Adungeon_wizardGameModeBase::AddRooms(int Index, int Depth)
{
	if (!RoomsToLoad.Contains(Index))
	{
		RoomsToLoad.Add(Index);
		if (Depth > 0)
		{
			for (int i : Generator->SpawnedRooms[Index]->NeighborsIndex)
			{
				AddRooms(i, Depth - 1);
			}
		}
	}
}

ARoom* Adungeon_wizardGameModeBase::SpawnRoomFromSave(URoomSave* RoomSave)
{
	ARoom * Room = GetWorld()->SpawnActor<ARoom>(RoomSave->RoomType, RoomSave->Transform);
	Room->IsUnfinished = !RoomSave->IsFinished;
	return Room;
}

void Adungeon_wizardGameModeBase::UpdateRooms()
{
	Adungeon_wizardGameModeBase* GM = this;
	//AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [GM] ()
	//{
			TArray<APlayerState*> Array = GM->GameState->PlayerArray;
			GM->RoomsToLoad.Empty();
			for (APlayerState* PlayerState : Array)
			{
				if (APlayerControllerBase* Controller = Cast<APlayerControllerBase>(PlayerState->GetPlayerController()))
				{
					GM->AddRooms(Controller->CurrentRoom, GM->LoadDistance);
				}
			}
			for (int i = GM->LoadedRooms.Num() - 1; i >= 0; i--)
			{
				bool ShouldUnload = true;
				for (int j : GM->RoomsToLoad)
				{
					if (GM->LoadedRooms[i] == j)
					{
						ShouldUnload = false;
						break;
					}
				}
				if (ShouldUnload)
				{
					int index = GM->LoadedRooms[i];
					//AsyncTask(ENamedThreads::GameThread, [GM, index]()
					//	{
					//		GM->NeedToBeLoaded.Remove(index);
							GM->UnloadRoom(index);
						if(GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("update rooms"));
						}
					//	});
				}
			}
			for (int i : GM->RoomsToLoad)
			{
				bool IsLoaded = false;
				for (int j : GM->LoadedRooms)
				{
					if (i == j)
					{
						IsLoaded = true;
						break;
					}
				}
				if (!IsLoaded)
				{
					//AsyncTask(ENamedThreads::GameThread, [GM, i]()
					//	{
							GM->NeedToBeLoaded.Add(i);
					//	});
				}
			}
		
	//});
}

void Adungeon_wizardGameModeBase::LoadRoom(int Index, int Depth)
{
	if (!LoadedRooms.Contains(Index))
	{
		Generator->LoadingStatus = Loading;
		FAsyncLoadGameFromSlotDelegate LoadedDelegate;
		LoadedDelegate.BindUObject(this, &Adungeon_wizardGameModeBase::SpawnLoaded, Index);
		UGameplayStatics::AsyncLoadGameFromSlot(GameName + FString::FromInt(Index), 0, LoadedDelegate);
	}
}

void Adungeon_wizardGameModeBase::SpawnLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame, int Index)
{
	URoomSave* LoadedRoom = Cast<URoomSave>(LoadedGame);
	if (LoadedRoom->IsFinished)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::FromInt(LoadedRoom->ResourcesInfo.Num() + LoadedRoom->ItemsInfo.Num()));
		}
		for (FResourceSave Info : LoadedRoom->ResourcesInfo)
		{
			ANaturalResource* Resource = GetWorld()->SpawnActor<ANaturalResource>(Info.ResourceType, Info.Transform, SpawnParams);
			Resource->ResourcesLeft = Info.ResourcesLeft;
			Resource->UpdateMesh(FVector(0, 0, 0), Info.ResourcesLeft);
			Generator->SpawnedRooms[Index]->NaturalResources.Add(Resource);
		}
		for (FItemSave Info : LoadedRoom->ItemsInfo)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("item")));
			}
			if (Info.CanBePickedUp)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("spawn")));
				}
				AItem* Item = GetWorld()->SpawnActor<AItem>(Info.ItemType, Info.Transform, SpawnParams);
				Item->SetActorScale3D(FVector(1, 1, 1));
				Item->CanBePickedUp = Info.CanBePickedUp;
				Item->CurrentRoom = Generator->SpawnedRooms[Index];
				Generator->SpawnedRooms[Index]->Items.AddUnique(Item);
			}
		}
		Generator->SpawnedRooms[Index]->Mesh->SetMaterial(0, Generator->Biomes[LoadedRoom->BiomeIndex].RoomMaterial);
		Generator->LoadingStatus = Idle;
	}
	else
	{
		Generator->SpawnedRooms[Index]->Doors.RemoveAt(LoadedRoom->FinishedDoor);
		Generator->FinishRoom(Generator->SpawnedRooms[Index]);
		if (!UGameplayStatics::SaveGameToSlot(LoadedRoom, GameName + FString::FromInt(Index), 0))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("failed to save")));
			}
		}
	}
	LoadedRooms.Add(Index);
}

void Adungeon_wizardGameModeBase::UnloadRoom(int Index)
{
	SaveRoom(Index);
	ARoom* Room = Generator->SpawnedRooms[Index];
	for (ANaturalResource * Resource : Room->NaturalResources)
	{
		Resource->Destroy();
	}
	for (AItem * Item : Room->Items)
	{
		Item->Destroy();
	}
	Room->NaturalResources.Empty();
	Room->Items.Empty();
	LoadedRooms.Remove(Index);
}

void Adungeon_wizardGameModeBase::SaveRoom(int Index)
{
	ARoom* Room = Generator->SpawnedRooms[Index];
	if (URoomSave * Save = Cast<URoomSave>(UGameplayStatics::LoadGameFromSlot(GameName + FString::FromInt(Index), 0)))
	{
		Save->IsFinished = !Room->IsUnfinished;
		Save->ResourcesInfo.Empty();
		Save->ItemsInfo.Empty();
		for (ANaturalResource* Resource : Room->NaturalResources)
		{
			if (IsValid(Resource))
			{
				Save->ResourcesInfo.Add(FResourceSave(Resource));
			}
			
		}
		for (AItem * Item : Room->Items)
		{
			if (IsValid(Item))
			{
				Save->ItemsInfo.Add(FItemSave(Item));
			}
		}
		UGameplayStatics::SaveGameToSlot(Save, GameName + FString::FromInt(Index), 0);
	}
}

void Adungeon_wizardGameModeBase::SaveGame()
{
	for (int RoomIndex : LoadedRooms)
	{
		SaveRoom(RoomIndex);
	}
	if (UGameplayStatics::SaveGameToSlot(Generator->PassagesSave, GameName + FString("Passages"), 0))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::FromInt(Generator->PassagesSave->PassagesInfo.Num()));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("failed to save passages")));
		}
	}
	if (UDungeonSave* LoadedGame = Cast<UDungeonSave>(UGameplayStatics::LoadGameFromSlot(GameName, 0)))
	{
		LoadedGame->RoomCount = Generator->RoomCount;
		
		if (UGameplayStatics::SaveGameToSlot(LoadedGame, GameName, 0))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("saved")));
			}
		}
	}
}
