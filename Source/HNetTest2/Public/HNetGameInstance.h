// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HNetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HNETTEST2_API UHNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	FString GetServerIP()const;

	void Init()override;

	UGameUserSettings* GetGameUserSettings();
private:
	UPROPERTY(config)
		FString ServerIP;
	

};
