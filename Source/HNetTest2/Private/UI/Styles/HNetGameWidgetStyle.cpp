// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetGameWidgetStyle.h"


FHNetGameStyle::FHNetGameStyle()
{
}

FHNetGameStyle::~FHNetGameStyle()
{
}

const FName FHNetGameStyle::TypeName(TEXT("FHNetGameStyle"));

const FHNetGameStyle& FHNetGameStyle::GetDefault()
{
	static FHNetGameStyle Default;
	return Default;
}

void FHNetGameStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

