// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUI.h"

void ULobbyUI::UISetUp() {
	AddToViewport();

	SetVisibility(ESlateVisibility::Visible);

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
}

bool ULobbyUI::Initialize() {
	if (!Super::Initialize()) {
		return false;
	}

	if (LeftButton) {
		LeftButton->OnClicked.AddDynamic(this, &ThisClass::LeftButtonClicked);
	}

	if (RightButton) {
		RightButton->OnClicked.AddDynamic(this, &ThisClass::RightButtonClicked);
	}

	if (StartButton) {
		StartButton->OnClicked.AddDynamic(this, &ThisClass::StartButtonClicked);
	}

	if (MainMenuButton) {
		MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::MainMenuButtonClicked);
	}

	return true;
}

void ULobbyUI::LeftButtonClicked() {

}

void ULobbyUI::RightButtonClicked() {

}

void ULobbyUI::StartButtonClicked() {

}

void ULobbyUI::MainMenuButtonClicked() {
}