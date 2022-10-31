// Fill out your copyright notice in the Description page of Project Settings.


#include "Faction.h"
#include "GameFramework/Pawn.h"


AFaction::AFaction()
{
	SetReplicates(true);

}

void AFaction::BeginPlay()
{
	FTimerHandle UnusedHandle;
	FTimerDelegate MyDelegate = FTimerDelegate::CreateUObject(this, &AFaction::Migrate);
	GetWorldTimerManager().SetTimer(UnusedHandle, MyDelegate, 20, true, 5);
}

void AFaction::ActorSpotted(AActor* Actor)
{
	if (APawn* Pawn = Cast<APawn>(Actor))
	{
		if (!Members.Contains(Pawn->GetController()))
		{
			for (AAICharacterController* Member : Members)
			{
				if (Member->Action == EAIAction::WANDER)
				{
					Member->SetAction(EAIAction::HUNT, nullptr, Pawn->GetActorLocation());
				}
			}
		}
	}
	
}

void AFaction::Migrate()
{
	bool CanMigrate = true;
	for (AAICharacterController* Member : Members)
	{
		if (Member->Action != EAIAction::WANDER)
		{
			CanMigrate = false;
			break;
		}
	}
	if (CanMigrate)
	{
		FVector Location = FindMigrateLocation();
		SetActorLocation(Location);
		for (AAICharacterController* Member : Members)
		{
			Member->SetAction(EAIAction::MIGRATE, nullptr, Location);
		}
	}
}
