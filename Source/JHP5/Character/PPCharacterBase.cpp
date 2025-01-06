#include "Character/PPCharacterBase.h"
#include "CharacterState/MainPlayerState.h"
#include "AbilitySystem/PPAbilitySystemComponent.h"

APPCharacterBase::APPCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	InitAbility();
}

void APPCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void APPCharacterBase::InitAbilityActorInfo()
{

}

void APPCharacterBase::InitAbility()
{
	
}

void APPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* APPCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

