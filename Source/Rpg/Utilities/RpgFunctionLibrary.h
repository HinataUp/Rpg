// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RpgFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URpgFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool CanSeeActor(const UWorld* World,
	                        FVector Location,
	                        const AActor* TargetActor,
	                        TArray<const AActor*> IgnoreActors = TArray<const AActor*>());

	static void DrawSightLine(const UWorld* World,
	                          FVector Start, FVector End, FColor Color, bool bPersistentLines,
	                          float LifeTime);
};
