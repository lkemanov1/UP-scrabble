#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <cstring>
using namespace std;

const int MAX_WORDS = 60000;
const int FILE_SIZE = 58110;
const int MAX_LETTERS = 26;
const int MAX_TILES = 100;
string word[MAX_WORDS];
fstream  dictionary;

void printMenu()
{
    cout<<"************************\n";
    cout<<"Scrabble\n";
    cout<<"1) Start new game    \n";
    cout<<"2) Settings          \n";
    cout<<"3) Add new word      \n";
    cout<<"4) Exit!             \n";
    cout<<"************************\n";
    cout<<"Select your option:";
}

/***
Binary searching through the dictionary to find if the word the player has chosen exists
*/
int findNearestPosition(int left, int right, string key)
{
    if (left > right)
        return right;

    if (left == right && word[left] != key)
        return left;

    int middle = (left + right) / 2;
    ///cout<<middle<<" "<<word[middle]<<endl;
    if (word[middle] == key)
        return middle;

    else if (word[middle] > key)
        return findNearestPosition(left, middle - 1, key);

    else
        return findNearestPosition(middle + 1, right, key);
}

bool binSearch(int left, int right, string key)
{
    if (left > right)
        return false;

    if (left == right && word[left] != key)
        return false;

    int middle = (left + right) / 2;
    ///cout<<middle<<" "<<word[middle]<<endl;
    if (word[middle] == key)
        return true;

    else if (word[middle] > key)
        return binSearch(left, middle - 1, key);

    else
        return binSearch(middle + 1, right, key);
}
/**
Checking the correctness of the typed word
*/
bool wordIsCorrect(string& playerWord, int yourTiles[])
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

bool searchNewWords(int wordCount, string key)
{
    for(int a=FILE_SIZE; a<wordCount; a++)
    {
        if(word[a]==key)
            return true;
    }
    return false;
}


int newTurn(const int givenLetters, char tile[], int(&yourTiles)[MAX_LETTERS], int wordCount, int points[])
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
    } while (!(wordIsCorrect(playerWord, yourTiles) && binSearch(0, wordCount, playerWord) || searchNewWords(wordCount,playerWord)));

    int currentScore = getScore(playerWord, points);

    cout << "Your score is " << currentScore << endl;
    cout << "====================" << endl;

    return currentScore;
}
void newRound(int playableTiles, char tile[], int(&yourTiles)[MAX_LETTERS], int wordCount, int points[], int numberRounds)
{
    system("CLS");
    unsigned totalScore=0;
    for (int round = 1; round <= numberRounds; round++)
    {
        totalScore += newTurn(playableTiles, tile, yourTiles, wordCount, points);
    }
    char exitButton;
    cout << "Your overall score is: " << totalScore << endl;
    cout << "Press any key to return to main menu:";
    cin >> exitButton;
    system("CLS");
    printMenu();
}

void settings(int &numberRounds, int &givenLetters)
{
    system("CLS");
    int option=0;
    cout<<"Settings\n";
    cout<<"1)Change number of letters\n";
    cout<<"2)Change number of rounds\n";
    cout<<"3)Return to main menu\n";


    while(option!=3)
    {
        cout<<"Select your option: ";
        cin>>option;
        if(option==1)
        {
            cout<<endl;
            cout<<"Change number of letters to:";
            cin>>givenLetters;
            cout<<endl;
        }
        else if(option==2)
        {
            cout<<endl;
            cout<<"Change number of rounds to:";
            cin>>numberRounds;
            cout<<endl;
        }
        else if(option==3)
        {
            system("CLS");
            printMenu();
            break;
        }
        else
        {
            cout<<"Wrong option!\nPlease select again!\n";
        }
    }

}
void addNewWord(int &wordCount)
{
    string newWord;

    cout<<"Type in the word you want to add to the dictionary:";
    int existingWordsCounter=0;
    do
    {
        if(existingWordsCounter>0)
        {
            cout<<"This word is already in the dictionary\n";
            cout<<"Type in another word or press x to return to main menu\n";

        }
        cin>>newWord;
        if(newWord=="x" || newWord=="X")
        {
            system("CLS");
            printMenu();
            return;
        }
    }while(binSearch(0,wordCount-1,newWord) || searchNewWords(wordCount,newWord));
    dictionary.clear();
    dictionary<<newWord<<endl;
    word[wordCount++]=newWord;
    system("CLS");
    printMenu();

}


void startMenu(int givenLetters, char tile[], int(&yourTiles)[MAX_LETTERS], int wordCount, int fileSize, int points[], int numberRounds)
{
    int option=0;
    printMenu();
    while(option!=4)
    {
        cin>>option;
        if(option==1) newRound(givenLetters,tile,yourTiles,wordCount,points,numberRounds);
        else if(option==2) settings(numberRounds,givenLetters);
        else if(option==3) addNewWord(wordCount);
        else if(option==4) break;
        else cout<<"\nWrong option.\nPlease choose again.\n";
    }
}
/**
adding new word to dictionary

*/



int main()
{
    ifstream tiles("tiles.txt");
    string currWord;
    string currTile;
    const int MAX_TILES = 101;
    char tile[MAX_TILES];
    int scrabblePoints[MAX_LETTERS] = { 1, 3, 3, 2, 1, 4, 2, 4, 1,
                                        8, 5, 1, 3, 1, 1, 3, 10,
                                        1, 1, 1, 1, 4, 4, 8, 4, 10 };
    int fileSize=0;
    int wordCount = 0;
    int tileCount = 0;
    int yourTiles[MAX_LETTERS];
    int playableTiles = 10;
    int numberRounds = 5;
    dictionary.open("dictionary.txt", ios::app | ios::in | ios::out);

    if (!dictionary.is_open() || !tiles.is_open())
        return 0;

    ///reading the words from the file
    while (getline(dictionary, currWord))
    {
        word[wordCount++] = currWord;
    }
    fileSize=wordCount;
    ///reading the frequence of the tiles
    while (getline(tiles, currTile))
    {
        tile[tileCount++] = currTile[0];
    }
    startMenu(playableTiles,tile,yourTiles,wordCount,fileSize,scrabblePoints,numberRounds);

    dictionary.close();
    tiles.close();
    return 0;
}
