// Fill out your copyright notice in the Description page of Project Settings.

#include "Rotatereflecter.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "CelestialPlayerController.h"
#include "Components/StaticMeshComponent.h"

ARotatereflecter::ARotatereflecter()
{
    PrimaryActorTick.bCanEverTick = true;

   
}

void ARotatereflecter::BeginPlay()
{
    Super::BeginPlay();
}

void ARotatereflecter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShouldRotate)
    {
        FRotator CurrentRotation = GetActorRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
        SetActorRotation(NewRotation);

        if (NewRotation.Equals(TargetRotation, 0.5f))
        {
            SetActorRotation(TargetRotation);
            bShouldRotate = false;
        }
    }
}

void ARotatereflecter::InteractAbility_Implementation()
{
    Super::InteractAbility_Implementation();
  /*  UE_LOG(LogTemp, Warning, TEXT("InteractAbility_Implementation called!"));

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    ACelestialPlayerController* CelestialPC = Cast<ACelestialPlayerController>(PC);
    if (CelestialPC)
    {
        CelestialPC->SetActiveReflector(this);
        bIsInteracting = true;
    }*/
    InteractWithReflector();
}

void ARotatereflecter::InteractWithReflector()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Player) return;
    RotateReflector("Right");
}

void ARotatereflecter::RotateReflector(FString Direction)
{
    TargetRotation = GetActorRotation();

    if (Direction == "Right")
    {
        TargetRotation.Yaw += 10.f;
    }
    else if (Direction == "Left")
    {
        TargetRotation.Yaw -= 10.f;
    }
    else if (Direction == "Forward")
    {
        TargetRotation.Yaw = 0.f;
    }
    else if (Direction == "Back")
    {
        TargetRotation.Yaw = 180.f;
    }

    TargetRotation.Normalize();
    bShouldRotate = true;
}

void ARotatereflecter::ReflectBeam(FVector HitPoint, FVector IncomingDirection, int32 RemainingBounces)
{
    if (RemainingBounces <= 0 || !MeshComponent)
    {
        if (!MeshComponent)
        {
            UE_LOG(LogTemp, Error, TEXT("MeshComponent is null! Make sure it's initialized."));
        }
        return;
    }

    FVector MirrorNormal = MeshComponent->GetForwardVector();
    FVector ReflectedDirection = IncomingDirection.MirrorByVector(MirrorNormal);

    float TraceLength = 5000.f;
    FVector OffsetStart = HitPoint + ReflectedDirection * 1.0f;
    FVector TraceEnd = OffsetStart + ReflectedDirection * TraceLength;

    FHitResult Hit;
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, OffsetStart, TraceEnd, ECC_Visibility);

    FVector BeamEnd = bHit ? Hit.ImpactPoint : TraceEnd;
    DrawDebugLine(GetWorld(), HitPoint, BeamEnd, FColor::Cyan, false, 0.1f, 0, 2.0f);

    if (bHit && Hit.GetActor()->IsA(ARotatereflecter::StaticClass()))
    {
        ARotatereflecter* NextMirror = Cast<ARotatereflecter>(Hit.GetActor());
        if (NextMirror && NextMirror != this)
        {
            NextMirror->ReflectBeam(Hit.ImpactPoint, ReflectedDirection, RemainingBounces - 1);
        }
    }
}

void ARotatereflecter::AddYawInput(float DeltaYaw)
{
    TargetRotation = GetActorRotation();
    TargetRotation.Yaw += DeltaYaw;
    TargetRotation.Normalize();
    bShouldRotate = true;
}

void ARotatereflecter::AddPitchInput(float DeltaPitch)
{
    TargetRotation -= GetActorRotation();
    TargetRotation.Pitch += DeltaPitch;
    TargetRotation.Normalize();
    bShouldRotate = true;
}
