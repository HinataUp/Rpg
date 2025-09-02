// Fill out your copyright notice in the Description page of Project Settings.


#include "RpgEnemy.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Component/LookAtActorComponent.h"
#include "Rpg/Actors/DodgeballProjectile.h"
#include "Rpg/AI/EnemyAIController.h"
#include "Rpg/AI/EnemyPlayerState.h"
#include "Rpg/Utilities/RpgFunctionLibrary.h"


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
	TargetActor = Player;
	LookAtActorComponent->SetTarget(Player);
}

void ARpgEnemy::LookAtTarget()
{
	AEnemyPlayerState* EnemyPS = Cast<AEnemyAIController>(GetController())->GetAIPlayerState();
#if WITH_EDITOR
	if (EnemyPS->bCanSeeActor)
	{
		URpgFunctionLibrary::DrawSightLine(GetWorld(), GetActorLocation(),
		                                   TargetActor->GetActorLocation(),
		                                   FColor::Blue, false, 0);
	}
#endif
	// 上一帧看不到 当前帧看到了，那么开始转向 (开启追踪和设计)
	// 上一帧看的到 当前帧数看不到，那么 不转向 (关闭设计 和 追踪)
	// 能看到 即可追踪 ，在攻击范围内 即可攻击
	if (EnemyPS->bCanSeeActor != EnemyPS->bPreviousCanSeeActor)
	{
		LookingTarget = EnemyPS->bCanSeeActor;
		if (LookingTarget)
		{
			// 转向
			LookAtActorComponent->LookAtActor();
			// 射击
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
	// 无论如何上一帧 都会按照当前帧状态更新
	EnemyPS->bPreviousCanSeeActor = EnemyPS->bCanSeeActor;
}

// Called every frame
void ARpgEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookAtTarget();
}


void ARpgEnemy::Attack() const
{
	AEnemyPlayerState* EnemyPS = Cast<AEnemyAIController>(GetController())->GetAIPlayerState();
	// 在攻击范围内（实现可到达， 且攻击距离内攻击，否则pass 此逻辑）
	if (EnemyPS->bCanAttackActor)
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

bool ARpgEnemy::CanMoveTo()
{
	// FString str = "MoveTo" + LexToString(LookingTarget);
	// GEngine->AddOnScreenDebugMessage(100, 0, FColor::Red, str);
	return LookingTarget;
}
