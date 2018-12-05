// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"

#include "HNetGameWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct HNETTEST2_API FHNetGameStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FHNetGameStyle();
	virtual ~FHNetGameStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FHNetGameStyle& GetDefault();


	UPROPERTY(EditAnyWhere, Category = Game)
		FSlateBrush FunctionAreaCoverBrush;

	UPROPERTY(EditAnyWhere, Category = Game)
		FButtonStyle InvisibleButtonStyle;

	UPROPERTY(EditAnyWhere, Category = Game)
		UMaterialInterface* PawnMat;

	UPROPERTY(EditAnyWhere, Category = Game)
		UMaterialInterface* PawnSelectionAreaMat;

	UPROPERTY(EditAnyWhere, Category = Game)
		UMaterialInterface* Trap;

	UPROPERTY(EditAnyWhere, Category = Game)
		UMaterialInterface* Delete;

};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UHNetGameWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FHNetGameStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
