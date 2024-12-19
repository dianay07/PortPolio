#include "Character/PPCharacterBase.h"


APPCharacterBase::APPCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Slot = CreateDefaultSubobject<USkeletalMeshComponent>("Slot");
	Slot->SetupAttachment(GetMesh(), FName("SlotSocket"));
	Slot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

