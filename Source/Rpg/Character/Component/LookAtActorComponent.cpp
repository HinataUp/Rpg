// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtActorComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Rpg/Utilities/DodgeballFunctionLibrary.h"


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

float ULookAtActorComponent::LooKAtActor() const
{
	const UWorld* World = GetWorld();
	if (!World && TargetActor == nullptr) { return -1; }
	TArray<const AActor*> IgnoreActors = {GetOwner(), TargetActor};

	if (!UDodgeballFunctionLibrary::CanSeeActor(World, GetComponentLocation(), TargetActor, IgnoreActors)) return -1;
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = TargetActor->GetActorLocation();
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);
	GetOwner()->SetActorRotation(LookAtRotator);
	return FVector::Dist(StartLocation, EndLocation);
}


// Called every frame
void ULookAtActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Distance = LooKAtActor();
	bCanSeeTarget = Distance >= 0;
}
