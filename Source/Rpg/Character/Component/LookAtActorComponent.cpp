// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtActorComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Rpg/AI/EnemyAIController.h"
#include "Rpg/AI/EnemyPlayerState.h"
#include "Rpg/Character/RpgEnemy.h"
#include "Rpg/Character/Controller/RpgPlayerController.h"
#include "Rpg/Utilities/RpgFunctionLibrary.h"


// Sets default values for this component's properties
ULookAtActorComponent::ULookAtActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookAtActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULookAtActorComponent::LookAtActor() const
{
	if (TargetActor == nullptr)
	{
		return;
	}
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = TargetActor->GetActorLocation();
	FRotator CurRotation = GetOwner()->GetActorRotation();
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);


	// 插值 平滑转向
	FRotator Rotator = FMath::RInterpTo(CurRotation, LookAtRotator, GetWorld()->GetDeltaSeconds(), 4.f);
	GetOwner()->SetActorRotation(Rotator);
}

void ULookAtActorComponent::CheckCanSeeActor() const
{
	const UWorld* World = GetWorld();
	AEnemyPlayerState* EnemyPS = Cast<AEnemyAIController>(GetOwner()->GetInstigatorController())->
		GetAIPlayerState();
	if (!World && TargetActor == nullptr) { return; }
	if (!EnemyPS) { return; }
	TArray<const AActor*> IgnoreActors = {GetOwner(), TargetActor};
	EnemyPS->Distance = -1;
	EnemyPS->bCanSeeActor = false;
	if (!URpgFunctionLibrary::CanSeeActor(World, GetComponentLocation(),
	                                      TargetActor, IgnoreActors)) { return; }
	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = TargetActor->GetActorLocation();
	EnemyPS->Distance = FVector::Dist(StartLocation, EndLocation);
	EnemyPS->bCanSeeActor = (EnemyPS->Distance >= 0) && (EnemyPS->Distance <= EnemyPS->MaxSightDis);
	EnemyPS->bCanAttackActor = EnemyPS->Distance <= EnemyPS->AttackMaxDis && EnemyPS->bCanSeeActor;
}


// Called every frame
void ULookAtActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckCanSeeActor();
}
