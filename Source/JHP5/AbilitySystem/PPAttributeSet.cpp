// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PPAttributeSet.h"
#include "Net/UnrealNetwork.h"

// 해당 속성 변경을 체크할 '담당자' 생성
void UPPAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPAttributeSet, Health, OldHealth);
}

void UPPAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPAttributeSet, MaxHealth, OldMaxHealth);
}

void UPPAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPAttributeSet, Stamina, OldStamina);
}

void UPPAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPPAttributeSet, MaxHealth, OldMaxStamina);
}

UPPAttributeSet::UPPAttributeSet()
{
	// consol -> showdebug abilitysystem
	InitHealth(70.0f);
	InitMaxHealth(100.0f);
	InitStamina(70.0f);
	InitMaxStamina(70.0f);
}

void UPPAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 모든 항목에 담당자 등록 ( COND_None = 무조건 )
	DOREPLIFETIME_CONDITION_NOTIFY(UPPAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPPAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}
