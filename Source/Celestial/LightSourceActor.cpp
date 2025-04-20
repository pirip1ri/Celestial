// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSourceActor.h"
#include "Rotatereflecter.h"  
#include "Components/ArrowComponent.h"

ALightSourceActor::ALightSourceActor()
{
    PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    DirectionArrow->SetupAttachment(Root);
}

void ALightSourceActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FireBeam();
}

void ALightSourceActor::FireBeam()  
{  
   FVector Start = DirectionArrow->GetComponentLocation();  
   FVector Direction = DirectionArrow->GetForwardVector();  
   FVector End = Start + Direction * BeamLength;  

   FHitResult Hit;  
   GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);  
   if (Hit.bBlockingHit)
   {
       End = Hit.ImpactPoint;

       if (ARotatereflecter* Mirror = Cast<ARotatereflecter>(Hit.GetActor()))
       {
           Mirror->ReflectBeam(Hit.ImpactPoint, Direction, 50);
       }
   }

   DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 0.05f, 0, 1.0f);


}




