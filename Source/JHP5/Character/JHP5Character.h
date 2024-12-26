// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MotionWarpingComponent.h"
#include "Character/PPCharacterBase.h"
#include "Logging/LogMacros.h"
#include "JHP5Character.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UMotionWarpingComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AJHP5Character : public APPCharacterBase
{
	GENERATED_BODY()

private:
	// SpringArm Component 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	// Camera Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	// MotionWarping Component ( Vault ) M
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

private:
	/* Input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	// Jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Look
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* VaultAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GuardAction;

public:
	AJHP5Character();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilityActionInfo();

protected:
	virtual void BeginPlay() override;

	/* Basic */
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();
	void StartGuard();
	void StopGuard();

	void ControlCamera(bool Input);

	/* Sprint */
protected:
	FTimerHandle TimerHandle;
	void Sprint();
	void StopSprint();

	UFUNCTION()
	void DrainStamina();

	/* Vault */
protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* VaultMontage;

	void Vault();
	void VaultMotionWarp();

private:
	FVector VaultStartPos;
	FVector VaultMiddlePos;
	FVector VaultLandPos;

	bool CanWarp;

public:
	UFUNCTION()
	void SetMovementMode();

protected:
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Return 
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArmComponent; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComponent; }
};
