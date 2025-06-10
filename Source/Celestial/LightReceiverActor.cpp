// Fill out your copyright notice in the Description page of Project Settings.


#include "LightReceiverActor.h"
#include "DebugHelper.h"
	
ALightReceiverActor::ALightReceiverActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("mesh");

}


void ALightReceiverActor::ActivateReceiver()
{
	Debug::Print("Activated!");
}

