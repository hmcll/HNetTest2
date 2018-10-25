// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HNetMenuGameMode.h"
#include "HNetMenuGameState.generated.h"

/**
 * 
 */
UCLASS()
class HNETTEST2_API AHNetMenuGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	void OnRoomStateChanged(FRoom& room);
	
	void OnRoomMoved(FRoom& room, bool isHostMoved);

	void OnOpponentLeft(AHNetMenuController* ControllerToCall);
};


