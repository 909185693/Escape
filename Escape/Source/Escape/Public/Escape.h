// Copyright 2018 by January. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ESCAPE_BUILD_SERVER UE_BUILD_DEVELOPMENT || UE_SERVER
#define ESCAPE_BUILD_GAME UE_BUILD_DEVELOPMENT || UE_GAME

#define ECC_Weapon ECC_GameTraceChannel1

#define SurfaceType_Flesh SurfaceType1

DECLARE_LOG_CATEGORY_EXTERN(LogEscape, Log, All);