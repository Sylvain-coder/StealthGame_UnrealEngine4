// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Door.h"
#include "Components/SkeletalMeshComponent.h"
#include "StealthGame/StealthGameCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));

	bOpen = false;	// par défaut la porte est fermée
	bUnlocked = false;	// par défaut la porte est verrouillée

	GlowEffectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GlowEffectBox"));
	GlowEffectBox->SetGenerateOverlapEvents(true);
	GlowEffectBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::EnteredObjectRadius);
	GlowEffectBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::LeftObjectRadius);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::Open()
{
	if (bOpen)
	{
		if (OpenAnimation)
		{
			MeshComponent->PlayAnimation(OpenAnimation, false);
		}
	}
	else
	{
		if (CloseAnimation)
		{
			MeshComponent->PlayAnimation(CloseAnimation, false);
		}
	}
}

void ADoor::Interact(AStealthGameCharacter* Character)
{
	if (Character)
	{
		if (!bUnlocked && LinkedKey)
		{
			for (AActor* Key : Character->GetInventory())
			{
				if (Key == LinkedKey)
				{
					bUnlocked = true;
					bOpen = !bOpen;  // bOpen passe de false à true ou de true à false (la porte s'ouvre ou se ferme et vice-versa)
					Open();
				}
			}
		}
		else
		{
			bOpen = !bOpen;  // bOpen passe de false à true ou de true à false (la porte s'ouvre ou se ferme et vice-versa)
			Open();
		}
		
	}
}


void ADoor::ToggleGlow(bool IsGlowing)
{
	MeshComponent->SetRenderCustomDepth(IsGlowing);
}

void ADoor::EnteredObjectRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->GetSuperClass()->GetName() == "StealthGameCharacter")
	{
		ToggleGlow(true);
	}
}

void ADoor::LeftObjectRadius(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->GetSuperClass()->GetName() == "StealthGameCharacter")
	{
		ToggleGlow(false);
	}
}

//// Called every frame
//void ADoor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

