// Fill out your copyright notice in the Description page of Project Settings.

#include "SHNetBoardItemWidget.h"
#include "SButton.h"
#include "STextBlock.h"
#include "VerticalBox.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHNetBoardItemWidget::Construct(const FArguments& InArgs)
{

	ChildSlot
	[
		SNew(SButton)
		.OnClicked_Lambda([&]() {
			if(!_IsTrapped)
			switch (Data.Type)
			{
			case ECardType::Delete:
				break;
			case ECardType::CoreObject:
			case ECardType::TrojanHorse:
			case ECardType::FakeTarget:
				OnClicked.ExecuteIfBound(id);
				return FReply::Handled();
				break;
			case ECardType::Trap:
				break;
			default:
				break;
			}
			if (_IsMovePoint)
				OnClicked.ExecuteIfBound(id);
			return FReply::Handled();
		})
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SAssignNew(Tag,STextBlock)
			]
			+SVerticalBox::Slot()
			[
				SAssignNew(Player,STextBlock)
			]
			+SVerticalBox::Slot()
			[
				SAssignNew(Trap,STextBlock)
			]
			+SVerticalBox::Slot()
			[
				SAssignNew(MovePoint,STextBlock)
			]
			+ SVerticalBox::Slot()
			[
				SAssignNew(IsTrapped, STextBlock)
			]
		]
	];
	
}
bool SHNetBoardItemWidget::IsMovePoint()
{
	return _IsMovePoint;
}

bool SHNetBoardItemWidget::IsMine() {
	return Data.isMine;
}

bool SHNetBoardItemWidget::IsCard()
{
	return Data.Type!=ECardType::Delete&&Data.Type!=ECardType::Trap;
}

bool SHNetBoardItemWidget::IsTrap()
{
	return _IsTrap;
}

void SHNetBoardItemWidget::AddMovePoint(){
	_IsMovePoint = true;
	MovePoint->SetText(FString("MovePoint"));
}

void SHNetBoardItemWidget::RemoveMovePoint(){
	_IsMovePoint = false;
	MovePoint->SetText(FString(""));
}

void SHNetBoardItemWidget::AddSelectedPoint(){
	_IsMovePoint = true;
	MovePoint->SetText(FString("SelectedPoint"));
}

void SHNetBoardItemWidget::RemoveSelectedPoint(){
	_IsMovePoint = false;
	MovePoint->SetText(FString(""));
}

void SHNetBoardItemWidget::AddMyTrap(){
	Player->SetText(FString("M"));
	Data.isMine = true;
	_IsTrap = true;
	Trap->SetText(FString("Trap"));
}

void SHNetBoardItemWidget::AddEnemyTrap() {
	Player->SetText(FString("E"));
	Data.isMine = false;
	_IsTrap = true;
	Trap->SetText(FString("Trap"));
}

void SHNetBoardItemWidget::RemoveTrap() {
	if (Data.Type == ECardType::Delete)
		Player->SetText(FString(""));
	_IsTrap = false;
	Trap->SetText(FString(""));
}

void SHNetBoardItemWidget::Trapped()
{
	_IsTrapped = true;
	Data.IsFlipped = true;
	IsTrapped->SetText(FString("IsTrapped"));
}

void SHNetBoardItemWidget::Delete() {
	Data = BoardItem();
	SetType(Data);
	RemoveTrap();
	FreeFromTrap();
}


void SHNetBoardItemWidget::FreeFromTrap()
{
	_IsTrapped = false;
	IsTrapped->SetText(FString(""));
}

void SHNetBoardItemWidget::Go(TSharedPtr<SHNetBoardItemWidget> To)
{
	bool ismine = Data.isMine;
	To->Data = Data;
	To->SetType(Data);
	Data = BoardItem();
	SetType(Data);
	if (IsTrap())
		if (ismine)
			AddMyTrap();
		else
			AddEnemyTrap();
}


void SHNetBoardItemWidget::SetType(ECardType In, bool IsMine)
{
	Data.isMine = IsMine;
	if (IsMine) {
		Player->SetText(FString("M"));
	}
	else {
		Player->SetText(FString("E"));
	}
	switch (In)
	{
	case ECardType::Delete:
		Tag->SetText(FString(""));
		break;
	case ECardType::CoreObject:
		Tag->SetText(FString("CoreObject"));
		break;
	case ECardType::TrojanHorse:
		Tag->SetText(FString("TrojanHorse"));
		break;
	case ECardType::FakeTarget:
		Tag->SetText(FString("FakeTarget"));
		break;
	default:
		break;
	}
	Data.Type = In;
}
void SHNetBoardItemWidget::SetType(BoardItem data)
{
	if (data.Type != ECardType::Delete) {
		if (data.isMine) {
			Player->SetText(FString("M"));
		}
		else {
			Player->SetText(FString("E"));
		}
	}
	else {
		Player->SetText(FString(""));
	}
	switch (data.Type)
	{
	case ECardType::Delete:
		Tag->SetText(FString(""));
		break;
	case ECardType::CoreObject:
		Tag->SetText(FString("CoreObject"));
		break;
	case ECardType::TrojanHorse:
		Tag->SetText(FString("TrojanHorse"));
		break;
	case ECardType::FakeTarget:
		Tag->SetText(FString("FakeTarget"));
		break;
	default:
		break;
	}
}
ECardType SHNetBoardItemWidget::GetType()
{
	return Data.Type;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
