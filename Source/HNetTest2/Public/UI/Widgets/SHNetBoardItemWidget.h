// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCoreEnums.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

struct BoardItem{
	bool IsFlipped = false;
	bool isMine = false;
	ECardType Type = ECardType::Delete;
};

DECLARE_DELEGATE_OneParam(OnClickedDelegate,int)

class HNETTEST2_API SHNetBoardItemWidget : public SCompoundWidget
{
	bool _IsMovePoint;

	bool _IsTrapped;

	bool _IsTrap;

	BoardItem Data;

public:
	SLATE_BEGIN_ARGS(SHNetBoardItemWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	TSharedPtr<class STextBlock> Tag;
	TSharedPtr<class STextBlock> Player;
	TSharedPtr<class STextBlock> Trap;
	TSharedPtr<class STextBlock> MovePoint;
	TSharedPtr<class STextBlock> IsTrapped;

	int id;

	bool IsMovePoint();

	bool IsMine();

	bool IsCard();

	bool IsTrap();

	void AddMovePoint();

	void RemoveMovePoint();

	void AddSelectedPoint();

	void RemoveSelectedPoint();

	void AddMyTrap();

	void Trapped();

	void Delete();

	void FreeFromTrap();

	void Go(TSharedPtr<SHNetBoardItemWidget> To);

	void AddEnemyTrap();

	void RemoveTrap();

	void SetType(ECardType In, bool IsMine);

	void SetType(BoardItem data);

	ECardType GetType();
	
	OnClickedDelegate OnClicked;
};
