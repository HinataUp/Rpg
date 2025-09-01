// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeballProjectile.h"

#include "Rpg/Character/RpgPlayer.h"
#include "Rpg/Character/Component/HealthComponent.h"


// Sets default values
ADodgeballProjectile::ADodgeballProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetSphereRadius(10.f);
	SphereComp->SetCollisionProfileName(FName("Dodgeball"));
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetSimulatePhysics(true);
	SphereComp->SetNotifyRigidBodyCollision(true);
	RootComponent = SphereComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1100.f;
}

// Called when the game starts or when spawned
void ADodgeballProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentHit.AddDynamic(this, &ADodgeballProjectile::OnHit);
	SetLifeSpan(5.f);
}

// Called every frame
void ADodgeballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADodgeballProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& OtherHit)
{
	ARpgPlayer* Player = Cast<ARpgPlayer>(OtherActor);
	if (Player != nullptr)
	{
		if (UHealthComponent* HealthComponent = Player->FindComponentByClass<UHealthComponent>())
		{
			HealthComponent->LoseHealth(Damage);
		}
		Destroy();
	}
}
