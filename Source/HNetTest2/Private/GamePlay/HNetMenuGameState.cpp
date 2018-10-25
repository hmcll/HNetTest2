// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetMenuGameState.h"
#include "HNetMenuGameMode.h"
#include "HNetMenuController.h"

void AHNetMenuGameState::OnRoomStateChanged(FRoom& room) {
	room.RoomClient->RoomStateChanged(room.RoomState);
	room.RoomHost->RoomStateChanged(room.RoomState);
	for (auto Controller : room.RoomVisitors) {
		Controller->RoomStateChanged(room.RoomState);
	}
}

void AHNetMenuGameState::OnRoomMoved(FRoom & room, bool isHostMoved)
{
	FString ThisMove = room.MoveStack.Last();
	auto CharArray = ThisMove.GetCharArray();
	AHNetMenuController* ControllerTo;
	if (isHostMoved) {
		ControllerTo = room.RoomClient;
	}
	else {
		ControllerTo = room.RoomHost;
	}
	if (CharArray[0] == 'M') {
		ControllerTo->EnemyMoved(CharArray[1]-1, CharArray[2]-1);
		//VisitorUpdate
	}
	else {
		ControllerTo->EnemyUsedCard(CharArray[1]-1, CharArray[2]-1);
		//VisitorUpdate
	}
}

void AHNetMenuGameState::OnOpponentLeft(AHNetMenuController* ControllerToCall)
{
	ControllerToCall->OpponentLeft();
}
