// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Base/ItemPart.h"
#include "HammerHandle.generated.h"

UCLASS()
class DUNGEON_WIZARD_API AHammerHandle : public AItemPart
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHammerHandle();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
