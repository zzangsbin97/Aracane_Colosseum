// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MultiplaySessionsSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainUIGameMode.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

protected:
	virtual bool Initialize() override;

private:
	APlayerController* PlayerController;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UFUNCTION(BlueprintCallable)
	void HostButtonClicked();

	UFUNCTION(BlueprintCallable)
	void JoinButtonClicked();

	UFUNCTION(BlueprintCallable)
	void SettingButtonClicked();

	UFUNCTION(BlueprintCallable)
	void QuitButtonClicked();

	//멀티플레이 세션을 관리하는 서브시스템 클래스의 포인터
	class UMultiplaySessionsSubsystem* MultiplaySessionsSubsystem;
};
