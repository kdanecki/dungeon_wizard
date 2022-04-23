// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "alchemik/Boo.h"

/**
 * 
 */

class Bar
{
public:
	Bar(float a);
	float b;
};

class DUNGEON_WIZARD_API Foo
{
public:
	Foo();
	~Foo();
	float a;
	Bar* bar1;
	Boo* booo;
};

