// Copyright Epic Games, Inc. All Rights Reserved.

#include "JHP5GameMode.h"
#include "Character/JHP5Character.h"
#include "UObject/ConstructorHelpers.h"

AJHP5GameMode::AJHP5GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/00_Character/BP_PPCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
