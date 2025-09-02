// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EnemyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AEnemyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="EyeSight")
	float MaxSightDis = 800.f;

	float Distance = 0.f;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackMaxDis = 500.f;

	// 用于检测 target 是否再实现范围内的, 看到 那么就可以移动过去
	UPROPERTY(VisibleAnywhere)
	bool bCanSeeActor = false;

	// 用于判断是否可以攻击
	UPROPERTY(VisibleAnywhere)
	bool bCanAttackActor = false;
	

	// 上一帧 是否看到target
	UPROPERTY(VisibleAnywhere)
	bool bPreviousCanSeeActor = false;

	UFUNCTION(BlueprintCallable)
	bool GetCanSeeActor() const { return bCanSeeActor; }

	UFUNCTION(BlueprintCallable)
	bool GetCanAttackActor() const { return bCanAttackActor; }
};
