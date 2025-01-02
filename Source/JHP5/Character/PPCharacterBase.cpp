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

UAbilitySystemComponent* APPCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

