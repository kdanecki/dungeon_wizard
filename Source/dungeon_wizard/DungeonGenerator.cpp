// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
//#include <iostream>
//#include <fstream>
//#include <direct.h>
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Foo.h"
#include "alchemik/Boo.h"
// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

EDrawDebugTrace::Type TraceVisibility = EDrawDebugTrace::ForOneFrame;

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

	
	//TArray<APassage*> Passages;
	//TArray<FDoorInfo> Doors;
	//Rooms.SetNum(2);
	//int RoomsLeft = 10;
	FActorSpawnParameters RoomSpawnParams;
	//FActorSpawnParameters PassSpawnParams;
	ARoom * Starting = GetWorld()->SpawnActor<ARoom>(RoomTypes[/*RandomInt(0, RoomTypes.Num() - 1)*/3], GetActorLocation(), GetActorRotation(), RoomSpawnParams);
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

	/*for (int i = 0; i < 3; i++)
	{
		AItem * Item = GetWorld()->SpawnActor<AItem>(ItemsToSpawn[3], FVector(300, 0, 200 + 100 * 1), GetActorRotation(), SpawnParams);
		Item->SetDefaults(1);
	}*/
	for (int i = 0; i < 1; i++)
	{
		AItem * Item = GetWorld()->SpawnActor<AItem>(ItemsToSpawn[4], FVector(300, 0, 200 + 100 * 1), GetActorRotation(), SpawnParams);
		Item->SetDefaults(10);
	}
	//Rooms[1] = GetWorld()->SpawnActor<ARoom>(RoomClass, GetActorLocation() + FVector(1500, 0 , 0), GetActorRotation(), SpawnParams);
/*	FRoomSpawnInfo SpawnInfo;
	SpawnInfo.XSize = RandomFloat(5, 10);
	SpawnInfo.YSize = RandomFloat(5, 10);
	SpawnInfo.ZSize = RandomFloat(3, 5);
	Doors.Add(FDoorInfo(1, 4.0, 1.5, 2.0));
	SpawnInfo.DoorsInfo.Add(FDoorInfo(1, 4.0, 1.5, 2.0));
	Rooms[0]->CreateRoom(SpawnInfo);
	SpawnInfo.XSize = RandomFloat(5, 10);
	SpawnInfo.YSize = RandomFloat(5, 10);
	SpawnInfo.ZSize = RandomFloat(3, 5);
	SpawnInfo.DoorsInfo.Empty();
	SpawnInfo.DoorsInfo.Add(FDoorInfo(3, 4.0, 1.5, 2.0));
	Rooms[1]->CreateRoom(SpawnInfo);
	
	FRotator Rot(0, 90 * (Doors[0].WallNumber - 1), 0);
	FVector Location = Rot.RotateVector(FVector(1, 0, 0)) * (float)(Rooms[0]->XSize / 0.02) + FVector(0, 0, Rooms[0]->ZSize / 0.02 - 5.0);
	APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location, GetActorRotation(), SpawnParams);
	Passage->CreatePassage(Rooms[1]->GetActorLocation(), FVector(100, 0, 0));*/
}

void ADungeonGenerator::FinishRoom(ARoom* Starting)
{
	FActorSpawnParameters RoomSpawnParams;
	FActorSpawnParameters PassSpawnParams;
	for (auto& Exit : Starting->Doors)
	{
		FVector Location = Starting->GetActorLocation();
		FRotator Rotation = Starting->GetActorRotation();
		FVector NextLocation = Location + Rotation.RotateVector(Exit.Location * RandomFloat(4, 10) + FVector(0, 0, RandomFloat(-500, 100)));
		FVector V = Rotation.RotateVector(Exit.Direction);
		FRotator NextRotation = V.ToOrientationRotator() + FRotator(0, 180, 0) + FRotator(0, RandomFloat(-30, 30), 0);


		bool CanSpawn = true;
		TArray<FHitResult> SweepResults1;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Starting);
		bool Hit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), NextLocation - FVector(10, 10, -365), NextLocation + FVector(10, 10, 385), FVector(700, 700, 700), FRotator(0, 0, 0), UEngineTypes::ConvertToTraceType(ECC_Camera), true, ActorsToIgnore, TraceVisibility, SweepResults1, true, FLinearColor::Gray, FLinearColor::Yellow);
		if (SweepResults1.Num() > 0)
		{
			CanSpawn = false;
		}
		/*for (int i = 0; i < SweepResults1.Num(); i++)
		{
			if (IsValid(SweepResults1[i].GetActor()))
			{
				CanSpawn = false;
				if (GEngine)
				{
		//			GEngine->AddOnScreenDebugMessage(-1, 1000.0f, FColor::Blue, (SweepResult.GetActor()->GetActorLabel() + SweepResult.Location.ToString()));
				}
			}
		}*/

		TArray<FHitResult> SweepResults;
		FRotator TraceRotation = (NextLocation - Location).ToOrientationRotator();
		Hit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), Location + FVector(0, 0, 200), NextLocation + FVector(0, 0, 200), FVector(300, 300, 400), TraceRotation, UEngineTypes::ConvertToTraceType(ECC_Camera), true, ActorsToIgnore, TraceVisibility, SweepResults, true, FLinearColor::Blue, FLinearColor::Red);
		if (CanSpawn)
		{
		//	CanSpawn = !Hit;
			/*if (GEngine && SweepResults.Num()>0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), SweepResults.Num()));
			}*/
			int a = 0;
			for (const FHitResult HitResult : SweepResults)
			{
				a++;
			}
			if (GEngine && a > 0)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%d"), a));
			}
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
		
		/*for (int i = 0; i < SweepResults.Num(); i++)
		{
			if (SweepResults[i].GetActor()->IsA(ARoom::StaticClass()))
			{
				CanSpawn = false;
				break;
			}
		}*/

		if (CanSpawn)
		{
			ARoom * Room = GetWorld()->SpawnActor<ARoom>(RoomTypes[RandomInt(0, RoomTypes.Num() - 1)], NextLocation, NextRotation, RoomSpawnParams);
			Room->number = RoomsLeft;
			UnfinishedRooms.Add(Room);
			SpawnedRooms.Add(Room);
			TArray<AActor*> IgnoredActors;
			IgnoredActors.Add(Starting);
			IgnoredActors.Add(Room);
			SpawnPassage(Location + Rotation.RotateVector(Exit.Location), Rotation.RotateVector(Exit.Direction * 1000), NextLocation + NextRotation.RotateVector(Room->Doors[0].Location) - (Location + Rotation.RotateVector(Exit.Location)), NextRotation.RotateVector(Room->Doors[0].Direction * -1000), Location, IgnoredActors);

			Room->Doors.RemoveAt(0);
		}
		else
		{
			if (GEngine)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("didn't spawn")));
			}
			V = Rotation.RotateVector(Exit.Direction);
			APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location + Rotation.RotateVector(Exit.Location), V.ToOrientationRotator(), PassSpawnParams);
			Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), FVector(100, 0, 0), FVector(100, 0, 0), FVector(100, 0, 0));
			Passage->Mesh->SetEndScale(FVector2D(0.1, 0.1));
		}
		
	}
	UnfinishedRooms.Remove(Starting);
}

void ADungeonGenerator::EndRoom(ARoom* Starting)
{
	FVector Location = Starting->GetActorLocation();
	FRotator Rotation = Starting->GetActorRotation();
	FActorSpawnParameters PassSpawnParams;
	for (auto& Exit : Starting->Doors)
	{
		FVector V = Rotation.RotateVector(Exit.Direction);
		APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location + Rotation.RotateVector(Exit.Location), V.ToOrientationRotator(), PassSpawnParams);
		Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), FVector(100, 0, 0), FVector(100, 0, 0), FVector(100, 0, 0));
		Passage->Mesh->SetEndScale(FVector2D(0.1, 0.1));
	}
	UnfinishedRooms.Remove(Starting);
}

void ADungeonGenerator::SpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent, FVector RoomLocation, TArray<AActor*> IgnoreActors)
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
			}
			FVector Location1 = Other->GetActorLocation();
			FVector End1 = Other->Mesh->GetEndPosition();
			FVector WorldEnd1 = Location1 + End1;
			FRotator R = FRotator(0, -90, 0);
			FRotator Rotation = (End).ToOrientationRotator() + FRotator(0, RandomFloat(-5, 5), 0);
			ARoom* Crossing = GetWorld()->SpawnActor<ARoom>(CrossingTypes[0], ((HitResults[i].ImpactPoint + BackwardHitResults[CrossingsNumber - 1 - i].ImpactPoint) / 2.0) -FVector(0, 0, 200), Rotation, FActorSpawnParameters());
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
			ForceSpawnPassage(Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Far].Location), Rotation.RotateVector(Crossing->Doors[Far].Direction * 1000), WorldEnd1 - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Far].Location)), Other->Mesh->GetEndTangent());
			ForceSpawnPassage(Location1, Other->Mesh->GetStartTangent(), Crossing->GetActorLocation() - Location1 + Rotation.RotateVector(Crossing->Doors[Near].Location), Rotation.RotateVector(Crossing->Doors[Near].Direction * -1000));
			Other->Destroy();
		}
		if(Crossings.Num()> 0) ForceSpawnPassage(Location, StartTangent, (Crossings[0]->GetActorLocation() + Crossings[0]->GetActorRotation().RotateVector(Crossings[0]->Doors[1].Location)) - Location, Crossings[0]->GetActorRotation().RotateVector(Crossings[0]->Doors[1].Direction * -1000));
		if(Crossings.Num() > 0) ForceSpawnPassage(Crossings[CrossingsNumber-1]->GetActorLocation() + Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Location), Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Direction * 1000), Location + End - (Crossings[CrossingsNumber - 1]->GetActorLocation() + Crossings[CrossingsNumber - 1]->GetActorRotation().RotateVector(Crossings[CrossingsNumber - 1]->Doors[0].Location)), EndTangent);
		for (int i = 0; i < CrossingsNumber - 1; i++)
		{
			ForceSpawnPassage((Crossings[i]->GetActorLocation() + Crossings[i]->GetActorRotation().RotateVector(Crossings[i]->Doors[0].Location)), Crossings[i]->GetActorRotation().RotateVector(Crossings[i]->Doors[0].Direction * 1000), (Crossings[i + 1]->GetActorLocation() + Crossings[i + 1]->GetActorRotation().RotateVector(Crossings[i + 1]->Doors[1].Location) - Crossings[i]->GetActorLocation()), Crossings[i + 1]->GetActorRotation().RotateVector(Crossings[i + 1]->Doors[1].Direction * -1000));
		}
	}
	else
	{
		FActorSpawnParameters PassSpawnParams;
		APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location, FRotator(0, 0, 0), PassSpawnParams);
		Passage->MeshParams = FMeshParams(FVector(0, 0, 0), StartTangent, End, EndTangent);
		Passage->OnRep_MeshParams();
	}
	/*FCollisionShape Shape = FCollisionShape::MakeBox(FVector(100, 100, 100));
	FHitResult SweepResult;
	FCollisionQueryParams QueryParams = FCollisionQueryParams("collision", true);
	QueryParams.AddIgnoredActors(IgnoreActors);
	GetWorld()->SweepSingleByChannel(SweepResult, Location, Location + (End), ((End).ToOrientationRotator() + FRotator(0, 45, 0)).Quaternion(), ECC_Visibility, Shape, QueryParams);
	
	if (IsValid(SweepResult.GetActor()))
	{
		if (SweepResult.GetActor()->IsA(APassage::StaticClass()))
		{
			if (GEngine)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Blue, ((TEXT("passage crossing %s") + RoomLocation.ToString())));
			}
			APassage *Other = Cast<APassage>(SweepResult.GetActor());
			FVector Start1 = SweepResult.GetActor()->GetActorLocation();
			FVector End1 = Other->Mesh->GetEndPosition();
			FRotator Rotation = FRotator(0, (End1).ToOrientationRotator().Yaw, 0);
			
			//ARoom* Crossing = GetWorld()->SpawnActor<ARoom>(CrossingTypes[0], SweepResult.Location, Rotation, FActorSpawnParameters());
			//Crossing->Mesh->SetMaterial(0, RedMaterial);
			ARoom* Crossing = GetWorld()->SpawnActor<ARoom>(CrossingTypes[0], ((Start1 + Start1 + End1) / (2.0) / End1.Size() + (Location + Location + End) / (2.0) / End.Size()) * ((End1.Size() * End.Size()) / (End.Size() + End1.Size())), Rotation, FActorSpawnParameters());
			//Crossing->Mesh->SetMaterial(0, RedMaterial);
			FVector WorldEnd1 = Start1 + End1;
			ForceSpawnPassage(Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[0].Location), Rotation.RotateVector(Crossing->Doors[0].Direction * 2000), WorldEnd1 - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[0].Location)), Other->Mesh->GetEndTangent());
			ForceSpawnPassage(Start1, Other->Mesh->GetStartTangent(), Crossing->GetActorLocation() - Start1 + Rotation.RotateVector(Crossing->Doors[1].Location), Rotation.RotateVector(Crossing->Doors[1].Direction * -2000));
			Other->Destroy();
			int Near;
			int Far;
			if ((Location - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[2].Location))).Size() < (Location - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[3].Location))).Size())
			{
				Near = 2;
				Far = 3;
			}
			else
			{
				Far = 2;
				Near = 3;
			}
			FActorSpawnParameters PassSpawnParams;
			APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location, FRotator(0, 0, 0), PassSpawnParams);
			Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), StartTangent, (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Near].Location)) - Location, Rotation.RotateVector(Crossing->Doors[Near].Direction * -1000));
			Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Far].Location), FRotator(0, 0, 0), PassSpawnParams);
			FVector WorldEnd = Location + End;
			Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), Rotation.RotateVector(Crossing->Doors[Far].Direction * 1000), WorldEnd - (Crossing->GetActorLocation() + Rotation.RotateVector(Crossing->Doors[Far].Location)) , EndTangent);
			//Passage->Mesh->SetMaterial(0, RedMaterial);
		}
		else
		{
			if (GEngine)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Blue, ((TEXT("room") + SweepResult.GetActor()->GetActorLocation().ToString() )));
			}
			FActorSpawnParameters PassSpawnParams;
			APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location, FRotator(0, 0, 0), PassSpawnParams);
			Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), StartTangent, End, EndTangent);
			//Passage->Mesh->SetMaterial(0, RedMaterial);
		}
		
	}
	else
	{
		FActorSpawnParameters PassSpawnParams;
		APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location, FRotator(0, 0, 0), PassSpawnParams);
		Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), StartTangent, End, EndTangent);
	}*/
}

APassage* ADungeonGenerator::ForceSpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent)
{
	FActorSpawnParameters PassSpawnParams;
	APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location, FRotator(0, 0, 0), PassSpawnParams);
	Passage->MeshParams = FMeshParams(FVector(0, 0, 0), StartTangent, End, EndTangent);
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