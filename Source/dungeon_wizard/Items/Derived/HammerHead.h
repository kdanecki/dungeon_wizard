// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Base/ToolPart.h"
#include "HammerHead.generated.h"

UCLASS()
class DUNGEON_WIZARD_API AHammerHead : public AToolPart
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHammerHead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
