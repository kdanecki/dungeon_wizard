// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Passage.h"
#include "Blockade.h"

#include "PassagesSave.generated.h"

/**
 * 
 */

USTRUCT()
struct FPassageSave
{
	GENERATED_BODY()

	FPassageSave() {}
	FPassageSave(APassage* Passage)
	{
		PassageType = Passage->GetClass();
		Transform = Passage->GetActorTransform();
		MeshParams = Passage->MeshParams;
		Material = Passage->StaticMesh->GetMaterial(0);
	}
	UPROPERTY()
	TSubclassOf<APassage> PassageType;
	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	FMeshParams MeshParams;
	UPROPERTY()
	UMaterialInterface* Material;

};

USTRUCT()
struct FBlockadeSave
{
	GENERATED_BODY()

	FBlockadeSave() {}
	FBlockadeSave(ABlockade* Blockade)
	{
		BlockadeType = Blockade->GetClass();
		Transform = Blockade->GetActorTransform();
		Material = Blockade->Mesh->GetMaterial(0);
	}
	
	UPROPERTY()
	TSubclassOf<AActor> BlockadeType;
	UPROPERTY()
	FTransform Transform;
	UPROPERTY()
	UMaterialInterface* Material;
};

UCLASS()
class DUNGEON_WIZARD_API UPassagesSave : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FPassageSave> PassagesInfo;
	UPROPERTY()
	TArray<FBlockadeSave> BlockadesInfo;
	
};
