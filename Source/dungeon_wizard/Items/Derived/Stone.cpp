// Fill out your copyright notice in the Description page of Project Settings.


#include "Stone.h"
#include "../../mixtures/kamien.h"

AStone::AStone()
{
	Detail = new Kamien(100);
	ResourceType = FString(TEXT("kamien"));
}
