// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
/*
Uclass::Uclass()
{
	class_id = class_cnt++;
	//classesTable[class_id].uclass = this;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%d "), class_id));
	}
}
*/
extern int game_started;
// Sets default values
/*int foofoo(const TCHAR* c, int b)
{
	const wchar_t *w = c;
	FString f = "BP_Stone2";
	FString f1 = "BP_Stone3";
	FString f2 = "BP_Stone4";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("%s "), w));
		b+=3;
	}
	else return -1;
	if (!wcscmp(w, *f)) b += 10;
	if (!wcscmp(w, *f1)) b += 100;
	if (!wcscmp(w, *f2)) b += 200;

	for (int i = 0; i < 10; i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("%d %d"), w[i], f.GetCharArray()[i]));
	}

	b+=2;
	return b;
}
*/
AItem::AItem()
{
	//create_elements();
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	PhysicsEnabled = true;
	ResourceType = TEXT("not defined");
	ResourceId = -1;
	CanBePickedUp = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component lol"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetIsReplicated(true);
	RootComponent = StaticMesh;
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	//StaticMesh->SetupAttachment(RootComponent);
	//StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	WidgetComponent->SetupAttachment(StaticMesh);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	if (game_started)
	{
		//ResourceType = TEXT("lalala");
		if (GEngine)
		{
		/*	FName name = this->GetFName();
			FString s = name.GetPlainNameString();
			
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, s);
		//	int b = foofoo(*s, 1);  
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%d "), b));
			size_t cnt;
			char n[128];
			size_t size = 2 * (s.Len() + 1);
			wcstombs_s(&cnt, n, size, *s, size - 1);
			Detail = new Resource(n, 1);
			classesTable[1].resource = Detail; 
			Detail->ue = this->GetClass();*/
		}
//		auto ResourceTypeAnsi = StringCast<ANSICHAR>(*ResourceType);
		
	}
}



// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("actor %s"), FString::Printf("test"));
	if (GEngine)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, ResourceType);
	}
	if (HasAuthority())
	{
	//	MySetActorEnableCollision(false);
	}
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, PhysicsEnabled);
	DOREPLIFETIME(AItem, Weight);
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GEngine && HasAuthority())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("%s"), CollisionEnabled ? TEXT("true") : TEXT("false")));
	}
	else if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("%s"), CollisionEnabled ? TEXT("true") : TEXT("false")));
	}
}

void AItem::SetDefaults(int Quantity)
{
}

int AItem::GetQuantity()
{
	return -1;
}

void AItem::MySetActorEnableCollision(bool Enabled, FVector Direction)
{
	//CollisionEnabled = Enabled;
	//SetActorEnableCollision(CollisionEnabled);
	PhysicsEnabled = Enabled;
	StaticMesh->SetSimulatePhysics(Enabled);
	if (Enabled)
	{
		FTimerHandle UnusedHandle;
		FTimerDelegate MyDelegate = FTimerDelegate::CreateUObject(this, &AItem::MyAddImpulse, Direction * 500 * StaticMesh->GetMass());
		GetWorldTimerManager().SetTimer(UnusedHandle, MyDelegate, 0.01, false);
	}
	
	//StaticMesh->SetSimulatePhysics(Enabled);
	if (GEngine)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%s"), Enabled ? TEXT("true") : TEXT("false")));
	}
}

void AItem::MyAddImpulse(FVector Direction)
{
	StaticMesh->AddImpulse(Direction);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("bum")));
	}
}

void AItem::OnRep_PhysicsEnabled()
{
	StaticMesh->SetSimulatePhysics(PhysicsEnabled);
	/*if (HasAuthority())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("updated")));
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("updated")));
	}*/
	//SetActorEnableCollision(CollisionEnabled);
	
}