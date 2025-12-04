// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"

void UMenu::MenuSetup() {
	//위젯을 뷰포트에 추가해 화면에 표시되도록 만듦
	AddToViewport();

	//위젯 가시성 설정
	SetVisibility(ESlateVisibility::Visible);

	//위젯이 포커스 받을 수 있도록 설정
	bIsFocusable = true;

	//현재 월드 객체를 가져옴
	UWorld* World = GetWorld();
	if (World) {
		//첫 번째 플레이어 컨트롤러 가져오기
		PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			//UI 모드 전용 입력을 위한 설정 구조체 생성
			FInputModeUIOnly InputModeData;

			//현재 위젯을 포커스할 대상으로 설정
			InputModeData.SetWidgetToFocus(TakeWidget());

			//마우스 커서를 화면 밖으로 이동하지 못하도록 설정하지 않음
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			//플레이어 컨트롤러에 UI 전용 입력 모드 적용
			PlayerController->SetInputMode(InputModeData);

			//마우스 커서를 화면에 표시하도록 설정
			PlayerController->SetShowMouseCursor(true);
		}
	}
	//게임 인스턴스를 가져와 멀티플레이 세션 서브시스템을 설정
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) {
		//GameInstance에 연결된 UMultiplaySessionsSubsystem 가져오기
		MultiplaySessionsSubsystem = GameInstance->GetSubsystem<UMultiplaySessionsSubsystem>();
	}
}

bool UMenu::Initialize() {
	if (!Super::Initialize()) {
		return false;
	}

	//HostButton이 존재하면 클릭 이벤트 바인딩
	if (HostButton) {
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	//JoinButton이 존재하면 클릭 이벤트 바인딩
	if (JoinButton) {
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	//SettingButton이 존재하면 클릭 이벤트 바인딩
	if (SettingButton) {
		SettingButton->OnClicked.AddDynamic(this, &ThisClass::SettingButtonClicked);
	}

	//QuitButton이 존재하면 클릭 이벤트 바인딩
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonClicked);
	}

	return true;
}

void UMenu::HostButtonClicked() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			TEXT("HostButtonClicked"));
	}

	if (MultiplaySessionsSubsystem) {
		MultiplaySessionsSubsystem->CreateGameSession();
		//플레이어 컨트롤러에 UI 전용 입력 모드 적용
		//PlayerController->SetInputMode(InputModeData);

		//마우스 커서 숨기기
		PlayerController->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

void UMenu::JoinButtonClicked() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			TEXT("JoinButtonClicked"));

	}
	if (MultiplaySessionsSubsystem) {
		MultiplaySessionsSubsystem->JoinGameSession();

		//마우스 커서 숨기기
		PlayerController->SetShowMouseCursor(false);

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

void UMenu::SettingButtonClicked() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green,
			TEXT("SettingButtonClicked"));
	}
}

void UMenu::QuitButtonClicked() {
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}