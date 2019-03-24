// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class HNetTest2ServerTarget : TargetRules   // Change this line as shown previously
{
    public HNetTest2ServerTarget(TargetInfo Target) : base(Target)  // Change this line as shown previously
    {
        Type = TargetType.Server;
        ExtraModuleNames.Add("HNetTest2");    // Change this line as shown previously
    }
}