// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "LightBeamSpawnerComponent.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"
#include "CelestialPlayerController.h"
#include "Rotatereflecter.h"
#include "InspectInteractables.h"
#include <Kismet/GameplayStatics.h>
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


 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("SpringAim");
	

	

	// Create the camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
    // Replace the line causing the error with the following code  
   
	BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));
	BeamMesh->SetupAttachment(GetMesh(), ChestSocketName);
	BeamMesh->SetRelativeLocation(FVector::ZeroVector);
	



	
	CameraBoom->SetupAttachment(RootComponent);


	

	CameraBoom->bDoCollisionTest = true;
	CameraBoom->ProbeSize = 8.0f;
	CameraBoom->ProbeChannel = ECC_Camera;

	BeamMesh->SetRelativeRotation(FRotator::ZeroRotator);

	BeamMesh->SetVisibility(false);
	BeamMesh->SetRelativeScale3D(FVector(0.f, 1.f, 1.f));

	bCanDash = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CameraBoom->CameraLagSpeed = 12.0f;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 12.0f;

	CameraBoom->bUsePawnControlRotation = true;
	// Store the default walk speed from the movement component
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SaveCameraDefaults();	

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

	CameraBoom->TargetArmLength = ArmLength;
	CameraBoom->SocketOffset = NormalArmOffset;

}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (bIsFiringBeam)
	{
		CastLight();
	}*/

	FVector Start = RootComponent->GetComponentLocation() + RootComponent->GetComponentRotation().RotateVector(SweepStartOffset);
	FVector End = Start + (FollowCamera->GetForwardVector() * MaximumInteractionDistance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);


	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::FindBetweenVectors(FVector::UpVector, (End - Start).GetSafeNormal()),
		ECC_Visibility,
		FCollisionShape::MakeCapsule(InteractionRadius, MaximumInteractionDistance * 0.5f),
		Params
	);

	/*if (bToggleDebugVisualiser)
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
	}*/

	HitInteractable = bHit ? Cast<AInteractable>(HitResult.GetActor()) : nullptr;
	
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
	else if (LastInteractable) 
	{
		LastInteractable = nullptr;

		if (InteractionPromptInstance)
		{
			InteractionPromptInstance->PlayFadeOut();
		}
	}

	UpdateBeam(DeltaTime);
	UpdateZoom(DeltaTime);


}
void APlayerCharacter::UpdateBeam(float DeltaTime)
{
	if (!BeamMesh || !BeamMesh->IsVisible())
		return;

	const float TargetLength = bIsBeamActive ? BeamMaxLength : 0.f;

	// Linear interpolation at a constant rate
	CurrentBeamLength = FMath::FInterpConstantTo(CurrentBeamLength, TargetLength, DeltaTime, BeamGrowthSpeed);

	const float BeamScaleZ = CurrentBeamLength / 300.f; // Adjust for your mesh's original size
	BeamMesh->SetRelativeScale3D(FVector(0.5f,0.5f, BeamScaleZ));
	BeamMesh->SetRelativeLocation(FVector(0.f, 0.f, -50.f * BeamScaleZ));

	// Align with chest socket
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		const FTransform SocketTransform = MeshComp->GetSocketTransform(ChestSocketName, RTS_World);
		FRotator BeamRotation = SocketTransform.GetRotation().Rotator();
		BeamRotation.Pitch -= 15.f;
		BeamMesh->SetWorldRotation(BeamRotation);
	}

	if (CurrentBeamLength <= KINDA_SMALL_NUMBER && !bIsBeamActive)
	{
		BeamMesh->SetVisibility(false);
	}


}

void APlayerCharacter::StartBeam()
{
	bIsBeamActive = true;

	if (!BeamMesh->IsVisible())
	{
		BeamMesh->SetVisibility(true);
	}

	// Optional reset
	// CurrentBeamLength = 0.f;
}

void APlayerCharacter::StopBeam()
{
	bIsBeamActive = false;
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
	if (!DashMontage || !GetMesh()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// Optional: Stop any montage to ensure DashMontage plays
		AnimInstance->Montage_Stop(0.1f); // short fade-out
		AnimInstance->Montage_Play(DashMontage);
	}
}

void APlayerCharacter::Dash()
{
	if (Controller && bCanDash)
	{
		bCanDash = false;

		// Play dash animation
		PlayDashMontage();

		FVector ForwardDir = GetActorForwardVector();
		FVector UpwardDir = FVector::UpVector;
		FVector DashVector = (ForwardDir * DashDistance) + (UpwardDir * VerticalDashDistance);

		LaunchCharacter(DashVector, true, true);

		// Start cooldown timer
		GetWorld()->GetTimerManager().SetTimer(
			DashCooldownTimerHandle,
			this,
			&APlayerCharacter::DashReload,
			DashCooldown,
			false
		);
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
void APlayerCharacter::CastLight()
{
	//FVector Start = GetMesh()->GetSocketLocation(LightBeamSocketName);
	FVector Direction = GetControlRotation().Vector();

	//LightBeamSpawner->SpawnBeam(Start, Direction);
	//bIsFiringBeam = true;
}

void APlayerCharacter::CastLightEnd()
{
	//bIsFiringBeam = false;
	//LightBeamSpawner->DestroyBeam();
}


bool APlayerCharacter::HasKey(FName KeyID) const
{
	return CollectedKeys.Contains(KeyID);
}


void APlayerCharacter::TryInteract()
{
	if (AInteractable* Inspect = Cast<AInteractable>(InspectClass))
	{
		LastInteractable->InteractAbility();

		if (AInspectInteractables* InspectInteract = Cast<AInspectInteractables>(LastInteractable))
		{
			InspectInteract->InteractAbility(); // Make sure this function exists and is meaningful
		}

		// Try to get the controller and check if it's valid
		if (ACelestialPlayerController* CelestialController = Cast<ACelestialPlayerController>(GetController()))
		{
			// If the interactable is a reflector, set it as active
			if (ARotatereflecter* Reflector = Cast<ARotatereflecter>(LastInteractable))
			{
				CelestialController->SetActiveReflector(Reflector);
				Reflector->bIsInteracting = true;
			}
		}

	}
	else
	{


		if (ACelestialPlayerController* C = Cast<ACelestialPlayerController>(GetController()))
		{


			AInspectInteractables* Inspectable = Cast<AInspectInteractables>(UGameplayStatics::GetActorOfClass(GetWorld(), AInspectInteractables::StaticClass()));
			Inspect = Cast<AInspectInteractables>(Inspectable);
			
			if (Inspectable == Cast<AInspectInteractables>(Inspect))
			{
				Inspectable->InteractAbility();
			}
			if (InteractionPromptInstance && LastInteractable)
			{
				InteractionPromptInstance->SetPromptText(LastInteractable->InteractionPromptText);
				InteractionPromptInstance->PlayFadeIn();
			}
		}


	}
}
		

		
// If the interactable is an inspectable one, optionally handle extra inspect logic here


void APlayerCharacter::ResetCameraAfterInspect()
{
	if (!CameraBoom) return;

	// Ensure collision is active
	CameraBoom->bDoCollisionTest = true;

	// Cache current values
	StartingArmLength = CameraBoom->TargetArmLength;
	StartingBoomRotation = CameraBoom->GetRelativeRotation();
	
	// Target values
	TargetBoomRotation = DefaultCameraBoomRotation;

	bIsResettingCamera = true;
	CameraResetTimer = 0.f;
	CameraBoom->ProbeSize = 8.0f;
}




void APlayerCharacter::SaveCameraDefaults()
{
	if (CameraBoom)
	{
		CameraBoom->ProbeSize = 8.0f;
		CameraBoom->bDoCollisionTest = true;
		InitialArmLength = CameraBoom->TargetArmLength;
		DefaultCameraBoomRotation = CameraBoom->GetRelativeRotation();
	}
}



void APlayerCharacter::ToggleZoom()
{
	bIsZoomedIn = !bIsZoomedIn;
}

void APlayerCharacter::UpdateZoom(float DeltaTime)
{
	if (!FollowCamera || !CameraBoom)
		return;

	// FOV Interpolation
	 TargetFOV = bIsZoomedIn ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, CameraZoomSpeed);
	FollowCamera->SetFieldOfView(NewFOV);

	// Arm Length Interpolation
	float TargetArmLength = bIsZoomedIn ? ZoomedArmLength : ArmLength;
	float NewArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength, DeltaTime, CameraZoomSpeed);
	CameraBoom->TargetArmLength = NewArmLength;

	// Socket Offset Interpolation (for shoulder effect)
	FVector TargetOffset = bIsZoomedIn ? ZoomedArmOffset : NormalArmOffset;
	FVector NewOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetOffset, DeltaTime, CameraZoomSpeed);
	CameraBoom->SocketOffset = NewOffset;
}
