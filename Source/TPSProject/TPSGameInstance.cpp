#include "TPSGameInstance.h"
#include "TPSUObjectCreateListener.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UTPSGameInstance::UTPSGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, UObjectCreateListener(nullptr)
{

}

void UTPSGameInstance::ClientTravel(const FString LevelName)
{
	UWorld* const World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: World is invalid"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: PlayerController 0 is invalid"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Call %s->ClientTravel to %s"), *PlayerController->GetName(), *LevelName);
	PlayerController->ClientTravel(LevelName, ETravelType::TRAVEL_Absolute, true);
}

void UTPSGameInstance::Init()
{
	Super::Init();

	if (!UObjectCreateListener)
	{
		UObjectCreateListener = new TPS::FUObjectCreateListener();
	}
}

void UTPSGameInstance::Shutdown()
{
	if (UObjectCreateListener)
	{
		delete UObjectCreateListener;
		UObjectCreateListener = nullptr;
	}

	Super::Shutdown();
}
