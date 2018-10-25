// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetClientEntryController.h"
#include "HNetGameInstance.h"
#include "SHNetMenuWidget.h"


void AHNetClientEntryController::BeginPlay() {

	UHNetGameInstance* Instance = StaticCast<UHNetGameInstance*>(GetGameInstance());
	if (Instance) {
		ClientTravel(Instance->GetServerIP(),ETravelType::TRAVEL_Absolute);
		
	}
}