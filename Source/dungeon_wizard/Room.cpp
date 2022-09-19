// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("inherited"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Size = 1;
}

// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*
void ARoom::CreateRoom()
{
/*	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(RoomMesh);
	Mesh->SetRelativeLocation(FVector(0, 0, 0));
	Mesh->SetMaterial(0, RoomMaterial);
	Mesh->RegisterComponent();*/
//}


/*
void ARoom::CreateRoom(FRoomSpawnInfo RoomSpawnInfo)
{
	XSize = RoomSpawnInfo.XSize;
	YSize = RoomSpawnInfo.YSize;
	ZSize = RoomSpawnInfo.ZSize;
	float X = RoomSpawnInfo.XSize;
	float Y = RoomSpawnInfo.YSize;
	float Z = RoomSpawnInfo.ZSize;
	TArray<FDoorInfo> Doors = RoomSpawnInfo.DoorsInfo;
	// floor
	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(WallMesh);
	Mesh->SetRelativeLocation(FVector(0, 0, 0));
	Mesh->SetMaterial(0, FloorMaterial);
	Mesh->SetWorldScale3D(FVector(X, Y, 0.1));
	Mesh->RegisterComponent();
	// walls
	for (int i = 1; i < 5; i++)
	{
		TArray<FDoorInfo> DoorsOnWall;
		USceneComponent* Wall = NewObject<USceneComponent>(this);
		Wall->SetupAttachment(RootComponent);
		float WallLength;
		float WallDistance;
		if (i % 2 == 1)
		{
			WallLength = YSize;
			WallDistance = XSize;
		}
		else
		{
			WallLength = XSize;
			WallDistance = YSize;
		}
		FRotator Rot(0, 90 * (i - 1), 0);
		FVector Location = Rot.RotateVector(FVector(1, 0, 0)) * (float)(WallDistance / 0.02) + FVector(0, 0, Z / 0.02 - 5.0);
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Blue, FString::Printf(TEXT("%f, %f, %f, %f"), Rot.RotateVector(FVector(1, 0, 0)).X, Rot.RotateVector(FVector(1, 0, 0)).Y, Rot.RotateVector(FVector(1, 0, 0)).Z, (float)(WallDistance / 0.02)));
		}
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Blue, FString::Printf(TEXT("%f, %f, %f"), Location.X, Location.Y, Location.Z));
		}*/
		/*Wall->SetRelativeLocation(Location);
		Wall->SetRelativeRotation(FRotator(0, (i-1) * 90, 0));
		Wall->RegisterComponent();
		for (int j = 0; j < Doors.Num(); j++)
		{
			if (Doors[j].WallNumber == i)
			{
				DoorsOnWall.Add(Doors[j]);
			}
		}
		DoorsOnWall.Sort();

		//no door
		if (DoorsOnWall.Num() == 0)
		{
			Mesh = NewObject<UStaticMeshComponent>(this);
			Mesh->SetupAttachment(Wall);
			Mesh->SetStaticMesh(WallMesh);
			Mesh->SetMaterial(0, WallMaterial);
			Mesh->SetRelativeLocation(FVector(0, 0, 0));
			Mesh->SetWorldScale3D(FVector(0.1, WallLength, Z));
			Mesh->RegisterComponent();
		}
		else //doors
		{
			//first part
			Mesh = NewObject<UStaticMeshComponent>(this);
			Mesh->SetupAttachment(Wall);
			Mesh->SetStaticMesh(WallMesh);
			Mesh->SetMaterial(0, WallMaterial);
			Mesh->SetRelativeLocation(FVector(0, (DoorsOnWall[0].Location - DoorsOnWall[0].XSize/2.0 - WallLength) / 0.02, 0));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%f - %f - %f = %f"), DoorsOnWall[0].Location, DoorsOnWall[0].XSize / 2.0, WallLength, (DoorsOnWall[0].Location - DoorsOnWall[0].XSize / 2.0 - WallLength) / 0.02));
			}
			Mesh->SetWorldScale3D(FVector(0.1, (DoorsOnWall[0].Location - DoorsOnWall[0].XSize/2.0), Z));
			Mesh->RegisterComponent();
			for (int j = 0; j < DoorsOnWall.Num(); j++)
			{
				//door
				Mesh = NewObject<UStaticMeshComponent>(this);
				Mesh->SetupAttachment(Wall);
				Mesh->SetStaticMesh(WallMesh);
				Mesh->SetMaterial(0, WallMaterial);
				Mesh->SetRelativeLocation(FVector(0, (DoorsOnWall[j].Location - WallLength/2.0) / 0.01, (DoorsOnWall[j].YSize ) / 0.02));
				Mesh->SetWorldScale3D(FVector(0.1, DoorsOnWall[j].XSize, Z- DoorsOnWall[j].YSize));
				Mesh->RegisterComponent();
				if (j != DoorsOnWall.Num()-1)
				{
					//next part
					Mesh = NewObject<UStaticMeshComponent>(this);
					Mesh->SetupAttachment(Wall);
					Mesh->SetStaticMesh(WallMesh);
					Mesh->SetMaterial(0, WallMaterial);
					Mesh->SetRelativeLocation(FVector(0, (((DoorsOnWall[j + 1].Location - DoorsOnWall[j + 1].XSize/2.0) - (DoorsOnWall[j].Location + DoorsOnWall[j].XSize/2.0))/2.0 - WallLength/2.0 + (DoorsOnWall[j].Location + DoorsOnWall[j].XSize / 2.0)) / 0.01, 0));
					Mesh->SetWorldScale3D(FVector(0.1, ((DoorsOnWall[j + 1].Location - DoorsOnWall[j + 1].XSize/2.0) - (DoorsOnWall[j].Location + DoorsOnWall[j].XSize/2.0)), Z));
					Mesh->RegisterComponent();
				}
			}
			//last part
			Mesh = NewObject<UStaticMeshComponent>(this);
			Mesh->SetupAttachment(Wall);
			Mesh->SetStaticMesh(WallMesh);
			Mesh->SetMaterial(0, WallMaterial);
			Mesh->SetRelativeLocation(FVector(0, (WallLength/2.0 - (WallLength/2.0 - (DoorsOnWall[DoorsOnWall.Num()-1].Location + DoorsOnWall[DoorsOnWall.Num()-1].XSize / 2.0) / 2.0)) / 0.01, 0));
			Mesh->SetWorldScale3D(FVector(0.1, (WallLength - (DoorsOnWall[DoorsOnWall.Num()-1].Location + DoorsOnWall[DoorsOnWall.Num()-1].XSize / 2.0)), Z));
			Mesh->RegisterComponent();
		}
	}/*
	// x minus
	Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(WallMesh);
	Mesh->SetMaterial(0, WallMaterial);
	Mesh->SetRelativeLocation(FVector(X/-0.02+5, 0, Z/0.02-5));
	Mesh->SetWorldScale3D(FVector(0.1, Y, Z));
	Mesh->RegisterComponent();

	// y minus
	Mesh = NewObject<UStaticMeshComponent>(this);
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStaticMesh(WallMesh);
	Mesh->SetMaterial(0, WallMaterial);
	Mesh->SetRelativeLocation(FVector(0, Y/-0.02+5, Z/0.02-5));
	Mesh->SetWorldScale3D(FVector(X, 0.1, Z));
	Mesh->RegisterComponent();
}*/

