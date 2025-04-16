// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"

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

	bCanDash = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Store the default walk speed from the movement component
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	if (InteractionPromptClass)
	{
		InteractionPromptInstance = CreateWidget<UInteractionPromptWidget>(GetWorld(), InteractionPromptClass);
		if (InteractionPromptInstance)
		{
			InteractionPromptInstance->AddToViewport();
		}
	}
	if (CollectableCounterClass)
	{
		CollectableCounterInstance = CreateWidget<UUserWidget>(GetWorld(), CollectableCounterClass);
		if (CollectableCounterInstance)
		{
			CollectableCounterInstance->AddToViewport();
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Don't forget this if you're overriding Tick

	FVector Start = RootComponent->GetComponentLocation() + RootComponent->GetComponentRotation().RotateVector(SweepStartOffset);
	FVector End = Start + (FollowCamera->GetForwardVector() * MaximumInteractionDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::FindBetweenVectors(FVector::UpVector, (End - Start).GetSafeNormal()),
		ECC_Visibility,
		FCollisionShape::MakeCapsule(InteractionRadius, MaximumInteractionDistance * 0.5f),
		Params
	);

	if (bToggleDebugVisualiser)
	{
		DrawDebugCapsule(
			GetWorld(),
			(Start + End) * 0.5f,
			MaximumInteractionDistance * 0.5f,
			InteractionRadius,
			FQuat::FindBetweenVectors(FVector::UpVector, (End - Start).GetSafeNormal()),
			FColor::Green,
			false,
			0.1f
		);
	}

	AInteractable* HitInteractable = bHit ? Cast<AInteractable>(HitResult.GetActor()) : nullptr;

	if (HitInteractable)
	{
		// Only update if it's a new interactable
		if (LastInteractable != HitInteractable)
		{
			LastInteractable = HitInteractable;

			if (InteractionPromptInstance)
			{
				InteractionPromptInstance->SetPromptText(HitInteractable->InteractionPromptText);
				InteractionPromptInstance->PlayFadeIn();
			}
		}
	}
	else if (LastInteractable) // We were looking at something, but not anymore
	{
		LastInteractable = nullptr;

		if (InteractionPromptInstance)
		{
			InteractionPromptInstance->PlayFadeOut();
		}
	}
}

void APlayerCharacter::Interact()
{
	if (Controller && LastInteractable)
	{
		UE_LOG(LogTemp, Display, TEXT("INTERACTTTTTT"));
		LastInteractable->InteractAbility();
	}
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
		AddControllerPitchInput(Value * LookUpSensitivity);
	}
}

void APlayerCharacter::Turn(float Value)
{
	if (Controller != nullptr)
	{
		AddControllerYawInput(Value * TurnSensitivity);
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

void APlayerCharacter::PlayDashMontage_Implementation()
{

}

void APlayerCharacter::Dash()
{
	if (Controller != nullptr && bCanDash)
	{
		bCanDash = false;

		FVector ForwardDir = GetActorRotation().Vector();
		FVector UpwardDir = FVector::UpVector;
		FVector DashVector = (ForwardDir * DashDistance) + (UpwardDir * VerticalDashDistance);

		LaunchCharacter(DashVector, true, true);

		// Start cooldown timer
		GetWorld()->GetTimerManager().SetTimer(DashCooldownTimerHandle, this, &APlayerCharacter::DashReload, DashCooldown, false);
	}
}

void APlayerCharacter::DashReload()
{
	bCanDash = true;
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

void APlayerCharacter::AddKey(FName Key)
{
	CollectedKeys.Add(Key);
	
}

bool APlayerCharacter::HasKey(FName KeyID) const
{
	return CollectedKeys.Contains(KeyID);
}
