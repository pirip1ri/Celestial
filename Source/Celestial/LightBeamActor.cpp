// LightBeamActor.cpp

#include "LightBeamActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ALightBeamActor::ALightBeamActor()
{
    PrimaryActorTick.bCanEverTick = true;

    BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));
    RootComponent = BeamMesh;
}

void ALightBeamActor::BeginPlay()
{
    Super::BeginPlay();
}

void ALightBeamActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ALightBeamActor::UpdateBeamTransform(FVector Start, FVector Direction)
{
    FVector End = Start + (Direction * MaxBeamLength);

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(BeamOwner);

    FHitResult Hit;
    bool bHit = GetWorld()->SweepSingleByChannel(
        Hit,
        Start,
        End,
        FQuat::FindBetweenVectors(FVector::UpVector, (End - Start).GetSafeNormal()), 
        ECC_Visibility,
        FCollisionShape::MakeCapsule(BeamRadius, MaxBeamLength * 0.5f),
        Params
    );

    float ActualLength = MaxBeamLength;
    if (bHit)
    {
        ActualLength = (Hit.ImpactPoint - Start).Size();
    }

    End = Start + Direction * ActualLength;

    FVector BeamCenter = (Start + End) * 0.5f;
    FVector BeamDirection = (End - Start).GetSafeNormal();
    float BeamLength = (End - Start).Size();

    // Set BeamMesh transform
    BeamMesh->SetWorldLocation(BeamCenter);

    FRotator BeamRotation = FRotationMatrix::MakeFromX(BeamDirection).Rotator();
    BeamMesh->SetWorldRotation(BeamRotation);

    BeamMesh->SetWorldScale3D(FVector(BeamLength / 100.f, 0.05f, 0.05f));
    BeamMesh->SetVisibility(true);

    UE_LOG(LogTemp, Display, TEXT("Updated Beam: Length = %f"), ActualLength);
}

