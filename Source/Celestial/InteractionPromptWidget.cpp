#include "InteractionPromptWidget.h"
#include "Components/TextBlock.h"

void UInteractionPromptWidget::SetPromptText(const FText& NewText)
{
	if (PromptText)
	{
		PromptText->SetText(NewText);
	}
}

void UInteractionPromptWidget::PlayFadeIn()
{
	if (FadeInOut)
	{
		PlayAnimation(FadeInOut, 0.f, 1, EUMGSequencePlayMode::Forward);
	}
}

void UInteractionPromptWidget::PlayFadeOut()
{
	if (FadeInOut)
	{
		PlayAnimation(FadeInOut, 0.f, 1, EUMGSequencePlayMode::Reverse);
	}
}
