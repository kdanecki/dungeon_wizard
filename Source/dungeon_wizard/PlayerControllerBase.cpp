// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

APlayerControllerBase::APlayerControllerBase()
{
	PlayerPawn = nullptr;
	ActionMode = EAction::NONE;
	src = nullptr;
	result = nullptr;
}

void APlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerControllerBase, ActionMode)
	DOREPLIFETIME(APlayerControllerBase, PlayerPawn)
	DOREPLIFETIME_CONDITION(APlayerControllerBase, Outcomes, COND_OwnerOnly)

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
			if (SelectedItems.Contains(Item))
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
			}
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

void APlayerControllerBase::SetActionMode_Implementation(EAction Mode)
{
	ActionMode = Mode;
}

void APlayerControllerBase::SwitchModes_Implementation()
{
	if (ActionMode == EAction::NONE)
	{
		SetActionMode(EAction::CRAFT);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("craft")));
		}
	}
	else
	{
		SetActionMode(EAction::NONE);
		for (int i = 0; i < SelectedItems.Num(); i++)
		{
			SelectedItems[i]->WidgetComponent->SetVisibility(false);
		}
		SelectedItems.Empty();
		Outcomes.Empty();
		OnRep_Outcomes();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("none")));
		}
	}
}

void APlayerControllerBase::CheckOutcomes_Implementation(const TArray<AItem*>& Items)
{
	Outcomes.Empty();
	Resource** res_table = (Resource**)calloc(Items.Num(), sizeof(Resource*));
	for (int i = 0; i < Items.Num(); i++)
	{
		res_table[i] = Items[i]->Detail;
	}
	if (src)
	{
		delete src;
	}
	if (result)
	{
		delete result;
	}
	src = new Mix_source(Items.Num(), res_table, PlayerPawn->player_skills, nullptr);
	result = new Mix_result();
	int a = mix(src, result);
	if (a == 2)
	{
		if (result->mix_count > 0)
		{
			for (int j = 0; j < result->mix_count; j++)
			{
				Outcomes.Add(result->mix_table[j]->res[0]->ue);
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("outcome")));
				}
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("this shouldn't happen")));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("can't craft")));
		}
	}
	OnRep_Outcomes();
}

void APlayerControllerBase::Craft_Implementation(int Index, const TArray<AItem*>& Items)
{
	if (Outcomes.Num() > Index)
	{
		src->mix = result->mix_table[Index];
		int b = mix(src, result);
		if (b == 0)
		{
			FActorSpawnParameters SpawnParams;
			for (int i = 0; i < result->res_count; i++)
			{
				AItem* Item = GetWorld()->SpawnActor<AItem>(result->res[i]->ue, PlayerPawn->GetActorLocation() + PlayerPawn->GetActorForwardVector() * 100, PlayerPawn->GetActorRotation(), SpawnParams);
				Item->Detail = result->res[i];
			}
			for (int i = 0; i < Items.Num(); i++)
			{
				if (Items[i]->Detail->props.quantity < 1)
				{
					delete Items[i]->Detail;
					Items[i]->Destroy();
				}
			}
		}
		else if (b ==1)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("waiting")));
			}
		}
	}
	Outcomes.Empty();
	OnRep_Outcomes();
	SwitchModes();
}

/*void APlayerControllerBase::ActionCraft()
{
	SetActionMode(EAction::CRAFT);
}*/
