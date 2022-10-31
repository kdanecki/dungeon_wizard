// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantResource.h"


AResourceBase* APlantResource::HandGather()
{
	if (ResourcesLeft > 0)
	{
		FActorSpawnParameters SpawnParams;
		AResourceBase* Resource = GetWorld()->SpawnActor<AResourceBase>(ResourceType, GetActorLocation() + FVector(0, 0, 200), GetActorRotation(), SpawnParams);
		ResourcesLeft--;
		UpdateMesh(FVector(0), ResourcesLeft);
		return Resource;
	}
	return nullptr;
}

void APlantResource::Grow()
{
	if (ResourcesLeft < MaxResources)
	{
		ResourcesLeft++;
		UpdateMesh(FVector(0), ResourcesLeft);
	}
}
