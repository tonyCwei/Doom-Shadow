// Copyright Epic Games, Inc. All Rights Reserved.

#include "DoomCharacter.h"
#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "UI/PlayerHUD.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Components/ChildActorComponent.h"
#include "BaseWeapon.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADoomCharacter

ADoomCharacter::ADoomCharacter()
{


	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;


	//Create Weapon Child Actor
	WeaponChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponChildActor"));
	WeaponChildActorComponent->SetupAttachment(FirstPersonCameraComponent);

	//Assign Main Weapon
	

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	// Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	// Mesh1P->SetOnlyOwnerSee(true);
	// Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	// Mesh1P->bCastDynamicShadow = false;
	// Mesh1P->CastShadow = false;
	// //Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	// Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void ADoomCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//Assign Main Weapon
	mainWeapon = Cast<ABaseWeapon>(WeaponChildActorComponent->GetChildActor());

	//Player HUD
	if (playerHUDClass) {
		playerHUD = CreateWidget<UPlayerHUD>(this->GetWorld(), playerHUDClass);
		if (playerHUD) {
			playerHUD->AddToViewport();
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ADoomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADoomCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADoomCharacter::Look);

		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ADoomCharacter::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ADoomCharacter::StopShoot);

		//Melee
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &ADoomCharacter::Melee);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ADoomCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADoomCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADoomCharacter::Shoot(const FInputActionValue& Value) {
	if (!canMelee) return;

	mainWeapon->FireWeapon();
	isShooting = true;

	FTimerHandle ShootMeleeHandle;
	GetWorld()->GetTimerManager().SetTimer(ShootMeleeHandle, [&]()
	{
		isShooting = false;
	}, 0.5, false);

}

void ADoomCharacter::StopShoot(const FInputActionValue& Value) {
	
	mainWeapon->StopFire();

}

void ADoomCharacter::Melee(const FInputActionValue& Value) {
	if (!canMelee || isShooting) return;

	canMelee = false;
	curWeaponClass = WeaponChildActorComponent->GetChildActorClass();

	if (fistClass) {
		WeaponChildActorComponent->SetChildActorClass(fistClass);
		mainWeapon = Cast<ABaseWeapon>(WeaponChildActorComponent->GetChildActor());
		mainWeapon->FireWeapon();
	}

	
	GetWorld()->GetTimerManager().SetTimer(MeleeHandle, [&]()
	{
		if (curWeaponClass) {
			WeaponChildActorComponent->SetChildActorClass(curWeaponClass);	
			mainWeapon = Cast<ABaseWeapon>(WeaponChildActorComponent->GetChildActor());
			canMelee = true;
		}
	}, meleeRate, false);
}


void ADoomCharacter::pickupWeapon(TSubclassOf<ABaseWeapon> WeaponClass) {
	WeaponChildActorComponent->SetChildActorClass(WeaponClass);
	mainWeapon = Cast<ABaseWeapon>(WeaponChildActorComponent->GetChildActor());
	GetWorldTimerManager().ClearTimer(MeleeHandle);
	canMelee = true;
}

