#include "Character/PPCharacterBase.h"


APPCharacterBase::APPCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
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

