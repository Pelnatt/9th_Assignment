#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NumberPlayerController.generated.h"

UCLASS()
class NUMBERBASEBALL_API ANumberPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UFUNCTION(Client, Reliable)
    void ClientReceiveMessage(const FString& Message);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> ChatWidgetClass;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    class UUserWidget* ChatWidget;
};