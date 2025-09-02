// Fill out your copyright notice in the Description page of Project Settings.


#include "RpgFunctionLibrary.h"

bool URpgFunctionLibrary::CanSeeActor(const UWorld* World, FVector Location, const AActor* TargetActor,
                                      TArray<const AActor*> IgnoreActors)
{
	if (!World && TargetActor == nullptr) { return false; }
	FHitResult Hit;
	const FVector Start = Location;
	const FVector End = TargetActor->GetActorLocation();
	constexpr ECollisionChannel CollisionChannel = ECC_Visibility;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoreActors);
	World->LineTraceSingleByChannel(Hit, Start, End, CollisionChannel, QueryParams);

	// Sweep Trace logic (not used, only for demonstration)
	/*
	// Rotation of the shape used in the Sweep Trace
	FQuat Rotation = FQuat::Identity;
	// Shape of the object used in the Sweep Trace
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(20.f, 20.f, 20.f));
	GetWorld()->SweepSingleByChannel(Hit,
									 Start,
									 End,
									 Rotation,
									 Channel,
									 Shape);
	*/


	return !Hit.bBlockingHit;
}

void URpgFunctionLibrary::DrawSightLine(const UWorld* World, FVector Start, FVector End,
                                              FColor Color, bool bPersistentLines, float LifeTime)
{

	float dis = FVector::Dist(Start, End);
	const FString Str = "distance: " + FString::FromInt(dis);
	GEngine->AddOnScreenDebugMessage(100, 2.f, FColor::Red, Str);
	DrawDebugLine(World, Start, End, Color, bPersistentLines, LifeTime);

}
