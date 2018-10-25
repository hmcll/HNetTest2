// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetTest2.h"
#include "Modules/ModuleManager.h"
#include "HNetStyle.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FHNetTest2Module, HNetTest2, "HNetTest2" );

void FHNetTest2Module::StartupModule()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(HNetStyle::GetStyleSetName());
	HNetStyle::Initalize();
}

void FHNetTest2Module::ShutdownModule()
{
	HNetStyle::ShutDown();
}
