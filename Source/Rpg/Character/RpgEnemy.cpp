// Fill out your copyright notice in the Description page of Project Settings.


#include "RpgEnemy.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Component/LookAtActorComponent.h"
#include "Rpg/Actors/DodgeballProjectile.h"


// Sets default values
ARpgEnemy::ARpgEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LookAtActorComponent = CreateDefaultSubobject<ULookAtActorComponent>(TEXT("Look At Actor Component"));
	LookAtActorComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARpgEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 一定要在构造函数中 先创建组件 然后才可在beginplay中使用，否则各种错误
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	LookAtActorComponent->SetTarget(Player);
}

// Called every frame
void ARpgEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bCanSeeActor = LookAtActorComponent->CanSeeTarget();
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
