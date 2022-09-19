// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

#include "alchemik/mixtures/resources.h"

#include "Item.generated.h"

/*class Uclass
{
public:
	static int class_cnt;
	Uclass();
	int class_id;
};*/

/*UENUM(BlueprintType)
enum class ElementType : uint8
{
	ID_KAMIEN = 0 UMETA(DisplayName = "ID_KAMIEN"),
	ID_NOZ_KAMIENNY = 1 UMETA(DisplayName = "ID_NOZ")
};*/

/*
USTRUCT()
struct FMoje
{
	GENERATED_BODY()

	UPROPERTY()
		int a;
	int b;
	Resource * Det;
};*/

UCLASS()
class DUNGEON_WIZARD_API AItem : public AActor
{
	GENERATED_BODY()
	
	

public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProperty) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UWidgetComponent* WidgetComponent;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;

	UFUNCTION(BlueprintCallable)
	void SetDefaults(int Quantity);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ResourceType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int ResourceId;


	UPROPERTY(BlueprintReadOnly)
		bool CanBePickedUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		float Weight;
	//Element_type t;

	//UPROPERTY(EditAnywhere)
	//	TMap<Element_type, int> MyMap;

	Resource * Detail;
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int GetQuantity();
	
	UFUNCTION()
		void MySetActorEnableCollision(bool Enabled, FVector Direction);
	UFUNCTION()
		void MyAddImpulse(FVector Direction);
	UPROPERTY(ReplicatedUsing=OnRep_PhysicsEnabled)
		bool PhysicsEnabled;
	UFUNCTION()
		void OnRep_PhysicsEnabled();



};
