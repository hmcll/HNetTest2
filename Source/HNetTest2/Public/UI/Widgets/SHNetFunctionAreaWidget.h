// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCoreEnums.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FunctionAreaButtonClickedDelegate,ECardType)

/**
 * 
 */
class HNETTEST2_API SHNetFunctionAreaWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHNetFunctionAreaWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FunctionAreaButtonClickedDelegate OnClicked;

	bool IsMyRound = false;

	ECardType SelectedCard = ECardType(-1);

	void CardSelection(ECardType Type);
};
