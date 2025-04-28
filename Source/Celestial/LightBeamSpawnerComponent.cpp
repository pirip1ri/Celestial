#include "LightBeamSpawnerComponent.h"
#include "LightBeamActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

ULightBeamSpawnerComponent::ULightBeamSpawnerComponent()
{

}

void ULightBeamSpawnerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void ULightBeamSpawnerComponent::SpawnBeam(FVector Start, FVector Direction)
{


    if (LightBeamActorClass && GetWorld())
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (!OwnerCharacter) return;

        //FVector Start = OwnerCharacter->GetMesh()->GetSocketLocation(LightBeamSocketName);
        //FRotator Rotation = OwnerCharacter->GetControlRotation();
        //FVector Direction = Rotation.Vector();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();
        if (!ActiveBeam)
        {
            ActiveBeam = GetWorld()->SpawnActor<ALightBeamActor>(LightBeamActorClass, Start, Direction.Rotation(), SpawnParams);
        }

        if (ActiveBeam)
        {

            ActiveBeam->UpdateBeamTransform(Start, Direction);
        }
    }
}

void ULightBeamSpawnerComponent::DestroyBeam()
{
    if (ActiveBeam)
    {
        ActiveBeam->Destroy();
        ActiveBeam = nullptr;
    }
}
