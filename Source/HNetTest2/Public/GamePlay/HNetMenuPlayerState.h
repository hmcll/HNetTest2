// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameCoreEnums.h"
#include "HNetMenuPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HNETTEST2_API AHNetMenuPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AHNetMenuPlayerState();

	UPROPERTY(Replicated)
		bool UpdateFlag = false;

	UPROPERTY(Replicated)
		bool MeGoingFirst;

	UPROPERTY(Replicated)
		int RoomID = -1;

	UPROPERTY(Replicated)
		bool IsHost;

	UPROPERTY(Replicated)
		TArray<ECardType> StartBoard;

	UFUNCTION(Reliable,Server, WithValidation)
		void OnRequestMatching();
	
	UFUNCTION(Reliable, Server, WithValidation)
		void OnMove(const FString& Move);

	UFUNCTION(Reliable, Server, WithValidation)
		void ClearUpdateFlag();

	UFUNCTION(Reliable, Server, WithValidation)
		void ClearUpAll();


};
