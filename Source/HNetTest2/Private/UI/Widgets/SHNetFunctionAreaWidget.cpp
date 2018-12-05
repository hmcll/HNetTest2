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
#include "HNetGameWidgetStyle.h"
#include "SlateOptMacros.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHNetFunctionAreaWidget::Construct(const FArguments& InArgs)
{
	auto Style = &HNetStyle::Get().GetWidgetStyle<FHNetGameStyle>("BPHNetGameStyle");
	PawnSelectionSectionMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GEngine->GetWorld(),Style->PawnSelectionAreaMat);
	PawnSelectionSectionMaterialInstance->SetVectorParameterValue(FName(TEXT("State")), FLinearColor(TrojanHorseNum == 0, CoreObjectNum == 0, FakeTargetNum == 0, 0));
	PawnSelectionSectionMaterialInstance->SetScalarParameterValue(FName(TEXT("IsLeftArea")), InArgs._IsLeft);
	PawnSelectionSectionBrush = new FSlateBrush();
	PawnSelectionSectionBrush->SetResourceObject(PawnSelectionSectionMaterialInstance);
	DeleteMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GEngine->GetWorld(),Style->Delete);
	DeleteMaterialInstance->SetScalarParameterValue(FName(TEXT("IsLeftArea")), InArgs._IsLeft);
	DeleteBrush = new FSlateBrush();
	DeleteBrush->SetResourceObject(DeleteMaterialInstance);
	TrapMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GEngine->GetWorld(),Style->Trap);
	TrapMaterialInstance->SetScalarParameterValue(FName(TEXT("IsLeftArea")), InArgs._IsLeft);
	TrapBrush = new FSlateBrush();
	TrapBrush->SetResourceObject(TrapMaterialInstance);
	HUDIsMyRound = InArgs._HUDIsMyRound;
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillHeight(1.f)
			[
				SNew(SBox)
				.WidthOverride(330.f)
				.HeightOverride(330.f)
				[
					SNew(SOverlay)
					+SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SImage)
						.Image(PawnSelectionSectionBrush)
					]
					+SOverlay::Slot()
					.Padding(130.0, 60.0, 120.0, 200.0)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SButton)
						.ButtonStyle(&Style->InvisibleButtonStyle)
						.OnClicked_Lambda([&] {CardSelection(ECardType::TrojanHorse); return FReply::Handled(); })
					]
					+SOverlay::Slot()
					.Padding(60.0, 185.0, 190.0, 70.0)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SButton)
						.ButtonStyle(&Style->InvisibleButtonStyle)
						.OnClicked_Lambda([&] {CardSelection(ECardType::CoreObject); return FReply::Handled(); })
					]
					+SOverlay::Slot()
					.Padding(200.0, 185.0, 60.0, 70.0)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SButton)
						.ButtonStyle(&Style->InvisibleButtonStyle)
						.OnClicked_Lambda([&] {CardSelection(ECardType::FakeTarget); return FReply::Handled(); })
					]
				]
			]
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.FillHeight(2.f)
			[
				SNew(SBox)
				.WidthOverride(250.f)
				.HeightOverride(700.f)
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.FillHeight(1)
					[
						SNew(SOverlay)
						+SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SImage)
							.Image(TrapBrush)
						]
						+SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(60.0, 100.0, 35.0, 75.0)
						[
							SNew(SButton)
							.ButtonStyle(&Style->InvisibleButtonStyle)
							.OnClicked_Lambda([&] {CardSelection(ECardType::Trap); return FReply::Handled(); })
						]
					]
					+SVerticalBox::Slot()
					.FillHeight(1)
					[
						SNew(SOverlay)
						+SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						[
							SNew(SImage)
							.Image(DeleteBrush)
						]
						+ SOverlay::Slot()
						.HAlign(HAlign_Fill)
						.VAlign(VAlign_Fill)
						.Padding(35.0, 75.0, 60.0, 110.0)
						[
							SNew(SButton)
							.ButtonStyle(&Style->InvisibleButtonStyle)
							.OnClicked_Lambda([&] {CardSelection(ECardType::Delete); return FReply::Handled(); })
						]
					]
				]
			]
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Visibility_Lambda([&]() {return IsMyRound ? EVisibility::Collapsed : EVisibility::Visible; })
			.Image(&Style->FunctionAreaCoverBrush)
		]

	];
	
}
void SHNetFunctionAreaWidget::CardSelection(ECardType Type)
{
	if (!*HUDIsMyRound)return;
	switch (Type)
	{
	case ECardType::Delete:
		if (ForceDeleteCountDown)return;
		break;
	case ECardType::CoreObject:
		if (CoreObjectNum == 0)return;
		break;
	case ECardType::TrojanHorse:
		if (TrojanHorseNum == 0)return;
		break;
	case ECardType::FakeTarget:
		if (FakeTargetNum == 0)return;
		break;
	case ECardType::Trap:
		break;
	default:
		break;
	}
	PawnSelectionSectionMaterialInstance->SetVectorParameterValue(FName(TEXT("State")), FLinearColor(TrojanHorseNum == 0, CoreObjectNum == 0, FakeTargetNum == 0, 0));
	DeleteMaterialInstance->SetScalarParameterValue(FName(TEXT("State")), ForceDeleteCountDown ? 2 : 0);
	TrapMaterialInstance->SetScalarParameterValue(FName(TEXT("State")), 0);
	if (SelectedCard == Type) {
		SelectedCard = ECardType(-1);
	}
	else {
		switch (Type)
		{
		case ECardType::Delete:
			DeleteMaterialInstance->SetScalarParameterValue(FName(TEXT("State")), 1);
			break;
		case ECardType::CoreObject:
			if (CoreObjectNum == 0)return;
			PawnSelectionSectionMaterialInstance->SetVectorParameterValue(FName(TEXT("State")), FLinearColor(TrojanHorseNum == 0, CoreObjectNum == 0, FakeTargetNum == 0, 2));
			break;
		case ECardType::TrojanHorse:
			PawnSelectionSectionMaterialInstance->SetVectorParameterValue(FName(TEXT("State")), FLinearColor(TrojanHorseNum == 0, CoreObjectNum == 0, FakeTargetNum == 0, 1));
			break;
		case ECardType::FakeTarget:
			PawnSelectionSectionMaterialInstance->SetVectorParameterValue(FName(TEXT("State")), FLinearColor(TrojanHorseNum == 0, CoreObjectNum == 0, FakeTargetNum == 0, 3));
			break;
		case ECardType::Trap:
			TrapMaterialInstance->SetScalarParameterValue(FName(TEXT("State")), 1);
			break;
		default:
			break;
		}
		SelectedCard = Type;
	}
	OnClicked.ExecuteIfBound(Type);
}

void SHNetFunctionAreaWidget::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(PawnSelectionSectionMaterialInstance);
	Collector.AddReferencedObject(DeleteMaterialInstance);
	Collector.AddReferencedObject(TrapMaterialInstance);
}

void SHNetFunctionAreaWidget::RoundStart(){
	ForceDeleteCountDown--;
	IsMyRound = true;
	if (ForceDeleteCountDown < 0)ForceDeleteCountDown = 0;
	PawnSelectionSectionMaterialInstance->SetVectorParameterValue(FName(TEXT("State")), FLinearColor(TrojanHorseNum == 0, CoreObjectNum == 0, FakeTargetNum == 0, 0));
	DeleteMaterialInstance->SetScalarParameterValue(FName(TEXT("State")), ForceDeleteCountDown ? 2 : 0);
	TrapMaterialInstance->SetScalarParameterValue(FName(TEXT("State")), 0);
}
void SHNetFunctionAreaWidget::RoundEnd(){
	PawnSelectionSectionMaterialInstance->SetVectorParameterValue(FName(TEXT("State")), FLinearColor(TrojanHorseNum == 0, CoreObjectNum == 0, FakeTargetNum == 0, 0));
	DeleteMaterialInstance->SetScalarParameterValue(FName(TEXT("State")), ForceDeleteCountDown ? 2 : 0);
	TrapMaterialInstance->SetScalarParameterValue(FName(TEXT("State")), 0);
	IsMyRound = false;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
