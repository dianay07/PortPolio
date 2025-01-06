// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PPAttributeSet.h"
#include "Net/UnrealNetwork.h"

UPPAttributeSet::UPPAttributeSet()
{
}

void UPPAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPPAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
}

void UPPAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPAttributeSet, Health, OldHealth);
}

void UPPAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPAttributeSet, Stamina, OldStamina);
}
