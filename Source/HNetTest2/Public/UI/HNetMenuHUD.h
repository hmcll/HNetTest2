// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameCoreEnums.h"
#include "HNetMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class HNETTEST2_API AHNetMenuHUD : public AHUD
{
	GENERATED_BODY()
public:

	AHNetMenuHUD();

	TSharedPtr<class SHNetMenuWidget> MenuWidget;
	
	TSharedPtr<class SHNetGameCoreWidget> GameCoreWidget;

	TSharedPtr<class SOpponentLeftWidget> OpponentLeftWidget;

	void BeginPlay()override;

	void RoomStart(bool MeGoingFirst, TArray<ECardType> InitBoard);

	void Fin(bool Win);

	void LeaveRoom();
};
