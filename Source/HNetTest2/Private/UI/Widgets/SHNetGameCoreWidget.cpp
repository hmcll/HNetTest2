// Fill out your copyright notice in the Description page of Project Settings.

#include "SHNetGameCoreWidget.h"
#include "SBoxPanel.h"
#include "SButton.h"
#include "SOverlay.h"
#include "SBox.h"
#include "SBox.h"
#include "SUniformGridPanel.h"
#include "SHNetBoardItemWidget.h"
#include "SHNetFunctionAreaWidget.h"
#include "HNetGameInstance.h"
#include "Engine.h"
#include "SlateOptMacros.h"
#include "HNetStyle.h"
#include "HNetMenuWidgetStyle.h"
#include "SHNetGameCoreWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHNetGameCoreWidget::Construct(const FArguments& InArgs)
{

	auto MenuStyle = &HNetStyle::Get().GetWidgetStyle<FHNetMenuStyle>("BPHNetMenuStyle");
	ChildSlot
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderImage(&MenuStyle->MenuBackGroundBrush)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillWidth(3.5f)
			[
				SAssignNew(LeftFunctionArea,SHNetFunctionAreaWidget)
				.IsLeft(true)
				.HUDIsMyRound(&IsMyRound)
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(9.f)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.Padding(BoxSize / 4.f)
				[
					SAssignNew(Board, SUniformGridPanel)
					.MinDesiredSlotHeight(BoxSize)
					.MinDesiredSlotWidth(BoxSize)
				]
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillWidth(3.5f)
			[
				SAssignNew(RightFunctionArea, SHNetFunctionAreaWidget)
				.HUDIsMyRound(&IsMyRound)
				.IsLeft(false)
			]
		]
	];

}

float SHNetGameCoreWidget::BoxSize = 0.f;

void SHNetGameCoreWidget::RoomStart(bool IsMeGoingFirst, TArray<ECardType> InitBoard, FVector2D MyColor, FVector2D EnemyColor)
{
	BoardItems.SetNum(81, false);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {

			Board->AddSlot(i, j)
				[
					SNew(SBox)
					.WidthOverride(BoxSize)
					.HeightOverride(BoxSize)
					[
						SNew(SHNetBoardItemWidget)
						.ID(PointToInt(FIntPoint(j, i)))
						.Board(this)
						.DataPointer(&BoardItems[PointToInt(FIntPoint(j, i))])
						.MyColor(MyColor)
						.EnemyColor(EnemyColor)
						.IsMyRound(&IsMyRound)
					]
				];
		}
	}
	IsMyRound = IsMeGoingFirst;
	IsLeftSide = IsMeGoingFirst;
	if (IsLeftSide) {
		LeftFunctionArea->OnClicked.BindRaw(this, &SHNetGameCoreWidget::CardSelected);
		MyFakeTargetNum = &LeftFunctionArea->FakeTargetNum;
		MyTrojanHorseNum = &LeftFunctionArea->TrojanHorseNum;
		MyCoreObjectNum = &LeftFunctionArea->CoreObjectNum;
		MyForceDeleteCountDown = &LeftFunctionArea->ForceDeleteCountDown;
		EnemyFakeTargetNum = &RightFunctionArea->FakeTargetNum;
		EnemyTrojanHorseNum = &RightFunctionArea->TrojanHorseNum;
		EnemyCoreObjectNum = &RightFunctionArea->CoreObjectNum;
		EnemyForceDeleteCountDown = &RightFunctionArea->ForceDeleteCountDown;
	}
	else {
		RightFunctionArea->OnClicked.BindRaw(this, &SHNetGameCoreWidget::CardSelected);
		MyFakeTargetNum = &RightFunctionArea->FakeTargetNum;
		MyTrojanHorseNum = &RightFunctionArea->TrojanHorseNum;
		MyCoreObjectNum = &RightFunctionArea->CoreObjectNum;
		MyForceDeleteCountDown = &RightFunctionArea->ForceDeleteCountDown;
		EnemyFakeTargetNum = &LeftFunctionArea->FakeTargetNum;
		EnemyTrojanHorseNum = &LeftFunctionArea->TrojanHorseNum;
		EnemyCoreObjectNum = &LeftFunctionArea->CoreObjectNum;
		EnemyForceDeleteCountDown = &LeftFunctionArea->ForceDeleteCountDown;
	}
	for (int i = 0; i < 18; i++) {
		switch (InitBoard[i]) {
		case ECardType::Delete:
			break;
		case ECardType::CoreObject:
		case ECardType::FakeTarget:
		case ECardType::TrojanHorse:
			int y = i / 9;
			int x = i % 9;
			BoardItems[PointToInt(FIntPoint(x, y))]->Type = InitBoard[i];
			BoardItems[PointToInt(FIntPoint(x, y))]->IsMine = IsMeGoingFirst;
			break;
		}
	}
	for (int i = 18; i < 36; i++) {
		switch (InitBoard[i]) {
		case ECardType::Delete:
			break;
		case ECardType::CoreObject:
		case ECardType::FakeTarget:
		case ECardType::TrojanHorse:
			int y = (i + 45) / 9;
			int x = (i + 45) % 9;
			BoardItems[PointToInt(FIntPoint(x, y))]->Type = InitBoard[i];
			BoardItems[PointToInt(FIntPoint(x, y))]->IsMine = !IsMeGoingFirst;
			break;
		}
	}
	if (IsMyRound)
		RoundStart();
	else {
		LeftFunctionArea->IsMyRound = true;
		ClearMist();
	}
}

inline FIntPoint SHNetGameCoreWidget::intToPoint(int id)
{
	return FIntPoint(id % 9, id / 9);
}

inline int SHNetGameCoreWidget::PointToInt(FIntPoint point)
{
	return point.Y * 9 + point.X;
}

inline TArray<FIntPoint> SHNetGameCoreWidget::GetCross(int id)
{
	FIntPoint BasePoint = intToPoint(id);

	return TArray<FIntPoint>({
		FIntPoint(BasePoint.X + 1,BasePoint.Y),
		FIntPoint(BasePoint.X,BasePoint.Y + 1),
		FIntPoint(BasePoint.X - 1,BasePoint.Y),
		FIntPoint(BasePoint.X,BasePoint.Y - 1)
		});
}

inline TArray<FIntPoint> SHNetGameCoreWidget::GetX(int id)
{
	FIntPoint BasePoint = intToPoint(id);

	return TArray<FIntPoint>({
		FIntPoint(BasePoint.X + 1,BasePoint.Y + 1),
		FIntPoint(BasePoint.X + 1,BasePoint.Y - 1),
		FIntPoint(BasePoint.X - 1,BasePoint.Y + 1),
		FIntPoint(BasePoint.X - 1,BasePoint.Y - 1)
		});
}

inline bool SHNetGameCoreWidget::IsValidPoint(FIntPoint point)
{
	return point.X >= 0 && point.X < 9 && point.Y >= 0 && point.Y < 9;
}

void SHNetGameCoreWidget::BoardItemOnClicked(int id) {
	if (IsMyRound) {
		if (SelectedItem == id) {//Cancel Click
			ResetAllMovePoint();
		}
		else if (SelectedCard != -1) {//Card Click
			if (SelectedCard) { //Other Selections
				for (auto i : MovePoints) {
					if (id == i) {
						switch (ECardType(SelectedCard))
						{
						case ECardType::CoreObject:
							(*MyCoreObjectNum)--;
							(*MyFakeTargetNum)++;
							BoardItems[id]->Type = ECardType(SelectedCard);
							BoardItems[id]->IsMine = true;
							if (BoardItems[id]->IsEnemyTrap) {
								BoardItems[id]->IsEnemyTrap = false;
								BoardItems[id]->IsFlipped = true;
								BoardItems[id]->IsTrapped = true;
							}
							break;
						case ECardType::TrojanHorse:
							(*MyTrojanHorseNum)--;
							(*MyFakeTargetNum)++;
							BoardItems[id]->Type = ECardType(SelectedCard);
							BoardItems[id]->IsMine = true;
							if (BoardItems[id]->IsEnemyTrap) {
								BoardItems[id]->IsEnemyTrap = false;
								BoardItems[id]->IsFlipped = true;
								BoardItems[id]->IsTrapped = true;
							}
							break;
						case ECardType::FakeTarget:
							(*MyFakeTargetNum)--;
							BoardItems[id]->Type = ECardType(SelectedCard);
							BoardItems[id]->IsMine = true;
							if (BoardItems[id]->IsEnemyTrap) {
								BoardItems[id]->IsEnemyTrap = false;
								BoardItems[id]->IsFlipped = true;
								BoardItems[id]->IsTrapped = true;
							}
							break;
						case ECardType::Trap:
							BoardItems[id]->IsMyTrap = true;
							break;
						default:
							break;
						}
						BoardCard.ExecuteIfBound(id, SelectedCard);
						ResetAllMovePoint();
						RoundEnd();
						return;
					}
				}
			}
			else { // Remove
				BoardItems[id]->Delete();
				(*MyForceDeleteCountDown) = 20;
				BoardCard.ExecuteIfBound(id, SelectedCard);
				RoundEnd();
				return;
			}
		}
		else if (SelectedItem == -1) {//New Click
			if (BoardItems[id]->IsCard() && BoardItems[id]->IsMine && !(BoardItems[id]->IsTrapped)) {
				SelectedItem = id;
				ShowMovePoint();
			}
		}
		else {// MoveClick
			for (auto i : MovePoints) {
				if (id == i) {
					MeAttack(i, SelectedItem);
					BoardMove.ExecuteIfBound(id, SelectedItem);
					ResetAllMovePoint();
					RoundEnd();
					return;
				}
			}
			if (BoardItems[id]->IsCard() && BoardItems[id]->IsMine) {
				ResetAllMovePoint();
				SelectedItem = id;
				ShowMovePoint();
			}
			return;
		}
	}
}

void SHNetGameCoreWidget::CardSelected(ECardType Type)
{
	if (IsMyRound) {
		if (SelectedItem != -1 || SelectedCard != -1) {
			if (SelectedCard == int(Type)) {
				ResetAllMovePoint();
				return;
			}
			ResetAllMovePoint();
		}
		switch (Type)
		{
		case ECardType::Delete:
			break;
		case ECardType::CoreObject:
		case ECardType::TrojanHorse:
			for (auto i : BoardItems) {
				if (i->IsMine && i->Type == ECardType::FakeTarget) {
					i->IsMovePoint = true;
					MovePoints.Add(i->ID);
				}
			}
			break;
		case ECardType::FakeTarget:
			for (auto i : BoardItems) {
				if (i->IsMine && i->IsCard()) {
					auto Cross = GetCross(i->ID);
					for (auto j : Cross) {
						if (IsValidPoint(j)) {
							if (!BoardItems[PointToInt(j)]->IsCard()) {
								BoardItems[PointToInt(j)]->IsMovePoint = true;
								MovePoints.Add(PointToInt(j));
							}
						}
					}
				}
			}
			break;
		case ECardType::Trap:
		{
			TSet<int> MovePointStack;
			for (auto i : BoardItems) {
				if (i->IsMine && i->IsCard()) {
					auto Cross = GetCross(i->ID);
					for (auto j : Cross) {
						if (IsValidPoint(j)) {
							if (!BoardItems[PointToInt(j)]->IsCard()) {
								MovePointStack.Add(PointToInt(j));
							}
						}
					}
				}
			}
			for (auto i : BoardItems) {
				if (i->IsMyTrap) {
					auto CenterCross = GetCross(i->ID);
					for (auto CenterCrossIter : CenterCross) {
						if(IsValidPoint(CenterCrossIter))
							MovePointStack.Remove(PointToInt(CenterCrossIter));
							auto SecondCross = GetCross(PointToInt(CenterCrossIter));
							for (auto SecondCrossIter : SecondCross) {
								if (IsValidPoint(SecondCrossIter))
									MovePointStack.Remove(PointToInt(SecondCrossIter));
							}
					}
				}
			}
			for (int i : MovePointStack) {
				MovePoints.Add(i);
				BoardItems[i]->IsMovePoint = true;
			}
		}
			break;
		default:
			break;
		}
	}
	SelectedCard = int(Type);
}

int SHNetGameCoreWidget::CheckWin()
{
	int MyC = 0, EnemyC = 0;
	for (auto i : BoardItems) {
		if (i->Type == ECardType::CoreObject) {
			if (i->IsMine) {
				MyC++;
			}
			else {
				EnemyC++;
			}
		}
		if (i->IsCard()) {
			auto Cross = GetCross(i->ID);
			for (auto j : Cross) {
				if(IsValidPoint(j))
					if (i->IsMine && BoardItems[PointToInt(j)]->IsEnemyTrap) {
						i->IsNearTrap = true;
					}
					else if (!i->IsMine && BoardItems[PointToInt(j)]->IsMyTrap) {
						i->IsNearTrap = true;
					}
			}
		}
	}
	if (!MyC) {
		return 2;
	}
	if (!EnemyC) {
		return 1;
	}
	return 0;
}

void SHNetGameCoreWidget::ClearMist() {

	for (auto i : BoardItems) {
		i->Misted = true;
	}
	for (auto i : BoardItems) {
		if (i->IsMine) {
			i->Misted = false;
			if (i->IsCard()) {
				auto Cross = GetCross(i->ID);
				for (FIntPoint j : Cross) {
					if (IsValidPoint(j)) {
						BoardItems[PointToInt(j)]->Misted = false;
					}
				}
			}
		}
		else if(i->IsMyTrap) {
			auto Cross = GetCross(i->ID);
			for (FIntPoint j : Cross) {
				if (IsValidPoint(j)) {
					if(BoardItems[PointToInt(j)]->IsCard())
						BoardItems[PointToInt(j)]->Misted = false;
				}
			}
		}
		else if(i->IsTrapped){
			i->Misted = false;
		}

	}
}

void SHNetGameCoreWidget::RoundStart() {
	ClearMist();
	int CheckWinRet = CheckWin();
	if (!CheckWinRet) {
		SelectedItem = -1;
		SelectedCard = -1;
		IsMyRound = true;
		if (IsLeftSide) {
			RightFunctionArea->RoundEnd();
			LeftFunctionArea->RoundStart();
		}
		else {
			RightFunctionArea->RoundStart();
			LeftFunctionArea->RoundEnd();
		}
		for (auto i : BoardItems) {
			if (i->IsMine&& i->IsTrapped) {
				if (i->IsFlipped) {
					i->IsTrapped = false;
				}
				else {
					i->IsFlipped = true;
				}
			}
		}
	}
	else {
		if (CheckWinRet == 1) {
			Win.ExecuteIfBound();
		}
		else {
			Lose.ExecuteIfBound();
		}
	}
}

void SHNetGameCoreWidget::RoundEnd() {
	int CheckWinRet = CheckWin();
	if (!CheckWinRet) {
		IsMyRound = false;
		if (!IsLeftSide) {
			RightFunctionArea->RoundEnd();
			LeftFunctionArea->RoundStart();
		}
		else {
			RightFunctionArea->RoundStart();
			LeftFunctionArea->RoundEnd();
		}
		for (auto i : BoardItems) {
			if (!(i->IsMine)&& i->IsTrapped) {
				if (i->IsFlipped) {
					i->IsTrapped = false;
				}
				else {
					i->IsFlipped = true;
				}
			}
		}
	}
	else {
		if (CheckWinRet == 1) {
			Win.ExecuteIfBound();
		}
		else {
			Lose.ExecuteIfBound();
		}
	}
	ClearMist();
}

void SHNetGameCoreWidget::ResetAllMovePoint() {
	if(IsValidPoint(intToPoint(SelectedItem)))
		BoardItems[SelectedItem]->IsSelected = false;
	SelectedItem = -1;
	SelectedCard = -1;
	for (int i : MovePoints) {
		BoardItems[i]->IsMovePoint = false;
	}
	MovePoints.Reset();
}

void SHNetGameCoreWidget::ShowMovePoint() {
	FIntPoint Base = intToPoint(SelectedItem);
	BoardItems[SelectedItem]->IsSelected = true;
	TArray<FIntPoint> Cross = GetCross(SelectedItem);
	for (auto Move : Cross) {
		if (!IsValidPoint(Move))continue;
		if (BoardItems[PointToInt(Move)]->Type == ECardType::Delete) {
			BoardItems[PointToInt(Move)]->IsMovePoint = true;
			MovePoints.Add(PointToInt(Move));
		}
		else {
			if (BoardItems[PointToInt(Move)]->IsMine) {
				int range = 0;
				FIntPoint BaseTemp = Move;
				FIntPoint MoveVector = Move - Base;
				while (IsValidPoint(BaseTemp + MoveVector) && BoardItems[PointToInt(BaseTemp)]->Type != ECardType::Delete && BoardItems[PointToInt(BaseTemp)]->IsMine) {
					range++;
					BaseTemp += MoveVector;
				}
				while (IsValidPoint(BaseTemp + MoveVector) && BoardItems[PointToInt(BaseTemp)]->Type == ECardType::Delete && range) {
					range--;
					BaseTemp += MoveVector;
				}
				BoardItems[PointToInt(BaseTemp)]->IsMovePoint = true;
				MovePoints.Add(PointToInt(BaseTemp));
			}
			else {
				if (!CheckIsWall(Move)) {
					BoardItems[PointToInt(Move)]->IsMovePoint = true;
					MovePoints.Add(PointToInt(Move));
				}
			}
		}
	}
}

void SHNetGameCoreWidget::MeAttack(int To, int From) {
	switch (BoardItems[To]->Type) {
	case ECardType::CoreObject:
		(*MyCoreObjectNum)++;
		break;
	case ECardType::FakeTarget:
		(*MyFakeTargetNum)++;
		break;
	case ECardType::TrojanHorse:
		(*MyTrojanHorseNum)++;
		break;
	}
	BoardItems[To]->IsTrapped = false;
	if (BoardItems[To]->IsEnemyTrap) {
		BoardItems[To]->IsEnemyTrap = false;
		BoardItems[To]->IsTrapped = true;
	}
	BoardItems[From]->Go(BoardItems[To]);
}

void SHNetGameCoreWidget::MeShoot(int To, int From) {
	int MoveNum;
	FIntPoint MoveVector;
	FIntPoint FromPoint, ToPoint;
	FromPoint = intToPoint(From);
	ToPoint = intToPoint(To);
	FIntPoint MoveRoute = ToPoint - FromPoint;
	if (!MoveRoute.X || !MoveRoute.Y) {//cross
		MoveNum = abs(MoveRoute.X + MoveRoute.Y);
		MoveVector = MoveRoute / MoveNum;
	}
	else {			   //X
		MoveNum = abs(MoveRoute.X);
		MoveVector = MoveRoute / MoveNum;
	}
	switch (BoardItems[To]->Type) {
	case ECardType::CoreObject:
		(*MyCoreObjectNum)++;
		break;
	case ECardType::FakeTarget:
		(*MyFakeTargetNum)++;
		break;
	case ECardType::TrojanHorse:
		(*MyTrojanHorseNum)++;
		break;
	}
	BoardItems[To]->IsTrapped = false;
	for (int i = 0; i <= MoveNum; i++) {
		auto ThisMove = BoardItems[PointToInt(MoveVector*i + FromPoint)];
		if (ThisMove->IsEnemyTrap) {
			ThisMove->IsEnemyTrap = false;
			BoardItems[To]->IsTrapped = true;
		}
	}
	BoardItems[From]->Go(BoardItems[To]);
}

void SHNetGameCoreWidget::EnemyMoved(int To, int From) {
	auto p = intToPoint(To) - intToPoint(From);
	if (p.X + p.Y == 1) {
		switch (BoardItems[To]->Type) {
		case ECardType::CoreObject:
			(*EnemyCoreObjectNum)++;
			break;
		case ECardType::FakeTarget:
			(*EnemyFakeTargetNum)++;
			break;
		case ECardType::TrojanHorse:
			(*EnemyTrojanHorseNum)++;
			break;
		}
		BoardItems[To]->IsTrapped = false;
		if (BoardItems[To]->IsMyTrap) {
			BoardItems[To]->IsMyTrap = false;
			BoardItems[To]->IsTrapped = true;
		}
		BoardItems[From]->Go(BoardItems[To]);
	}
	else { // Shooted
		int MoveNum;
		FIntPoint MoveVector;
		FIntPoint FromPoint, ToPoint;
		FromPoint = intToPoint(From);
		ToPoint = intToPoint(To);
		FIntPoint MoveRoute = ToPoint - FromPoint;
		if (!MoveRoute.X || !MoveRoute.Y) {//cross
			MoveNum = abs(MoveRoute.X + MoveRoute.Y);
			MoveVector = MoveRoute / MoveNum;
		}
		else {			   //X
			MoveNum = abs(MoveRoute.X);
			MoveVector = MoveRoute / MoveNum;
		}
		switch (BoardItems[To]->Type) {
		case ECardType::CoreObject:
			(*EnemyCoreObjectNum)++;
			break;
		case ECardType::FakeTarget:
			(*EnemyFakeTargetNum)++;
			break;
		case ECardType::TrojanHorse:
			(*EnemyTrojanHorseNum)++;
			break;
		}
		BoardItems[To]->IsTrapped = false;
		for (int i = 0; i <= MoveNum; i++) {
			auto ThisMove = BoardItems[PointToInt(MoveVector*i + FromPoint)];
			if (ThisMove->IsMyTrap) {
				ThisMove->IsMyTrap = false;
				BoardItems[To]->IsTrapped = true;
			}
		}
		BoardItems[From]->Go(BoardItems[To]);
	}
	RoundStart();
}

void SHNetGameCoreWidget::EnemyCard(int To, int Card) {

	switch (ECardType(Card))
	{
	case ECardType::Delete:
		BoardItems[To]->Delete();
		(*EnemyForceDeleteCountDown) = 20;
		break;
	case ECardType::CoreObject:
		(*EnemyCoreObjectNum)--;
		if (BoardItems[To]->IsMyTrap) {
			BoardItems[To]->IsMyTrap = false;
			BoardItems[To]->IsFlipped = true;
			BoardItems[To]->IsTrapped = true;
		}
		BoardItems[To]->Type = ECardType(Card);
		BoardItems[To]->IsMine = false;
		break;
	case ECardType::TrojanHorse:
		(*EnemyTrojanHorseNum)--;
		if (BoardItems[To]->IsMyTrap) {
			BoardItems[To]->IsMyTrap = false;
			BoardItems[To]->IsFlipped = true;
			BoardItems[To]->IsTrapped = true;
		}
		BoardItems[To]->Type = ECardType(Card);
		BoardItems[To]->IsMine = false;
		break;
	case ECardType::FakeTarget:
		(*EnemyFakeTargetNum)--;
		if (BoardItems[To]->IsMyTrap) {
			BoardItems[To]->IsMyTrap = false;
			BoardItems[To]->IsFlipped = true;
			BoardItems[To]->IsTrapped = true;
		}
		BoardItems[To]->Type = ECardType(Card);
		BoardItems[To]->IsMine = false;
		break;
	case ECardType::Trap:
		BoardItems[To]->IsEnemyTrap = true;
		break;
	default:
		break;
	}
	RoundStart();
}

bool SHNetGameCoreWidget::CheckUp(int TraceDownCount, FIntPoint BasePoint, FIntPoint DownVector) {
	do {
		if (IsValidPoint(BasePoint) && BoardItems[PointToInt(BasePoint)]->IsCard());
		else return false;
		TraceDownCount--;
		BasePoint += DownVector;
	} while (TraceDownCount);
	return true;
}

bool SHNetGameCoreWidget::CheckDown(FIntPoint BasePoint, FIntPoint DownVector, FIntPoint UpVector) {
	for (int TraceDownCount = 1; IsValidPoint(BasePoint) && BoardItems[PointToInt(BasePoint)]->IsCard(); BasePoint += DownVector, TraceDownCount++) {
		if (CheckUp(TraceDownCount, BasePoint + UpVector, UpVector))return true;
	}
	return false;
}

bool SHNetGameCoreWidget::CheckIsWall(FIntPoint Move)
{
	FVector2D MoveVector = Move - intToPoint(SelectedItem);
	FVector2D LeftUpVec = MoveVector.GetRotated(45), RightUpVec = MoveVector.GetRotated(-45), LeftDownVec = MoveVector.GetRotated(135), RightDownVec = MoveVector.GetRotated(-135);
	FIntPoint LeftUp = FIntPoint((LeftUpVec * 2).X, (LeftUpVec * 2).Y), RightUp = FIntPoint((RightUpVec * 2).X, (RightUpVec * 2).Y), LeftDown = FIntPoint((LeftDownVec * 2).X, (LeftDownVec * 2).Y), RightDown = FIntPoint((RightDownVec * 2).X, (RightDownVec * 2).Y);
	if (IsValidPoint(Move + LeftUp) && BoardItems[PointToInt(Move + LeftUp)]->IsCard() && IsValidPoint(Move + RightUp) && BoardItems[PointToInt(Move + RightUp)]->IsCard()) {
		return true;
	}
	else {
		if (CheckDown(Move + RightDown, RightDown, RightUp))return true;
		else return CheckDown(Move + LeftDown, LeftDown, LeftUp);
	}
	return false;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
