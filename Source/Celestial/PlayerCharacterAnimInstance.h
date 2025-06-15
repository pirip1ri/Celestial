// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
class APlayerCharacter;
UCLASS()
class CELESTIAL_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	APlayerCharacter* Player;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool bAccelerating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "bool")
	bool bIsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "float")
	float PlayerSpeed;
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayerMoveVelocity();
	virtual void NativeInitializeAnimation();

};
