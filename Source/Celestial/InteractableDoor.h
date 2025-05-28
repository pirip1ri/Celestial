#pragma once  

#include "CoreMinimal.h"  
#include "Interactable.h"  
#include "Delegates/Delegate.h"
#include "UObject/ScriptMacros.h"
#include "PlayerCharacter.h"
#include "InteractableDoor.generated.h"  

/**  
*  
*/  
UENUM(BlueprintType)
enum class EDoorState : uint8
{
    Locked UMETA(DisplayName = "Locked"),
    Open   UMETA(DisplayName = "Open")
};
 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDoorStateChanged, EDoorState, NewState);

UCLASS()  
class CELESTIAL_API AInteractableDoor : public AInteractable  
{  
  GENERATED_BODY()  

public:  
  AInteractableDoor();

 virtual void InteractAbility_Implementation()override;
 bool GetbIsTriggerPuzzle() { return bIsTriggerByPuzzle; }
  UPROPERTY(EditAnywhere, Category = "Door")  
  FName RequiredKeyID;  

protected:
    bool bIsTriggerByPuzzle;

private:  
  UPROPERTY(EditAnywhere, Category = "Door")  
  EDoorState DoorState = EDoorState::Locked;  

  UPROPERTY(BlueprintAssignable, Category = "Door")  
  FOnDoorStateChanged OnDoorStateChanged;  
protected:
  UFUNCTION(BlueprintCallable, Category = "Door")  
  void SetDoorState(EDoorState NewState);  

  UFUNCTION(BlueprintCallable, Category = "Door")  
  EDoorState GetDoorState() const { return DoorState; }  


  FRotator OpenRotation;
  FRotator ClosedRotation;
};
