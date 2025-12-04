// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplaySessionsSubsystem.h"

UMultiplaySessionsSubsystem::UMultiplaySessionsSubsystem()
:CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)) {
	//Online Subsystem 가져오기
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	//Online Subsystem이 정상적으로 가져와졌다면, 세션 인터페이스 가져오기
	if (OnlineSubsystem) {
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		//디버그 메시지
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
				FString::Printf(TEXT("Find Session: %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
		}
	}
}

void UMultiplaySessionsSubsystem::CreateGameSession() {
	//세션 생성 불가능할 경우 return
	if (!OnlineSessionInterface.IsValid()) {
		//디버그 메시지
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
				FString::Printf(TEXT("OnlineSessionInterface.IsValid: %s"), OnlineSessionInterface.IsValid()));
		}
		return;
	}

	//세션이 이미 존재할 경우 삭제
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) {
		OnlineSessionInterface->DestroySession(NAME_GameSession);

		//디버그 메시지
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
			FString::Printf(TEXT("Destroyed Session")));
	}

	//세션 생성 완료 이벤트 핸들러 추가
	OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	//새로운 세션 설정 생성
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = false;
	SessionSettings->NumPublicConnections = 2;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	//세션 설정에서 MatchType이라는 키를 생성하고 BattleRoyale로 값 설정
	//ViaOnlineServiceAndPing: 세션 정보를 온라인 서비스에 등록, 핑 값을 기반으로 다른 플레이어에게 공개됨
	SessionSettings->Set(FName("MatchType"), FString("BattleRoyale"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	//현재 플레이어의 Net ID를 가져와 세션 생성 요청
	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void UMultiplaySessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	if (bWasSuccessful) {
		if (GEngine) {
			//디버그 메시지
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
					FString::Printf(TEXT("Created Session: %s"), *SessionName.ToString()));
			}
		}

		UWorld* World = GetWorld();
		if (World) {
			//ServerTravle을 수행해 해당 맵으로 이동
			World->ServerTravel(FString("/Game/Map/Lobby?Listen"));	

			//디버그 메시지
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue,
				FString::Printf(TEXT("Lobby")));
		}
	}
	else {
		//디버그 메시지
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
				TEXT("Failed to Create Session"));
		}
	}
}

void UMultiplaySessionsSubsystem::JoinGameSession() {
	//온라인 세션 인터페이스가 유효한지 확인
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}

	//세션 검색 완료 이벤트 델리게이트 등록
	//세션 검색이 완료되면 OnFindSessionsComplete() 실행
	OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	//새로운 세션 검색 객체 생성(TsharedPtr 사용하여 자동 메모리 관리)
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void UMultiplaySessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful) {
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}

	//검색된 모든 세션을 순회하며 정보 출력
	for (auto Result : SessionSearch->SearchResults) {
		//현재 검색된 세션의 ID를 문자열로 가져오기
		FString Id = Result.GetSessionIdStr();

		//현재 세션의 호스트 유저 이름 가져오기
		FString UserName = Result.Session.OwningUserName;

		FString MatchType;
		//SessionSettings에서 MatchType 키값을 가져와 변수에 저장
		//세션이 생성될 때 설정된 게임 모드를 불러옴
		Result.Session.SessionSettings.Get(FName("MatchType"), MatchType);

		//세션 정보 디버그 메시지
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
				FString::Printf(TEXT("Id: %s | UserName: %s"), *Id, *UserName));
		}
		if (MatchType == FString("BattleRoyale")) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
				FString::Printf(TEXT("Join Match Type: %s"), *MatchType));

			//세션 참가 완료 이벤트가 발생했을 때 실행할 델리게이트
			OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

			//스팀 멀티에서 중요
			Result.Session.SessionSettings.bUseLobbiesIfAvailable = true;	//클라이언트가 세션에 참가할 때, 로비 시스템을 사용할 수 있도록 설정
			Result.Session.SessionSettings.bUsesPresence = true;			//세션이 Presence를 활용하도록 설정(초대 기능)
			Result.Session.SessionSettings.bUseLobbiesIfAvailable = true;
			//

			//현재 월드에서 첫 번째 로컬 플레이어 객체를 가져옴
			//ULcoalPlayer에 로컬에서 조작하는 플레이어 정보 담음
			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

			//온라인 세션 인터페이스를 사용하여 특정 세션에 참가 요청 보내기
			//*LocalPlayer->GetPreferredUniqueNetId(): 현재 로컬 플레이어의 고유 네트워크 ID를 가져와 세션 참가 요청
			//Name_GameSession: 참가하려는 세션의 이름
			//Result: 참가하려는 세션의 검색 결과
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
		}
	}
}

void UMultiplaySessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (!OnlineSessionInterface.IsValid()) {
		return;
	}

	//서버의 접속 주소를 저장할 문자열 변수
	FString Address;

	//현재 참가한 세션의 접속 주소 가져오기
	//NAME_GameSession: 참가한 세션의 이름
	//Address: 세션의 접속 주소

	//디버그 메시지
	if (OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			FString::Printf(TEXT("Join Address: %s"), *Address));
	}

	//현재 로컬 플레이어의 컨트롤러 가져오기
	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PlayerController) {
		//클라이언트가 해당 주소로 이동하여 서버에 접속
		//ETravelType::TRAVEL_Absolute: 절대 주소 기반 이동 방식
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

void UMultiplaySessionsSubsystem::DestroyGameSession() {
	/*
	auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) {
		OnlineSessionInterface->DestroySession(NAME_GameSession);
		OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

		//디버그 메시지
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
				TEXT("Destroyed Session"));
		}
	}
	*/
}

void UMultiplaySessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {
	if (bWasSuccessful) {

	}
}