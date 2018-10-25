// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCoreEnums.h"
#include "Widgets/SCompoundWidget.h"



/**
(0,0)				(0,8)
	* * * * * * * * *
	* * * * * * * * *
	* * * * * * * * *
	* * * * * * * * *
	* * * * * * * * *
	* * * * * * * * *	
	* * * * * * * * *
	* * * * * * * * *
	* * * * * * * * *
(8,0)				(8,8)

 */

DECLARE_DELEGATE_TwoParams(BordMoveDelegate,int,int);

DECLARE_DELEGATE(FinishDelegate);

class HNETTEST2_API SHNetGameCoreWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHNetGameCoreWidget)
	{}
	SLATE_END_ARGS()
	

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	bool IsMyRound = false;

	TSharedPtr<class SUniformGridPanel> Board;

	TSharedPtr<class SHNetFunctionAreaWidget> LeftFunctionArea;

	TSharedPtr<class SHNetFunctionAreaWidget> RightFunctionArea;

	TArray<TSharedPtr<class SHNetBoardItemWidget>> BoardItems;

	bool IsLeftSide;

	int MyFakeTargetNum = 7, MyTrojanHorseNum = 0, MyCoreObjectNum = 0;

	int EnemyFakeTargetNum = 7, EnemyTrojanHorseNum = 0, EnemyCoreObjectNum = 0;

	bool MyForceDelete = true, EnemyForceDelete = true;

	int SelectedItem = -1, SelectedCard = -1;

	static inline bool IsValidPoint(FIntPoint point);

	TSet<int> MovePoints;

public:

	BordMoveDelegate BoardCard;

	BordMoveDelegate BoardMove;

	FinishDelegate Win;

	FinishDelegate Lose;
	
	static float BoxSize;

	void EnemyMoved(int To, int From);

	void EnemyCard(int To, int Card);

	UFUNCTION()
	void CardSelected(ECardType Type);

	void RoomStart(bool IsMeGoingFirst, TArray<ECardType> InitBoard);

private:
	void BoardItemOnClicked(int id);

	int CheckWin();

	void RoundStart();

	void RoundEnd();

	void ResetAllMovePoint();

	void ShowMovePoint();

	void MeAttack(int To, int From);

	void MeShoot(int To, int From);

	static inline FIntPoint intToPoint(int id);

	static inline int PointToInt(FIntPoint point);

	static inline TArray<FIntPoint> GetCross(int id);

	static inline TArray<FIntPoint> GetX(int id);

	bool CheckIsWall(FIntPoint Move);

	bool CheckDown(FIntPoint BasePoint, FIntPoint DownVector, FIntPoint UpVector);

	bool CheckUp(int TraceDownCount, FIntPoint BasePoint, FIntPoint DownVector);
};
