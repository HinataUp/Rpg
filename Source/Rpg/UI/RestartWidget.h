// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class RPG_API URestartWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnExitClicked();

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ExitButton;
};
