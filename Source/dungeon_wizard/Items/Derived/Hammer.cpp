// Fill out your copyright notice in the Description page of Project Settings.


#include "Hammer.h"


// Sets default values
AHammer::AHammer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RequiredParts.Add(AHammerHandle::StaticClass());
	//RequiredParts.Add(AHammerHead::StaticClass());
	ResourceType = FString(TEXT("mlotek"));
}

// Called when the game starts or when spawned
void AHammer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AHammer::Craft(const TArray<AItemPart*>& Parts)
{
	if (Parts[0]->IsA(AHammerHead::StaticClass()))
	{
		Head = Parts[0]->Stats;
		Handle = Parts[1]->Stats;
	}
	else
	{
		Head = Parts[1]->Stats;
		Handle = Parts[0]->Stats;
	}
}


