// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameCoreEnums.h"
#include "HNetMenuGameMode.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(RoomStateChangeDelegate)

DECLARE_MULTICAST_DELEGATE(RoomMovedDelegate)


class AHNetMenuController;

USTRUCT()
struct FRoom {
	GENERATED_BODY();
	AHNetMenuController* RoomHost;
	AHNetMenuController* RoomClient;
	TArray<AHNetMenuController*> RoomVisitors;
	TArray<FString> MoveStack;
	ERoomState RoomState = ERoomState::Waiting;
};

UCLASS()
class HNETTEST2_API AHNetMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHNetMenuGameMode();
	
	TMap<int, FRoom> Rooms;

	int RoomID = 1000;

	void Logout(AController* PlayerLeaving) override;

	void OnRequestMatching(AHNetMenuController* Controller);

	void OnMove(FString Move, bool isHost, int Roomid);

	void RemoveRoom(int RoomID);

	void BeginPlay() override;
};
