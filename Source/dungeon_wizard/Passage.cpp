// Fill out your copyright notice in the Description page of Project Settings.


#include "Passage.h"


// Sets default values
APassage::APassage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("Spline Mesh"));
	SetRootComponent(StaticMesh);
	SplineMesh->SetupAttachment(StaticMesh);
	//Mesh->SetupAttachment(RootComponent);
	//Mesh->SetStaticMesh(FloorMesh);
	//Mesh->SetMaterial(0, FloorMaterial);
}

// Called when the game starts or when spawned
void APassage::BeginPlay()
{
	Super::BeginPlay();
	
}

void APassage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APassage, MeshParams);
}

// Called every frame
void APassage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*
void APassage::CreatePassage(FVector EndWorldLocation, FVector EndDirection)
{
	//UPROPERTY()
	USplineMeshComponent * Mesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	Mesh->SetStaticMesh(FloorMesh);
	Mesh->SetMaterial(0, FloorMaterial);
	Mesh->SetStartScale(FVector2D(1, 1));
	Mesh->SetEndScale(FVector2D(1, 1));
	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetStartAndEnd(FVector(0, 0, 0), FVector(100, 0, 0), EndWorldLocation - GetActorLocation(), FVector(100, 0, 0));
	
	Mesh->RegisterComponent();
	
}*/

void APassage::OnRep_MeshParams()
{
	SplineMesh->SetStartAndEnd(MeshParams.Start, MeshParams.StartTangent, MeshParams.End, MeshParams.EndTangent);
	SplineMesh->SetStartScale(MeshParams.StartSize);
	SplineMesh->SetEndScale(MeshParams.EndSize);
}