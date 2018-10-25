// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetMenuHUD.h"
#include "SHNetMenuWidget.h"
#include "HNetMenuController.h"
#include "SHNetGameCoreWidget.h"
#include "HNetMenuPlayerState.h"
#include "HNetGameInstance.h"
#include "GameCoreEnums.h"
#include "SOpponentLeftWidget.h"
#include "SlateBasics.h"
#include "Engine.h"


AHNetMenuHUD::AHNetMenuHUD() {
	if(GEngine&&GEngine->GameViewport) {
		SAssignNew(MenuWidget,SHNetMenuWidget);
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
	}
}

void AHNetMenuHUD::BeginPlay()
{
	auto PlayerController = dynamic_cast<AHNetMenuController*>(GetOwningPlayerController());
	MenuWidget->StartButtonOnClickedDelegate.BindUObject(PlayerController, &AHNetMenuController::StartMatching);
}

void AHNetMenuHUD::RoomStart(bool MeGoingFirst, TArray<ECardType> InitBoard)
{
	if (auto Instance = StaticCast<UHNetGameInstance*>(GetWorld()->GetGameInstance()))
		if (auto GameUserSettings = Instance->GetGameUserSettings())
			SHNetGameCoreWidget::BoxSize = (GameUserSettings->GetScreenResolution().Y)/ 9.5f;
	if (GEngine&&GEngine->GameViewport) {
		GEngine->GameViewport->RemoveViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
		MenuWidget.Reset();
		auto PlayerController = dynamic_cast<AHNetMenuController*>(GetOwningPlayerController());
		SAssignNew(GameCoreWidget, SHNetGameCoreWidget);
		GameCoreWidget->BoardMove.BindUObject(PlayerController, &AHNetMenuController::MeMoved);
		GameCoreWidget->BoardCard.BindUObject(PlayerController, &AHNetMenuController::MeUsedCard);
		GameCoreWidget->Win.BindUObject(PlayerController, &AHNetMenuController::Win);
		GameCoreWidget->Lose.BindUObject(PlayerController, &AHNetMenuController::Lose);
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameCoreWidget.ToSharedRef()));
		GameCoreWidget->RoomStart(MeGoingFirst,InitBoard);
	}

}

void AHNetMenuHUD::Fin(bool Win){
	if (GEngine&&GEngine->GameViewport) {
		if (!OpponentLeftWidget) {
			SAssignNew(OpponentLeftWidget, SOpponentLeftWidget);
			OpponentLeftWidget->LeaveRoom.BindUObject(this, &AHNetMenuHUD::LeaveRoom);
		}
		OpponentLeftWidget->Win = Win;
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(OpponentLeftWidget.ToSharedRef()));
	}
}

void AHNetMenuHUD::LeaveRoom(){
	if (GEngine&&GEngine->GameViewport) {
		GEngine->GameViewport->RemoveViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameCoreWidget.ToSharedRef()));
		GameCoreWidget.Reset();
		SAssignNew(MenuWidget, SHNetMenuWidget);
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
	}
	auto PlayerController = dynamic_cast<AHNetMenuController*>(GetOwningPlayerController());
	PlayerController->LeaveRoom();
}
