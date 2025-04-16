// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPickup.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "InteractionPromptWidget.h"

AKeyPickup::AKeyPickup()
    
{
	KeyID = ("Key");
}

void AKeyPickup::CollectItem()
{
    Super::CollectItem(); 
    //Checking player
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {   //Added key collection logic to player character
        // to track collected keys;
        Player->AddKey(KeyID);
        UE_LOG(LogTemp, Warning, TEXT("Key Collected: %s"), *KeyID.ToString());
       

    }
}