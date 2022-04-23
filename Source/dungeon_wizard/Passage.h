// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"
#include "Net/UnrealNetwork.h"

#include "Passage.generated.h"

USTRUCT()
struct FMeshParams
{
	GENERATED_BODY()

	FMeshParams()
	{
		Start = FVector(0, 0, 0);
		StartTangent = FVector(0, 0, 0);
		End = FVector(0, 0, 0);
		EndTangent = FVector(0, 0, 0);
	}
	FMeshParams(FVector Start1, FVector StartTangent1, FVector End1, FVector EndTangent1)
	{
		Start = Start1;
		StartTangent = StartTangent1;
		End = End1;
		EndTangent = EndTangent1;
	}

	UPROPERTY()
		FVector Start;
	UPROPERTY()
		FVector StartTangent;
	UPROPERTY()
		FVector End;
	UPROPERTY()
		FVector EndTangent;
};


UCLASS()
class DUNGEON_WIZARD_API APassage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APassage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProperty) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		USplineMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		UStaticMesh* FloorMesh;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* FloorMaterial;

	UPROPERTY(ReplicatedUsing=OnRep_MeshParams)
		FMeshParams MeshParams;
	UFUNCTION()
		void OnRep_MeshParams();
	//void CreatePassage(FVector EndWorldLocation, FVector EndDirection);
};
