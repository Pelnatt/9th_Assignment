#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NumberGameMode.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANumberGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ANumberGameMode();

    UFUNCTION(BlueprintCallable, Category = "NumberGame")
    void ProcessGuess(const FString& Guess, APlayerController* Player);

protected:
    virtual void BeginPlay() override;

private:
    FString ServerNumber;
    int32 HostAttempts;
    int32 GuestAttempts;
    bool bIsHostTurn;

    void ResetGame(const FString& ResultMessage);
    void SwitchTurn();
    void BroadcastMessage(const FString& Message);
};