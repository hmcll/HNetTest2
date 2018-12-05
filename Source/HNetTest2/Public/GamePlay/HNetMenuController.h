// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HNetMenuGameMode.h"
#include "HNetMenuController.generated.h"

/**
 * 
 */
UCLASS()
class HNETTEST2_API AHNetMenuController : public APlayerController ,public TSharedFromThis<AHNetMenuController>
{
	GENERATED_BODY()
	
public:
	AHNetMenuController();

	UFUNCTION()
	void StartMatching();
	
	UFUNCTION(Client, Reliable, WithValidation)
		void RoomStateChanged(ERoomState ToState);

	UFUNCTION(Client, Reliable, WithValidation)
		void OpponentLeft();
	
	ERoomState StateTo = ERoomState(-1);

	FVector2D MyColor = FVector2D(0,0.1);

	FVector2D EnemyColor = FVector2D(0.7,0.8);

	bool RoomEnd;

	void StartGame();

	void LeaveRoom();

	UFUNCTION()
	void Win();

	UFUNCTION()
	void Lose();

	void Tick(float DeltaTime)override;

	UFUNCTION()
		void MeMoved(int To, int From);

	UFUNCTION()
		void MeUsedCard(int To, int Card);

	UFUNCTION(Client, Reliable, WithValidation)
		void EnemyMoved(int To, int From);

	UFUNCTION(Client, Reliable, WithValidation)
		void EnemyUsedCard(int To, int Card);

protected:
	virtual void BeginPlay()override;
	
	
};
