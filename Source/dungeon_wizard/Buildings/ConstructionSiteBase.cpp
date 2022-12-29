// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructionSiteBase.h"

// Sets default values
AConstructionSiteBase::AConstructionSiteBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConstructionSiteBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConstructionSiteBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AConstructionSiteBase::DeliverResource(FString ItemType, float Quantity)
{
	for (int i = 0; i < RequiredResources.Num(); i++)
	{
		if (RequiredResources[i].ItemType == ItemType)
		{
			if (RequiredResources[i].Quantity <= Quantity)
			{
				float Used = RequiredResources[i].Quantity;
				WorkToBeDone += RequiredResources[i].AddedWork * Used;
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, RequiredResources[i].ItemType + ": " + FString::SanitizeFloat(Used));
				RequiredResources.RemoveAt(i);
				return Used;
			}
			else
			{
				RequiredResources[i].Quantity -= Quantity;
				WorkToBeDone += RequiredResources[i].AddedWork * Quantity;
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, RequiredResources[i].ItemType + ": " + FString::SanitizeFloat(Quantity));
				return Quantity;
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("no resource found"));
	return 0.0f;
}

float AConstructionSiteBase::WorkDone(float Skill, ATool* Tool)
{
	if (Tool)
	{
		if (AcceptedTools.Contains(Tool->GetClass()))
		{
			if (WorkToBeDone <= Skill)
			{
				if (RequiredResources.Num() == 0)
				{
					FActorSpawnParameters SpawnInfo;
					GetWorld()->SpawnActor<ABuildingBase>(Building, GetActorLocation(), GetActorRotation(), SpawnInfo);
					Destroy();
					return Skill;
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("deliver more resources"));
					return 0;
				}
			}
			else
			{
				WorkToBeDone -= Skill;
				return Skill;
			}
		}
	}
	else if (!RequiresTools)
	{
		if (WorkToBeDone <= Skill)
		{
			if (RequiredResources.Num() == 0)
			{
				FActorSpawnParameters SpawnInfo;
				GetWorld()->SpawnActor<ABuildingBase>(Building, GetActorLocation(), GetActorRotation(), SpawnInfo);
				Destroy();
				return Skill;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("deliver more resources"));
				return WorkToBeDone;
			}
		}
		else
		{
			WorkToBeDone -= Skill;
			return Skill;
		}
	}
	return -1;
}

