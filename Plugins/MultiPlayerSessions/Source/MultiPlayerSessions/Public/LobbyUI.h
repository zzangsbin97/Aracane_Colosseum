// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplaySessionsSubsystem.h"
#include "LobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UISetUp();

public:
	APlayerController* PlayerController;

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* LeftButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RightButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UFUNCTION(BlueprintCallable)
	void LeftButtonClicked();

	UFUNCTION(BlueprintCallable)
	void RightButtonClicked();

	UFUNCTION(BlueprintCallable)
	void MainMenuButtonClicked();

	UFUNCTION(BlueprintCallable)
	void StartButtonClicked();
};
