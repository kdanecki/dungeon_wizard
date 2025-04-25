// Fill out your copyright notice in the Description page of Project Settings.


#include "ToolHandle.h"

// Sets default values
AToolHandle::AToolHandle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PartOf = AHammer::StaticClass();
	//ResourceType = FString(TEXT("raczka mlotka"));
}

// Called when the game starts or when spawned
void AToolHandle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToolHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

