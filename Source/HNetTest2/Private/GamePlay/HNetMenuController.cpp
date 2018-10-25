// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetMenuController.h"
#include "HNetMenuHUD.h"
#include "SHNetGameCoreWidget.h"
#include "SHNetMenuWidget.h"
#include "EngineUtils.h"
#include "HNetMenuPlayerState.h"
#include "HNetMenuGameMode.h"

void AHNetMenuController::BeginPlay()
{
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

AHNetMenuController::AHNetMenuController() {
	bShowMouseCursor = true;
	RoomEnd = true;
}

void AHNetMenuController::StartMatching() {
	StaticCast<AHNetMenuPlayerState*>(PlayerState)->OnRequestMatching();
}

void AHNetMenuController::RoomStateChanged_Implementation(ERoomState ToState)
{
	StateTo = ToState;
}

bool AHNetMenuController::RoomStateChanged_Validate(ERoomState ToState)
{
	return true;
}

void AHNetMenuController::OpponentLeft_Implementation()
{
	if (!RoomEnd) {
		StaticCast<AHNetMenuHUD*>(GetHUD())->Fin(true);
	}
}

bool AHNetMenuController::OpponentLeft_Validate()
{
	return true;
}

void AHNetMenuController::StartGame() {

	auto HNetPlayerState = StaticCast<AHNetMenuPlayerState*>(PlayerState);

	bool MeGoingFirst = HNetPlayerState->MeGoingFirst;
	StaticCast<AHNetMenuHUD*>(GetHUD())->RoomStart(MeGoingFirst, HNetPlayerState->StartBoard);
}

void AHNetMenuController::LeaveRoom()
{
	auto HNetPlayerState = StaticCast<AHNetMenuPlayerState*>(PlayerState);
	HNetPlayerState->ClearUpAll();
}

void AHNetMenuController::Win(){
	StaticCast<AHNetMenuHUD*>(GetHUD())->Fin(true);
	RoomEnd = true;
}

void AHNetMenuController::Lose(){
	StaticCast<AHNetMenuHUD*>(GetHUD())->Fin(false);
	RoomEnd = true;
}

void AHNetMenuController::Tick(float DeltaTime)
{
	if (!IsRunningDedicatedServer()) {
		auto HNetPlayerState = StaticCast<AHNetMenuPlayerState*>(PlayerState);
		if (HNetPlayerState) {
			if (StateTo != ERoomState(-1) && HNetPlayerState->UpdateFlag) {
				switch (StateTo)
				{
				case ERoomState::Waiting:
					break;
				case ERoomState::Gaming:
					StartGame();
					RoomEnd = false;
					break;
				case ERoomState::Ended:
					break;
				default:
					break;
				}

				HNetPlayerState->ClearUpdateFlag();
				StateTo = ERoomState(-1);
			}
		}
	}
}

void AHNetMenuController::MeMoved(int To, int From) {
	FString Pharsed;
	Pharsed.AppendChar('M');
	Pharsed.AppendChar(To + 1);
	Pharsed.AppendChar(From + 1);
	StaticCast<AHNetMenuPlayerState*>(PlayerState)->OnMove(Pharsed);
}

void AHNetMenuController::MeUsedCard(int To, int Card) {
	FString Pharsed;
	Pharsed.AppendChar('C');
	Pharsed.AppendChar(To + 1);
	Pharsed.AppendChar(Card + 1);
	StaticCast<AHNetMenuPlayerState*>(PlayerState)->OnMove(Pharsed);
}

void AHNetMenuController::EnemyMoved_Implementation(int To, int From) {
	StaticCast<AHNetMenuHUD*>(GetHUD())->GameCoreWidget->EnemyMoved(To, From);
}

bool AHNetMenuController::EnemyMoved_Validate(int To, int From)
{
	return true;
}

void AHNetMenuController::EnemyUsedCard_Implementation(int To, int Card) {
	StaticCast<AHNetMenuHUD*>(GetHUD())->GameCoreWidget->EnemyCard(To, Card);
}

bool AHNetMenuController::EnemyUsedCard_Validate(int To, int Card)
{
	return true;
}
