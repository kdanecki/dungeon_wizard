// Fill out your copyright notice in the Description page of Project Settings.


#include "HammerHandle.h"

#include "Hammer.h"


// Sets default values
AHammerHandle::AHammerHandle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PartOf = AHammer::StaticClass();
	ResourceType = FString(TEXT("raczka mlotka"));
	MixFunction = mix_Raczka_mlotka;
}

// Called when the game starts or when spawned
void AHammerHandle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHammerHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

