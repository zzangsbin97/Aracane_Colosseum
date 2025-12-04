// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MainUIGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API AMainUIGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;
};
