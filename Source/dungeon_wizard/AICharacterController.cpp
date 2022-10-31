// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterController.h"
#include "Faction.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAICharacterController::SetAction(EAIAction NewAction, AActor* Actor, FVector Location)
{
	Action = NewAction;
	UBlackboardComponent* MyBlackboard = GetBlackboardComponent();
	MyBlackboard->SetValueAsEnum(FName("Action"), (uint8)NewAction);
	switch (NewAction)
	{
	case EAIAction::FIGHT:
	{
		MyBlackboard->SetValueAsObject(FName("Enemy"), Actor);
		break;
	}
	case EAIAction::HUNT:
	{
		MyBlackboard->SetValueAsVector(FName("EnemyLocation"), Location);
		break;
	}
	case EAIAction::WANDER:
		break;
	case EAIAction::MIGRATE:
	{
		MyBlackboard->SetValueAsVector(FName("Location"), Location);
		break;
	}
	default:
		break;
	}
}
