// Copyright Epic Games, Inc. All Rights Reserved.


#include "HOPEGameModeBase.h"
#include "HOPECharacter.h"
#include "HOPEUserWidget.h"

AHOPEGameModeBase::AHOPEGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ACharacter> BP_Char(TEXT("Blueprint'/Game/Blueprints/BP_HOPECharacter.BP_HOPECharacter_C'"));

	if (BP_Char.Succeeded())
	{
		DefaultPawnClass = BP_Char.Class;
	}


	static ConstructorHelpers::FClassFinder<UHOPEUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/Blueprints/BP_HUD.BP_HUD_C'"));

	if (UI_HUD.Succeeded())
	{
		HUD_Class = UI_HUD.Class;

		CurrentWidget = CreateWidget(GetWorld(), HUD_Class);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}