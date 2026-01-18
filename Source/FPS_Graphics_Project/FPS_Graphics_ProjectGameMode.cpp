// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPS_Graphics_ProjectGameMode.h"
#include "FPS_Graphics_ProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPS_Graphics_ProjectGameMode::AFPS_Graphics_ProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
