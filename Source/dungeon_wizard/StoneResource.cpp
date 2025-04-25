// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneResource.h"

void AStoneResource::Gather(float Force, EToolType ToolType, FVector Location)
{
	if (EffectiveTools.Contains(ToolType))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("mined")));
		}
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AResourceBase>(ResourceType, Location, GetActorRotation(), SpawnParams);
		ResourcesLeft--;
		if (ResourcesLeft == 0)
		{
			Destroy();
		}
		else
		{
			UpdateMesh(Location, ResourcesLeft);
		}
	}
}