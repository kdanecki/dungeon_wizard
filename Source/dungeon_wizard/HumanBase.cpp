// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanBase.h"
#include "alchemik/player.h"
#include <string>

// Sets default values
AHumanBase::AHumanBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//UpdateWidget = 0;

	Skills.Carrying = 100;
	Skills.WoodCutting = 10;
	Skills.StoneGathering = 5;
	Skills.Mining = 5;
	Skills.Building = 25;
	Skills.Fighting = 10;
	Skills.FruitGathering = 20;
	Skills.Foraging = 10;
	Skills.Farming = 10;

	InventoryWeight = 0;

	Hotbar.SetNum(5);
}
/*
const char * bo(const char * foo, const char * oo)
{
	return foo;
}*/
// Called when the game starts or when spawned
void AHumanBase::BeginPlay()
{
	Super::BeginPlay();


	

	if (GEngine)
	{
//		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("bla bla %f"), Skills.Carrying));
	}
	//auto s = bo("kamieñ", "jjj");
	//int a = strlen(s);
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%d"), a));	
	}
}

void AHumanBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHumanBase, RightHand);
	DOREPLIFETIME(AHumanBase, LeftHand);
	DOREPLIFETIME(AHumanBase, Clothes);
	DOREPLIFETIME(AHumanBase, Hotbar);
	DOREPLIFETIME(AHumanBase, InventoryWeight);


}


// Called every frame
void AHumanBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHumanBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHumanBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHumanBase::MoveRight);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AHumanBase::LookHorizontal);
	PlayerInputComponent->BindAxis("LookVertical", this, &AHumanBase::LookVertical);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AHumanBase::Action);
	PlayerInputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AHumanBase::SecondaryAction);
	PlayerInputComponent->BindAction("Tool0", IE_Pressed, this, &AHumanBase::RightSelectTool0);
	PlayerInputComponent->BindAction("Tool1", IE_Pressed, this, &AHumanBase::RightSelectTool1);
	PlayerInputComponent->BindAction("Tool2", IE_Pressed, this, &AHumanBase::RightSelectTool2);
	PlayerInputComponent->BindAction("Tool3", IE_Pressed, this, &AHumanBase::RightSelectTool3);
	PlayerInputComponent->BindAction("Tool4", IE_Pressed, this, &AHumanBase::RightSelectTool4);
	PlayerInputComponent->BindAction("LTool0", IE_Pressed, this, &AHumanBase::LeftSelectTool0);
	PlayerInputComponent->BindAction("LTool1", IE_Pressed, this, &AHumanBase::LeftSelectTool1);
	PlayerInputComponent->BindAction("LTool2", IE_Pressed, this, &AHumanBase::LeftSelectTool2);
	PlayerInputComponent->BindAction("LTool3", IE_Pressed, this, &AHumanBase::LeftSelectTool3);
	PlayerInputComponent->BindAction("LTool4", IE_Pressed, this, &AHumanBase::LeftSelectTool4);
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &AHumanBase::OpenInventory);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Talk", IE_Pressed, this, &AHumanBase::Talk);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AHumanBase::Drop);
	PlayerInputComponent->BindAction("Drop2", IE_Pressed, this, &AHumanBase::Drop2);
	//PlayerInputComponent->BindAction("Mix", IE_Pressed, this, &AHumanBase::Drop);
}


void AHumanBase::RightPickUp_Implementation(AItem* Item)
{
	if (Item->CanBePickedUp)
	{
		InventoryWeight += Item->Weight;
		RightHand = Item;
		Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Item->SetActorRelativeLocation(FVector(0, 100, 0));
		Item->MySetActorEnableCollision(false);
		Item->CanBePickedUp = false;
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("can't pickup")));
	}
}

void AHumanBase::LeftPickUp_Implementation(AItem* Item)
{
	if (Item->CanBePickedUp)
	{
		InventoryWeight += Item->Weight;
		LeftHand = Item;
		Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Item->SetActorRelativeLocation(FVector(0, -100, 0));
		Item->MySetActorEnableCollision(false);
		Item->CanBePickedUp = false;
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("can't pickup")));
		}
	}
}

void AHumanBase::RightDrop_Implementation()
{
	if (RightHand)
	{
		InventoryWeight -= RightHand->Weight;
		RightHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		RightHand->MySetActorEnableCollision(true);
		RightHand->CanBePickedUp = true;
		RightHand = nullptr;
	}
}

void AHumanBase::LeftDrop_Implementation()
{
	if (LeftHand)
	{
		InventoryWeight -= LeftHand->Weight;
		LeftHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		LeftHand->MySetActorEnableCollision(true);
		LeftHand->CanBePickedUp = true;
		LeftHand = nullptr;
	}
}

/*void AHumanBase::MoveToRightHand_Implementation(AItem* Item, AContainerBase* OldContainer)
{
	OldContainer->RemoveItemFromInventory(Item);
	RightHand = Item;
	Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Item->SetActorRelativeLocation(FVector(0, 100, 0));
}

void AHumanBase::MoveToLeftHand_Implementation(AItem* Item, AContainerBase* OldContainer)
{
	OldContainer->RemoveItemFromInventory(Item);
	LeftHand = Item;
	Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Item->SetActorRelativeLocation(FVector(0, -100, 0));
}

void AHumanBase::MoveToContainerFromRightHand_Implementation(AItem* Item, AContainerBase* Container)
{
	Container->AddItemToInventory(Item);
	RightHand = nullptr;
	Item->AttachToActor(Container, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AHumanBase::MoveToContainerFromLeftHand_Implementation(AItem* Item, AContainerBase* Container)
{
	Container->AddItemToInventory(Item);
	LeftHand = nullptr;
	Item->AttachToActor(Container, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}*/

void AHumanBase::ChangeItemContainer_Implementation(AItem* Item, int NewId, AContainerBase* NewContainer, int OldId, AContainerBase* OldContainer)
{
	if (OldId == 0)
	{
		OldContainer->RemoveItemFromInventory(Item);
		//Clothes.Updater = !Clothes.Updater;
	}
	else if (OldId == 1)
	{
		RightHand = nullptr;
	}
	else if (OldId == 2)
	{
		LeftHand = nullptr;
	}
	else if (OldId >= 3)
	{
		Clothes.Remove(Cast<AClothes>(Item));
	}
	if (NewId == 0)
	{
		NewContainer->AddItemToInventory(Item);
		Item->AttachToActor(NewContainer, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		if (AContainerBase* Container = Cast<AContainerBase>(Item))
		{
			RemoveFromHotbar(Container);
		}
		//Clothes.Updater = !Clothes.Updater;
	}
	else if (NewId == 1)
	{
		RightHand = Item;
		Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Item->SetActorRelativeLocation(FVector(0, 100, 0));
		//Clothes.Updater = !Clothes.Updater;
		if (AContainerBase* Container = Cast<AContainerBase>(Item))
		{
			RemoveFromHotbar(Container);
		}
	}
	else if (NewId == 2)
	{
		LeftHand = Item;
		Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Item->SetActorRelativeLocation(FVector(0, -100, 0));
		//Clothes.Updater = !Clothes.Updater;
		if (AContainerBase* Container = Cast<AContainerBase>(Item))
		{
			RemoveFromHotbar(Container);
		}
	}
	else if (NewId >= 3)
	{
		Equip(Cast<AClothes>(Item));
	}
	//UpdateWidget = !UpdateWidget;
	//UpdateInventoryWidget(1);
}

void AHumanBase::Equip_Implementation(AClothes* Item)
{
	switch (Item->BodyPart)
	{
	case EBodyPart::HEAD:
		Clothes.Head.Add(Item);
		break;
	case EBodyPart::BODY:
		Clothes.Body.Add(Item);
		break;
	case EBodyPart::LEGS:
		Clothes.Legs.Add(Item);
		break;
	case EBodyPart::FEET:
		Clothes.Feet.Add(Item);
		break;
	case EBodyPart::HANDS:
		Clothes.Hands.Add(Item);
		break;
	default:
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, FString::Printf(TEXT("NO PART")));
		}
		break;
	}
	Item->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Item->SetActorRelativeLocation(FVector(-100, 0, 0));
}

void AHumanBase::Drop2()
{
	LeftDrop();
}

void AHumanBase::Drop()
{
	RightDrop();
	/*if (IsValid(SelectedTool))
	{
		InventoryWeight -= SelectedTool->Weight;
		SelectedTool->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		SelectedTool->MySetActorEnableCollision(true);
		SelectedTool = nullptr;
	}
	else
	{
		for (int i = Resources.Num() - 1; i >= 0; i--)
		{
			InventoryWeight -= Resources[i]->Quantity * Resources[i]->WeightPerQuantity;
			Resources[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Resources[i]->MySetActorEnableCollision(true);
			Resources.RemoveAt(i);
		}
	}*/
}

void AHumanBase::RightSelectTool(int Index)
{
	if (IsValid(RightHand) || IsValid(Hotbar[Index]))
	{
		RightSelectServer(Index);
	}
}

void AHumanBase::RightSelectServer_Implementation(int Index)
{
	if (IsValid(Hotbar[Index]))
	{
		if (!IsValid(RightHand) && Hotbar[Index]->Items.Num() > 0)
		{
			RightHand = Hotbar[Index]->Items[Hotbar[Index]->Items.Num() - 1];
			Hotbar[Index]->RemoveItemFromInventory(Hotbar[Index]->Items[Hotbar[Index]->Items.Num() - 1]);
		}
		else if (IsValid(RightHand))
		{
			Hotbar[Index]->AddItemToInventory(RightHand);
			RightHand = nullptr;
		}
	}
	else 
	{
		if (AContainerBase* Container = Cast<AContainerBase>(RightHand))
		{
			Equip(Container);
			RightHand = nullptr;
			Hotbar[Index] = Container;
		}
	}
}

void AHumanBase::LeftSelectTool(int Index)
{
	if (IsValid(LeftHand) || IsValid(Hotbar[Index]))
	{
		LeftSelectServer(Index);
	}
}

void AHumanBase::LeftSelectServer_Implementation(int Index)
{
	if (IsValid(Hotbar[Index]))
	{
		if (!IsValid(LeftHand) && Hotbar[Index]->Items.Num() > 0)
		{
			LeftHand = Hotbar[Index]->Items[Hotbar[Index]->Items.Num() - 1];
			Hotbar[Index]->RemoveItemFromInventory(Hotbar[Index]->Items[Hotbar[Index]->Items.Num() - 1]);
		}
		else if (IsValid(LeftHand))
		{
			Hotbar[Index]->AddItemToInventory(LeftHand);
			LeftHand = nullptr;
		}
	}
	else
	{
		if (AContainerBase* Container = Cast<AContainerBase>(LeftHand))
		{
			Equip(Container);
			LeftHand = nullptr;
			Hotbar[Index] = Container;
		}
	}
}

void AHumanBase::RightSelectTool0()
{
	RightSelectTool(0);
}

void AHumanBase::RightSelectTool1()
{
	RightSelectTool(1);
}

void AHumanBase::RightSelectTool2()
{
	RightSelectTool(2);
}

void AHumanBase::RightSelectTool3()
{
	RightSelectTool(3);
}

void AHumanBase::RightSelectTool4()
{
	RightSelectTool(4);
}

void AHumanBase::LeftSelectTool0()
{
	LeftSelectTool(0);
}

void AHumanBase::LeftSelectTool1()
{
	LeftSelectTool(1);
}

void AHumanBase::LeftSelectTool2()
{
	LeftSelectTool(2);
}

void AHumanBase::LeftSelectTool3()
{
	LeftSelectTool(3);
}

void AHumanBase::LeftSelectTool4()
{
	LeftSelectTool(4);
}

void AHumanBase::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
}

void AHumanBase::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
}

void AHumanBase::LookHorizontal(float value)
{
	AddControllerYawInput(value);

}
// 	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT(" %f "), value));

void AHumanBase::LookVertical(float value)
{
	AddControllerPitchInput(value * -1);
}

void AHumanBase::Action()
{
	AItem* Item = Cast<AItem>(LookingAt);
	if (Item)
	{
		if (Skills.Carrying >= InventoryWeight + Item->Weight)
		{
			if (!IsValid(RightHand))
			{
				RightPickUp(Item);
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("already carrying")));
				}
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("too heavy")));
			}
		}
	}
}

void AHumanBase::SecondaryAction()
{
	AItem* Item = Cast<AItem>(LookingAt);
	if (Item)
	{
		if (Skills.Carrying >= InventoryWeight + Item->Weight)
		{
			if (!IsValid(LeftHand))
			{
				LeftPickUp(Item);
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("already carrying")));
				}
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("too heavy")));
			}
		}
	}

}



void AHumanBase::Talk()
{

}

void AHumanBase::OpenInventory()
{
	CreateInventoryWidget();
	/*for (int i = 0; i < Clothes.Body.Num(); i++)
	{
		if (GEngine && IsValid(Clothes.Body[i]))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, (TEXT("item ") + Clothes.Body[i]->GetActorLabel()));
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("invalid")));
			}
		}
	}*/
	/*if (Resources.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("nothing"));
	}
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AItem>(classesTable[1].resource->ue, GetActorLocation() + FVector(50, 0, 0), GetActorRotation(), SpawnParams);
	/*for (int i = 0; i < Resources.Num(); i++)
	{
		if (Resources[i])
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, Resources[i]->ItemType + ": " + FString::SanitizeFloat(Resources[i]->Quantity));
		}
	}
//	Uclass *u = new Uclass();
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d %d"), Resources[0]->class_id, Resources[1]->class_id));
	}*/
	/*Player *p = new Player();
	FActorSpawnParameters SpawnParams;
	if (Resources.Num() == 2)
	{
		Resource *r = nullptr;// mix(Resources[0]->Detail, Resources[1]->Detail, p->skills);
		if (r)
		{
			AItem* Item = GetWorld()->SpawnActor<AItem>(r->ue, GetActorLocation() + GetActorForwardVector() * 100, GetActorRotation(), SpawnParams);
			Item->Detail = r;
			if (Resources[1]->Detail->quantity == 0)
			{
				Resources[1]->Destroy();
				Resources.RemoveAt(1);

			}
			if (Resources[0]->Detail->quantity == 0)
			{
				Resources[0]->Destroy();
				Resources.RemoveAt(0);
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("failed to craft")));
			}
		}
	}
	else if (Resources.Num() == 1 && IsValid(Tools[0]))
	{
		Resource *r = nullptr;// mix(Resources[0]->Detail, Tools[0]->Detail, p->skills);
		if (r)
		{
			AItem* Item = GetWorld()->SpawnActor<AItem>(r->ue, GetActorLocation() + GetActorForwardVector() * 100, GetActorRotation(), SpawnParams);
			Item->Detail = r;
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("can't craft")));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("too many / too few items to craft")));
		}
	}
	*/
}

/*void AHumanBase::OnRep_UpdateWidget()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("on rep")));
	}
	//UpdateInventoryWidget(0);
}*/

void AHumanBase::Craft(TArray<AItem*> Items, FString str)
{
	Player *p = new Player();
	FActorSpawnParameters SpawnParams;
	Resource** res_table = (Resource**) calloc(Items.Num(), sizeof(Resource*));
	for (int i = 0; i < Items.Num(); i++)
	{
		res_table[i] = Items[i]->Detail;
	}
	if (Items.Num() == 2)
	{
		Result * r = new Result;
		r->res = nullptr;
		//FString str1 = FString(ANSI_TO_TCHAR("nó¿ kamienny"));
		int a = mix_skill(p->skills, TCHAR_TO_UTF8(*str), r, 2, res_table);
		if (r->res)
		{
			AItem* Item = GetWorld()->SpawnActor<AItem>(r->res->ue, GetActorLocation() + GetActorForwardVector() * 100, GetActorRotation(), SpawnParams);
			Item->Detail = r->res;
			//PickUp(Item);
			if (Items[1]->Detail->quantity == 0)
			{
				Items[1]->Destroy();
				Items.RemoveAt(1);

			}
			if (Items[0]->Detail->quantity == 0)
			{
				Items[0]->Destroy();
				Items.RemoveAt(0);
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("failed to craft")));
			}
		}
	}
	/*else if (Items.Num() == 1 && IsValid(Tools[0]))
	{
		Resource *r = nullptr;// mix(Items[0]->Detail, Tools[0]->Detail, p->skills);
		if (r)
		{
			AItem* Item = GetWorld()->SpawnActor<AItem>(r->ue, GetActorLocation() + GetActorForwardVector() * 100, GetActorRotation(), SpawnParams);
			Item->Detail = r;
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("can't craft")));
			}
		}
	}*/
	else if (Items.Num() > 2)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("too many items to craft")));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("too few items to craft")));
		}
	}
}

void AHumanBase::AddToHotbar_Implementation(AContainerBase* Container, int Index)
{
	for (int i = 0; i < Hotbar.Num(); i++)
	{
		if (Hotbar[i] == Container)
		{
			Hotbar[i] = nullptr;
		}
	}
	Hotbar[Index] = Container;
}

void AHumanBase::RemoveFromHotbar_Implementation(AContainerBase* Container)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("removing")));
	}
	for (int i = 0; i < Hotbar.Num(); i++)
	{
		if (Hotbar[i] == Container)
		{
			Hotbar[i] = nullptr;
		}
	}
}
