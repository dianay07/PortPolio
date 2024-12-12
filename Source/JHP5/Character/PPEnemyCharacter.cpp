 #include "Character/PPEnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/PPAbilitySystemComponent.h"
#include "AbilitySystem/PPAttributeSet.h"


 APPEnemyCharacter::APPEnemyCharacter()
 {
 	AbilitySystemComponent = CreateDefaultSubobject<UPPAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);

    AttributeSet = CreateDefaultSubobject<UPPAttributeSet>("AttributeSet");
 }
