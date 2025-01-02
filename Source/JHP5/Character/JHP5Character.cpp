// Copyright Epic Games, Inc. All Rights Reserved.

#include "JHP5Character.h"

#include <AbilitySystemComponent.h>

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CharacterState/MainPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AJHP5Character

AJHP5Character::AJHP5Character()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// SpringArm Components
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 550.0f; // The camera follows at this distance behind the character	
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Camera Component 
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// MotionWarping Component
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	/* AnimInstance */
	/*const TSubclassOf<UAnimInstance> instance = ConstructorHelpers::FClassFinder<UAnimInstance>(TEXT("/Script/Engine.AnimBlueprint'/Game/01_Character/ABP_Character.ABP_Character_C'")).Class;
	if (instance != nullptr)
	{
		GetMesh()->SetAnimInstanceClass(instance);
	}*/

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AJHP5Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 서버에 어빌리티 엑터 초기화
	InitAbilityActionInfo();
}

void AJHP5Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 클라이언트에 어빌리티 엑터 초기화
	InitAbilityActionInfo();
}

void AJHP5Character::InitAbilityActionInfo()
{
	AMainPlayerState* MainPlayerState = GetPlayerState<AMainPlayerState>();
	check(MainPlayerState); // ??
	MainPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MainPlayerState, this);
	AbilitySystemComponent = MainPlayerState->GetAbilitySystemComponent();
	AttributeSet = MainPlayerState->GetAttributeSet();
}

void AJHP5Character::BeginPlay()
{
	Super::BeginPlay();

	// TODO 시작시 세팅
}

//////////////////////////////////////////////////////////////////////////
// Input

void AJHP5Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
//
//void AJHP5Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	// Set up action bindings
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
//		
//		// Jumping
//		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
//		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
//
//		// Moving
//		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJHP5Character::Move);
//
//		// Looking
//		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AJHP5Character::Look);
//
//		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AJHP5Character::Sprint);
//		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AJHP5Character::StopSprint);
//
//		EnhancedInputComponent->BindAction(VaultAction, ETriggerEvent::Started, this, &AJHP5Character::Vault);
//
//		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Ongoing, this, &AJHP5Character::StartGuard);
//		EnhancedInputComponent->BindAction(GuardAction, ETriggerEvent::Completed, this, &AJHP5Character::StopGuard);
//	}
//	else
//	{
//		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
//	}
//}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Input & Basic
void AJHP5Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AJHP5Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AJHP5Character::Attack()
{
	
}

void AJHP5Character::StartGuard()
{

}

void AJHP5Character::StopGuard()
{
}

void AJHP5Character::ControlCamera(bool Input)
{
}

void AJHP5Character::SetMovementMode()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

/* Sprint */
void AJHP5Character::Sprint()
{
	// 스테미나 사용 0.1초 마다 호출
	// FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AJHP5Character::DrainStamina);
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, true);

	GetCharacterMovement()->MaxWalkSpeed = 750.0f;
}

void AJHP5Character::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AJHP5Character::DrainStamina()
{
	
}

/* Vault */
void AJHP5Character::Vault()
{
	FCollisionShape SweepSphere = FCollisionShape::MakeSphere(5.0f);
	TArray<FHitResult> Out;

	FVector ActorLocation = this->GetActorLocation();
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	for (int i = 0; i < 3; i++)
	{
		FVector SweepStartLocation = ActorLocation + FVector(0, 0, (i * 30));
		FVector SweepEndLocation = SweepStartLocation + (GetActorForwardVector() * 180.0f);

		//GetWorld()->SweepMultiByChannel(Out, SweepStartLoaction, FVector::Zero(), FQuat::Identity, ECollisionChannel::ECC_Visibility, SweepSphere);

		if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), SweepStartLocation, SweepEndLocation, 5.0f,
			UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoreActors,
			EDrawDebugTrace::ForDuration, Out, true))
		{
			for (int j = 0; j < 6; j++)
			{
				FVector Start2 = Out[0].Location + FVector(0.0f, 0.0f, 100.0f) + GetActorForwardVector() * (j * 50);
				FVector End2 = Start2 - FVector(0.0f, 0.0f, 100.0f);

				TArray<FHitResult> Out2;

				if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
					Start2, End2, 10.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoreActors, EDrawDebugTrace::ForDuration, Out2, true))
				{
					if (j == 0)
					{
						VaultStartPos = Out2[0].Location;

						DrawDebugSphere(GetWorld(), VaultStartPos, 10.0f, 12,
							FColor::Emerald, false, 10.0f);
					}

					VaultMiddlePos = Out2[0].Location;
					CanWarp = true;

					DrawDebugSphere(GetWorld(), VaultStartPos, 10.0f, 12,
						FColor::Yellow, false, 10.0f);

				}
				// 발견되지 않아 착지 지점이 된곳
				else
				{
					TArray<FHitResult> Out3;

					if (UKismetSystemLibrary::LineTraceMulti(GetWorld(), Start2, End2 - FVector(0, 0, 1000.0f),
						UEngineTypes::ConvertToTraceType(ECC_Visibility),
						false, IgnoreActors, EDrawDebugTrace::ForDuration, Out3, true, FColor::Blue))
					{
						VaultLandPos = Out3[0].Location;
					}

					//DrawDebugLine(GetWorld(), Start2, End2, FColor::Blue, false, 10.0f);
					break;
				}
			}

			break;
		}
	}

	VaultMotionWarp();

	UE_LOG(LogTemp, Warning, TEXT("Start Vault"));
}

void AJHP5Character::VaultMotionWarp()
{
	float MinRange = GetMesh()->GetComponentLocation().Z - 50.0f;
	float MaxRange = GetMesh()->GetComponentLocation().Z + 50.0f;

	if (CanWarp && VaultLandPos.Z >= MinRange && VaultLandPos.Z <= MaxRange)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		SetActorEnableCollision(false);

		if (MotionWarpingComponent)
		{
			FMotionWarpingTarget WarpingStartTarget;
			WarpingStartTarget.Name = "VaultStart";
			WarpingStartTarget.Location = VaultStartPos;
			WarpingStartTarget.Rotation = GetActorRotation();
			MotionWarpingComponent->AddOrUpdateWarpTarget(WarpingStartTarget);

			FMotionWarpingTarget WarpingMiddleTarget;
			WarpingMiddleTarget.Location = VaultMiddlePos;
			WarpingMiddleTarget.Name = "VaultMiddle";
			WarpingMiddleTarget.Rotation = GetActorRotation(); // 수정
			MotionWarpingComponent->AddOrUpdateWarpTarget(WarpingMiddleTarget);

			FMotionWarpingTarget WarpingLandTarget;
			WarpingLandTarget.Location = VaultLandPos;
			WarpingLandTarget.Name = "VaultLand";
			WarpingLandTarget.Rotation = GetActorRotation(); // 수정
			MotionWarpingComponent->AddOrUpdateWarpTarget(WarpingLandTarget);

			PlayAnimMontage(VaultMontage);

			FOnMontageEnded MontageEndedDelegate;
			MontageEndedDelegate.BindUFunction(this, FName("SetMovementMode"));
			
			//GetAnimInstance()->Montage_SetEndDelegate(MontageEndedDelegate, VaultMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MotionWarpingComponent is not initialized!"));
		}
	}
}