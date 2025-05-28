// Fill out your copyright notice in the Description page of Project Settings.

#include "Rotatereflecter.h"
#include "PuzzleDoorInteractable.h" 
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "CelestialPlayerController.h"
#include "Components/StaticMeshComponent.h"

ARotatereflecter::ARotatereflecter()
{
    PrimaryActorTick.bCanEverTick = true;
    MirrorStandMesh = CreateDefaultSubobject<UStaticMeshComponent>("stand");

    Reflectiveball = CreateEditorOnlyDefaultSubobject<UStaticMeshComponent>(" Reflectiveball");
    Reflectiveball->SetupAttachment(MeshComponent);
    RootComponent->SetupAttachment(nullptr);
    MirrorStandMesh->SetupAttachment(RootComponent);      
    // Stay stationary
    MeshComponent->SetupAttachment(RootComponent);

 
   
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
        FRotator CurrentRotation = MeshComponent->GetComponentRotation();
        FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.0f);
        MeshComponent->SetWorldRotation(NewRotation);

        // Check if we're very close to the target rotation
        if (NewRotation.Equals(TargetRotation, 0.5f) || NewRotation.ContainsNaN())
        {
            MeshComponent->SetWorldRotation(TargetRotation); // Snap to final rotation
            bShouldRotate = false;
           
        }
    }

    
}


//OLD
//void ARotatereflecter::InteractAbility_Implementation()
//{
//    Super::InteractAbility_Implementation();
//  
//
//  
//}

void ARotatereflecter::InteractWithReflector()
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
    if (!Player) return;
    RotateReflector("Right");
}
// Old System//
void ARotatereflecter::RotateReflector(FString Direction)
{
    TargetRotation = MeshComponent->GetComponentRotation(); 

    if (Direction == "Right")
    {
        TargetRotation.Yaw += -10.f;
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
    //bShouldRotate = true;
}
void ARotatereflecter::OnInteract(APlayerCharacter* PlayerCharacter)
{
    
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    ACelestialPlayerController* CelestialPC = Cast<ACelestialPlayerController>(PC);
    if (CelestialPC)
    {
        CelestialPC->SetActiveReflector(this);
        TargetRotation = MeshComponent->GetComponentRotation();
        bIsInteracting = true;
    }
    //InteractWithReflector();
     /*AddYawInput(400);
     AddPitchInput(400);*/
}
//bool ARotatereflecter::IsInteracting() const
//{
//    return bIsInteracting;
//}
//
//void ARotatereflecter::StartInteraction()
//{
//    bIsInteracting = true;
//}
//
//void ARotatereflecter::EndInteraction()
//{
//    bIsInteracting = false;
//}

void ARotatereflecter::ReflectBeam(FVector HitPoint, FVector IncomingDirection, int32 RemainingBounces)
{
    if (RemainingBounces <= 0 || !MeshComponent) return;

    FVector MirrorNormal = MeshComponent->GetForwardVector();
    FVector ReflectedDirection = IncomingDirection.MirrorByVector(MirrorNormal);

    float TraceLength = 5000.f;
    FVector OffsetStart = HitPoint + ReflectedDirection;
    FVector TraceEnd = OffsetStart + ReflectedDirection * TraceLength;

    FHitResult Hit;
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, OffsetStart, TraceEnd, ECC_Visibility);

    FVector BeamEnd = bHit ? Hit.ImpactPoint : TraceEnd;
    DrawDebugLine(GetWorld(), HitPoint, BeamEnd, FColor::Cyan, false, 0.1f, 0, 2.0f);

    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();

        if (HitActor->IsA(ARotatereflecter::StaticClass()))
        {
            ARotatereflecter* NextMirror = Cast<ARotatereflecter>(HitActor);
            if (NextMirror && NextMirror != this)
            {
                NextMirror->ReflectBeam(Hit.ImpactPoint, ReflectedDirection, RemainingBounces - 1);
                //for fixed positioning
                bShouldRotate = false;
            }
        }
        else if (HitActor->IsA(APuzzleDoorInteractable::StaticClass()))
        {
            APuzzleDoorInteractable* Door = Cast<APuzzleDoorInteractable>(HitActor);
            if (Door && !Door->GetbIsTriggerPuzzle())
            {
                Door->HitTarget();
                bShouldRotate = false;
            }
        }
    }


}
void ARotatereflecter::AddYawInput(float DeltaYaw)
{
    if (!FMath::IsNearlyZero(DeltaYaw))
    {
        TargetRotation.Yaw += DeltaYaw;
        bShouldRotate = true;
    }
}



void ARotatereflecter::AddPitchInput(float DeltaPitch)
{
    float NewPitch = FMath::Clamp(TargetRotation.Pitch + DeltaPitch, -80.0f, 80.0f);
    if (!FMath::IsNearlyEqual(NewPitch, TargetRotation.Pitch))
    {
        TargetRotation.Pitch = NewPitch;
        bShouldRotate = true;
    }
}