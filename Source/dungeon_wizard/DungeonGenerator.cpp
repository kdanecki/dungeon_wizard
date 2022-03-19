// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator.h"
//#include <iostream>
//#include <fstream>
//#include <direct.h>


// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	Generate();

}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("added %d"), UnfinishedRooms.Num()));
	}
	/*while (UnfinishedRooms.Num() > 0 && RoomsLeft > 0)
	{
		FinishRoom(UnfinishedRooms[0]);
		RoomsLeft--;
	}
	while (UnfinishedRooms.Num() > 0)
	{
		EndRoom(UnfinishedRooms[0]);
	}*/
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
		FVector NextLocation = Location + Rotation.RotateVector(Exit.Location * RandomFloat(4, 10) + FVector(0, 0, RandomFloat(-100, 100)));
		FVector V = Rotation.RotateVector(Exit.Direction);
		FRotator NextRotation = V.ToOrientationRotator() + FRotator(0, 180, 0) + FRotator(0, RandomFloat(-30, 30), 0);


		bool CanSpawn = true;
		FHitResult SweepResult;

		FCollisionShape Shape = FCollisionShape::MakeBox(FVector(700, 700, 700));
		GetWorld()->SweepSingleByChannel(SweepResult, NextLocation - FVector(10, 10, 10), NextLocation + FVector(10, 10, 10), FQuat(), ECC_Visibility, Shape, FCollisionQueryParams("collision", true, Starting));
		if (IsValid(SweepResult.GetActor()))
		{
			CanSpawn = false;
		}
		else
		{
			TArray<FHitResult> SweepResults;
			Shape = FCollisionShape::MakeBox(FVector(150, 150, 150));
			GetWorld()->SweepMultiByChannel(SweepResults, Location, NextLocation + (Location - NextLocation).Normalize() * 700, FQuat(), ECC_Visibility, Shape, FCollisionQueryParams("collision", true, Starting));
			for (int i = 0; i < SweepResults.Num(); i++)
			{
				if (SweepResults[i].GetActor()->IsA(ARoom::StaticClass()))
				{
					CanSpawn = false;
					break;
				}
			}
		}
		if (CanSpawn)
		{
			ARoom * Room = GetWorld()->SpawnActor<ARoom>(RoomTypes[RandomInt(0, RoomTypes.Num() - 1)], NextLocation, NextRotation, RoomSpawnParams);
			UnfinishedRooms.Add(Room);
			SpawnedRooms.Add(Room);
			TArray<AActor*> IgnoredActors;
			IgnoredActors.Add(Starting);
			IgnoredActors.Add(Room);
			SpawnPassage(Location + Rotation.RotateVector(Exit.Location), Rotation.RotateVector(Exit.Direction * 1000), NextLocation + NextRotation.RotateVector(Room->Doors[0].Location) - (Location + Rotation.RotateVector(Exit.Location)), NextRotation.RotateVector(Room->Doors[0].Direction * -1000), Location, IgnoredActors);
			//APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location + Rotation.RotateVector(Exit.Location), FRotator(0, 0, 0), PassSpawnParams);
			//Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), Rotation.RotateVector(Exit.Direction * 200), NextLocation + NextRotation.RotateVector(Room->Doors[0].Location) - (Location + Rotation.RotateVector(Exit.Location)), NextRotation.RotateVector(Room->Doors[0].Direction * -1000));

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
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(100, 100, 100));
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
				GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Blue, ((TEXT("passage crossing %s") + RoomLocation.ToString())));
			}
			APassage *Other = Cast<APassage>(SweepResult.GetActor());
			FVector Start1 = SweepResult.GetActor()->GetActorLocation();
			FVector End1 = Other->Mesh->GetEndPosition();
			FRotator Rotation = FRotator(0, (End1).ToOrientationRotator().Yaw, 0);
			
			//ARoom* Crossing = GetWorld()->SpawnActor<ARoom>(CrossingTypes[0], SweepResult.Location, Rotation, FActorSpawnParameters());
			//Crossing->Mesh->SetMaterial(0, RedMaterial);
			ARoom* Crossing = GetWorld()->SpawnActor<ARoom>(CrossingTypes[0], ((Start1 + Start1 + End1) / (2.0) / End1.Size() + (Location + Location + End) / (2.0) / End.Size()) * ((End1.Size() * End.Size()) / (End.Size() + End1.Size())), Rotation, FActorSpawnParameters());
			Crossing->Mesh->SetMaterial(0, RedMaterial);
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
			Passage->Mesh->SetMaterial(0, RedMaterial);
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Blue, ((TEXT("room") + SweepResult.GetActor()->GetActorLocation().ToString() )));
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
	}
}

APassage* ADungeonGenerator::ForceSpawnPassage(FVector Location, FVector StartTangent, FVector End, FVector EndTangent)
{
	FActorSpawnParameters PassSpawnParams;
	APassage* Passage = GetWorld()->SpawnActor<APassage>(PassageClass, Location, FRotator(0, 0, 0), PassSpawnParams);
	Passage->Mesh->SetStartAndEnd(FVector(0, 0, 0), StartTangent, End, EndTangent);
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