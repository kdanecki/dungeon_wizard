// Copyright Epic Games, Inc. All Rights Reserved.

//#pragma optimize("", off)
#pragma once
#include "dungeon_wizardGameModeBase.h"
#include "alchemik/mixtures/resources.h"
#include "alchemik/mixtures/mixture.h"
//#include "alchemik/player.h"
//#include "Item.h"

/*
class Uclass
{
public:
	static int class_cnt;
	Uclass();
	int class_id;
};*/

//int Uclass::class_cnt;
int game_started = 0;


void Adungeon_wizardGameModeBase::InitGame(const FString& MapName, const FString& Options, FString&ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	//Uclass::class_cnt = 0;
	game_started = 1;
	/*int count = 0;
	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(AItem::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
		{
			count++;
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, It->GetName());
			}
		}
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), count));
	}*/
}



void Adungeon_wizardGameModeBase::BeginPlay()
{
	Super::BeginPlay();
//	create_elements();
	elements = new Elements();
	mixtures = new Mixtures();

	create_mixtures();
	create_skills();

	resources = new Resources();
	for (int i = 0; i < MyResources.Num(); i++)
	{
		AItem * Item = Cast<AItem>(MyResources[i]->GetDefaultObject());
		Resource *r = new Resource((Element_type)(Item->ResourceId), -1);
		r->ue = MyResources[i];
		resources->add(r);
	}
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<ADungeonGenerator>(Generator, FVector(0, 0, 0), FRotator(0, 30, 0), SpawnParams);
	int a = 10;
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), a));
	}
}