#include "MainPlayerState.h"

#include "AbilitySystem/PPAbilitySystemComponent.h"
#include "AbilitySystem/PPAttributeSet.h"


AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPPAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UPPAttributeSet>("AttributeSet");

	/* 서버가 클라이언트 업데이트를 시도하는 빈도
	 플레이어 상태에 변경이 생기면 업데이트, 동기화 시도 */
	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
