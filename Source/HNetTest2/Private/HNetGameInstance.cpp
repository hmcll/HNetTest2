// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetGameInstance.h"
#include "Engine.h"


FString UHNetGameInstance::GetServerIP() const{
	return ServerIP;
}

UGameUserSettings* UHNetGameInstance::GetGameUserSettings() {
	if (GEngine) {
		return GEngine->GameUserSettings;
	}
	return nullptr;
}

void UHNetGameInstance::Init()
{
	if (!IsDedicatedServerInstance()) {
#ifdef PLATFORM_WINDOWS
	GetGameUserSettings()->RequestResolutionChange(1600, 900, EWindowMode::Type::Windowed);
#endif // PLATFORM_WINDOWS

	}
}
