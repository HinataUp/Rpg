// Fill out your copyright notice in the Description page of Project Settings.


#include "RpgEnemy.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rpg/Actors/DodgeballProjectile.h"


// Sets default values
ARpgEnemy::ARpgEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARpgEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARpgEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const ACharacter* RpgPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);
	bCanSeeActor = LookAtActor(RpgPlayer);
	if (bCanSeeActor != bPreviousCanSeeActor)
	{
		if (bCanSeeActor)
		{
			// 这俩的函数 attack 用的是委托的方法，不可以包含形参
			GetWorldTimerManager().SetTimer(ThrowTimerHandle,
			                                this,
			                                &ARpgEnemy::Attack,
			                                ThrowingInterval,
			                                true,
			                                ThrowingDelay);
		}
		else
		{
			GetWorldTimerManager().ClearTimer(ThrowTimerHandle);
		}
	}
	bPreviousCanSeeActor = bCanSeeActor;
}

bool ARpgEnemy::CanSeeActor(const AActor* TargetActor)
{
	const UWorld* World = GetWorld();
	if (!World && TargetActor == nullptr) { return false; }
	FHitResult Hit;
	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = TargetActor->GetActorLocation();
	constexpr ECollisionChannel CollisionChannel = ECC_Visibility;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(TargetActor);
	World->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, CollisionChannel, QueryParams);
	return !Hit.bBlockingHit;
}

bool ARpgEnemy::LookAtActor(const AActor* TargetActor)
{
	const UWorld* World = GetWorld();
	if (!World && TargetActor == nullptr) { return false; }
	if (CanSeeActor(TargetActor))
	{
		FVector StartLocation = GetActorLocation();
		FVector EndLocation = TargetActor->GetActorLocation();
		distance = FVector::Dist(StartLocation, EndLocation);
		FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);
		SetActorRotation(LookAtRotator);

#if WITH_EDITOR
		FString str = "distance: " + FString::FromInt(distance);
		GEngine->AddOnScreenDebugMessage(100, 2.f, FColor::Red, str);
		DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 0, false);
#endif
		return true;
	}
	return false;
}


void ARpgEnemy::Attack() const
{
	if (CanAttack())
	{
		ShotBall();
	}
}

void ARpgEnemy::ShotBall() const
{
	UWorld* World = GetWorld();
	if (!World) { return; }
	FVector StartLocation = GetActorLocation();
	FVector ActorForward = GetActorForwardVector();
	float SpawnDistance = 40.f;
	FVector SpawnLocation = StartLocation + (ActorForward * SpawnDistance);
	World->SpawnActor<ADodgeballProjectile>(DodgeballProjectileClass, SpawnLocation, GetActorRotation());
}
