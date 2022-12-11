// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "AIHelpers.h"
#include "Items/Derived/Hammer.h"
#include "Items/Derived/HammerHandle.h"
#include "Items/Derived/HammerHead.h"

APlayerControllerBase::APlayerControllerBase()
{
	PlayerPawn = nullptr;
	CurrentRoom = 0;
	ActionMode = EAction::NONE;
	//KnownTools.Add(AHammer::StaticClass());
	//KnownTools.Append(AHammer::StaticClass()->GetDefaultObject<AHammer>()->RequiredParts);
}

void APlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerControllerBase, ActionMode)
	DOREPLIFETIME(APlayerControllerBase, PlayerPawn)
//	DOREPLIFETIME_CONDITION(APlayerControllerBase, Outcomes, COND_OwnerOnly)

}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerPawn = Cast<AHumanBase>(InPawn);
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &APlayerControllerBase::Action);
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &APlayerControllerBase::SecondaryAction);
	InputComponent->BindAction("SwitchModes", IE_Pressed, this, &APlayerControllerBase::SwitchModes);
	//InputComponent->BindAction("Craft", IE_Pressed, this, &APlayerControllerBase::Craft);
	
//s	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerControllerBase::SwitchModes);
}


//if (ATool* Tool = Cast<ATool>(Item))
//{
//	if (AHumanBase* Enemy = Cast<AHumanBase>(LookingAt))
//	{
//
//	}
//	if (ANaturalResource* Resource = Cast<ANaturalResource>(LookingAt))
//	{
//
//	}
//}


void APlayerControllerBase::Action()
{
	switch (ActionMode)
	{
	case EAction::NONE:
	{
		// item in hand
		if (ATool* Tool = Cast<ATool>(PlayerPawn->RightHand))
		{
			if (ANaturalResource* Resource = Cast<ANaturalResource>(PlayerPawn->LookingAt))
			{
				PlayerPawn->GatherAnimation(Resource, Tool);
			}
			else
			{
				PlayerPawn->AttackAnimation(nullptr, Tool);
			}
		}
		else if (AConsumable* Food = Cast<AConsumable>(PlayerPawn->RightHand))
		{
			PlayerPawn->Eat(Food);
			PlayerPawn->RightHand = nullptr;
		}
		// nothing in hand
		else if (!IsValid(PlayerPawn->RightHand))
		{
			if (ANaturalResource* Resource = Cast<ANaturalResource>(PlayerPawn->LookingAt))
			{
				if (AResourceBase* Item = Resource->HandGather())
				{
					PlayerPawn->RightPickUp(Item);
				}
			}
			else if (AItem* Item = Cast<AItem>(PlayerPawn->LookingAt))
			{
				if (PlayerPawn->UeSkills.Carrying >= PlayerPawn->InventoryWeight + Item->Weight)
				{
					if (!IsValid(PlayerPawn->RightHand))
					{
						PlayerPawn->RightPickUp(Item);
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
		break;
	}
	case EAction::CRAFT:
	{
		if (AItem* Item = Cast<AItem>(PlayerPawn->LookingAt))
		{
			ItemSelected(Item);
			/*if (SelectedItems.Contains(Item))
			{
				SelectedItems.Remove(Item);
				Item->WidgetComponent->SetVisibility(false);
				CheckOutcomes(SelectedItems);
			}
			else
			{
				SelectedItems.Add(Item);
				Item->WidgetComponent->SetVisibility(true);
				CheckOutcomes(SelectedItems);
			}*/
		}
		break;
	}
	default:
		break;
	}
}

void APlayerControllerBase::SecondaryAction()
{
	AItem* Item = Cast<AItem>(PlayerPawn->LookingAt);
	if (Item)
	{
		if (PlayerPawn->UeSkills.Carrying >= PlayerPawn->InventoryWeight + Item->Weight)
		{
			if (!IsValid(PlayerPawn->LeftHand))
			{
				PlayerPawn->LeftPickUp(Item);
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

void APlayerControllerBase::ItemSelected(AItem* Item)
{
	if (SelectedItems.Num() > 0)
	{
		if (Cast<AResourceBase>(SelectedItems[0]))
		{
			SelectedTools.Add(Item);
			UpdateWidget();
		}
		else if (Cast<AItemPart>(SelectedItems[0]))
		{
			if (AItemPart* Part = Cast<AItemPart>(Item))
			{
				if (RequiredParts.Contains(Part->GetClass()))
				{
					SelectedItems.Add(Part);
					RequiredParts.Remove(Part->GetClass());
					if (RequiredParts.Num() == 0)
					{
						CanCraft = true;
						UpdateWidget();
					}
				}
				else
				{
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("parts don't match"));
					}
				}
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("can't add material to part"));
				}
			}
			
		}
	}
	else
	{
		SelectedItems.Add(Item);
		if (AResourceBase* Resource = Cast<AResourceBase>(Item))
		{
			UpdateWidget();
		}
		else if (AItemPart* Part = Cast<AItemPart>(Item))
		{
			CraftingItem = Part->PartOf;
			RequiredParts = CraftingItem->GetDefaultObject<AToolBase>()->RequiredParts;
			RequiredParts.Remove(Part->GetClass());
			
			UpdateWidget();
		}
	}
}

void APlayerControllerBase::CraftItem(int Index, const TArray<AItemPart*>& ItemParts)
{
	FActorSpawnParameters SpawnParameters;
	AToolBase* Tool = GetWorld()->SpawnActor<AToolBase>(KnownTools[Index], PlayerPawn->GetActorLocation() + FVector(100, 0, 0), PlayerPawn->GetActorRotation(), SpawnParameters);
	Tool->Craft(ItemParts);
}

void APlayerControllerBase::CraftPart(int Index, AResourceBase* Resource)
{
	if (Skladnik* skladnik = KnownItemParts[Index]->GetDefaultObject<AItemPart>()->MixFunction(Resource->Detail))
	{
		FActorSpawnParameters SpawnParameters;
		AItemPart* Part = GetWorld()->SpawnActor<AItemPart>(KnownItemParts[Index], PlayerPawn->GetActorLocation() + FVector(100, 0, 0), PlayerPawn->GetActorRotation(), SpawnParameters);
		Part->Stats = FPartStats(skladnik->quality, skladnik->wytrzymalosc, skladnik->latwosc_uzycia);
		Part->Details = skladnik;
		Resource->Destroy();
	}
}

void APlayerControllerBase::SetActionMode_Implementation(EAction Mode)
{
	ActionMode = Mode;
}

void APlayerControllerBase::SwitchModes_Implementation()
{
	if (ActionMode == EAction::NONE)
	{
		ShowCraftingWidget();
		HideGameWidget();
		SetActionMode(EAction::CRAFT);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("craft")));
		}
	}
	else
	{
		HideCraftingWidget();
		ShowGameWidget();
		SetActionMode(EAction::NONE);
		for (int i = 0; i < SelectedItems.Num(); i++)
		{
			SelectedItems[i]->WidgetComponent->SetVisibility(false);
		}
		SelectedItems.Empty();
		RequiredParts.Empty();
		CanCraft = false;
		RequiredParts.Empty();
		CraftingItem = nullptr;
		SelectedTools.Empty();
		//Outcomes.Empty();
		//OnRep_Outcomes();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("none")));
		}
	}
}