// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "UnrealNetwork.h"
#include "Online.h"

class FHNetTest2Module : public FDefaultModuleImpl {
public :
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};