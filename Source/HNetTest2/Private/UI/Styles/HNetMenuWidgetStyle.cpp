// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetMenuWidgetStyle.h"


FHNetMenuStyle::FHNetMenuStyle()
{
}

FHNetMenuStyle::~FHNetMenuStyle()
{
}

const FName FHNetMenuStyle::TypeName(TEXT("FHNetMenuStyle"));

const FHNetMenuStyle& FHNetMenuStyle::GetDefault()
{
	static FHNetMenuStyle Default;
	return Default;
}

void FHNetMenuStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

