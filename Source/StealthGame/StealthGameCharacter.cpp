// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/InteractableInterface.h"

//////////////////////////////////////////////////////////////////////////
// AStealthGameCharacter

AStealthGameCharacter::AStealthGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	InteractionDistance = 500.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStealthGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AStealthGameCharacter::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStealthGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStealthGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AStealthGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AStealthGameCharacter::LookUpAtRate);
}


void AStealthGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStealthGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Fonction qui va être appelée lors de l'interaction avec les objets
void AStealthGameCharacter::Interact()
{
	FVector Start = GetFollowCamera()->GetComponentLocation();	// point de départ de la "line trace" d'interaction
	FVector End = Start + GetFollowCamera()->GetForwardVector() * InteractionDistance;	// point d'arrivée de la "line trace" d'interaction
	FHitResult HitResult;	// premier point d'intersection entre le rayon et l'objet

	if (PerformLineTrace(HitResult, Start, End, true))
	{
		UE_LOG(LogTemp, Warning, TEXT("LA LINE TRACE A TRAVERSE QUELQUE CHOSE"))	// permet d'afficher un message lors de l'intersection entre le rayon
																					// et un objet dans le Message log
		if (IInteractableInterface* Interface = Cast<IInteractableInterface>(HitResult.GetActor())) // on récupère l'objet auquel appartient le point
																								   // d'intersection avec le rayon tracé
		{
			Interface->Interact(this);
		}
	}	
}

bool AStealthGameCharacter::PerformLineTrace(FHitResult& HitResultParam, FVector StartParam, FVector EndParam, bool DrawDebug)
{
	if (DrawDebug)	// si ce paramètre est mis à true, alors on trace la ligne pour le débogage
	{
		DrawDebugLine(GetWorld(), StartParam, EndParam, FColor::Red, false, 2.0f, 0, 2.0f);		// trace une ligne (pour le débogage) symbolisant le rayon
	}

	FCollisionQueryParams Params;	// paramètres additionnels utilisés par la trace
	Params.AddIgnoredActor(this);	// on ignore le personnage (joueur)

	return GetWorld()->LineTraceSingleByChannel(HitResultParam, StartParam, EndParam, ECollisionChannel::ECC_WorldStatic, Params);
}

void AStealthGameCharacter::AddItemToInventory(AActor* Item)
{
	if (Item)
	{
		Inventory.Add(Item);

		UE_LOG(LogTemp, Warning, TEXT("L'ITEM A ETE AJOUTE A L'INVENTAIRE"))	// permet d'afficher un message indiquant que l'item a été mis dans l'inventaire
	}
}

void AStealthGameCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AStealthGameCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
