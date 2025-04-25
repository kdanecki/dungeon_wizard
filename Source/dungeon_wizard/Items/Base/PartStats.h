#pragma once
#include "PartStats.generated.h"
USTRUCT(BlueprintType)
struct FPartStats
{
	GENERATED_BODY()
	FPartStats()
	{
		
	}
	FPartStats(int Quality, int Durability, int UseSpeed, int UsePrecision)
		: Quality(Quality),
		  Durability(Durability),
		  UseSpeed(UseSpeed),
		  UsePrecision(UsePrecision)
	{
	}

	UPROPERTY(BlueprintReadOnly)
	int Quality;
	UPROPERTY(BlueprintReadOnly)
	int Durability;
	UPROPERTY(BlueprintReadOnly)
	int UseSpeed;
	UPROPERTY(BlueprintReadOnly)
	int UsePrecision;

	FPartStats operator*(FPartStats other)
	{
		return FPartStats(this->Quality * other.Quality, this->Durability * other.Durability, this->UseSpeed * other.UseSpeed, this->UsePrecision * other.UsePrecision);
	}
};
