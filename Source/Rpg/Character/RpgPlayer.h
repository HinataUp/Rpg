// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RpgCharacterBase.h"
#include "RpgPlayer.generated.h"

UCLASS()
class RPG_API ARpgPlayer : public ARpgCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARpgPlayer();
	void BeginPlay();
	void Tick(float DeltaTime);


	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// 存储动画蓝图类
	UPROPERTY()
	TSubclassOf<UAnimInstance> LoadedAnimBPClass;

protected:
	UFUNCTION(BlueprintCallable, Category = "Character Setup")
	void LoadSkeletalMesh(const FString& MeshPath1);

	UFUNCTION(BlueprintCallable, Category = "Character Setup")
	void LoadAnimation(const FString& AnimationPath);
};
