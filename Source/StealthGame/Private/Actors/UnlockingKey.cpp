// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/UnlockingKey.h"
#include "Components/StaticMeshComponent.h"
#include "StealthGame/StealthGameCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AUnlockingKey::AUnlockingKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UnlockingKeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnlockingKey Mesh"));
	RootComponent = UnlockingKeyMesh;

	GlowEffectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GlowEffectBox"));
	GlowEffectBox->SetGenerateOverlapEvents(true);
	GlowEffectBox->OnComponentBeginOverlap.AddDynamic(this, &AUnlockingKey::EnteredObjectRadius);
	GlowEffectBox->OnComponentEndOverlap.AddDynamic(this, &AUnlockingKey::LeftObjectRadius);
}

// Called when the game starts or when spawned
void AUnlockingKey::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnlockingKey::Interact(AStealthGameCharacter* Character)
{
	if (Character)
	{
		Character->AddItemToInventory(this);
		UnlockingKeyMesh->SetHiddenInGame(true);
		UnlockingKeyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AUnlockingKey::ToggleGlow(bool IsGlowing)
{
	UnlockingKeyMesh->SetRenderCustomDepth(IsGlowing);
}

void AUnlockingKey::EnteredObjectRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->GetSuperClass()->GetName() == "StealthGameCharacter")
	{
		ToggleGlow(true);
	}

}

void AUnlockingKey::LeftObjectRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->GetSuperClass()->GetName() == "StealthGameCharacter")
	{
		ToggleGlow(false);
	}
}

//// Called every frame
//void AUnlockingKey::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

