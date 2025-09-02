// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class AEnemyPlayerState;

UCLASS()
class RPG_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 指定使用的PlayerState类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TSubclassOf<AEnemyPlayerState> AIPlayerStateClass;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InitPlayerState() override;

	// 获取AI PlayerState的便捷函数
	UFUNCTION(BlueprintCallable, Category = "AI")
	AEnemyPlayerState* GetAIPlayerState();
};
