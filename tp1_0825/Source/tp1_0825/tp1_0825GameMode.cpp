// Copyright Epic Games, Inc. All Rights Reserved.

#include "tp1_0825GameMode.h"
#include "tp1_0825Character.h"
#include "UObject/ConstructorHelpers.h"

Atp1_0825GameMode::Atp1_0825GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
