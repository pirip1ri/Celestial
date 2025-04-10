#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionPromptWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class CELESTIAL_API UInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Call these from PlayerCharacter
	UFUNCTION(BlueprintCallable)
	void SetPromptText(const FText& NewText);

	UFUNCTION(BlueprintCallable)
	void PlayFadeIn();

	UFUNCTION(BlueprintCallable)
	void PlayFadeOut();

protected:
	// Bindings from UMG
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PromptText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeInOut;
};
