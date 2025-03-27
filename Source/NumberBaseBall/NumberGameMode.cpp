#include "NumberGameMode.h"
#include "GameFramework/PlayerController.h"
#include "NumberGameLibrary.h"
#include "NumberPlayerController.h"

ANumberGameMode::ANumberGameMode()
{
    HostAttempts = 3;
    GuestAttempts = 3;
    bIsHostTurn = true;
}

void ANumberGameMode::BeginPlay()
{
    Super::BeginPlay();
    ServerNumber = UNumberGameLibrary::GenerateRandomNumber();
    BroadcastMessage("게임이 시작됐다! Host부터 시작해.");
}

void ANumberGameMode::ProcessGuess(const FString& Guess, APlayerController* Player)
{
    if (!Player || !Guess.StartsWith("/")) return;

    FString NumberGuess = Guess.Mid(1);
    bool bIsHost = (GetNumPlayers() > 0 && Player == GetWorld()->GetFirstPlayerController());
    if (bIsHost != bIsHostTurn) return;

    int32 Strikes, Balls;
    bool bIsOut;
    UNumberGameLibrary::CalculateResult(ServerNumber, NumberGuess, Strikes, Balls, bIsOut);

    FString Result;
    if (bIsOut)
    {
        Result = "OUT";
        if (bIsHost) HostAttempts--;
        else GuestAttempts--;
    }
    else
    {
        Result = FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
        if (bIsHost) HostAttempts--;
        else GuestAttempts--;
    }

    FString PlayerName = bIsHost ? "Host" : "Guest";
    BroadcastMessage(FString::Printf(TEXT("%s: %s → %s"), *PlayerName, *NumberGuess, *Result));

    if (Strikes == 3)
    {
        ResetGame(FString::Printf(TEXT("%s Won!! 다시 게임이 시작됐다."), *PlayerName));
        return;
    }

    if (HostAttempts <= 0 && GuestAttempts > 0)
    {
        ResetGame("Guest Won!! 다시 게임이 시작됐다.");
        return;
    }
    else if (GuestAttempts <= 0 && HostAttempts > 0)
    {
        ResetGame("Host Won!! 다시 게임이 시작됐다.");
        return;
    }
    else if (HostAttempts <= 0 && GuestAttempts <= 0)
    {
        ResetGame("무승부군. 다시 게임을 시작하지");
        return;
    }

    SwitchTurn();
}

void ANumberGameMode::ResetGame(const FString& ResultMessage)
{
    ServerNumber = UNumberGameLibrary::GenerateRandomNumber();
    HostAttempts = 3;
    GuestAttempts = 3;
    bIsHostTurn = true;
    BroadcastMessage(ResultMessage);
}

void ANumberGameMode::SwitchTurn()
{
    bIsHostTurn = !bIsHostTurn;
    BroadcastMessage(bIsHostTurn ? "Host 차례야." : "Guest 차례야.");
}

void ANumberGameMode::BroadcastMessage(const FString& Message)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->PlayerState)
        {
            ANumberPlayerController* NumberPC = Cast<ANumberPlayerController>(PC);
            if (NumberPC)
            {
                NumberPC->ClientReceiveMessage(Message);
            }
        }
    }
}