// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DoomCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADoomCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	// UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	// USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	/** Melee Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MeleeAction;
	
public:
	ADoomCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for shooting input */
	void Shoot(const FInputActionValue& Value);

	void StopShoot(const FInputActionValue& Value);

	/** Called for melee input */
	void Melee(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	//USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	//Weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* WeaponChildActorComponent;	
	
	UPROPERTY(VisibleAnywhere)
	class ABaseWeapon* mainWeapon;


	//Melee
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseWeapon> fistClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseWeapon> curWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	float meleeRate = 0.5f;

	bool canMelee = true;

	FTimerHandle MeleeHandle;



	bool isShooting = false;

//Different Types of Bullets
	//Pistol
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 bullet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 maxBullet = 200;
	
	//Shotgun
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 shell;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 maxShell = 50;

	//Rocket Launcher
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 rocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 maxRocket = 50;
	
	//Laser
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 cell;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 maxCell = 300;

	//Player HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerHUD> playerHUDClass;

	class UPlayerHUD* playerHUD;
	


public:
	//Getter
	UFUNCTION(BlueprintCallable)
	int32 getBullet() const {return bullet;}

	UFUNCTION(BlueprintCallable)
	int32 getShell() const {return shell;}

	UFUNCTION(BlueprintCallable)
	int32 getRocket() const {return rocket;}

	UFUNCTION(BlueprintCallable)
	int32 getCell() const {return cell;}

	UFUNCTION(BlueprintCallable)
	int32 getMaxBullet() const {return maxBullet;}

	UFUNCTION(BlueprintCallable)
	int32 getMaxShell() const {return maxShell;}

	UFUNCTION(BlueprintCallable)
	int32 getMaxRocket() const {return maxRocket;}

	UFUNCTION(BlueprintCallable)
	int32 getMaxCell() const {return maxCell;}

	UFUNCTION()
	UPlayerHUD* getPlayerHUD() { return playerHUD; }

	UFUNCTION(BlueprintCallable)
	bool getCanMelee() const {return canMelee;}

	//DecreaseAmmmo
	UFUNCTION(BlueprintCallable)
	void decreaseBullet() { bullet--; }

	UFUNCTION(BlueprintCallable)
	void decreaseShell() { shell--; }

	UFUNCTION(BlueprintCallable)
	void decreaseRocket() { rocket--; }

	UFUNCTION(BlueprintCallable)
	void decreaseCell() { cell--; }

	UFUNCTION(BlueprintCallable)
	void setCell(int32 newCell) { cell = newCell; }

	UFUNCTION(BlueprintCallable)
	void setCanMelee(bool newVal)  {canMelee = newVal;}


	//Weapon Pickup
	UFUNCTION()
	void pickupWeapon(TSubclassOf<ABaseWeapon> WeaponClass);
};

