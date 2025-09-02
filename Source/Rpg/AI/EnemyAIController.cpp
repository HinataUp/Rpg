// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "EnemyPlayerState.h"


// Sets default values
AEnemyAIController::AEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bWantsPlayerState = true;
	AIPlayerStateClass = AEnemyPlayerState::StaticClass();
}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyAIController::InitPlayerState()
{
	Super::InitPlayerState();
	// 使用AI 自定义的ps
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		// 如果还没有PlayerState，创建一个
		if (PlayerState == nullptr || PlayerState.GetClass() != AIPlayerStateClass)
		{
			// 创建自定义PlayerState实例
			PlayerState = GetWorld()->SpawnActor<AEnemyPlayerState>(AIPlayerStateClass);
			GEngine->AddOnScreenDebugMessage(100, 2.f, FColor::Red, "EnemyPS create");
			if (PlayerState)
			{
				PlayerState->SetOwner(this);
				// PlayerState->SetReplicates(true);
			}
		}
	}
}

AEnemyPlayerState* AEnemyAIController::GetAIPlayerState()
{
	return Cast<AEnemyPlayerState>(PlayerState);
}
