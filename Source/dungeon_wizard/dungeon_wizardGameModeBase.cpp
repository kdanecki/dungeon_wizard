// Copyright Epic Games, Inc. All Rights Reserved.

#pragma optimize("", off)
#include "dungeon_wizardGameModeBase.h"
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
	create_elements();
	for (int i = 0; i < Resources.Num(); i++)
	{
		AItem * Item = Cast<AItem>(Resources[i]->GetDefaultObject());
		FString s = Item->ResourceType;
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Blue, s);
		}
		
		//size_t cnt;
		//char n[128];
		//size_t size = 2 * (s.Len() + 1);
		//wcstombs_s(&cnt, n, size, *s, size - 1);
		char* c = TCHAR_TO_UTF8(*s);
		int a = strlen(c);
		//c = StringCast<>(*s);
		//int b = strlen(c);
		//c = TCHAR_TO_OEM(*s);
		//int d = strlen(c);
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, FString::Printf(TEXT("to utf 8 %d,"), a));
		}
		Resource *r = new Resource(c, -1);
		r->ue = Resources[i];
		//classesTable[i].resource = r;
		
	}
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<ADungeonGenerator>(Generator, FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	int a = 10;
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), a));
	}
}