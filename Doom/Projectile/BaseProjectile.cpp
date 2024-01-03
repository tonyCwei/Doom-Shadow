// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setting up components
	// if (!RootComponent) {
	// 	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	// }
	
	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = sphereCollision;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);

	projectileFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ProjectileFlipBook"));
	projectileFlipbookComponent->SetupAttachment(RootComponent);

	
	
	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	//projectileMovement->SetupAttachment(RootComponent);
	//projectileMovement->InitialSpeed = projectileSpeed;
	//projectileMovement->MaxSpeed = projectileSpeed;
	projectileMovement->ProjectileGravityScale = 0.0f;

	//Setting life span of the projectile
	//InitialLifeSpan = 3.0f;

}

void ABaseProjectile::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	projectileMovement->InitialSpeed = projectileSpeed;
	projectileMovement->MaxSpeed = projectileSpeed;
}



// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	sphereCollision->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	//UE_LOG(LogTemp, Display, TEXT("%f"), projectileDamage);

	//May change later
	//AActor* MyOwner = UGameplayStatics::GetPlayerCharacter(this,0);
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) {
		Destroy();
		return;
	}

	//Apply Damage
	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner) {
		//UE_LOG(LogTemp, Display, TEXT("apply damaged called"));
		UGameplayStatics::ApplyDamage(OtherActor, projectileDamage, MyOwnerInstigator, this, DamageTypeClass);
	}
	

	//Set flipbook and destroy, may need set scale
	projectileFlipbookComponent->SetWorldScale3D(destroyScale);
	projectileFlipbookComponent->SetFlipbook(destroyFlipbook);
	
	//Destroy after destroyFlipbook finishes playing
	FTimerHandle ProjectileTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ProjectileTimerHandle, [&]()
	{
	   Destroy();
	}, projectileFlipbookComponent->GetFlipbookLength()*0.9, false);
	//multiplied 0.9 for delay time in order to prevent flipbook from looping back to the first frame

}



