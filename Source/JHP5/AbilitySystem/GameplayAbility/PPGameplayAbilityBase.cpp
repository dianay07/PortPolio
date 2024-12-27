// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/PPGameplayAbilityBase.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"

void UPPGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

	if (MontageToPlay != nullptr && AnimInstance != nullptr && AnimInstance->GetActiveMontageInstance() == nullptr)
	{
		TArray<FActiveGameplayEffectHandle>	AppliedEffects;

		// Apply GameplayEffects
		TArray<const UGameplayEffect*> Effects;
		GetGameplayEffectsWhileAnimating(Effects);
		if (Effects.Num() > 0)
		{
			UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
			for (const UGameplayEffect* Effect : Effects)
			{
				FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect, 1.f, MakeEffectContext(Handle, ActorInfo));
				if (EffectHandle.IsValid())
				{
					AppliedEffects.Add(EffectHandle);
				}
			}
		}

		UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
		if (AbilitySystemComponent)
		{
			float MontageDuration = AbilitySystemComponent->PlayMontage(this, ActivationInfo, MontageToPlay, PlayRate, SectionName);
			//float MontageDuration = AbilitySystemComponent->PlayMontageForMesh(
			//	nullptr,    // Use default mesh
			//	MontageToPlay,
			//	PlayRate,
			//	SectionName
			//);

			if (MontageDuration > 0.f)
			{
				// Bind Montage End Delegate
				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &UPPGameplayAbilityBase::OnMontageEnded, ActorInfo->AbilitySystemComponent, AppliedEffects);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);

				if (SectionName != NAME_None)
				{
					AnimInstance->Montage_JumpToSection(SectionName, MontageToPlay);
				}
			}
		}

		///*if (ActorInfo->IsNetAuthority())
		//{
		//	MulticastPlayMontage(ActorInfo->AvatarActor.Get(), MontageToPlay, PlayRate, SectionName);
		//}
		//else
		//{
		//	ServerPlayMontage(MontageToPlay, PlayRate, SectionName);
		//}*/



		////float const Duration = AnimInstance->Montage_Play(MontageToPlay, PlayRate);

		//FOnMontageEnded EndDelegate;
		//EndDelegate.BindUObject(this, &UPPGameplayAbilityBase::OnMontageEnded, ActorInfo->AbilitySystemComponent, AppliedEffects);
		//AnimInstance->Montage_SetEndDelegate(EndDelegate);

		//if (SectionName != NAME_None)
		//{
		//	AnimInstance->Montage_JumpToSection(SectionName);
		//}
	}
}

//void UPPGameplayAbilityBase::ServerPlayMontage_Implementation(UAnimMontage* Montage, float InPlayRate, FName StartSection)
//{
//	MulticastPlayMontage(GetAvatarActorFromActorInfo(), Montage, InPlayRate, StartSection);
//	//MulticastPlayMontage(GetOwningActorFromActorInfo(), Montage, InPlayRate, StartSection);
//}
//
//void UPPGameplayAbilityBase::MulticastPlayMontage_Implementation(AActor* TargetActor, UAnimMontage* Montage, float InPlayRate, FName StartSection)
//{
//	UAnimInstance* AnimInstance = TargetActor->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
//	float const Duration = AnimInstance->Montage_Play(Montage, InPlayRate);
//}


void UPPGameplayAbilityBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponentPtr, TArray<FActiveGameplayEffectHandle> AppliedEffects)
{
	// Remove any GameplayEffects that we applied
	UAbilitySystemComponent* const AbilitySystemComponent = AbilitySystemComponentPtr.Get();
	if (AbilitySystemComponent)
	{
		for (FActiveGameplayEffectHandle Handle : AppliedEffects)
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
		}
	}

	ensure(CurrentActorInfo);

	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPPGameplayAbilityBase::GetGameplayEffectsWhileAnimating(TArray<const UGameplayEffect*>& OutEffects) const
{
	OutEffects.Append(GameplayEffectsWhileAnimating);

	for (TSubclassOf<UGameplayEffect> EffectClass : GameplayEffectClassesWhileAnimating)
	{
		if (EffectClass)
		{
			OutEffects.Add(EffectClass->GetDefaultObject<UGameplayEffect>());
		}
	}
}