// Fill out your copyright notice in the Description page of Project Settings.


#include "Blockade.h"


// Sets default values
ABlockade::ABlockade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ABlockade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlockade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

