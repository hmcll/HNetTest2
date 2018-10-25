// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE(StartButtonClicked)

/**
 * 
 */
class HNETTEST2_API SHNetMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHNetMenuWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedPtr<SWidgetSwitcher> BaseSwitcher;
	
	StartButtonClicked StartButtonOnClickedDelegate;

	const struct FHNetMenuStyle* MenuStyle;
};
