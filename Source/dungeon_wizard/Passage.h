// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"

#include "Passage.generated.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		USplineMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
		UStaticMesh* FloorMesh;
	UPROPERTY(EditAnywhere)
		UMaterialInterface* FloorMaterial;
	//void CreatePassage(FVector EndWorldLocation, FVector EndDirection);
};
