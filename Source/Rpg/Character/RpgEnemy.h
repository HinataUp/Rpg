// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RpgNpc.h"
#include "Component/LookAtActorComponent.h"
#include "RpgEnemy.generated.h"

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

	FTimerHandle ThrowTimerHandle;
	float ThrowingInterval = 2.f;
	float ThrowingDelay = 0.5f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool CanAttack() const;

	UFUNCTION()
	void Attack() const;

	UFUNCTION()
	void ShotBall() const;

private:
	float distance = 0.f;

	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackMaxDis = 1000.f;

	bool bCanSeeActor = false;

	bool bPreviousCanSeeActor = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=DodgeBall, meta=(AllowPrivateAccess=true))
	TSubclassOf<ADodgeballProjectile> DodgeballProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LookAt, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULookAtActorComponent> LookAtActorComponent;
	
};

FORCEINLINE bool ARpgEnemy::CanAttack() const
{
	return LookAtActorComponent->Distance <= AttackMaxDis;
}
