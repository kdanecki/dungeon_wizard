// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Base/Tool.h"
#include "HammerHandle.h"
#include "HammerHead.h"

#include "Hammer.generated.h"

UCLASS()
class DUNGEON_WIZARD_API AHammer : public ATool
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHammer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Craft(const TArray<AItemPart*>& Parts) override;
	FPartStats Head;
	FPartStats Handle;
};
