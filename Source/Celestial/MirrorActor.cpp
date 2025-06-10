// Fill out your copyright notice in the Description page of Project Settings.


#include "MirrorActor.h"
#include "LightReceiverActor.h"


AMirrorActor::AMirrorActor()
{
    PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    MirrorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MirrorMesh->SetupAttachment(Root);
}

void AMirrorActor::ReflectBeam(FVector HitPoint, FVector IncomingDirection)  
{  
   FVector MirrorNormal = MirrorMesh->GetForwardVector();
   FVector ReflectedDir = IncomingDirection.MirrorByVector(MirrorNormal); 

   FVector EndPoint = HitPoint + ReflectedDir * 10.0f;  
   FHitResult Hit;  
   GetWorld()->LineTraceSingleByChannel(Hit, HitPoint, EndPoint, ECC_Visibility);  

   DrawDebugLine(GetWorld(), HitPoint, EndPoint, FColor::Cyan, false, 1.0f, 0, 2.0f);  
}

