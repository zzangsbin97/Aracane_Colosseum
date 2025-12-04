// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "MultiplaySessionsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplaySessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UMultiplaySessionsSubsystem();
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface;

//////////////////////////////////////////////////세션 생성
public:
	//세션 생성 함수
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();
	
	//세션 생성 후 콜백
	void OnCreateSessionComplete(FName SesisonName, bool bWasSuccessful);

private:
	//세션 생성 이벤트를 처리하기 위한 델리게이트
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
//////////////////////////////////////////////////


//////////////////////////////////////////////////세션 검색
public:
	//세션 검색 후 콜백
	void OnFindSessionsComplete(bool bWasSuccessful);

	//세션 검색이 완료되었을 때 실행될 델리게이트
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;

	//세션 검색을 수행하는 객체(검색 결과 저장)
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
//////////////////////////////////////////////////


//////////////////////////////////////////////////세션 참가
public:
	//게임 세션 참가
	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	//세션 참가 후 콜백
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
//////////////////////////////////////////////////

//////////////////////////////////////////////////세션 삭제
public:
	//세션 삭제
	UFUNCTION(BlueprintCallable)
	void DestroyGameSession();

	//세션 삭제 후 콜백
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

private:
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
//////////////////////////////////////////////////
};
