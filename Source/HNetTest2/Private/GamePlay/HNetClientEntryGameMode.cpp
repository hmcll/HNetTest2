// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetClientEntryGameMode.h"
#include "HNetClientEntryController.h"

AHNetClientEntryGameMode::AHNetClientEntryGameMode() {
	PlayerControllerClass = AHNetClientEntryController::StaticClass();
}


