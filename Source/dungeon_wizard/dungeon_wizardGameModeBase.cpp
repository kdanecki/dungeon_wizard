// Copyright Epic Games, Inc. All Rights Reserved.

#pragma optimize("", off)
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
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
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
				Passage->StaticMesh->SetMaterial(0, Info.Material);
				//Passage->MeshParams = Info.MeshParams;
				//Passage->OnRep_MeshParams();
			}
			for (FBlockadeSave Info : PassagesSave->BlockadesInfo)
			{
				ABlockade* Blockade = GetWorld()->SpawnActor<ABlockade>(Info.BlockadeType, Info.Transform, SpawnParams);
				Blockade->Mesh->SetMaterial(0, Info.Material);
			}
		}
		for (int i = 0; i < LoadDistance; ++i)
		{
			UpdateRooms();
		}
	}
	else
	{
		Generator->Generate(LoadedRooms);
		for (int i = 0; i < LoadDistance; ++i)
		{
			UpdateRooms();
		}
	}
	FTimerHandle UnusedHandle;
	FTimerDelegate MyDelegate = FTimerDelegate::CreateUObject(this, &Adungeon_wizardGameModeBase::RoomLoader);
	GetWorldTimerManager().SetTimer(UnusedHandle, MyDelegate, RoomLoadingDelay, true);
	//FTimerHandle UnusedHandle2;
	//FTimerDelegate MyDelegate2 = FTimerDelegate::CreateUObject(this, &Adungeon_wizardGameModeBase::UpdateRooms);
	//GetWorldTimerManager().SetTimer(UnusedHandle2, MyDelegate2, 1, true);
}

void Adungeon_wizardGameModeBase::Tick(float DeltaTime)
{
	/*if (NeedToBeLoaded.Num() > 0 && Generator->LoadingStatus == ELoadingStatus::Idle)
	{
		LoadRoom(NeedToBeLoaded[0], 0);
	}*/
}

void Adungeon_wizardGameModeBase::RoomLoader()
{
	/*if (NeedToBeLoaded.Num() > 0 && Generator->LoadingStatus == Idle)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("loaded %d"), NeedToBeLoaded[0]));
		}
		LoadRoom(NeedToBeLoaded[0], 0);
		NeedToBeLoaded.RemoveAt(0);
	}*/
	if (NeedToBeLoaded.Num() > 0)
	{
    	LoadRoom(NeedToBeLoaded[0], 0);
    	NeedToBeLoaded.RemoveAt(0);
		UpdateRooms();
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
	Room->IsFinished = RoomSave->IsFinished;
	Room->FinishedDoor = RoomSave->FinishedDoor;
	Room->NeighborsIndex = RoomSave->NextRooms;
	Room->BiomeIndex = RoomSave->BiomeIndex;
	return Room;
}

void Adungeon_wizardGameModeBase::UpdateRooms()
{
	TArray<APlayerState*> Array = GameState->PlayerArray;
	RoomsToLoad.Empty();
	for (APlayerState* PlayerState : Array)
	{
		if (APlayerControllerBase* Controller = Cast<APlayerControllerBase>(PlayerState->GetPlayerController()))
		{
			AddRooms(Controller->CurrentRoom, LoadDistance);
		}
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString::FromInt(LoadedRooms.Num()));
	}
	for (int i = LoadedRooms.Num() - 1; i >= 0; i--)
	{
		bool ShouldUnload = true;
		for (int j : RoomsToLoad)
		{
			if (LoadedRooms[i] == j)
			{
				ShouldUnload = false;
				break;
			}
		}
		if (ShouldUnload)
		{
			int index = LoadedRooms[i];
			//AsyncTask(ENamedThreads::GameThread, [GM, index]()
			//	{
			//		NeedToBeLoaded.Remove(index);
					UnloadRoom(index);
				if(GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("update rooms"));
				}
			//	});
			
		}
	}
	for (int i : RoomsToLoad)
	{
		bool IsLoaded = false;
		for (int j : LoadedRooms)
		{
			if (i == j)
			{
				IsLoaded = true;
				break;
			}
		}
		if (!IsLoaded)
		{
			NeedToBeLoaded.AddUnique(i);
			//LoadRoom(i, 0);
			//AsyncTask(ENamedThreads::GameThread, [GM, i]()
			//	{
					
			//	});
		}
	}
		
	//});
}

void Adungeon_wizardGameModeBase::LoadRoom(int Index, int Depth)
{
	if (Generator->SpawnedRooms[Index]->IsFinished)
	{
		FAsyncLoadGameFromSlotDelegate LoadedDelegate;
		LoadedDelegate.BindUObject(this, &Adungeon_wizardGameModeBase::SpawnLoaded, Index);
		UGameplayStatics::AsyncLoadGameFromSlot(GameName + FString::FromInt(Index), 0, LoadedDelegate);
		LoadedRooms.Add(Index);
	}
	else
	{
		//Generator->LoadingStatus == ELoadingStatus::Loading;
		Generator->FinishRoom(Generator->SpawnedRooms[Index]);
		LoadedRooms.Add(Index);
	}
	
}

void Adungeon_wizardGameModeBase::SpawnLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame, int Index)
{
	URoomSave* LoadedRoom = Cast<URoomSave>(LoadedGame);
	Generator->SpawnedRooms[Index]->Mesh->SetMaterial(0, Generator->Biomes[LoadedRoom->BiomeIndex].RoomMaterial);
	Generator->SpawnedRooms[Index]->LoadResources(LoadedRoom->ResourcesInfo);
	//LoadedRooms.Add(Index);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::FromInt(LoadedRoom->ResourcesInfo.Num() + LoadedRoom->ItemsInfo.Num()));
		}*/
		/*for (FResourceSave Info : LoadedRoom->ResourcesInfo)
		{
			ANaturalResource* Resource = GetWorld()->SpawnActor<ANaturalResource>(Info.ResourceType, Info.Transform, SpawnParams);
			Resource->ResourcesLeft = Info.ResourcesLeft;
			Resource->UpdateMesh(FVector(0, 0, 0), Info.ResourcesLeft);
			Generator->SpawnedRooms[Index]->NaturalResources.Add(Resource);
		}*/
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
		//Generator->SpawnedRooms[Index]->Mesh->SetMaterial(0, Generator->Biomes[LoadedRoom->BiomeIndex].RoomMaterial);
	
}

void Adungeon_wizardGameModeBase::UnloadRoom(int Index)
{
	SaveRoom(Index);
	ARoom* Room = Generator->SpawnedRooms[Index];
	Room->Mesh->SetMaterial(0, Generator->RedMaterial);
	for (ANaturalResource * Resource : Room->NaturalResources)
	{
		if (IsValid(Resource))
		{
			Resource->Destroy();
		}
	}
	for (AItem * Item : Room->Items)
	{
		if (IsValid(Item))
		{
			Item->Destroy();
		}
	}
	Room->NaturalResources.Empty();
	Room->Items.Empty();
	LoadedRooms.Remove(Index);
	if (GEngine)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, FString("unload ") + FString::FromInt(Index));
	}
}

void Adungeon_wizardGameModeBase::SaveRoom(int Index)
{
	ARoom* Room = Generator->SpawnedRooms[Index];
	if (URoomSave * Save = Cast<URoomSave>(UGameplayStatics::CreateSaveGameObject(URoomSave::StaticClass())))
	{
		Save->RoomType = Room->GetClass();
		Save->Transform = Room->GetTransform();
		Save->NextRooms = Room->NeighborsIndex;
		Save->BiomeIndex = Room->BiomeIndex;
		Save->IsFinished = Room->IsFinished;
		Save->FinishedDoor = Room->FinishedDoor;
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
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, Item->GetActorLabel());
				}
				Save->ItemsInfo.Add(FItemSave(Item));
			}
		}
		UGameplayStatics::SaveGameToSlot(Save, GameName + FString::FromInt(Index), 0);
	}
}

void Adungeon_wizardGameModeBase::SaveAllRooms()
{
	for (int i = 0; i < Generator->SpawnedRooms.Num(); ++i)
	{
		if (LoadedRooms.Contains(i) || !Generator->SpawnedRooms[i]->IsFinished)
		{
			SaveRoom(i);
		}
	}
}

void Adungeon_wizardGameModeBase::SavePassages()
{
	UPassagesSave* PassagesSave = Cast<UPassagesSave>(UGameplayStatics::CreateSaveGameObject(UPassagesSave::StaticClass()));
	TArray<AActor*> Passages;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APassage::StaticClass(), Passages);
	for (AActor* Actor : Passages)
	{
		PassagesSave->PassagesInfo.Add(FPassageSave(Cast<APassage>(Actor)));
	}
	TArray<AActor*> Blockades;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockade::StaticClass(), Blockades);
	for (AActor* Actor : Blockades)
	{
		PassagesSave->BlockadesInfo.Add(FBlockadeSave(Cast<ABlockade>(Actor)));
	}
	UGameplayStatics::SaveGameToSlot(PassagesSave, GameName + FString("Passages"), 0);
}

void Adungeon_wizardGameModeBase::SaveGame()
{
	/*for (int RoomIndex : LoadedRooms)
	{
		SaveRoom(RoomIndex);
	}*/
	SaveAllRooms();
	SavePassages();

	UDungeonSave* GameSave = Cast<UDungeonSave>(UGameplayStatics::CreateSaveGameObject(UDungeonSave::StaticClass()));
	GameSave->RoomCount = Generator->RoomCount;
	GameSave->CurrentRoom = Cast<APlayerControllerBase>(GameState->PlayerArray[0]->GetPlayerController())->CurrentRoom;
	if (UGameplayStatics::SaveGameToSlot(GameSave, GameName, 0))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("saved")));
		}
	}
}
