// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/GameplayAbilityComboBase.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#pragma optimize("", off)
void UGameplayAbilityComboBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

	if (MontageToPlay != nullptr && AnimInstance != nullptr)
	{
		UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
		if (AbilitySystemComponent)
		{
			UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,   // Owning Ability
				NAME_None, // Task Name (태그로 구분 가능)
				MontageToPlay, // 재생할 애니메이션 몽타주
				PlayRate,   // 플레이 속도
				SectionName, // StartSectionName (특정 섹션에서 시작하려면 지정)
				true,   // bStopWhenAbilityEnds (능력 종료 시 애니메이션 멈춤 여부)
				1.0f     // AnimRootMotionTranslationScale (루트 모션 배율)
			);

			if (MontageTask)
			{
				MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbilityComboBase::OnEndAbility);
				MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbilityComboBase::OnEndAbility);
				MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbilityComboBase::OnEndAbility);

				MontageTask->ReadyForActivation();
			}

			UAbilityTask_WaitGameplayEvent* ComboWindowOpenEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
				this,
				FGameplayTag::RequestGameplayTag(FName("Event.ComboWindowOpen")),
				nullptr,
				false,
				true
			);

			if (ComboWindowOpenEvent)
			{
				ComboWindowOpenEvent->EventReceived.AddDynamic(this, &UGameplayAbilityComboBase::OnComboWindowOpen);

				ComboWindowOpenEvent->ReadyForActivation();
			}

			UAbilityTask_WaitGameplayEvent* ComboWindowEndEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
				this,
				FGameplayTag::RequestGameplayTag(FName("Event.ComboWindowEnd")),
				nullptr,
				false,
				true
			);

			if (ComboWindowEndEvent)
			{
				ComboWindowEndEvent->EventReceived.AddDynamic(this, &UGameplayAbilityComboBase::OnComboWindowEnd);

				ComboWindowEndEvent->ReadyForActivation();
			}
		}
	}
}
#pragma optimize("", on)

#pragma optimize("", off)
void UGameplayAbilityComboBase::OnEndAbility()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;

	Super::EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
#pragma optimize("", on)

void UGameplayAbilityComboBase::OnComboWindowOpen(FGameplayEventData Payload)
{
	FGameplayAbilityActorInfo ActorInfo = GetActorInfo(); // 현재 Ability의 ActorInfo 가져오기
	if (!ActorInfo.AbilitySystemComponent.IsValid())
	{
		return;
	}


	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo.AbilitySystemComponent.Get();
	if (AbilitySystemComponent)
	{
		if (PrevComboWindowTag.IsValid())
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(PrevComboWindowTag);
		}

		if (CurrentComboWindowTag.IsValid())
		{
			AbilitySystemComponent->AddLooseGameplayTag(CurrentComboWindowTag);
		}
	}
}

void UGameplayAbilityComboBase::OnComboWindowEnd(FGameplayEventData Payload)
{
	FGameplayAbilityActorInfo ActorInfo = GetActorInfo(); // 현재 Ability의 ActorInfo 가져오기
	if (!ActorInfo.AbilitySystemComponent.IsValid())
	{
		return;
	}


	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo.AbilitySystemComponent.Get();
	if (AbilitySystemComponent)
	{
		if (CurrentComboWindowTag.IsValid())
		{
			AbilitySystemComponent->RemoveLooseGameplayTag(CurrentComboWindowTag);
		}
	}

	OnEndAbility();
}
