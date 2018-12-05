// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCoreEnums.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

struct BoardItem{
	//Properties Travel With Pawn
	bool IsFlipped;
	bool IsMine;
	ECardType Type = ECardType::Delete;

	//Properties Modified when Travelling
	bool IsNearTrap;
	bool IsTrapped;

	//Maintained By Board
	bool IsMovePoint;
	bool IsSelected;
	bool IsMyTrap;
	bool IsEnemyTrap;
	bool Misted;
	int ID;
	inline void Delete() {
		IsFlipped = IsMine = IsMovePoint = IsSelected = IsTrapped = IsMyTrap = IsEnemyTrap = IsNearTrap = false;
		Type = ECardType::Delete;
	}

	BoardItem() {
		Delete();
	}

	inline bool IsCard()const {
		return int(Type) >= 1 && int(Type) <= 3;
	}
	inline void Go(BoardItem* to) {
		to->Type = Type;
		to->IsFlipped = IsFlipped;
		to->IsMine = IsMine;
		to->IsNearTrap = false;
		Type = ECardType::Delete;
		IsFlipped = false;
		IsMine = false;
		IsNearTrap = false;
		IsTrapped = false;
	}
};

DECLARE_DELEGATE_OneParam(OnClickedDelegate,int)

class HNETTEST2_API SHNetBoardItemWidget : public SCompoundWidget, public FGCObject
{

	BoardItem Data;

public:
	SLATE_BEGIN_ARGS(SHNetBoardItemWidget)
	{}
	SLATE_ARGUMENT(FVector2D, MyColor)
	SLATE_ARGUMENT(FVector2D, EnemyColor)
	SLATE_ARGUMENT(int, ID)
	SLATE_ARGUMENT(class SHNetGameCoreWidget*, Board)
	SLATE_ARGUMENT(bool*, IsMyRound)
	SLATE_ARGUMENT(BoardItem**, DataPointer)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	OnClickedDelegate OnClicked;

	UPROPERTY()
	UMaterialInstanceDynamic* PawnMaterialInstance;

	FSlateBrush* PawnBrush;
	void AddReferencedObjects(FReferenceCollector& Collector)override;

	bool* IsMyRound;
};
