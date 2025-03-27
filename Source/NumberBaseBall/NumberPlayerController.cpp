#include "NumberPlayerController.h"
#include "Blueprint/UserWidget.h"

void ANumberPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (ChatWidgetClass && IsLocalController())
    {
        ChatWidget = CreateWidget<UUserWidget>(this, ChatWidgetClass);
        if (ChatWidget)
        {
            ChatWidget->AddToViewport();
        }
    }
}

void ANumberPlayerController::ClientReceiveMessage_Implementation(const FString& Message)
{
    if (ChatWidget)
    {
        if (UFunction* UpdateChatFunc = ChatWidget->FindFunction(FName("UpdateChat")))
        {
            struct FParams { FString Message; };
            FParams Params{ Message };
            ChatWidget->ProcessEvent(UpdateChatFunc, &Params);
        }
    }
}