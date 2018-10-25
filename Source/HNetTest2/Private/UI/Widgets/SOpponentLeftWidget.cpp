// Fill out your copyright notice in the Description page of Project Settings.

#include "SOpponentLeftWidget.h"
#include "SOverlay.h"
#include "SButton.h"
#include "SImage.h"
#include "HNetMenuWidgetStyle.h"
#include "HNetStyle.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SOpponentLeftWidget::Construct(const FArguments& InArgs)
{
	auto MenuStyle = &HNetStyle::Get().GetWidgetStyle<FHNetMenuStyle>("BPHNetMenuStyle");
	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNew(SImage)
			.ColorAndOpacity(MenuStyle->OpponentLeftBackground)
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.FillHeight(1)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
			SNew(STextBlock)
			.Font(MenuStyle->MenuFont_Button)
			.ColorAndOpacity(FColor::White)
			.Text_Lambda([&]() {return FText::FromString(Win ? TEXT("YOU WIN") : TEXT("YOU LOSE")); })
			]
			+SVerticalBox::Slot()
			.FillHeight(0.5)
			+SVerticalBox::Slot()
			.FillHeight(1)
			[
				SNew(SBox)
				.HeightOverride(300.f)
				.WidthOverride(700.f)
				[
					SNew(SButton)
					.ButtonStyle(&MenuStyle->TitleButtonStyle)
					.OnClicked_Lambda([&]() {
						LeaveRoom.ExecuteIfBound();
						return FReply::Handled();
					})
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Font(MenuStyle->MenuFont_Button)
						.Text(FText::FromString(TEXT("EXIT")))
					]
				]
			]
		]
	];

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
