// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACollectable::ACollectable()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetMobility(EComponentMobility::Movable);
    RootComponent = StaticMesh;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);

    // Ensure it appears in the editor and works properly
    TriggerBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
    TriggerBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));


    // Collision settings
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetGenerateOverlapEvents(true);
    TriggerBox->SetCollisionObjectType(ECC_WorldDynamic);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Overlap);

    // Bind overlap function
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::CheckOverlap);

    CollectEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CollectEffect"));
    CollectEffect->SetupAttachment(RootComponent);
    CollectEffect->SetAutoActivate(false);  // Starts off

    CollectSound = nullptr;
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
    Super::BeginPlay();
    InitialLocation = GetActorLocation();
    RunningTime = 0.0f;
}

void ACollectable::CheckOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Display, TEXT("Check Overlap called"));
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
    {
        PlayerCharacter->CollectedCollectables++;
        CollectItem();
        UE_LOG(LogTemp, Display, TEXT("Collected Collectables: %d"), PlayerCharacter->CollectedCollectables);
    }
}

void ACollectable::CollectItem()
{

    if (CollectEffect)
    {
        CollectEffect->Activate();
    }

    if (CollectSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, CollectSound, GetActorLocation());
    }

    StaticMesh->SetVisibility(false);
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetLifeSpan(3.0f);
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RunningTime += DeltaTime;

    float DeltaHeight = FMath::Sin(RunningTime * BobbingSpeed) * BobbingAmplitude;
    FVector NewLocation = InitialLocation;
    NewLocation.Z += DeltaHeight;

    SetActorLocation(NewLocation);

    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += RotationRate * DeltaTime; // 
    SetActorRotation(NewRotation);
}

