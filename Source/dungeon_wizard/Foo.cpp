// Fill out your copyright notice in the Description page of Project Settings.


#include "Foo.h"

Bar::Bar(float a)
{
	b = a + 2;
}

Foo::Foo()
{
	a = 10;
	bar1 = new Bar(a);
	booo = new Boo(a);
}

Foo::~Foo()
{
}
