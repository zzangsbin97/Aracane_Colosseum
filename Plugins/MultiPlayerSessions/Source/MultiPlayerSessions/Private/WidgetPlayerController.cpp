// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetPlayerController.h"

void AWidgetPlayerController::BeginPlay() {
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}