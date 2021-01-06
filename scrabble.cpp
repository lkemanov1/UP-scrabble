#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <cstring>
using namespace std;

const int DICTIONARY_SIZE = 58800;
const int  MAX_LETTERS = 26;
const int  MAX_TILES = 100;
/***
Binary searching through the dictionary to find if the word the player has chosen exists
*/
bool binSearch(int left, int right, string key, string word[])
{
    if (left > right)
        return false;

    if (left == right && word[left] != key)
        return false;

    int middle = (left + right) / 2;

    if (word[middle] == key)
        return true;

    else if (word[middle] > key)
        return binSearch(left, middle - 1, key, word);

    else
        return binSearch(middle + 1, right, key, word);
}
/**
Checking the correctness of the typed word
*/
bool wordIsCorrect(string& playerWord, int yourTiles[], int wordCount)
{
    int usedTiles[MAX_LETTERS] = { 0 };

    for (int a = 0; a < playerWord.size(); a++)
    {
        int currentTile = playerWord[a] - 'a';
        /**
        Converting the uppercase letters to lowercase
        */
        if (isupper(playerWord[a]))
            playerWord[a] = tolower(playerWord[a]);
        /**
        Checking if the word consists of non alphabetical characters
        */
        if (!isalpha(playerWord[a]))
            return false;
        /**
        incrementing the number of used tiles of this letter
        */
        usedTiles[currentTile]++;
        /**
        checking if the player has used a tile which is not given or already has been used
        */
        if (yourTiles[currentTile] < usedTiles[currentTile])
            return false;
    }
    return true;
}


void generateTiles(const int givenLetters, char tile[], int(&yourTiles)[MAX_LETTERS])
{

    int used[MAX_TILES];
    int letter[MAX_LETTERS];
    memset(yourTiles, 0, sizeof(yourTiles));
    cout << "Your tiles are:\n";

    srand(time(NULL));
    memset(used, 0, sizeof(used));

    for (int a = 0; a < givenLetters; a++)
    {
        int tileNumber;
        do
        {
            tileNumber = rand() % MAX_TILES;
        } while (used[tileNumber]);
        used[tileNumber] = 1;

        char newLetter = tile[tileNumber];
        yourTiles[newLetter - 'a']++;
        cout << newLetter << " ";
    }
    cout << endl;
}

int getScore(string playerWord, int scrabblePoints[])
{
    int playerScore = 0;
    for (int a = 0; a < playerWord.size(); a++)
    {
        playerScore += scrabblePoints[playerWord[a] - 'a'];
    }
    return playerScore;
}

int newRound(const int givenLetters, char tile[], int(&yourTiles)[MAX_LETTERS], int wordCount, string word[], int points[])
{
    generateTiles(givenLetters, tile, yourTiles);
    string playerWord;
    int countOfWrongWords = 0;
    do
    {

        if (countOfWrongWords > 0)
        {
            cout << "Wrong word!\nTry again!\n";
        }
        cin >> playerWord;
        countOfWrongWords++;
        if (playerWord == "SKIP!")
        {
            cout << "You skipped your turn!\n";
            return 0;

        }
    } while (!(wordIsCorrect(playerWord, yourTiles, wordCount) && binSearch(0, wordCount, playerWord, word)));
    int currentScore = getScore(playerWord, points);
    cout << "Your score is " << currentScore << endl;
    cout << "====================" << endl;
    return currentScore;
}

/**
adding new word to dictionary

*/
/**
void addNewWord(fstream &dictionary, string newWord)
{
    dictionary.clear();
    dictionary<<newWord<<endl;
}*/


int main()
{
    ifstream tiles("tiles.txt");
    fstream  dictionary;

    string word[DICTIONARY_SIZE];
    string currWord;
    string currTile;
    const int MAX_TILES = 101;
    char tile[MAX_TILES];
    int scrabblePoints[MAX_LETTERS] = { 1, 3, 3, 2, 1, 4, 2, 4, 1,
                                        8, 5, 1, 3, 1, 1, 3, 10,
                                        1, 1, 1, 1, 4, 4, 8, 4, 10 };

    int wordCount = 0;
    int tileCount = 0;
    int yourTiles[MAX_LETTERS];
    int playableTiles = 7;
    int numberRounds = 5;
    int totalScore = 0;
    dictionary.open("dictionary.txt", ios::app | ios::in | ios::out);

    if (!dictionary.is_open() || !tiles.is_open())
        return 0;

    ///reading the words from the file
    while (getline(dictionary, currWord))
    {
        word[wordCount++] = currWord;
    }
    ///reading the frequence of the tiles
    while (getline(tiles, currTile))
    {
        tile[tileCount++] = currTile[0];
    }

    for (int round = 1; round <= numberRounds; round++)
    {
        totalScore += newRound(playableTiles, tile, yourTiles, wordCount, word, scrabblePoints);
    }
    cout << "Your overall score is: " << totalScore << endl;
    dictionary.close();
    tiles.close();
    return 0;
}
