#pragma once

namespace Debug
{
	static void Print(const FString& msg, const FColor& Color = FColor::MakeRandomColor(), int32 Inkey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Inkey, 6.f, Color, msg);
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
	}
}