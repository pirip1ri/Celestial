// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	SprintMultiplier = 1.5f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Store the default walk speed from the movement component
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller != nullptr)
	{
		if (FMath::Abs(Value) > DeadZoneThreshold) // Ignore small values
		{
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Controller != nullptr)
	{
		if (FMath::Abs(Value) > DeadZoneThreshold) // Ignore small values
		{
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			AddMovementInput(Direction, Value);
		}
	}
}

void APlayerCharacter::LookUp(float Value)
{
	if (Controller != nullptr)
	{
		AddControllerPitchInput(Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	if (Controller != nullptr)
	{
		AddControllerYawInput(Value);
	}
}

void APlayerCharacter::StartSprint()
{
	if (Controller != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * SprintMultiplier;
	}
}

void APlayerCharacter::StopSprint()
{
	if (Controller != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	}
}

void APlayerCharacter::StartCrouch()
{
	if (Controller != nullptr)
	{
		Crouch();
	}
}

void APlayerCharacter::StopCrouch()
{
	if (Controller != nullptr)
	{
		UnCrouch();
	}
}

void APlayerCharacter::Interact()
{
	if (Controller != nullptr)
	{
		// Perform a line trace or check nearby objects
		FHitResult HitResult;
		FVector Start = GetActorLocation(); // Start of the line trace is where the player is
		FVector End = Start + (GetActorForwardVector() * 100.f); // End point = 200m away from Start point in the direction the player is looking in

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // ignore self

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params)) // Is there something nearby?
		{
			//AInteractableObject* Interactable = Cast<AInteractableObject>(HitResult.GetActor()); // Is that something an interactable object?
			//if (Interactable) // If so, interact with it
			//{
			//	Interactable->InteractAbility();
			//}
			UE_LOG(LogTemp, Display, TEXT("Interact with object"));
		}
	}
}


