// Fill out your copyright notice in the Description page of Project Settings.

#include "SHNetFunctionAreaWidget.h"
#include "HorizontalBox.h"
#include "VerticalBox.h"
#include "SButton.h"
#include "SOverlay.h"
#include "SBox.h"
#include "SImage.h"
#include "GameCoreEnums.h"
#include "HNetStyle.h"
#include "HNetMenuWidgetStyle.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHNetFunctionAreaWidget::Construct(const FArguments& InArgs)
{
	auto MenuStyle = &HNetStyle::Get().GetWidgetStyle<FHNetMenuStyle>("BPHNetMenuStyle");

	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(25.f, 50.f)
				.AutoWidth()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(100.f)
					[
						SNew(SButton)
						.OnClicked_Lambda([&]() {CardSelection(ECardType::Trap); return FReply::Handled(); })
					]
				]
				+ SHorizontalBox::Slot()
				.Padding(25.f, 50.f)
				.AutoWidth()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
					.HeightOverride(100.f)
					.WidthOverride(100.f)
					[
						SNew(SButton)
						.OnClicked_Lambda([&]() {CardSelection(ECardType::Delete); return FReply::Handled(); })
					]
				]
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(50.f)
			[
				SNew(SBox)
				.HeightOverride(200.f)
				.WidthOverride(200.f)
				[
					SNew(SButton)
					.OnClicked_Lambda([&]() {CardSelection(ECardType::FakeTarget); return FReply::Handled(); })
				]
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(50.f)
			[
				SNew(SBox)
				.HeightOverride(200.f)
				.WidthOverride(200.f)
				[
					SNew(SButton)
					.OnClicked_Lambda([&]() {CardSelection(ECardType::CoreObject); return FReply::Handled(); })
				]
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Padding(50.f)
			[
				SNew(SBox)
				.HeightOverride(200.f)
				.WidthOverride(200.f)
				[
					SNew(SButton)
					.OnClicked_Lambda([&]() {CardSelection(ECardType::TrojanHorse); return FReply::Handled(); })
				]
			]
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Visibility_Lambda([&]() {return IsMyRound ? EVisibility::Collapsed : EVisibility::Visible; })
			.Image(&MenuStyle->FunctionAreaCoverBrush)
		]
	];
	
}
void SHNetFunctionAreaWidget::CardSelection(ECardType Type)
{
	OnClicked.ExecuteIfBound(Type);
	if (int(SelectedCard) != -1) {
		switch (Type)
		{
		case ECardType::Delete:
			break;
		case ECardType::CoreObject:
			break;
		case ECardType::TrojanHorse:
			break;
		case ECardType::FakeTarget:
			break;
		case ECardType::Trap:
			break;
		default:
			break;
		}
	}
	SelectedCard = Type;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
