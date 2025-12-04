// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WidgetPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API AWidgetPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
};
