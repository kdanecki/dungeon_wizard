// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Base/ToolPart.h"
#include "PartStats.h"
#include "ToolHandle.generated.h"

UCLASS()
class DUNGEON_WIZARD_API AToolHandle : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AToolHandle();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FPartStats Stats;
};
