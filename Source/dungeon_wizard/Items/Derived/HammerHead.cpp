// Fill out your copyright notice in the Description page of Project Settings.


#include "HammerHead.h"

#include "Hammer.h"


// Sets default values
AHammerHead::AHammerHead()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PartOf = AHammer::StaticClass();
	ResourceType = FString(TEXT("glowka mlotka"));
//	MixFunction = mix_Glowka_mlotka;
}

// Called when the game starts or when spawned
void AHammerHead::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHammerHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

