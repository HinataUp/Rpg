// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RpgNpc.h"
#include "Component/LookAtActorComponent.h"
#include "RpgEnemy.generated.h"

class AEnemyPlayerState;
class ULookAtActorComponent;
class ADodgeballProjectile;

UCLASS()
class RPG_API ARpgEnemy : public ARpgNpc
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARpgEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void LookAtTarget();

	FTimerHandle ThrowTimerHandle;
	float ThrowingInterval = 3.f;
	float ThrowingDelay = 1.f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Attack() const;

	UFUNCTION()
	void ShotBall() const;

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FORCEINLINE AActor* GetTargetActor() { return TargetActor; }

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool CanMoveTo();

private:
	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	// 属性尽可能定义在 state里面
	// controller 和 character 可以直接 getplayerstate ，但是 ai controller 需要设置bWantsPlayerState = true;
	// 需要手动指定 对应的 player state （由于我们自定义了）
	// UPROPERTY(EditAnywhere, Category="EnemyState")
	// TObjectPtr<AEnemyPlayerState> EnemyPS;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=DodgeBall, meta=(AllowPrivateAccess=true))
	TSubclassOf<ADodgeballProjectile> DodgeballProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LookAt, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULookAtActorComponent> LookAtActorComponent;

	UPROPERTY()
	bool LookingTarget = false;
};
