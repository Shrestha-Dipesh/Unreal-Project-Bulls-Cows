// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

;void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
 
    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else{
        ProcessGuess(PlayerInput);
    }   
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len(); 
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Lives: %i"), Lives);
    PrintLine(TEXT("Press tab to enter the console..."));

}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again!"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)
    {
        PrintLine("You have Won!");
        EndGame();
        return;
    }
    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The word is %i letters."), HiddenWord.Len());
        return;
    }
    if (!IsIsogram(Guess))
    {
        PrintLine("No repeating letters, guess again");
        return;
    }
    PrintLine(TEXT("You have lost a life"));
    --Lives;
    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("Out of lives"));
        PrintLine(TEXT("Hidden word: %s"), *HiddenWord);
        EndGame();
        return;
    }
    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Lives Remaining: %i"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for (FString Word : WordList)
    {
        if (Word.Len() == 5)
        {
            if (IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }   
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}