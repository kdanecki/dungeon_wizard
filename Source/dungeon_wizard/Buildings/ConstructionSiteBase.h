// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBase.h"
#include "../Items/Base/Tool.h"

#include "ConstructionSiteBase.generated.h"

USTRUCT()
struct FRequiredResource
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FString ItemType;
	UPROPERTY(EditAnywhere)
		float Quantity;
	UPROPERTY(EditAnywhere)
		float AddedWork;
};


UCLASS()
class DUNGEON_WIZARD_API AConstructionSiteBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConstructionSiteBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABuildingBase> Building;
	UPROPERTY(EditAnywhere)
	TArray<FRequiredResource> RequiredResources;
	UPROPERTY(EditAnywhere)
	float WorkToBeDone;
	UPROPERTY(EditAnywhere)
		TArray < TSubclassOf<ATool> > AcceptedTools;
	UPROPERTY(EditAnywhere)
		bool RequiresTools;

	float DeliverResource(FString ItemType, float Quantity);
	float WorkDone(float Skill, ATool* Tool);

};
