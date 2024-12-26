 #include "Character/PPEnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/PPAbilitySystemComponent.h"
#include "AbilitySystem/PPAttributeSet.h"


 APPEnemyCharacter::APPEnemyCharacter()
 {
 	AbilitySystemComponent = CreateDefaultSubobject<UPPAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UPPAttributeSet>("AttributeSet");
 }

 void APPEnemyCharacter::BeginPlay()
 {
     Super::BeginPlay();
     
    
     AbilitySystemComponent->InitAbilityActorInfo(this, this);
 }
