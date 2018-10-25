// Fill out your copyright notice in the Description page of Project Settings.

#include "SHNetMenuWidget.h"
#include "SlateOptMacros.h"
#include "SBorder.h"
#include "SWidgetSwitcher.h"
#include "STextBlock.h"
#include "SBox.h"
#include "SImage.h"
#include "SButton.h"
#include "HNetStyle.h"
#include "HNetMenuWidgetStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHNetMenuWidget::Construct(const FArguments& InArgs)
{
	MenuStyle = &HNetStyle::Get().GetWidgetStyle<FHNetMenuStyle>("BPHNetMenuStyle");
	ChildSlot
	[
		SNew(SBorder)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderImage(&MenuStyle->MenuBackGroundBrush)
		[
			SAssignNew(BaseSwitcher,SWidgetSwitcher)
			+SWidgetSwitcher::Slot() // Home 0
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.FillHeight(1.f)
				[
					SNew(SImage)
					.Image(&MenuStyle->TitleBrush)
				]
				+SVerticalBox::Slot()
				.FillHeight(0.5)
				+SVerticalBox::Slot()
				.FillHeight(1.f)
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
					.WidthOverride(600.f)
					[
					SNew(SButton)
					.ButtonStyle(&MenuStyle->TitleButtonStyle)
					.OnClicked_Lambda([&]() {
						StartButtonOnClickedDelegate.ExecuteIfBound();
						BaseSwitcher->SetActiveWidgetIndex(1);
						return FReply::Handled();
					}) 
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
					SNew(STextBlock)
					.Font(MenuStyle->MenuFont_Button)
					.ColorAndOpacity(FColor::White)
					.Text(FText::FromString(TEXT("START")))
					]
					]
				]
				+SVerticalBox::Slot()
				.FillHeight(0.3)
				+SVerticalBox::Slot()
				.FillHeight(1.f)
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
					.WidthOverride(600.f)
					[
					SNew(SButton)
					.ButtonStyle(&MenuStyle->TitleButtonStyle)
					.OnClicked_Lambda([&]() {
//						StartButtonOnClickedDelegate.ExecuteIfBound();
//						BaseSwitcher->SetActiveWidgetIndex(1);
						return FReply::Handled();
					}) 
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
					SNew(STextBlock)
					.Font(MenuStyle->MenuFont_Button)
					.ColorAndOpacity(FColor::White)
					.Text(FText::FromString(TEXT("JOIN")))
					]
					]
				]
			]
			+ SWidgetSwitcher::Slot() // LOADING 1
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Font(MenuStyle->MenuFont_Loading)
				.Text(FText::FromString(TEXT("LOADING")))
			]
		]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
