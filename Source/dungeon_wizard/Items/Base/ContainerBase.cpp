// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerBase.h"

AContainerBase::AContainerBase()
{
	Items.SetNum(0);
	MaxCapacity = 0;
	Capacity = 0;
	bReplicates = true;
	ParentContainer = nullptr;
}

void AContainerBase::BeginPlay()
{
	Capacity = MaxCapacity;
}

void AContainerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AContainerBase, Items);
	DOREPLIFETIME(AContainerBase, ParentContainer);
	DOREPLIFETIME(AContainerBase, Capacity);

}


void AContainerBase::AddItemToInventory(AItem* Item)
{
	Items.Add(Item);
	Capacity -= Item->Weight;
	Weight += Item->Weight;
	if (ParentContainer)
	{
		ParentContainer->Capacity -= Item->Weight;
		ParentContainer->Weight += Item->Weight;
	}
	AContainerBase* Container = Cast<AContainerBase>(Item);
	if (Container)
	{
		Container->ParentContainer = this;
	}
}

void AContainerBase::RemoveItemFromInventory(AItem* Item)
{
	Items.Remove(Item);
	Capacity += Item->Weight;
	Weight -= Item->Weight;
	if (ParentContainer)
	{
		ParentContainer->Capacity += Item->Weight;
		ParentContainer->Weight -= Item->Weight;
	}
	AContainerBase* Container = Cast<AContainerBase>(Item);
	if (Container)
	{
		Container->ParentContainer = nullptr;
	}
}

bool AContainerBase::CanContain(AItem* Item)
{
	if ((CheckIfContains(Item) || Capacity >= Item->Weight) && CheckParentContainer(Item))
	{
		AContainerBase* Container = Cast<AContainerBase>(Item);
		if (Container)
		{
			return CheckForInventoryLoops(Container, this);
		}
		return true;
	}
	return false;
}

bool AContainerBase::CheckIfContains(AItem* Item)
{
	if (Items.Contains(Item))
	{
		return true;
	}
	for (int i = 0; i < Items.Num(); i++)
	{
		AContainerBase* Container = Cast<AContainerBase>(Items[i]);
		if (Container)
		{
			if (Container->CheckIfContains(Item))
			{
				return true;
			}
		}
	}
	return false;
}

bool AContainerBase::CheckParentContainer(AItem* Item)
{
	if (ParentContainer)
	{
		if (ParentContainer->Items.Contains(Item))
		{
			return true;
		}
		return (ParentContainer->Capacity >= Item->Weight && ParentContainer->CheckParentContainer(Item));
	}
	return true;
}

bool AContainerBase::CheckForInventoryLoops(AContainerBase* Container, AContainerBase* MyContainer)
{
	if (Container != MyContainer)
	{
		for (int i = 0; i < Container->Items.Num(); i++)
		{
			AContainerBase* Container2 = Cast<AContainerBase>(Container->Items[i]);
			if (Container2)
			{
				if (!CheckForInventoryLoops(Container2, MyContainer))
				{
					return false;
				}
			}
		}
		return true;
	}
	return false;
}
