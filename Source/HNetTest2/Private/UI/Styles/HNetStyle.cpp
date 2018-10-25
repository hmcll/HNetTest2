// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetStyle.h"
#include "SlateGameResources.h"

TSharedPtr<FSlateStyleSet> HNetStyle::HNetStyleInstance = NULL;

void HNetStyle::Initalize()
{
	if(!HNetStyleInstance.IsValid()) {
		HNetStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*HNetStyleInstance);
	}
}

FName HNetStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("HNetStyle"));
	return StyleSetName;
}

void HNetStyle::ShutDown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*HNetStyleInstance);
	ensure(HNetStyleInstance.IsUnique());
	HNetStyleInstance.Reset();
}

ISlateStyle & HNetStyle::Get()
{
	return *HNetStyleInstance;
}

TSharedRef<FSlateStyleSet> HNetStyle::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(HNetStyle::GetStyleSetName(), "/Game/Style", "/Game/Style");
	return StyleRef;
}
