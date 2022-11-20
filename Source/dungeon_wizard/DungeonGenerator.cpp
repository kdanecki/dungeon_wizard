// Fill out your copyright notice in the Description page of Project Settings.
//#pragma optimize("", off)

#include "DungeonGenerator.h"
//#include <iostream>
//#include <fstream>
//#include <direct.h>

#include "Kismet/KismetMathLibrary.h"
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

void ADungeonGenerator::Generate(TArray<int>& LoadedRooms)
{
	FActorSpawnParameters RoomSpawnParams;
	ARoom* Starting = GetWorld()->SpawnActor<ARoom>(RoomTypes[StartingRoom], GetActorLocation(), GetActorRotation(), RoomSpawnParams);
	RoomCount = 1;
	Starting->BiomeIndex = StartingBiome;
	SpawnedRooms.Add(Starting);
	FinishRoom(Starting);
	LoadedRooms.Add(0);
/*	for (int i = 1; i < 5; ++i)
	{
		FinishRoom(SpawnedRooms[i]);
		LoadedRooms.Add(i);
	}*/
}

void ADungeonGenerator::FinishRoom(ARoom* Starting/*, URoomSave* Save1*/)
{
	FActorSpawnParameters PassSpawnParams;
	FActorSpawnParameters RoomSpawnParams;
	if (Starting->FinishedDoor != -1)
	{
		Starting->Doors.RemoveAt(Starting->FinishedDoor);
	}
	for (auto& Exit : Starting->Doors)
	{
		// magic calculation
		FVector Location = Starting->GetActorLocation();
		FRotator Rotation = Starting->GetActorRotation();

		int SelectedType = RandomInt(0, RoomTypes.Num() - 1);
		FVector Dimensions = RoomTypes[SelectedType].GetDefaultObject()->Dimensions;
		int SelectedDoor = RandomInt(0, RoomTypes[SelectedType].GetDefaultObject()->Doors.Num() - 1);
		FDoorInfo NextExit = RoomTypes[SelectedType].GetDefaultObject()->Doors[SelectedDoor];
		int SelectedPassage = RandomInt(0, PassageTypes.Num() - 1);
		
		float PassageLength = 400 + RandomFloat(0, 2000); //200 + (0, 2000)

		FVector sel = RoomTypes[SelectedType].GetDefaultObject()->Doors[SelectedDoor].Direction;
		FVector V = Rotation.RotateVector(Exit.Direction);
		float cos = FVector::DotProduct(NextExit.Direction, -V);
		float rad = FMath::Acos(cos);
		FVector axis = FVector::CrossProduct(NextExit.Direction, -V);
		axis.Normalize();
		FVector V1 = FVector(1, 0, 0);
		FVector V2 = V1.RotateAngleAxis(FMath::RadiansToDegrees(rad), axis);
	
		FRotator NextRotation = V2.ToOrientationRotator() + FRotator(0, PassageTypes[SelectedPassage].GetDefaultObject()->Angle, 0);
		
		FVector NextLocation = Location + Rotation.RotateVector(Exit.Location) - NextRotation.RotateVector(NextExit.Location) + V.ToOrientationRotator().RotateVector(PassageTypes[SelectedPassage].GetDefaultObject()->Length);
		
		//FVector NextLocation = ;
		//FRotator NextRotation;
		
		// check if can spawn
		bool CanSpawn = true;
		TArray<FHitResult> SweepResults1;
		TArray<AActor*> ActorsToIgnore;
		Starting->Collision->GetOverlappingActors(ActorsToIgnore);
		ActorsToIgnore.Add(Starting);
		bool Hit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), NextLocation - FVector(10, 10, (10 - Dimensions.Z / 2.0)), NextLocation + FVector(10, 10, 10 + Dimensions.Z / 2.0), FVector(Dimensions.X * 0.5, Dimensions.Y * 0.5, Dimensions.Z * 0.5), NextRotation, UEngineTypes::ConvertToTraceType(ECC_Camera), true, ActorsToIgnore, TraceVisibility, SweepResults1, true, FLinearColor::Gray, FLinearColor::Yellow);
		if (SweepResults1.Num() > 0)
		{
			CanSpawn = false;
		}

		TArray<FHitResult> SweepResults;
		FRotator TraceRotation = (NextLocation - Location).ToOrientationRotator();
		Hit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Location + FVector(0, 0, 200), NextLocation + FVector(0, 0, 200), FVector(300, 300, 400), TraceRotation, UEngineTypes::ConvertToTraceType(ECC_Camera), true, ActorsToIgnore, TraceVisibility, SweepResults, true, FLinearColor::Blue, FLinearColor::Red);
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
		
		// spawn
		if (CanSpawn)
		{
			ARoom * Room = GetWorld()->SpawnActor<ARoom>(RoomTypes[SelectedType], NextLocation, NextRotation, RoomSpawnParams);
			SpawnedRooms.Add(Room);
			Room->Index = RoomCount++;
			if (SpawnedRooms.Num() != RoomCount)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Red, FString::Printf(TEXT("Index not matching room count")));
				}
			}
			Room->NeighborsIndex.Add(Starting->Index);
			Starting->NeighborsIndex.Add(Room->Index);
			if (RandomFloat(0, 1) > 0.5)
			{
				Room->BiomeIndex = Starting->BiomeIndex;
			}
			else
			{
				Room->BiomeIndex = RandomInt(0, Biomes.Num() - 1);
			}
			UMaterialInterface* Material = Biomes[Starting->BiomeIndex].RoomMaterial;
			TArray<AActor*> IgnoredActors;
			Starting->Collision->GetOverlappingActors(IgnoredActors);
			IgnoredActors.Add(Starting);
			IgnoredActors.Add(Room);
			ForceSpawnPassage(SelectedPassage, Location + Rotation.RotateVector(Exit.Location), V.ToOrientationRotator(), Material);
			Room->FinishedDoor = SelectedDoor;
		}
		else
		{
			FVector V123 = Rotation.RotateVector(Exit.Direction);
			ABlockade* Blockade = GetWorld()->SpawnActor<ABlockade>(BlockadeTypes[0], (Location + Rotation.RotateVector(Exit.Location)), (-V123).ToOrientationRotator());
			Blockade->Mesh->SetMaterial(0, Biomes[Starting->BiomeIndex].RoomMaterial);
		}
	
	}
	Starting->IsFinished = true;
	
	// Faction
	if (RandomFloat(0, 1) > 0.9)
	{
		AFaction *Faction = GetWorld()->SpawnActor<AFaction>(FactionType, Starting->GetActorLocation(), FRotator(0, 0, 0));
		int num = RandomInt(1, 5);
		int RaceIndex = RandomInt(0, Races.Num() - 1);
		for (int i = 0; i < num; i++)
		{
			AAICharacterController* Controller = SpawnCharacter(Races[RaceIndex], Starting->GetActorLocation() + FVector(RandomFloat(-300, 300), RandomFloat(-300, 300), 200));
			Faction->Members.Add(Controller);
			Controller->Faction = Faction;
		}
	}


	// Biome
	FBiome Biome = Biomes[Starting->BiomeIndex];
	Starting->Mesh->SetMaterial(0, Biome.RoomMaterial);
	if (Biome.PossibleResources.Num() > 0)
	{
		Starting->GenerateResources(Biome.PossibleResources);
	}
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
	/*FRotator TraceRotation = End.ToOrientationRotator() + FRotator(0, 1, 0);
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
			ARoom* Crossing = GetWorld()->SpawnActor<ARoom>(CrossingTypes[0], ((HitResults[i].ImpactPoint start comment + BackwardHitResults[CrossingsNumber - 1 - i].ImpactPoint#1#)start comment / 2.0 #1#) - FVector(0, 0, 200), Rotation, FActorSpawnParameters());
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
	}*/
}

APassage* ADungeonGenerator::ForceSpawnPassage(int Type, FVector Location, FRotator Rotation, UMaterialInterface* Material)
{
	FActorSpawnParameters PassSpawnParams;
	APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageTypes[Type], Location, Rotation, PassSpawnParams);
	Passage->Mesh->SetMaterial(0, Material);
	//Passage->MeshParams = FMeshParams(FVector(0, 0, 0), StartTangent, End, EndTangent, StartSize, EndSize);
	//Passage->OnRep_MeshParams();
	//return Passage;
	return nullptr;
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