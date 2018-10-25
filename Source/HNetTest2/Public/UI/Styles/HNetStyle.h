// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"

/**
 * 
 */
class HNETTEST2_API HNetStyle
{
public:
	static void Initalize();

	static FName GetStyleSetName();

	static void ShutDown();

	static ISlateStyle& Get();
	
private:
	static TSharedRef<FSlateStyleSet> Create();

	static TSharedPtr<FSlateStyleSet> HNetStyleInstance;
};
