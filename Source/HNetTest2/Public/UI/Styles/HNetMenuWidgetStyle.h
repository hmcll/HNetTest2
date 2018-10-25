// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"

#include "HNetMenuWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct HNETTEST2_API FHNetMenuStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FHNetMenuStyle();
	virtual ~FHNetMenuStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FHNetMenuStyle& GetDefault();

	UPROPERTY(EditAnyWhere, Category = Menu)
		FSlateBrush MenuBackGroundBrush;

	UPROPERTY(EditAnyWhere, Category = Menu)
		FSlateBrush TitleBrush;

	UPROPERTY(EditAnyWhere, Category = Menu)
		FButtonStyle TitleButtonStyle;

	UPROPERTY(EditAnyWhere, Category = Menu)
		FSlateFontInfo MenuFont_Button;	

	UPROPERTY(EditAnyWhere, Category = Menu)
		FSlateFontInfo MenuFont_Loading;

	UPROPERTY(EditAnyWhere, Category = Menu)
		FColor OpponentLeftBackground;

	UPROPERTY(EditAnyWhere, Category = Game)
		FSlateBrush FunctionAreaCoverBrush;

};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UHNetMenuWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FHNetMenuStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
