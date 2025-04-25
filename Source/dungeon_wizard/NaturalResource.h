// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
//#include "GameFramework/Actor.h"
#include "Items/Base/ResourceBase.h"
#include "Items/Base/Tool.h"

#include "NaturalResource.generated.h"

UCLASS()
class DUNGEON_WIZARD_API ANaturalResource : public ADynamicMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANaturalResource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterialInterface* Material;

	UPROPERTY(EditAnywhere)
		TArray<EToolType> EffectiveTools;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AResourceBase> ResourceType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ResourcesLeft;

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateMesh(FVector Location, int Left);
	virtual void Gather(float Force, EToolType ToolType, FVector Location);
	virtual AResourceBase* HandGather();
};
