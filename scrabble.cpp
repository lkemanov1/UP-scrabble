/**
*
* Solution to course project # 8
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2020/2021
*
* @author Lachezar Kemanov
* @idnumber 62615
* @compiler GCC
*
* <Main file>
*
*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <cstring>
#include<string>

using namespace std;

const int MAX_WORDS = 60000;
const int FILE_SIZE = 58110;
const int MAX_LETTERS = 26;
const int MAX_TILES = 100;


void printMenu()
{
    cout << "----------------------\n";
    cout << "*      Scrabble      *\n";
    cout << "----------------------\n";
    cout << "* 1) Start new game. *\n";
    cout << "----------------------\n";
    cout << "* 2)    Settings.    *\n";
    cout << "----------------------\n";
    cout << "* 3)  Add new word.  *\n";
    cout << "----------------------\n";
    cout << "* 4)      Exit.      *\n";
    cout << "----------------------\n";
    cout << "\nSelect your option: ";
}

/***
Binary searching through the dictionary to find if the word the player has chosen exists
*/

bool binSearch(int left, int right, string key, string(&word)[MAX_WORDS])
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
bool wordIsCorrect(string& playerWord, int(&yourTiles)[MAX_LETTERS])
{
    int usedTiles[MAX_LETTERS] = { 0 };

    for (unsigned a = 0; a < playerWord.size(); a++)
    {
        /**
        Converting the uppercase letters to lowercase
        */
        if (isupper(playerWord[a]))
            playerWord[a] = tolower(playerWord[a]);

        int currentTile = playerWord[a] - 'a';

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

    int used[MAX_TILES];/// is the tile used yet. There are several tiles for each letter
    const int isUsed = 1;
    memset(yourTiles, 0, sizeof(yourTiles));
    cout << "Your tiles are:\n";

    srand(time(NULL));///making sure the random generator is random every time the program runs
    memset(used, 0, sizeof(used));

    for (int a = 0; a < givenLetters; a++)
    {
        int tileNumber;
        ///getting random tile until we get one that is not used yet
        do
        {
            tileNumber = rand() % MAX_TILES;
        } while (used[tileNumber]);

        used[tileNumber] = isUsed;///marking the used tile as such

        char newTile = tile[tileNumber];
        yourTiles[newTile - 'a']++;
        cout << newTile << " ";
    }
    cout << endl;
}

int getScore(string playerWord, int scrabblePoints[])
{
    int playerScore = 0;
    for (unsigned a = 0; a < playerWord.size(); a++)
    {
        playerScore += scrabblePoints[playerWord[a] - 'a'];///for each letter add the corresponding points
    }
    return playerScore;
}

bool searchNewWords(int wordCount, string key, string(&word)[MAX_WORDS])
{
    ///searching the new words which are not sorted so we can't binary search them
    for (int a = FILE_SIZE; a < wordCount; a++)
    {
        if (word[a] == key)
            return true;
    }
    return false;
}


int newRound(const int givenLetters, char tile[], int wordCount, int points[], string(&word)[MAX_WORDS])
{
    int yourTiles[MAX_LETTERS];
    generateTiles(givenLetters, tile, yourTiles);
    string playerWord;
    int countOfWrongWords = 0;///counter if the player has already tried to use invalid word

    /**
    Typing new word until its valid or the player skips the turn
    Checking if the word is in the sorted list or in the in the list of new words
    Checking if the chosen word can be written with the given letters
    */
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
    } while (!(wordIsCorrect(playerWord, yourTiles) && (binSearch(0, FILE_SIZE - 1, playerWord, word) || searchNewWords(wordCount, playerWord, word))));

    int currentScore = getScore(playerWord, points);

    cout << "Your score is " << currentScore << endl;
    cout << "====================" << endl;

    return currentScore;
}

void newGame(int playableTiles, char tile[], int wordCount, int points[], int numberRounds, string(&word)[MAX_WORDS])
{
    system("CLS");
    unsigned totalScore = 0;
    for (int round = 1; round <= numberRounds; round++)
    {
        cout << "Round " << round << endl;
        totalScore += newRound(playableTiles, tile, wordCount, points, word);
    }
    string exitButton;
    cout << "Your overall score is: " << totalScore << endl;
    cout << "Type something and then press enter to return to main menu:";
    cin >> exitButton;
    system("CLS");
    printMenu();
}

void settings(int& numberRounds, int& givenLetters)
{
    system("CLS");
    int option = 0;
    cout << "Settings\n";
    cout << "1)Change number of letters\n";
    cout << "2)Change number of rounds\n";
    cout << "3)Return to main menu\n";


    while (option != 3)
    {
        cout << "Select your option: ";
        cin >> option;
        ///changing the number of letters that are given in the game
        if (option == 1)
        {
            cout << endl;
            cout << "Change number of letters to:";
            cin >> givenLetters;
            cout << endl;
        }
        ///changing the number of rounds played
        else if (option == 2)
        {
            cout << endl;
            cout << "Change number of rounds to:";
            cin >> numberRounds;
            cout << endl;
        }
        ///return to the menu
        else if (option == 3)
        {
            system("CLS");
            printMenu();
            break;
        }
        else
        {
            cout << "No such option!\nPlease select again!\n";
        }
    }

}

bool validWord(string& newWord)
{
    for (unsigned a = 0; a < newWord.size(); a++)
    {
        if (!isalpha(newWord[a]))
        {
            return false;
        }
        if (isupper(newWord[a]))
        {
            newWord[a] = tolower(newWord[a]);
        }
    }
    return true;
}
void addNewWord(int& wordCount, string(&word)[MAX_WORDS], fstream& dictionary)
{
    system("CLS");
    string newWord;

    cout << "Type in the word you want to add to the dictionary or press x to return to menu: ";
    int existingWordsCounter = 0;///counter whether a word has been typed already

    /**
    typing a new word while the word is not in the dictionary
    or the word does not consist only with English letters
    or the player decides to return to the menu
    */

    do
    {
        if (existingWordsCounter > 0)
        {
            cout << "This word is already in the dictionary or is invalid\n";
            cout << "Type in another word or press x to return to main menu: ";

        }

        cin >> newWord;
        ///Exit condition
        if ((newWord == "x" || newWord == "X"))
        {
            system("CLS");
            printMenu();
            return;
        }
        existingWordsCounter++;
    } while (binSearch(0, wordCount - 1, newWord, word) || searchNewWords(wordCount, newWord, word) || !validWord(newWord));

    dictionary.clear();
    dictionary << endl << newWord;
    word[wordCount++] = newWord;
    system("CLS");
    printMenu();

}


void startMenu(int givenLetters, char tile[], int wordCount, int points[], int numberRounds, string(&word)[MAX_WORDS], fstream& dictionary)
{
    printMenu();
    string option = "0";
    string option1 = "1", option2 = "2", option3 = "3", option4 = "4";

    while (option != option4)
    {
        cin >> option;

        if (option == option1)
        {
            newGame(givenLetters, tile, wordCount, points, numberRounds, word);
        }
        else if (option == option2)
        {
            settings(numberRounds, givenLetters);
        }
        else if (option == option3)
        {
            addNewWord(wordCount, word, dictionary);
        }
        else if (option == option4)
        {
            cout << "Thank you for playing!!!\n";
            break;
        }
        else
        {
            cout << "\nNo such option.\nPlease select again: ";
        }
    }
}


int main()
{
    ifstream tiles("tiles.txt");
    string currWord;
    string currTile;
    string word[MAX_WORDS];
    fstream  dictionary;

    char tile[MAX_TILES];

    int scrabblePoints[MAX_LETTERS] = { 1, 3, 3, 2, 1, 4, 2, 4, 1,
                                        8, 5, 1, 3, 1, 1, 3, 10,
                                        1, 1, 1, 1, 4, 4, 8, 4, 10
    };
    int wordCount = 0;
    int tileCount = 0;
    int yourTiles[MAX_LETTERS];
    int playableTiles = 10;
    int numberRounds = 5;

    dictionary.open("dictionary.txt", ios::app | ios::in | ios::out);

    if (!dictionary.is_open() || !tiles.is_open())
    {
        cout << "File couldn't open.\n";
        return 0;
    }
    ///reading the words from the file
    while (getline(dictionary, currWord))
    {
        word[wordCount++] = currWord;
    }
    ///reading the frequency of the tiles
    while (getline(tiles, currTile))
    {
        tile[tileCount++] = currTile[0];
    }

    startMenu(playableTiles, tile, wordCount, scrabblePoints, numberRounds, word, dictionary);

    dictionary.close();
    tiles.close();
    return 0;
}
