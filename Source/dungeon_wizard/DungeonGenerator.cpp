// Fill out your copyright notice in the Description page of Project Settings.
//#pragma optimize("", off)

#include "DungeonGenerator.h"
//#include <iostream>
//#include <fstream>
//#include <direct.h>

//#include "Kismet/KismetMathLibrary.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "DungeonSave.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TraceVisibility = EDrawDebugTrace::Persistent;
	GameName = TEXT("");
}

//EDrawDebugTrace::Type TraceVisibility = EDrawDebugTrace::Persistent;

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	//Generate();
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	/*if (UnfinishedRooms.Num() > 0 && RoomsLeft > 0)
	{
		FinishRoom(UnfinishedRooms[0]);
		RoomsLeft--;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), RoomsLeft));
		}
	}
	else if (UnfinishedRooms.Num() > 0)
	{
		EndRoom(UnfinishedRooms[0]);
	}*/
}

void ADungeonGenerator::Generate()
{
	FActorSpawnParameters RoomSpawnParams;
	ARoom* Starting = GetWorld()->SpawnActor<ARoom>(RoomTypes[StartingRoom], GetActorLocation(), GetActorRotation(), RoomSpawnParams);
	Starting->Doors.Insert(FDoorInfo(), 0);
	Starting->BiomeIndex = StartingBiome;
	SpawnedRooms.Add(Starting);

	if (URoomSave* Save = Cast<URoomSave>(UGameplayStatics::CreateSaveGameObject(URoomSave::StaticClass())))
	{
		Save->RoomType = RoomTypes[StartingRoom];
		Save->Transform = FTransform(GetActorRotation(), GetActorLocation());
		Save->BiomeIndex = StartingBiome;

		if (UGameplayStatics::SaveGameToSlot(Save, GameName + FString::FromInt(0), 0))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Generated Successfully")));
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Generation Failed")));
			}
		}
	}
	if (UDungeonSave* DungeonSave = Cast<UDungeonSave>(UGameplayStatics::CreateSaveGameObject(UDungeonSave::StaticClass())))
	{
		DungeonSave->RoomCount = 1;
		DungeonSave->CurrentRoom = 0;
		if (UGameplayStatics::SaveGameToSlot(DungeonSave, GameName, 0))
		{
		}
	}
	PassagesSave = Cast<UPassagesSave>(UGameplayStatics::CreateSaveGameObject(UPassagesSave::StaticClass()));
	if (IsValid(PassagesSave))
	{
		UGameplayStatics::SaveGameToSlot(PassagesSave, GameName + FString("Passages"), 0);
	}
	if (IsValid(PassagesSave))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("valid")));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("invalid")));
		}
	}
	
	FActorSpawnParameters SpawnParams;
	for (int i = 0; i < 1; i++)
	{
		AItem * Item = GetWorld()->SpawnActor<AItem>(ItemsToSpawn[4], FVector(300, 0, 200 + 100 * 1), GetActorRotation(), SpawnParams);
		Item->SetDefaults(10);
	}
}

void ADungeonGenerator::FinishRoom(ARoom* Starting1/*, URoomSave* Save1*/)
{
	FActorSpawnParameters PassSpawnParams;
	TWeakObjectPtr<ADungeonGenerator> Generator = this;
	TWeakObjectPtr<ARoom> Starting = Starting1;
	LoadingTasks = Starting->Doors.Num();
	AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [Starting, Generator] ()
	{
		FActorSpawnParameters RoomSpawnParams;
		for (auto& Exit : Starting->Doors)
		{
			FVector Location = Starting->GetActorLocation();
			FRotator Rotation = Starting->GetActorRotation();

			int SelectedType = Generator->RandomInt(0, Generator->RoomTypes.Num() - 1);
			FVector Dimensions = Generator->RoomTypes[SelectedType].GetDefaultObject()->Dimensions;
			int SelectedDoor = Generator->RandomInt(0, Generator->RoomTypes[SelectedType].GetDefaultObject()->Doors.Num() - 1);
			FDoorInfo NextExit = Generator->RoomTypes[SelectedType].GetDefaultObject()->Doors[SelectedDoor];

			float PassageLength = 400 + Generator->RandomFloat(0, 2000); //200 + (0, 2000)

			FVector sel = Generator->RoomTypes[SelectedType].GetDefaultObject()->Doors[SelectedDoor].Direction;
			FVector V = Rotation.RotateVector(Exit.Direction);
			float cos = FVector::DotProduct(NextExit.Direction, -V);
			float rad = FMath::Acos(cos);
			FVector axis = FVector::CrossProduct(NextExit.Direction, -V);
			axis.Normalize();
			FVector V1 = FVector(1, 0, 0);
			FVector V2 = V1.RotateAngleAxis(FMath::RadiansToDegrees(rad), axis);
		
			FRotator NextRotation = V2.ToOrientationRotator() + FRotator(0, Generator->RandomFloat(-90 * (PassageLength / ((Dimensions.Length() + Starting->Dimensions.Length()) / 2.0)), 90 * (PassageLength / ((Dimensions.Length() + Starting->Dimensions.Length()) / 2.0))), 0);
			
			float Weight = Generator->RandomFloat(0.4, 0.6);
			float OffsetLength = Generator->RandomFloat(-PassageLength / 2.0, PassageLength / 2.0);
			FVector Offset = Rotation.RotateVector(FRotator(0, 90, 0).RotateVector(Exit.Direction) * OffsetLength * Weight) - NextRotation.RotateVector(FRotator(0, 90, 0).RotateVector(NextExit.Direction) * OffsetLength * (1 - Weight)) + FVector(0, 0, Generator->RandomFloat(-PassageLength / 3.0, PassageLength / 5.0));
			FVector NextLocation = Location + Rotation.RotateVector(Exit.Location + Exit.Direction * PassageLength * Weight) - NextRotation.RotateVector(NextExit.Location + NextExit.Direction * PassageLength * (1 - Weight)) + Offset;
			
			bool CanSpawn = true;
			TArray<FHitResult> SweepResults1;
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(Starting.Get());
			bool Hit = UKismetSystemLibrary::BoxTraceMulti(Generator->GetWorld(), NextLocation - FVector(10, 10, (10 - Dimensions.Z / 2.0)), NextLocation + FVector(10, 10, 10 + Dimensions.Z / 2.0), FVector(Dimensions.X * 0.5, Dimensions.Y * 0.5, Dimensions.Z * 0.5), NextRotation, UEngineTypes::ConvertToTraceType(ECC_Camera), true, ActorsToIgnore, Generator->TraceVisibility, SweepResults1, true, FLinearColor::Gray, FLinearColor::Yellow);
			if (SweepResults1.Num() > 0)
			{
				CanSpawn = false;
			}

			TArray<FHitResult> SweepResults;
			FRotator TraceRotation = (NextLocation - Location).ToOrientationRotator();
			Hit = UKismetSystemLibrary::BoxTraceMulti(Generator->GetWorld(), Location + FVector(0, 0, 200), NextLocation + FVector(0, 0, 200), FVector(300, 300, 400), TraceRotation, UEngineTypes::ConvertToTraceType(ECC_Camera), true, ActorsToIgnore, Generator->TraceVisibility, SweepResults, true, FLinearColor::Blue, FLinearColor::Red);
			if (CanSpawn)
			{
				for (int i = 0; i < SweepResults.Num(); i++)
				{
					if (SweepResults[i].GetActor())
					{
						if (SweepResults[i].GetActor()->IsA(ARoom::StaticClass()))
						{
							CanSpawn = false;
							if (GEngine)
							{
								//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, SweepResults[i].GetActor()->GetActorLabel());
							}
							break;
						}
						else
						{
							if (GEngine)
							{
								//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("crossing")));
							}
						}
					}
				}
			}

			if (CanSpawn)
			{
				AsyncTask(ENamedThreads::GameThread, [Generator, SelectedType, NextLocation, NextRotation, Location, Rotation, RoomSpawnParams, SelectedDoor, Exit, Starting]()
				{
					ARoom * Room = Generator->GetWorld()->SpawnActor<ARoom>(Generator->RoomTypes[SelectedType], NextLocation, NextRotation, RoomSpawnParams);
					Generator->SpawnedRooms.Add(Room);
					Room->Index = Generator->RoomCount;
					if (Generator->SpawnedRooms.Num() != Generator->RoomCount + 1)
					{
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, FString::Printf(TEXT("Index not matching room count")));
						}
					}
					Room->NeighborsIndex.Add(Starting->Index);
					Starting->NeighborsIndex.Add(Room->Index);
					if (Generator->RandomFloat(0, 1) > 0.5)
					{
						Room->BiomeIndex = Starting->BiomeIndex;
					}
					else
					{
						Room->BiomeIndex = Generator->RandomInt(0, Generator->Biomes.Num() - 1);
					}
					UMaterialInterface* Material = Generator->Biomes[Starting->BiomeIndex].RoomMaterial;
					/*if (URoomSave* NewSave = Cast<URoomSave>(UGameplayStatics::CreateSaveGameObject(URoomSave::StaticClass())))
					{
						NewSave->RoomType = Generator->RoomTypes[SelectedType];
						NewSave->Transform = FTransform(NextRotation, NextLocation);
						NewSave->BiomeIndex = Room->BiomeIndex;

						if (Generator->RandomFloat(0, 1) >= 0.5)
						{
							Material = Generator->Biomes[NewSave->BiomeIndex].RoomMaterial;
						}
						NewSave->NextRooms.Add(Starting->Index);
		//				Save->NextRooms.Add(Generator->RoomCount);

						NewSave->FinishedDoor = SelectedDoor;

						if (UGameplayStatics::SaveGameToSlot(NewSave, Generator->GameName + FString::FromInt(Generator->RoomCount++), 0))
						{
					
						}
						else
						{
							if (GEngine)
							{
								GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("failed to save")));
							}
						}
					}*/

					//Room->number = RoomsLeft;
					//UnfinishedRooms.Add(Room);
					//SpawnedRooms.Add(Room);
					TArray<AActor*> IgnoredActors;
					IgnoredActors.Add(Starting.Get());
					IgnoredActors.Add(Room);
			

					Generator->SpawnPassage(Location + Rotation.RotateVector(Exit.Location), Rotation.RotateVector(Exit.Direction * 1000), NextLocation + NextRotation.RotateVector(Room->Doors[SelectedDoor].Location) - (Location + Rotation.RotateVector(Exit.Location)), NextRotation.RotateVector(Room->Doors[SelectedDoor].Direction * -1000), Exit.Size, Room->Doors[SelectedDoor].Size, IgnoredActors, Material);
					Generator->LoadingTasks -= 1;
					if (Generator->LoadingTasks == 0)
					{
						Generator->LoadingStatus = Idle;
					}
					//Room->Doors.RemoveAt(SelectedDoor);
					});
			}
			else
			{
				AsyncTask(ENamedThreads::GameThread, [Exit, Rotation, Generator, Location]
				{
					FVector V = Rotation.RotateVector(Exit.Direction);
					Generator->GetWorld()->SpawnActor<AActor>(Generator->BlockadeTypes[0], (Location + Rotation.RotateVector(Exit.Location)), (-V).ToOrientationRotator());
					Generator->LoadingTasks -=1;
					if (Generator->LoadingTasks == 0)
					{
						Generator->LoadingStatus = Idle;
					}
				});
			}
		
		}
		//AsyncTask(ENamedThreads::GameThread, [Generator, Starting, Save]
		//{
			// Faction
			/*if (Generator->RandomFloat(0, 0.5) > 0.9)
			{
				AFaction *Faction = Generator->GetWorld()->SpawnActor<AFaction>(Generator->FactionType, Starting->GetActorLocation(), FRotator(0, 0, 0));
				int num = Generator->RandomInt(1, 5);
				int RaceIndex = Generator->RandomInt(0, Generator->Races.Num() - 1);
				for (int i = 0; i < num; i++)
				{
					AAICharacterController* Controller = Generator->SpawnCharacter(Generator->Races[RaceIndex], Starting->GetActorLocation() + FVector(Generator->RandomFloat(-300, 300), Generator->RandomFloat(-300, 300), 200));
					Faction->Members.Add(Controller);
					Controller->Faction = Faction;
				}
			}


			// Biome
			FBiome Biome = Generator->Biomes[Starting->BiomeIndex];
			Starting->Mesh->SetMaterial(0, Biome.RoomMaterial);
			if (Biome.PossibleResources.Num() > 0)
			{
				float Sum = 0;
				for (int i = 0; i < Biome.PossibleResources.Num(); i++)
				{
					Sum += Biome.PossibleResources[i].Rarity;
				}
				while (Starting->Size > 0)
				{
					float Rand = Generator->RandomFloat(0, Sum);
					for (auto& Resource : Biome.PossibleResources)
					{
						Sum -= Resource.Rarity;
						if (Sum <= Rand)
						{
							start comment  FVector V = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Resource.ConeDirection, Resource.ConeAngle);
							TArray<AActor*> ActorsToIgnore;
							FHitResult HitResult;
							FVector Center = Starting->GetActorLocation() + (Starting->Dimensions.Z) / 2.0;
							UKismetSystemLibrary::LineTraceSingle(GetWorld(), Center, Center + V * Starting->Dimensions.Length(), UEngineTypes::ConvertToTraceType(ECC_Camera), true, ActorsToIgnore, TraceVisibility, HitResult, false, FLinearColor::Blue, FLinearColor::Red); end comment
							Generator->SpawnResource(Resource, Starting->GetActorLocation() + FVector(0, 0, (Starting->Dimensions.Z) / 2.0), Starting->Dimensions.Length() / 2.0, Starting.Get(), Save.Get());
							break;
						}
					}
					Starting->Size -= 1;
				}
			}*/
			Generator->UnfinishedRooms.Remove(Starting.Get());
		//});
	});
}

void ADungeonGenerator::EndRoom(ARoom* Starting)
{
	FVector Location = Starting->GetActorLocation();
	FRotator Rotation = Starting->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (auto& Exit : Starting->Doors)
	{
		FVector V = Rotation.RotateVector(Exit.Direction);
		AActor* New = GetWorld()->SpawnActor<AActor>(BlockadeTypes[0], Location + Rotation.RotateVector(Exit.Location), (-V).ToOrientationRotator(), SpawnParams);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("block")));
		}
		/*APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageTypes[0], Location + Rotation.RotateVector(Exit.Location), V.ToOrientationRotator(), PassSpawnParams);
		Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), FVector(100, 0, 0), FVector(100, 0, 0), FVector(100, 0, 0));
		Passage->Mesh->SetStartScale(Exit.Size);
		Passage->Mesh->SetEndScale(FVector2D(0.1, 0.1));*/
	}
	UnfinishedRooms.Remove(Starting);
}

void ADungeonGenerator::SpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent, FVector2D StartSize, FVector2D EndSize, TArray<AActor*> IgnoreActors, UMaterialInterface* Material)
{
	FRotator TraceRotation = End.ToOrientationRotator() + FRotator(0, 1, 0);
	TArray<FHitResult> HitResults;
	TArray<FHitResult> BackwardHitResults;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Location + FVector(0, 0, 200), Location + End + FVector(0, 0, 200), FVector(300, 300, 400), TraceRotation, UEngineTypes::ConvertToTraceType(ECC_Camera), true, IgnoreActors, TraceVisibility, HitResults, true, FLinearColor::Gray, FLinearColor::Green);
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Location + End + FVector(0, 0, 200), Location + FVector(0, 0, 200), FVector(300, 300, 400), TraceRotation, UEngineTypes::ConvertToTraceType(ECC_Camera), true, IgnoreActors, TraceVisibility, BackwardHitResults, true, FLinearColor::Gray, FLinearColor::White);

	if (HitResults.Num() > 0)
	{
		int CrossingsNumber = HitResults.Num();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("%d"), CrossingsNumber));
		}
		TArray<ARoom*> Crossings;
		for (int i = 0; i < CrossingsNumber; i++)
		{
			APassage * Other = Cast<APassage>(HitResults[i].GetActor());
			if (!Other)
			{
			//	CrossingsNumber--;
				continue;
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, FString::Printf(TEXT("this shouldn't happen")));
				}
			}
			FVector Location1 = Other->GetActorLocation();
			FVector End1 = Other->Mesh->GetEndPosition();
			FVector WorldEnd1 = Location1 + End1;
			FRotator R = FRotator(0, -90, 0);
			FRotator Rotation = (End).ToOrientationRotator() + FRotator(0, RandomFloat(-5, 5), 0);
			ARoom* Crossing = GetWorld()->SpawnActor<ARoom>(CrossingTypes[0], ((HitResults[i].ImpactPoint /* + BackwardHitResults[CrossingsNumber - 1 - i].ImpactPoint*/)/* / 2.0 */) - FVector(0, 0, 200), Rotation, FActorSpawnParameters());
			Crossings.Add(Crossing);
			int Near;
			int Far;
			if ((Location1 - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[2].Location))).Size() < (Location1 - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[3].Location))).Size())
			{
				Near = 2;
				Far = 3;
			}
			else
			{
				Far = 2;
				Near = 3;
			}
			ForceSpawnPassage(Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Far].Location), Rotation.RotateVector(Crossing->Doors[Far].Direction * 1000), WorldEnd1 - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Far].Location)), Other->Mesh->GetEndTangent(), Crossing->Doors[Far].Size, Other->Mesh->GetEndScale(), Material);
			ForceSpawnPassage(Location1, Other->Mesh->GetStartTangent(), Crossing->GetActorLocation() - Location1 + Rotation.RotateVector(Crossing->Doors[Near].Location), Rotation.RotateVector(Crossing->Doors[Near].Direction * -1000), Other->Mesh->GetStartScale(), Crossing->Doors[Near].Size, Material);
			Other->Destroy();
		}
		if(Crossings.Num()> 0) ForceSpawnPassage(Location, StartTangent, (Crossings[0]->GetActorLocation() + Crossings[0]->GetActorRotation().RotateVector(Crossings[0]->Doors[1].Location)) - Location, Crossings[0]->GetActorRotation().RotateVector(Crossings[0]->Doors[1].Direction * -1000), StartSize, Crossings[0]->Doors[1].Size, Material);
		if(Crossings.Num() > 0) ForceSpawnPassage(Crossings[CrossingsNumber-1]->GetActorLocation() + Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Location), Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Direction * 1000), Location + End - (Crossings[CrossingsNumber - 1]->GetActorLocation() + Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Location)), EndTangent, Crossings[CrossingsNumber - 1]->Doors[0].Size, EndSize, Material);
		for (int i = 0; i < Crossings.Num() - 1; i++)
		{
			ForceSpawnPassage((Crossings[i]->GetActorLocation() + Crossings[i]->GetActorRotation().RotateVector(Crossings[i]->Doors[0].Location)), Crossings[i]->GetActorRotation().RotateVector(Crossings[i]->Doors[0].Direction * 1000), (Crossings[i + 1]->GetActorLocation() + Crossings[i + 1]->GetActorRotation().RotateVector(Crossings[i + 1]->Doors[1].Location) - Crossings[i]->GetActorLocation()), Crossings[i + 1]->GetActorRotation().RotateVector(Crossings[i + 1]->Doors[1].Direction * -1000), Crossings[i]->Doors[0].Size, Crossings[i+1]->Doors[1].Size, Material);
		}
	}
	else
	{
		ForceSpawnPassage(Location, StartTangent, End, EndTangent, StartSize, EndSize, Material);
	}
}

APassage* ADungeonGenerator::ForceSpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent, FVector2D StartSize, FVector2D EndSize, UMaterialInterface* Material)
{
	FActorSpawnParameters PassSpawnParams;
	APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageTypes[0], Location, FRotator(0, 0, 0), PassSpawnParams);
	Passage->Mesh->SetMaterial(0, Material);
	Passage->MeshParams = FMeshParams(FVector(0, 0, 0), StartTangent, End, EndTangent, StartSize, EndSize);
	Passage->OnRep_MeshParams();
	PassagesSave->PassagesInfo.Add(FPassageSave(Passage));
	return Passage;
}

/*	std::ifstream myfile("Text.txt");
	int a;
	myfile >> a;
	printf("hello 123 \r\n");
	TCHAR *buff = new TCHAR[128];
	wchar_t * costam = _wgetcwd(buff, 128);
//	system("dir");
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%s"), buff));
	}*/

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%s"), SweepResult.bBlockingHit ? TEXT("true") : TEXT("false")));
	}*/
	//RoomSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;


	/*float MinimalDistance = 1500;
	for (int i = 0; i < SpawnedRooms.Num(); i++)
	{
		if (abs(NextLocation.X - SpawnedRooms[i]->GetActorLocation().X) < MinimalDistance && abs(NextLocation.Y - SpawnedRooms[i]->GetActorLocation().Y) < MinimalDistance && abs(NextLocation.Z - SpawnedRooms[i]->GetActorLocation().Z) < MinimalDistance)
		{
			CanSpawn = false;
			break;
		}
	}*/