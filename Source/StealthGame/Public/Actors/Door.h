// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "Door.generated.h"

UCLASS()
class STEALTHGAME_API ADoor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "StealthGame Door")
	class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "StealthGame Door")
	class UAnimationAsset* OpenAnimation;

	UPROPERTY(EditAnywhere, Category = "StealthGame Door")
	class UAnimationAsset* CloseAnimation;

	bool bOpen;

	UFUNCTION()
	void Open();

	bool bUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StealthGame | Key");
	AActor* LinkedKey;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* GlowEffectBox;

public:
	virtual void Interact(class AStealthGameCharacter* Character) override;

	// Active le glow effect sur la clé
	void ToggleGlow(bool IsGlowing);

	UFUNCTION()
		void EnteredObjectRadius(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void LeftObjectRadius(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
