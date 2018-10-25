// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetMenuGameMode.h"
#include "HNetMenuPlayerState.h"
#include "HNetMenuGameState.h"
#include "HNetMenuController.h"
#include "GameCoreEnums.h"
#include "HNetMenuHUD.h"
#include "UnrealMathUtility.h"

AHNetMenuGameMode::AHNetMenuGameMode() {
	PlayerControllerClass = AHNetMenuController::StaticClass();
	HUDClass = AHNetMenuHUD::StaticClass();
	PlayerStateClass = AHNetMenuPlayerState::StaticClass();
	GameStateClass = AHNetMenuGameState::StaticClass();
}

void AHNetMenuGameMode::BeginPlay()
{
}

void AHNetMenuGameMode::Logout(AController* PlayerLeaving) {
	AHNetMenuController* Controller = StaticCast<AHNetMenuController*>(PlayerLeaving);
	if (Controller) {
		AHNetMenuPlayerState* State = StaticCast<AHNetMenuPlayerState*>(PlayerLeaving->PlayerState);
		if (State) {
			if (FRoom* RoomToLeave = Rooms.Find(State->RoomID)) {
				if (RoomToLeave->RoomClient == Controller) {
					StaticCast<AHNetMenuGameState*>(GameState)->OnOpponentLeft(RoomToLeave->RoomHost);
				}
				else if (RoomToLeave->RoomHost == Controller) {
					StaticCast<AHNetMenuGameState*>(GameState)->OnOpponentLeft(RoomToLeave->RoomClient);
				}
			}
		}
	}
}

void AHNetMenuGameMode::OnRequestMatching(AHNetMenuController* Controller)
{
	if (Controller) {
		for (auto& i : Rooms) {
			if (i.Value.RoomHost) {
				i.Value.RoomClient = Controller;
				i.Value.RoomState = ERoomState::Gaming;
				StaticCast<AHNetMenuPlayerState*>(Controller->PlayerState)->IsHost = false;
				StaticCast<AHNetMenuPlayerState*>(Controller->PlayerState)->RoomID = i.Key;
				TArray<ECardType> StartBoard;
				for (int i = 0; i < 3; i++) {
					StartBoard.Push(ECardType::CoreObject);
					StartBoard.Push(ECardType::FakeTarget);
					StartBoard.Push(ECardType::TrojanHorse);
					StartBoard.Push(ECardType::Delete);
					StartBoard.Push(ECardType::Delete);
					StartBoard.Push(ECardType::Delete);
				}
				TArray<ECardType> OutBoard;

				auto state = StaticCast<AHNetMenuGameState*>(GameState);

				StartBoard.Sort([](ECardType, ECardType) {return FMath::RandBool(); }); // Shuffle
				OutBoard.Append(StartBoard);
				StartBoard.Sort([](ECardType, ECardType) {return FMath::RandBool(); }); // Shuffle
				OutBoard.Append(StartBoard);

				auto HostState = StaticCast<AHNetMenuPlayerState*>(i.Value.RoomHost->PlayerState);
				auto ClientState = StaticCast<AHNetMenuPlayerState*>(Controller->PlayerState);

				HostState->StartBoard = OutBoard;
				ClientState->StartBoard = OutBoard;

				HostState->UpdateFlag = true;
				ClientState->UpdateFlag = true;

				bool HostGointFirst = FMath::RandBool();
				if (HostGointFirst) {
					HostState->MeGoingFirst = true;
					ClientState->MeGoingFirst = false;
				}
				else {
					HostState->MeGoingFirst = false;
					ClientState->MeGoingFirst = true;
				}
				state->OnRoomStateChanged(i.Value);
				UE_LOG(LogGameMode, Warning, TEXT("RoomStart%d"), i.Key);
				return;

			}
		}
		Rooms.Add(RoomID);
		Rooms[RoomID].RoomHost = Controller;
		StaticCast<AHNetMenuPlayerState*>(Controller->PlayerState)->IsHost = true;
		StaticCast<AHNetMenuPlayerState*>(Controller->PlayerState)->RoomID = RoomID;
		Rooms[RoomID].RoomState = ERoomState::Waiting;
		UE_LOG(LogGameMode, Warning, TEXT("RoomWaiting%d"), RoomID);
		RoomID++;
	}
	else {
		UE_LOG(LogActor, Warning, TEXT("Invalid Controller"))
	}
}

void AHNetMenuGameMode::OnMove(FString Move, bool isHost, int Roomid)
{
	Rooms[Roomid].MoveStack.Push(Move);
	StaticCast<AHNetMenuGameState*>(GameState)->OnRoomMoved(Rooms[Roomid], isHost);
}

void AHNetMenuGameMode::RemoveRoom(int RoomID)
{
	if (Rooms.Find(RoomID)) {
		Rooms.Remove(RoomID);
		UE_LOG(LogGameMode, Warning, TEXT("RoomRemoved%d"),RoomID);
	}
}
