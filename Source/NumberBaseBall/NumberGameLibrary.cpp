#include "NumberGameLibrary.h"
#include "Math/RandomStream.h"

// Fisher-Yates 셔플 알고리즘 구현
static void ShuffleArray(TArray<int32>& Array, FRandomStream& RandomStream)
{
    int32 LastIndex = Array.Num() - 1;
    for (int32 i = LastIndex; i > 0; i--)
    {
        int32 Index = RandomStream.RandRange(0, i);
        if (i != Index)
        {
            Array.Swap(i, Index);
        }
    }
}

FString UNumberGameLibrary::GenerateRandomNumber()
{
    TArray<int32> Digits = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    FRandomStream RandomStream(FDateTime::Now().GetTicks());

    // TArray::Shuffle 대신 수동 셔플 사용
    ShuffleArray(Digits, RandomStream);

    return FString::Printf(TEXT("%d%d%d"), Digits[0], Digits[1], Digits[2]);
}

void UNumberGameLibrary::CalculateResult(const FString& ServerNumber, const FString& Guess, int32& Strikes, int32& Balls, bool& bIsOut)
{
    Strikes = 0;
    Balls = 0;
    bIsOut = true;

    if (!IsValidGuess(Guess) || ServerNumber.Len() != 3)
    {
        return;
    }

    for (int32 i = 0; i < 3; i++)
    {
        TCHAR ServerDigit = ServerNumber[i];
        for (int32 j = 0; j < 3; j++)
        {
            if (Guess[j] == ServerDigit)
            {
                if (i == j)
                {
                    Strikes++;
                }
                else
                {
                    Balls++;
                }
                bIsOut = false;
            }
        }
    }
}

bool UNumberGameLibrary::IsValidGuess(const FString& Guess)
{
    if (Guess.Len() != 3)
    {
        return false;
    }

    TSet<TCHAR> UniqueDigits;
    for (TCHAR Digit : Guess)
    {
        if (!FChar::IsDigit(Digit) || Digit == '0')
        {
            return false;
        }
        if (UniqueDigits.Contains(Digit))
        {
            return false;
        }
        UniqueDigits.Add(Digit);
    }
    return true;
}