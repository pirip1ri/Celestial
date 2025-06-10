// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPickup.h"
#include "PlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "InteractionPromptWidget.h"
#include "DebugHelper.h"
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
       Debug::Print(FString::Printf(TEXT("Player Has key: %s"), *KeyID.ToString()));
    }
}