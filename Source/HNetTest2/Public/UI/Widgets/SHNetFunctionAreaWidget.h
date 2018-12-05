// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCoreEnums.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FunctionAreaButtonClickedDelegate,ECardType)

/**
 * 
 */
class HNETTEST2_API SHNetFunctionAreaWidget : public SCompoundWidget, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SHNetFunctionAreaWidget)
	{}
	SLATE_ARGUMENT(bool,IsLeft)
	SLATE_ARGUMENT(bool*, HUDIsMyRound)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FunctionAreaButtonClickedDelegate OnClicked;

	int FakeTargetNum = 7, TrojanHorseNum = 0, CoreObjectNum = 0;

	int ForceDeleteCountDown = 0;

	bool IsMyRound = false;
	bool* HUDIsMyRound;

	
	ECardType SelectedCard = ECardType(-1);

	UPROPERTY()
	UMaterialInstanceDynamic* PawnSelectionSectionMaterialInstance;

	UPROPERTY()
	UMaterialInstanceDynamic* DeleteMaterialInstance;

	UPROPERTY()
	UMaterialInstanceDynamic* TrapMaterialInstance;

	FSlateBrush* PawnSelectionSectionBrush;

	FSlateBrush* DeleteBrush;

	FSlateBrush* TrapBrush;

	bool IsLeft;

	void CardSelection(ECardType Type);

	void AddReferencedObjects(FReferenceCollector& Collector)override;

	void RoundStart();

	void RoundEnd();
};
