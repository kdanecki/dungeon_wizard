// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
//#include <iostream>
//#include <fstream>
//#include <direct.h>

#include "Kismet/KismetSystemLibrary.h"
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
	/*Super::Tick(DeltaTime);
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("left: %d, num %d"), UnfinishedRooms.Num(), RoomsLeft));
	}
	int i = 0;
	if (UnfinishedRooms.Num() > 0 && RoomsLeft > 0)
	{
		//if (i == 0)
		//{
			FinishRoom(UnfinishedRooms[0]);
			RoomsLeft--;
			i = 1;
		//}
		//else
		//{
		//	i = 0;
		//}

	}
	else if (UnfinishedRooms.Num() > 0)
	{
		EndRoom(UnfinishedRooms[0]);
	}
	*/
}

void ADungeonGenerator::Generate()
{
	FActorSpawnParameters RoomSpawnParams;
	//RoomSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//FActorSpawnParameters PassSpawnParams;
	ARoom * Starting = GetWorld()->SpawnActor<ARoom>(RoomTypes[/*RandomInt(0, RoomTypes.Num() - 1)*/1], GetActorLocation(), GetActorRotation(), RoomSpawnParams);
	UnfinishedRooms.Add(Starting);
	SpawnedRooms.Add(Starting);
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("added %d"), UnfinishedRooms.Num()));
	}
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
	FActorSpawnParameters RoomSpawnParams;
	FActorSpawnParameters PassSpawnParams;
	for (auto& Exit : Starting->Doors)
	{
		FVector Location = Starting->GetActorLocation();
		FRotator Rotation = Starting->GetActorRotation();
		
		FVector NextLocation = Location + Rotation.RotateVector(Exit.Location + Exit.Direction * 3000 + FVector(RandomFloat(-1000, 1000), RandomFloat(-1000, 1000), RandomFloat(-500, 100)));
		FVector V = Rotation.RotateVector(Exit.Direction);
		FRotator NextRotation = V.ToOrientationRotator() + FRotator(0, 180, 0) + FRotator(0, RandomFloat(-30, 30), 0);

		int SelectedType = RandomInt(0, RoomTypes.Num() - 1);
		FVector Dimensions = RoomTypes[SelectedType].GetDefaultObject()->Dimensions;

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
			Room->number = RoomsLeft;
			UnfinishedRooms.Add(Room);
			SpawnedRooms.Add(Room);
			TArray<AActor*> IgnoredActors;
			IgnoredActors.Add(Starting);
			IgnoredActors.Add(Room);
			SpawnPassage(Location + Rotation.RotateVector(Exit.Location), Rotation.RotateVector(Exit.Direction * 1000), NextLocation + NextRotation.RotateVector(Room->Doors[0].Location) - (Location + Rotation.RotateVector(Exit.Location)), NextRotation.RotateVector(Room->Doors[0].Direction * -1000), Exit.Size,Room->Doors[0].Size, IgnoredActors);

			Room->Doors.RemoveAt(0);
		}
		else
		{
			V = Rotation.RotateVector(Exit.Direction);
			GetWorld()->SpawnActor<AActor>(BlockadeTypes[0], (Location + Rotation.RotateVector(Exit.Location)), (-V).ToOrientationRotator());
			////APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageTypes[0], Location + Rotation.RotateVector(Exit.Location), V.ToOrientationRotator(), PassSpawnParams);
			//Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), FVector(100, 0, 0), FVector(100, 0, 0), FVector(100, 0, 0));
			//Passage->Mesh->SetStartScale(Exit.Size);
			//Passage->Mesh->SetEndScale(FVector2D(0.1, 0.1));
		}
		
	}
	TArray<UItemSpawner*> Components;
	Starting->GetComponents<UItemSpawner>(Components);
	
	for (int i = 0; i < Components.Num(); i++)
	{
		UItemSpawner* Component = Components[i];
		float Sum = 0;
		for (auto& Item : Component->PossibleItems)
		{
			Sum += Item.Value;
		}
		float Rand = RandomFloat(0, Sum);
		for (auto& Item : Component->PossibleItems)
		{
			Sum -= Item.Value;
			if (Sum <= Rand)
			{
				if (IsValid(Item.Key))
				{
					AActor* NewActor = GetWorld()->SpawnActor<AActor>(Item.Key, Component->GetComponentLocation(), Component->GetComponentRotation());
					if (AItem* NewItem = Cast<AItem>(NewActor))
					{
						NewItem->SetDefaults(RandomInt(1, 3));
					}
				}
				break;
			}
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

void ADungeonGenerator::SpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent, FVector2D StartSize, FVector2D EndSize, TArray<AActor*> IgnoreActors)
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
				CrossingsNumber--;
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
			ForceSpawnPassage(Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Far].Location), Rotation.RotateVector(Crossing->Doors[Far].Direction * 1000), WorldEnd1 - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Far].Location)), Other->Mesh->GetEndTangent(), Crossing->Doors[Far].Size, Other->Mesh->GetEndScale());
			ForceSpawnPassage(Location1, Other->Mesh->GetStartTangent(), Crossing->GetActorLocation() - Location1 + Rotation.RotateVector(Crossing->Doors[Near].Location), Rotation.RotateVector(Crossing->Doors[Near].Direction * -1000), Other->Mesh->GetStartScale(), Crossing->Doors[Near].Size);
			Other->Destroy();
		}
		if(Crossings.Num()> 0) ForceSpawnPassage(Location, StartTangent, (Crossings[0]->GetActorLocation() + Crossings[0]->GetActorRotation().RotateVector(Crossings[0]->Doors[1].Location)) - Location, Crossings[0]->GetActorRotation().RotateVector(Crossings[0]->Doors[1].Direction * -1000), StartSize, Crossings[0]->Doors[1].Size);
		if(Crossings.Num() > 0) ForceSpawnPassage(Crossings[CrossingsNumber-1]->GetActorLocation() + Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Location), Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Direction * 1000), Location + End - (Crossings[CrossingsNumber - 1]->GetActorLocation() + Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Location)), EndTangent, Crossings[CrossingsNumber - 1]->Doors[0].Size, EndSize);
		for (int i = 0; i < CrossingsNumber - 1; i++)
		{
			ForceSpawnPassage((Crossings[i]->GetActorLocation() + Crossings[i]->GetActorRotation().RotateVector(Crossings[i]->Doors[0].Location)), Crossings[i]->GetActorRotation().RotateVector(Crossings[i]->Doors[0].Direction * 1000), (Crossings[i + 1]->GetActorLocation() + Crossings[i + 1]->GetActorRotation().RotateVector(Crossings[i + 1]->Doors[1].Location) - Crossings[i]->GetActorLocation()), Crossings[i + 1]->GetActorRotation().RotateVector(Crossings[i + 1]->Doors[1].Direction * -1000), Crossings[i]->Doors[0].Size, Crossings[i+1]->Doors[1].Size);
		}
	}
	else
	{
		ForceSpawnPassage(Location, StartTangent, End, EndTangent, StartSize, EndSize);
		/*FActorSpawnParameters PassSpawnParams;
		APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location, FRotator(0, 0, 0), PassSpawnParams);
		Passage->MeshParams = FMeshParams(FVector(0, 0, 0), StartTangent, End, EndTangent, StartSize, EndSize);
		Passage->OnRep_MeshParams();*/
	}
}

APassage* ADungeonGenerator::ForceSpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent, FVector2D StartSize, FVector2D EndSize)
{
	FActorSpawnParameters PassSpawnParams;
	APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageTypes[0], Location, FRotator(0, 0, 0), PassSpawnParams);
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