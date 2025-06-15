// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Player = Cast<APlayerCharacter>(TryGetPawnOwner());
}
void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{

	PlayerMoveVelocity();

}



void UPlayerCharacterAnimInstance::PlayerMoveVelocity()
{
	if (Player)
	{
		FVector Velocity = Player->GetVelocity();
		PlayerSpeed = Velocity.Size();
		bAccelerating = Player->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;
		bIsInAir = Player->GetCharacterMovement()->IsFalling();


		//Crouch = Player->bIsCrouched;


	}
}