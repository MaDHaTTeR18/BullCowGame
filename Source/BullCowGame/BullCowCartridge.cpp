// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordList/SecretWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    Words=GetValidWords(Words);
    SetUpGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        SetUpGame();
    }
    else{
            ProcessGuess(Input);
        }
}
void UBullCowCartridge::SetUpGame()
{   //FMath::RandRange(0,(GetValidWords(Words).Num()-1))
    SecretWord = Words[FMath::RandRange(0,Words.Num()-1)];
    Lives =SecretWord.Len();
    bGameOver = false;
    // Welcoming the player
    PrintLine(TEXT("Hello \nWelcome to the Bull & Cows Game"));
    //PrintLine(TEXT("Secret Word is %s."),*SecretWord);
    PrintLine(TEXT("Guess the %i letter word."),SecretWord.Len());
    PrintLine(TEXT("You have %i lives remaining."),Lives);
    PrintLine(TEXT("Please type in your answer.\nPress Enter to continue...."));

}
void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT(".....Game Over.....\n\n.....Press Enter to Play Again....."));
}
void UBullCowCartridge::ProcessGuess(const FString& PlayerInput)
{

    if(SecretWord==PlayerInput)
        {
            PrintLine(TEXT("You Have Guessed Correctly. \nYou Have Won"));
            EndGame();
            return;
        }else
            {
              --Lives;// Reducing Life for Making Mistake
             // Checking if life >0
              if(Lives<=0)
                {
                    ClearScreen();
                    PrintLine(TEXT("Wrong Answer."));
                    PrintLine(TEXT("The word is %s."),*SecretWord);
                    PrintLine(TEXT("<<<<+ You Have LOST +>>>>"));
                    EndGame();
                    return;
                }else
                    {   FBullCowCount Score = GetBullCows(*PlayerInput);
                        if(PlayerInput.Len() != SecretWord.Len())
                            {
                                PrintLine(TEXT("Wrong Answer."));
                                PrintLine(TEXT("You world is not the same length.\nLength of the secret word is %i."),SecretWord.Len());
                                PrintLine(TEXT("You have %i lives remaining."),Lives);
                                PrintLine(TEXT("You have %i Bulls and %i Cows."),Score.Bulls,Score.Cows);
                                PrintLine(TEXT("Please Try Again."));

                            }
                            else
                                {
                                    PrintLine(TEXT("Wrong Answer."));
                                    PrintLine(TEXT("You world is the same length."));
                                    PrintLine(TEXT("You have %i lives remaining."),Lives);
                                    if(IsIsoGram(*PlayerInput))
                                    {
                                        PrintLine(TEXT("You Word is %s.\nWhich is an Isogram"),*PlayerInput);
                                        PrintLine(TEXT("You have %i Bulls and %i Cows."),Score.Bulls,Score.Cows);
                                        PrintLine(TEXT("Please Try Again."));
                                    }else
                                    {
                                        PrintLine(TEXT("You Word is %s.\nWhich is not an Isogram"),*PlayerInput);
                                        PrintLine(TEXT("You have %i Bulls and %i Cows."),Score.Bulls,Score.Cows);
                                        PrintLine(TEXT("Please Try Again."));
                                    }
                                }
                    }

            }
}
bool UBullCowCartridge::IsIsoGram(const FString& Word) const
{
    for (int32 index=0;index<Word.Len();index++)
    {
        for (int32 k=index+1;k<Word.Len();k++)
        {
            if(Word[index]==Word[k])
            {
                return false;
            }
        }
    }

return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordsList) const
{
    TArray<FString> ValidWords;
    for(FString Word: WordsList)
    {
        if(Word.Len()>=4 && Word.Len()<=8 && IsIsoGram(*Word))
        {
            ValidWords.Emplace(*Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Word) const
{
    FBullCowCount Count;

    for(int32 GuessIndex=0; GuessIndex< Word.Len(); GuessIndex++)
    {
        if(Word[GuessIndex]==SecretWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex=0; HiddenIndex<SecretWord.Len();HiddenIndex++)
        {
            if(Word[GuessIndex]==SecretWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}