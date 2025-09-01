// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Rpg/Character/Controller/RpgPlayerController.h"

void URestartWidget::OnRestartClicked()
{
	ARpgPlayerController* PlayerController = Cast<ARpgPlayerController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideRestartWidget();
	}
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}

void URestartWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void URestartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (RestartButton != nullptr)
	{
		RestartButton->OnClicked.AddDynamic(this, &URestartWidget::OnRestartClicked);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &URestartWidget::OnExitClicked);
	}
}
