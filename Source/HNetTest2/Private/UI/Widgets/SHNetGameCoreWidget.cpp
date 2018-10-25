// Fill out your copyright notice in the Description page of Project Settings.

#include "SHNetGameCoreWidget.h"
#include "HorizontalBox.h"
#include "VerticalBox.h"
#include "SButton.h"
#include "SOverlay.h"
#include "SBox.h"
#include "SUniformGridPanel.h"
#include "SHNetBoardItemWidget.h"
#include "SHNetFunctionAreaWidget.h"
#include "HNetGameInstance.h"
#include "Engine.h"
#include "SlateOptMacros.h"
#include "SHNetGameCoreWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHNetGameCoreWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.FillWidth(3.5f)
		[
			SAssignNew(LeftFunctionArea,SHNetFunctionAreaWidget)
		]
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillWidth(9.f)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(STextBlock)
				.ColorAndOpacity(FColor::White)
				.Text_Lambda([&]() {return IsMyRound ? FText::FromString(TEXT("MyRound")) : FText::FromString(TEXT("EenmyRound")); })
			]
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
		]
		];
	BoardItems.SetNum(81, false);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {

			Board->AddSlot(i, j)
				[
					SNew(SBox)
					.WidthOverride(BoxSize)
				.HeightOverride(BoxSize)
				[
					SAssignNew(BoardItems[PointToInt(FIntPoint(j, i))], SHNetBoardItemWidget)
				]
				];
			;
			BoardItems[PointToInt(FIntPoint(j, i))]->id = PointToInt(FIntPoint(j, i));

			BoardItems[PointToInt(FIntPoint(j, i))]->OnClicked.BindRaw(this, &SHNetGameCoreWidget::BoardItemOnClicked);
		}
	}
}

float SHNetGameCoreWidget::BoxSize = 0.f;

void SHNetGameCoreWidget::RoomStart(bool IsMeGoingFirst, TArray<ECardType> InitBoard)
{
	IsMyRound = IsMeGoingFirst;
	IsLeftSide = IsMeGoingFirst;
	if (IsLeftSide) {
		LeftFunctionArea->OnClicked.BindRaw(this, &SHNetGameCoreWidget::CardSelected);
	}
	else {
		RightFunctionArea->OnClicked.BindRaw(this, &SHNetGameCoreWidget::CardSelected);
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
			BoardItems[PointToInt(FIntPoint(x, y))]->SetType(InitBoard[i], IsMeGoingFirst);
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
			BoardItems[PointToInt(FIntPoint(x, y))]->SetType(InitBoard[i], !IsMeGoingFirst);
			break;
		}
	}
	if (IsMyRound)
		RoundStart();
	else
		LeftFunctionArea->IsMyRound = true;


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
							MyCoreObjectNum--;
							MyFakeTargetNum++;
							BoardItems[id]->SetType(ECardType(SelectedCard), true);
							if (BoardItems[id]->IsTrap() && !BoardItems[id]->IsMine()) {
								BoardItems[id]->RemoveTrap();
								BoardItems[id]->Trapped();
							}
							break;
						case ECardType::TrojanHorse:
							MyTrojanHorseNum--;
							MyFakeTargetNum++;
							BoardItems[id]->SetType(ECardType(SelectedCard), true);
							if (BoardItems[id]->IsTrap() && !BoardItems[id]->IsMine()) {
								BoardItems[id]->RemoveTrap();
								BoardItems[id]->Trapped();
							}
							break;
						case ECardType::FakeTarget:
							MyFakeTargetNum--;
							BoardItems[id]->SetType(ECardType(SelectedCard), true);
							if (BoardItems[id]->IsTrap() && !BoardItems[id]->IsMine()) {
								BoardItems[id]->RemoveTrap();
								BoardItems[id]->Trapped();
							}
							break;
						case ECardType::Trap:
							BoardItems[id]->AddMyTrap();
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
				BoardCard.ExecuteIfBound(id, SelectedCard);
				ResetAllMovePoint();
				RoundEnd();
				return;
			}
		}
		else if (SelectedItem == -1) {//New Click
			if (BoardItems[id]->IsCard() && BoardItems[id]->IsMine()) {
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
			if (BoardItems[id]->IsCard() && BoardItems[id]->IsMine()) {
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
				if (i->IsMine() && i->GetType() == ECardType::FakeTarget) {
					i->AddMovePoint();
					MovePoints.Add(i->id);
				}
			}
			break;
		case ECardType::FakeTarget:
			for (auto i : BoardItems) {
				if (i->IsMine() && i->IsCard()) {
					auto Cross = GetCross(i->id);
					for (auto j : Cross) {
						if (IsValidPoint(j)) {
							if (!BoardItems[PointToInt(j)]->IsCard()) {
								BoardItems[PointToInt(j)]->AddMovePoint();
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
				if (i->IsMine() && i->IsCard()) {
					auto Cross = GetCross(i->id);
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
				if (i->IsTrap()&&i->IsMine()) {
					auto CenterCross = GetCross(i->id);
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
				BoardItems[i]->AddMovePoint();
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
		if (i->GetType() == ECardType::CoreObject) {
			if (i->IsMine()) {
				MyC++;
			}
			else {
				EnemyC++;
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

void SHNetGameCoreWidget::RoundStart() {
	int CheckWinRet = CheckWin();
	if (!CheckWinRet) {
		SelectedItem = -1;
		SelectedCard = -1;
		IsMyRound = true;
		if (IsLeftSide) {
			RightFunctionArea->IsMyRound = false;
			LeftFunctionArea->IsMyRound = true;
		}
		else {
			RightFunctionArea->IsMyRound = true;
			LeftFunctionArea->IsMyRound = false;
		}
		for (auto i : BoardItems) {
			if (i->IsCard() && i->IsMine() && i->IsTrapped) {
				i->FreeFromTrap();
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
			RightFunctionArea->IsMyRound = false;
			LeftFunctionArea->IsMyRound = true;
		}
		else {
			RightFunctionArea->IsMyRound = true;
			LeftFunctionArea->IsMyRound = false;
		}
		for (auto i : BoardItems) {
			if (i->IsCard() && !i->IsMine() && i->IsTrapped) {
				i->FreeFromTrap();
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


void SHNetGameCoreWidget::ResetAllMovePoint() {
	if(IsValidPoint(intToPoint(SelectedItem)))
		BoardItems[SelectedItem]->RemoveSelectedPoint();
	SelectedItem = -1;
	SelectedCard = -1;
	for (int i : MovePoints) {
		BoardItems[i]->RemoveMovePoint();
	}
	MovePoints.Reset();
}

void SHNetGameCoreWidget::ShowMovePoint() {
	FIntPoint Base = intToPoint(SelectedItem);
	BoardItems[SelectedItem]->AddSelectedPoint();
	TArray<FIntPoint> Cross = GetCross(SelectedItem);
	for (auto Move : Cross) {
		if (!IsValidPoint(Move))continue;
		if (BoardItems[PointToInt(Move)]->GetType() == ECardType::Delete) {
			BoardItems[PointToInt(Move)]->AddMovePoint();
			MovePoints.Add(PointToInt(Move));
		}
		else {
			if (BoardItems[PointToInt(Move)]->IsMine()) {
				int range = 0;
				FIntPoint BaseTemp = Move;
				FIntPoint MoveVector = Move - Base;
				while (IsValidPoint(BaseTemp + MoveVector) && BoardItems[PointToInt(BaseTemp)]->GetType() != ECardType::Delete && BoardItems[PointToInt(BaseTemp)]->IsMine()) {
					range++;
					BaseTemp += MoveVector;
				}
				while (IsValidPoint(BaseTemp + MoveVector) && BoardItems[PointToInt(BaseTemp)]->GetType() == ECardType::Delete && range) {
					range--;
					BaseTemp += MoveVector;
				}
				BoardItems[PointToInt(BaseTemp)]->AddMovePoint();
				MovePoints.Add(PointToInt(BaseTemp));
			}
			else {
				if (!CheckIsWall(Move)) {
					BoardItems[PointToInt(Move)]->AddMovePoint();
					MovePoints.Add(PointToInt(Move));
				}
			}
		}
	}
}

void SHNetGameCoreWidget::MeAttack(int To, int From) {
	switch (BoardItems[To]->GetType()) {
	case ECardType::CoreObject:
		MyCoreObjectNum++;
		break;
	case ECardType::FakeTarget:
		MyFakeTargetNum++;
		break;
	case ECardType::TrojanHorse:
		MyTrojanHorseNum++;
		break;
	}
	if (BoardItems[To]->IsTrapped)
		BoardItems[To]->FreeFromTrap();
	if (BoardItems[To]->IsTrap() && !BoardItems[To]->IsMine()) {
		BoardItems[To]->RemoveTrap();
		BoardItems[To]->Trapped();
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
	switch (BoardItems[To]->GetType()) {
	case ECardType::CoreObject:
		MyCoreObjectNum++;
		break;
	case ECardType::FakeTarget:
		MyFakeTargetNum++;
		break;
	case ECardType::TrojanHorse:
		MyTrojanHorseNum++;
		break;
	}
	if (BoardItems[To]->IsTrapped)
		BoardItems[To]->FreeFromTrap();
	for (int i = 0; i <= MoveNum; i++) {
		auto ThisMove = BoardItems[PointToInt(MoveVector*i + FromPoint)];
		if (ThisMove->IsTrap() && !ThisMove->IsMine()) {
			ThisMove->RemoveTrap();
			BoardItems[To]->Trapped();
		}
	}
	BoardItems[From]->Go(BoardItems[To]);
}

void SHNetGameCoreWidget::EnemyMoved(int To, int From) {
	auto p = intToPoint(To) - intToPoint(From);
	if (p.X + p.Y == 1) {
		switch (BoardItems[To]->GetType()) {
		case ECardType::CoreObject:
			EnemyCoreObjectNum++;
			break;
		case ECardType::FakeTarget:
			EnemyFakeTargetNum++;
			break;
		case ECardType::TrojanHorse:
			EnemyTrojanHorseNum++;
			break;
		}
		if (BoardItems[To]->IsTrapped)
			BoardItems[To]->FreeFromTrap();
		if (BoardItems[To]->IsTrap() && BoardItems[To]->IsMine()) {
			BoardItems[To]->RemoveTrap();
			BoardItems[To]->Trapped();
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
		switch (BoardItems[To]->GetType()) {
		case ECardType::CoreObject:
			EnemyCoreObjectNum++;
			break;
		case ECardType::FakeTarget:
			EnemyFakeTargetNum++;
			break;
		case ECardType::TrojanHorse:
			EnemyTrojanHorseNum++;
			break;
		}
		if (BoardItems[To]->IsTrapped)
			BoardItems[To]->FreeFromTrap();
		for (int i = 0; i <= MoveNum; i++) {
			auto ThisMove = BoardItems[PointToInt(MoveVector*i + FromPoint)];
			if (ThisMove->IsTrap() && ThisMove->IsMine()) {
				ThisMove->RemoveTrap();
				BoardItems[To]->Trapped();
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
		break;
	case ECardType::CoreObject:
		EnemyCoreObjectNum--;
		if (BoardItems[To]->IsTrap() && BoardItems[To]->IsMine()) {
			BoardItems[To]->RemoveTrap();
			BoardItems[To]->Trapped();
		}
		BoardItems[To]->SetType(ECardType(Card), false);
		break;
	case ECardType::TrojanHorse:
		EnemyTrojanHorseNum--;
		if (BoardItems[To]->IsTrap() && BoardItems[To]->IsMine()) {
			BoardItems[To]->RemoveTrap();
			BoardItems[To]->Trapped();
		}
		BoardItems[To]->SetType(ECardType(Card), false);
		break;
	case ECardType::FakeTarget:
		EnemyFakeTargetNum--;
		if (BoardItems[To]->IsTrap() && BoardItems[To]->IsMine()) {
			BoardItems[To]->RemoveTrap();
			BoardItems[To]->Trapped();
		}
		BoardItems[To]->SetType(ECardType(Card), false);
		break;
	case ECardType::Trap:
		BoardItems[To]->AddEnemyTrap();
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
