// Fill out your copyright notice in the Description page of Project Settings.

#include "MainUIGameMode.h"

void AMainUIGameMode::BeginPlay() {
	Super::BeginPlay();
	ChangeWidget(StartingWidgetClass);
}

void AMainUIGameMode::ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass) {
	if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}
	}
}