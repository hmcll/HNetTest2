// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HNetClientEntryController.generated.h"

/**
 * 
 */
UCLASS()
class HNETTEST2_API AHNetClientEntryController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void BeginPlay()override;
	
};
