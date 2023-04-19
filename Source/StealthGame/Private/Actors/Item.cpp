// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item.h"
#include "Components/StaticMeshComponent.h"
#include "StealthGame/StealthGameCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::Interact(AStealthGameCharacter* Character)
{
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("INTERACTION : %s"), *GetName());
	}
}

//// Called every frame
//void AItem::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

