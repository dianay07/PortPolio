#include "MainPlayerState.h"

#include "AbilitySystem/PPAbilitySystemComponent.h"
#include "AbilitySystem/PPAttributeSet.h"


AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPPAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPPAttributeSet>("AttributeSet");

	/* ������ Ŭ���̾�Ʈ ������Ʈ�� �õ��ϴ� ��
	 �÷��̾� ���¿� ������ ����� ������Ʈ, ����ȭ �õ� */
	NetUpdateFrequency = 100.0f;
}

void AMainPlayerState::BeginPlay()
{
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
