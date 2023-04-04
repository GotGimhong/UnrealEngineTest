#pragma once

#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

namespace TPS
{
	class FUObjectCreateListener;
}

UCLASS()
class UTPSGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()

public:
	// 以异步方式切换关卡
	UFUNCTION(BlueprintCallable)
	void ClientTravel(FString LevelName);

public:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	// 让 FUObjectCreateListener 的生命周期与 GameInstance 保持一致
	const TPS::FUObjectCreateListener* UObjectCreateListener;
};
