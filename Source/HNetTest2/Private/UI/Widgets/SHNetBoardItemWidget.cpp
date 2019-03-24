// Fill out your copyright notice in the Description page of Project Settings.

#include "SHNetBoardItemWidget.h"
#include "SButton.h"
#include "STextBlock.h"
#include "SImage.h"
#include "SBoxPanel.h"
#include "SlateOptMacros.h"
#include "HNetStyle.h"
#include "HNetGameWidgetStyle.h"
#include "SHNetGameCoreWidget.h"
#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHNetBoardItemWidget::Construct(const FArguments& InArgs)
{
	auto Style = &HNetStyle::Get().GetWidgetStyle<FHNetGameStyle>("BPHNetGameStyle");
	PawnMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GEngine->GetWorld(), Style->PawnMat);
	PawnBrush = new FSlateBrush();
	PawnBrush->SetResourceObject(PawnMaterialInstance);
	Data.ID = InArgs._ID;
	IsMyRound = InArgs._IsMyRound;
	PawnMaterialInstance->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(FVector4(InArgs._MyColor,InArgs._EnemyColor)));
	OnClicked.BindRaw(InArgs._Board, &SHNetGameCoreWidget::BoardItemOnClicked);
	*InArgs._DataPointer = &Data;
	ChildSlot
	[
		SNew(SButton)
		.ButtonStyle(&Style->InvisibleButtonStyle)
		.OnClicked_Lambda([&]() {
			OnClicked.ExecuteIfBound(Data.ID);
			return FReply::Handled();
		})
		[
			SNew(SImage)
			.Image_Lambda([&]() {
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("PawnID")), int(Data.Type)-1);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("Misted")), Data.Misted);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsFlipped")), Data.IsFlipped);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsMine")), Data.IsMine);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsNearTrap")), Data.IsNearTrap);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsMovePoint")), Data.IsMovePoint);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsMyRound")), *IsMyRound);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsMyTrap")), Data.IsMyTrap);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsEnemyTrap")), Data.IsEnemyTrap);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsSelected")), Data.IsSelected);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsTrapped")), Data.IsTrapped);
				PawnMaterialInstance->SetScalarParameterValue(FName(TEXT("IsSteppingOnMyTrap")), Data.IsMyTrap && Data.IsCard());


				return PawnBrush; 
			})
		]	
	];
	
}

void SHNetBoardItemWidget::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(PawnMaterialInstance);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
