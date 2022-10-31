// Fill out your copyright notice in the Description page of Project Settings.


#include "NaturalResource.h"


// Sets default values
ANaturalResource::ANaturalResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void ANaturalResource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANaturalResource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANaturalResource::Gather(ATool* Tool, FVector Location)
{
}

AResourceBase* ANaturalResource::HandGather()
{
	return nullptr;
}

