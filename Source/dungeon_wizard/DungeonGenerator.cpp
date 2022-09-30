// Fill out your copyright notice in the Description page of Project Settings.
#pragma optimize("", off)

#include "DungeonGenerator.h"
//#include <iostream>
//#include <fstream>
//#include <direct.h>

//#include "Kismet/KismetMathLibrary.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Foo.h"
#include "alchemik/Boo.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TraceVisibility = EDrawDebugTrace::Persistent;
}

//EDrawDebugTrace::Type TraceVisibility = EDrawDebugTrace::Persistent;

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	Generate();
	Foo *f = new Foo();
	if (GEngine)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%f"), f->a));
	}
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("bar: %f"), f->bar1->b));
	}
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("boo: %d"), f->booo->bo));
	}
	Boo *b = new Boo(20);
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("%d"), b->bo));
	}
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
	ARoom * Starting = GetWorld()->SpawnActor<ARoom>(RoomTypes[StartingRoom], GetActorLocation(), GetActorRotation(), RoomSpawnParams);
	Starting->BiomeIndex = StartingBiome;
	UnfinishedRooms.Add(Starting);
	//SpawnedRooms.Add(Starting);
	while (UnfinishedRooms.Num() > 0 && RoomsLeft > 0)
	{
		FinishRoom(UnfinishedRooms[0]);
		RoomsLeft--;
	}
	while (UnfinishedRooms.Num() > 0)
	{
		EndRoom(UnfinishedRooms[0]);
	}

	
	FActorSpawnParameters SpawnParams;

	for (int i = 0; i < 1; i++)
	{
		AItem * Item = GetWorld()->SpawnActor<AItem>(ItemsToSpawn[4], FVector(300, 0, 200 + 100 * 1), GetActorRotation(), SpawnParams);
		Item->SetDefaults(10);
	}
}

void ADungeonGenerator::FinishRoom(ARoom* Starting)
{
	// passages
	FActorSpawnParameters RoomSpawnParams;
	FActorSpawnParameters PassSpawnParams;
	for (auto& Exit : Starting->Doors)
	{
		FVector Location = Starting->GetActorLocation();
		FRotator Rotation = Starting->GetActorRotation();

		int SelectedType = RandomInt(0, RoomTypes.Num() - 1);
		FVector Dimensions = RoomTypes[SelectedType].GetDefaultObject()->Dimensions;
		int SelectedDoor = RandomInt(0, RoomTypes[SelectedType].GetDefaultObject()->Doors.Num() - 1);
		FDoorInfo NextExit = RoomTypes[SelectedType].GetDefaultObject()->Doors[SelectedDoor];

		float PassageLength = 400 + RandomFloat(0, 2000); //200 + (0, 2000)

		FVector sel = RoomTypes[SelectedType].GetDefaultObject()->Doors[SelectedDoor].Direction;
		FVector V = Rotation.RotateVector(Exit.Direction);
		float cos = FVector::DotProduct(NextExit.Direction, -V);
		float rad = FMath::Acos(cos);
		FVector axis = FVector::CrossProduct(NextExit.Direction, -V);
		axis.Normalize();
		FVector V1 = FVector(1, 0, 0);
		FVector V2 = V1.RotateAngleAxis(FMath::RadiansToDegrees(rad), axis);
		
		FRotator NextRotation = V2.ToOrientationRotator() + FRotator(0, RandomFloat(-90 * (PassageLength / ((Dimensions.Length() + Starting->Dimensions.Length()) / 2.0)), 90 * (PassageLength / ((Dimensions.Length() + Starting->Dimensions.Length()) / 2.0))), 0);

		
		
		float Weight = RandomFloat(0.4, 0.6);
		float OffsetLength = RandomFloat(-PassageLength / 2.0, PassageLength / 2.0);
		FVector Offset = Rotation.RotateVector(FRotator(0, 90, 0).RotateVector(Exit.Direction) * OffsetLength * Weight) - NextRotation.RotateVector(FRotator(0, 90, 0).RotateVector(NextExit.Direction) * OffsetLength * (1 - Weight)) + FVector(0, 0, RandomFloat(-PassageLength / 3.0, PassageLength / 5.0));
		FVector NextLocation = Location + Rotation.RotateVector(Exit.Location + Exit.Direction * PassageLength * Weight) - NextRotation.RotateVector(NextExit.Location + NextExit.Direction * PassageLength * (1 - Weight)) + Offset;
		

		bool CanSpawn = true;
		TArray<FHitResult> SweepResults1;
		TArray<AActor*> ActorsToIgnore;
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

		if (CanSpawn)
		{
			ARoom * Room = GetWorld()->SpawnActor<ARoom>(RoomTypes[SelectedType], NextLocation, NextRotation, RoomSpawnParams);
			if (RandomFloat(0, 1) > 0.5)
			{
				Room->BiomeIndex = Starting->BiomeIndex;
			}
			else
			{
				Room->BiomeIndex = RandomInt(0, Biomes.Num() - 1);
			}
			Room->number = RoomsLeft;
			UnfinishedRooms.Add(Room);
			//SpawnedRooms.Add(Room);
			TArray<AActor*> IgnoredActors;
			IgnoredActors.Add(Starting);
			IgnoredActors.Add(Room);
			UMaterialInterface* Material = Biomes[Starting->BiomeIndex].RoomMaterial;
			if (RandomFloat(0, 1) >= 0.5)
			{
				Material = Biomes[Room->BiomeIndex].RoomMaterial;
			}
			SpawnPassage(Location + Rotation.RotateVector(Exit.Location), Rotation.RotateVector(Exit.Direction * 1000), NextLocation + NextRotation.RotateVector(Room->Doors[SelectedDoor].Location) - (Location + Rotation.RotateVector(Exit.Location)), NextRotation.RotateVector(Room->Doors[SelectedDoor].Direction * -1000), Exit.Size, Room->Doors[SelectedDoor].Size, IgnoredActors, Material);

			Room->Doors.RemoveAt(SelectedDoor);
		}
		else
		{
			V = Rotation.RotateVector(Exit.Direction);
			GetWorld()->SpawnActor<AActor>(BlockadeTypes[0], (Location + Rotation.RotateVector(Exit.Location)), (-V).ToOrientationRotator());
		}
		
	}
	// Faction
	if (RandomFloat(0, 0.5) > 0.9)
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
		float Sum = 0;
		for (int i = 0; i < Biome.PossibleResources.Num(); i++)
		{
			Sum += Biome.PossibleResources[i].Rarity;
		}
		while (Starting->Size > 0)
		{
			float Rand = RandomFloat(0, Sum);
			for (auto& Resource : Biome.PossibleResources)
			{
				Sum -= Resource.Rarity;
				if (Sum <= Rand)
				{
					/*FVector V = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Resource.ConeDirection, Resource.ConeAngle);
					TArray<AActor*> ActorsToIgnore;
					FHitResult HitResult;
					FVector Center = Starting->GetActorLocation() + (Starting->Dimensions.Z) / 2.0;
					UKismetSystemLibrary::LineTraceSingle(GetWorld(), Center, Center + V * Starting->Dimensions.Length(), UEngineTypes::ConvertToTraceType(ECC_Camera), true, ActorsToIgnore, TraceVisibility, HitResult, false, FLinearColor::Blue, FLinearColor::Red);*/
					SpawnResource(Resource, Starting->GetActorLocation() + FVector(0, 0, (Starting->Dimensions.Z) / 2.0), Starting->Dimensions.Length() / 2.0, Starting);
					break;
				}
			}
			Starting->Size -= 1;
		}
	}
	


	UnfinishedRooms.Remove(Starting);
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