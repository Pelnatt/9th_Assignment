#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NumberGameLibrary.generated.h"

UCLASS()
class NUMBERBASEBALL_API UNumberGameLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "NumberGame")
    static FString GenerateRandomNumber();

    UFUNCTION(BlueprintCallable, Category = "NumberGame")
    static void CalculateResult(const FString& ServerNumber, const FString& Guess, int32& Strikes, int32& Balls, bool& bIsOut);

    UFUNCTION(BlueprintCallable, Category = "NumberGame")
    static bool IsValidGuess(const FString& Guess);
};