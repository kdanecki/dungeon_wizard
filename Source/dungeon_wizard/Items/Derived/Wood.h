// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Base/ResourceBase.h"
#include "Wood.generated.h"

UCLASS()
class DUNGEON_WIZARD_API AWood : public AResourceBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
