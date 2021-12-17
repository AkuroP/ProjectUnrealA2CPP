// Copyright Epic Games, Inc. All Rights Reserved.

#include "A2CPPGameMode.h"
#include "A2CPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AA2CPPGameMode::AA2CPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
