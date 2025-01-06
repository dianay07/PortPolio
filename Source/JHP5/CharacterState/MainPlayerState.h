#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class JHP5_API AMainPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
protected:
	// AbilitySystem Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

public:
	AMainPlayerState();

	virtual void BeginPlay() override;
	
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }



};
