// LightBeamActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightBeamActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class CELESTIAL_API ALightBeamActor : public AActor
{
    GENERATED_BODY()

public:
    ALightBeamActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void UpdateBeamTransform(FVector Start, FVector Direction);
private:
    UPROPERTY()
    AActor* BeamOwner;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BeamMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Beam")
    float MaxBeamLength = 2000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Beam")
    float BeamRadius = 10.f;
};
