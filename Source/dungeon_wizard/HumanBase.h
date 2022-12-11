// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/Base/Tool.h"
#include "Items/Base/ResourceBase.h"
#include "Items/Base/Item.h"
#include "Items/Base/Clothes.h"
#include "Items/Base/ContainerBase.h"
#include "Items/Base/Consumable.h"
#include "NaturalResource.h"

#include "HumanBase.generated.h"


USTRUCT(BlueprintType)
struct FSkills
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		float Carrying;
	UPROPERTY(BlueprintReadWrite)
		float WoodCutting;
	UPROPERTY(BlueprintReadWrite)
		float StoneGathering;
	UPROPERTY(BlueprintReadWrite)
		float Mining;
	UPROPERTY(BlueprintReadWrite)
		float Building;
	UPROPERTY(BlueprintReadWrite)
		float Fighting;
	UPROPERTY(BlueprintReadWrite)
		float FruitGathering;
	UPROPERTY(BlueprintReadWrite)
		float Foraging;
	UPROPERTY(BlueprintReadWrite)
		float Farming;
};

USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly)
		TArray<AClothes*> Head;
	UPROPERTY(BlueprintReadOnly)
		TArray<AClothes*> Body;
	UPROPERTY(BlueprintReadOnly)
		TArray<AClothes*> Legs;
	UPROPERTY(BlueprintReadOnly)
		TArray<AClothes*> Feet;
	UPROPERTY(BlueprintReadOnly)
		TArray<AClothes*> Hands;
	//UPROPERTY()
		//bool Updater;
	void Remove(AClothes* Item)
	{
		Head.Remove(Item);
		Body.Remove(Item);
		Legs.Remove(Item);
		Feet.Remove(Item);
		Hands.Remove(Item);
	}
};

UCLASS()
class DUNGEON_WIZARD_API AHumanBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProperty) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent)
		void Die();
	// attributes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		float HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		float Nourishment;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		float MaxNourishment;
	UFUNCTION()
		void ManageHunger();

	// skills
	UPROPERTY(BlueprintReadWrite)
		FSkills UeSkills;
	// Inventory

	UPROPERTY(Replicated)
		float InventoryWeight;


	UPROPERTY(BlueprintReadOnly, Replicated)
		AItem* RightHand;
	UPROPERTY(BlueprintReadOnly, Replicated)
		AItem* LeftHand;
	UPROPERTY(BlueprintReadOnly, Replicated)
		FInventory Clothes;
	UPROPERTY(ReplicatedUsing=OnRep_IgnoreArray)
		TArray<TObjectPtr<AActor>> IgnoreArray;
	UFUNCTION()
		void OnRep_IgnoreArray();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void RightPickUp(AItem* Item);
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void LeftPickUp(AItem* Item);
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void RightDrop();
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void LeftDrop();

	UPROPERTY(BlueprintReadOnly, Replicated)
		TArray<AContainerBase*> Hotbar;
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void AddToHotbar(AContainerBase* Container, int Index);
	UFUNCTION(Server, Reliable)
		void RemoveFromHotbar(AContainerBase* Container);
	/*UFUNCTION(Server, Reliable, BlueprintCallable)
		void MoveToRightHand(AItem* Item, AContainerBase* OldContainer);
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void MoveToLeftHand(AItem* Item, AContainerBase* OldContainer);
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void MoveToContainerFromRightHand(AItem* Item, AContainerBase* Container);
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void MoveToContainerFromLeftHand(AItem* Item, AContainerBase* Container);*/
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void ChangeItemContainer(AItem* Item, int NewId, AContainerBase* NewContainer, int OldId, AContainerBase* OldContainer);
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Equip(AClothes* Item);
	//UPROPERTY()
		//TArray<>
	void Drop2();
	void Drop();
	
	
	void RightSelectTool(int Index);
	UFUNCTION(Server, Reliable)
		void RightSelectServer(int Index);
	void LeftSelectTool(int Index);
	UFUNCTION(Server, Reliable)
		void LeftSelectServer(int Index);
	void RightSelectTool0();
	void RightSelectTool1();
	void RightSelectTool2();
	void RightSelectTool3();
	void RightSelectTool4();
	void LeftSelectTool0();
	void LeftSelectTool1();
	void LeftSelectTool2();
	void LeftSelectTool3();
	void LeftSelectTool4();

	//movement
	virtual void MoveForward(float value);
	virtual void MoveRight(float value);
	virtual void LookHorizontal(float value);
	virtual void LookVertical(float value);

	// Actions
	UPROPERTY(BlueprintReadWrite)
		AActor* LookingAt;
	UPROPERTY(BlueprintReadWrite)
		FVector LookingAtLocation;

	UFUNCTION()
		void Action();
	UFUNCTION()
		void SecondaryAction();

	UFUNCTION(Server, Reliable)
		void UseItem(AItem* Item);
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Gather(ANaturalResource* Resource, ATool* Tool);
	UFUNCTION(BlueprintImplementableEvent)
		void GatherAnimation(ANaturalResource* Resource, ATool* Tool);
	UFUNCTION(Server, Reliable, BlueprintCallable)
		void Attack(AActor* Enemy, ATool* Weapon);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void AttackAnimation(AActor* Enemy, ATool* Weapon);
	UFUNCTION(Server, Reliable)
		void Eat(AConsumable* Food);

	virtual void Talk();
	//UFUNCTION(BlueprintCallable)


	//player only
	UFUNCTION(BlueprintImplementableEvent)
		void CreateInventoryWidget();
	/*UFUNCTION()
		void OnRep_UpdateWidget();
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateInventoryWidget(const int Id);*/
	UFUNCTION(BlueprintCallable)
		void Craft(TArray<AItem*> Items, FString str);
	void OpenInventory();

	

};
