#include "helper.h"
#include <iostream>
#include <climits>
#include <vector>
#include <iomanip>
#include <array>
#include <random>
#include <ctime>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
using namespace std;

class Intro
{
    friend class Alien;

private:
    string choice_;
    int rows_, col_, zombie_, NUKE_;
    bool changed_;
    vector<vector<char>> map_;

public:
    int rows, col, zombie;
    Intro(int rows = 5, int col = 9, int zombie = 1, int NUKE = 1, string choice = "", bool changed = false);
    void changeSettings();
    void displayIntro();
    void displayGame();
    void mapinit(int rows, int col, int zombie);
    void newBoard(Intro &intro, int rows, int col, int zombie, bool changed, int NUKE);
    void setObject(int col, int rows, char ch);
    int getRows() const;
    int getCol() const;
    int getZombie() const;
    bool getChanged() const;
    bool isRock(int x, int y);
    bool isZombie(int x, int y);
    char getObject(int x, int y);
    void removeTrail();
    bool isArrow(int x, int y);
    bool isHealth(int x, int y);
    bool isPod(int x, int y);
    void setZombie(int newZombie);
    bool isObject(int x, int y);
    void init();
    vector<vector<char>> getMap();
    void setMap(const vector<vector<char>> &newMap);
    void setRows(int row);
    void setCol(int col);
    void setZombies(int zombiess);
    bool isNUKE();
    int getNUKE() const;
    void setNUKE(int nuke);
};

class Alien
{
private:
    int life_, attack_, range_, x_, y_, zombie2_, stamina_;
    bool redir_;
    char alien_; // 'A'
    string dir_; // up, down, left, right and other commands
    int zombie_life[5] = {100, 150, 200, 250, 300};
    int zombie_attack[4] = {5, 10, 15, 20};
    int zombie_range[3] = {1, 2, 3};
    vector<int> random_life;
    vector<int> random_attack;
    vector<int> random_range;
    vector<char> zombies = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    vector<int> zombieCoordX;
    vector<int> zombieCoordY;
    int count = 0;

public:
    Alien(int life = 100, int attack = 0, int range = 0, int stamina = 5);
    void alienPos(Intro &intro);
    void move(Intro &intro);
    void alienDisplay(Intro &intro, Alien &alien);
    void charAttri(Intro &intro);
    void randomAttri(Intro &intro);
    int newAlienPosX(Intro &intro);
    int newAlienPosY(Intro &intro);
    void alienUp(Intro &intro);
    void alienLeft(Intro &intro);
    void alienRight(Intro &intro);
    void alienDown(Intro &intro);
    void moveLeft(Intro &intro);
    void moveRight(Intro &intro);
    void moveUp(Intro &intro);
    void moveDown(Intro &intro);
    void changeArrow(Intro &intro);
    void zombiePos(Intro &intro);
    void showHelp(Intro &intro);
    void zombieMove(Intro &intro);
    vector<int> getCoordX(Intro &intro);
    vector<int> getCoordY(Intro &intro);
    void closestZombie(Intro &intro);
    void hitZombie(Intro &intro, int x, int y);
    void deadZombie(Intro &intro);
    bool isMoveValid(int x, int y);
    void zombieAttack(Intro &intro, char zombie, int zombiex, int zombiey, int range, int attack);
    bool isWin(Intro &intro);
    bool isLose();
    void result(Intro &intro);
    void playAgain(Intro &intro);
    void resetGame(Intro &intro);
    void saveGame(Intro &intro);
    void loadGame(Intro &intro);
};

void Alien::loadGame(Intro &intro)
{
    string filename;
    cout << "Enter the file name to load the game progress from:  ";
    cin >> filename;
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Failed to open file for reading." << endl;
        return;
    }
    // load game settings
    int rows = 0, col = 0, zombiess = 0, NUKE = 0;
    file >> rows;
    intro.setRows(rows);

    file >> col;
    intro.setCol(col);

    file >> zombiess;
    intro.setZombies(zombiess);

    file >> stamina_;
    file >> NUKE;
    intro.setNUKE(NUKE);

    // Load alien settings
    file >> x_;
    file >> y_;
    file >> life_;
    file >> attack_;

    // map loading
    std::vector<std::vector<char>> newMap;
    newMap.resize(intro.getRows(), std::vector<char>(intro.getCol()));
    for (int i = 0; i < intro.getRows(); i++)
    {
        for (int j = 0; j < intro.getCol(); j++)
        {
            char c;
            file >> c;
            if (c == '-')
            {
                newMap[i][j] = ' ';
            }
            else
            {
                newMap[i][j] = c;
            }
        }
    }
    intro.setMap(newMap);

    cout << "Game data loaded from file.\n";

    // Load zombie attributes
    for (int i = 0; i <= intro.getZombie(); i++)
    {
        int life, range, attack, col, row;
        file >> life >> attack >> range >> col >> row;
        random_life[i] = life;
        random_attack[i] = attack;
        random_range[i] = range;
        zombieCoordX[i] = col;
        zombieCoordY[i] = row;
    }

    file.close();
}

void Alien::saveGame(Intro &intro)
{
    cout << "Enter  the file name to save your progress => ";

    cin >> dir_;
    ofstream file(dir_);
    dir_.clear();
    cout << endl;
    cout << "Game saved." << endl;
    pf::Pause();
    pf::ClearScreen();
    if (file.is_open())
    {

        file << intro.getRows() << endl;
        file << intro.getCol() << endl;

        file << intro.getZombie() << endl;
        file << stamina_ << endl;
        file << intro.getNUKE() << endl;

        file << newAlienPosX(intro) << endl;

        file << newAlienPosY(intro) << endl;

        file << life_ << endl;

        file << attack_ << endl;

        for (int i = 0; i < intro.getRows(); i++)
        {
            for (int j = 0; j < intro.getCol(); j++)
            {
                if (intro.getMap()[i][j] == ' ')
                {
                    file << '-';
                }
                else
                {
                    file << intro.getMap()[i][j];
                }
            }
            file << endl;
        }

        for (int i = 0; i < intro.getZombie(); i++)
        {
            file << random_life[i] << endl;
            file << random_attack[i] << endl;
            file << random_range[i] << endl;
            file << zombieCoordX[i] << endl;
            file << zombieCoordY[i] << endl;
        }
    }
    file.close();
}

void Alien::resetGame(Intro &intro)
{
    zombies.clear();
    zombies = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    intro.setZombie(0);
    x_ = 0;
    y_ = 0;
    zombieCoordX.clear();
    zombieCoordY.clear();
    random_life.clear();
    random_range.clear();
    random_attack.clear();
    intro.init();
    stamina_ = 5;
    intro.mapinit(intro.getRows(), intro.getCol(), intro.getZombie());
    randomAttri(intro);
    alienPos(intro);
    zombiePos(intro);
    intro.displayIntro();
    life_ = 100;
}

void Alien::playAgain(Intro &intro)
{
    cout << "Do you want to play again? (y/n) =>  ";
    dir_.clear();
    cin >> dir_;
    cout << endl;
    transform(dir_.begin(), dir_.end(), dir_.begin(), ::tolower);
    if (dir_ == "y")
    {
        cout << "Very well! You may proceed." << endl;
        pf::Pause();
        pf::ClearScreen();
        dir_.clear();
        resetGame(intro);
    }
    else if (dir_ == "n")
    {
        cout << "See you next time!" << endl;
        pf::Pause();
        pf::ClearScreen();
        dir_.clear();
        exit(0);
    }
    else
    {
        while (true)
        {
            cout << "Please enter a valid input. (y/n) => ";
            dir_.clear();
            cin >> dir_;
            transform(dir_.begin(), dir_.end(), dir_.begin(), ::tolower);
            cout << endl;
            if (dir_ == "n")
            {
                cout << "See you next time!" << endl;
                pf::Pause();
                pf::ClearScreen();
                dir_.clear();
                exit(0);
            }
            else if (dir_ == "y")
            {
                cout << "Very well! You may proceed." << endl;
                pf::Pause();
                pf::ClearScreen();
                dir_.clear();
                resetGame(intro);

                break;
            }
        }
    }
}

void Alien::result(Intro &intro)
{
    if (isWin(intro))
    {
        cout << "You Win!" << endl;
        playAgain(intro);
    }
    else if (isLose())
    {
        cout << "You Lose!" << endl;
        playAgain(intro);
    }
}

bool Alien::isWin(Intro &intro)
{
    if (intro.getZombie() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Alien::isLose()
{
    if (life_ == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Alien::zombieAttack(Intro &intro, char zombies, int zombieCoordX, int zombieCoordY, int range, int random_attack)
{

    // if alien is within the range
    for (int j = 1; j <= range; j++)
    {
        // if alien is above zombie and assuring that zombie does not look beyond border
        if (zombieCoordY + j < intro.getRows() + 1 && intro.getObject(zombieCoordX, zombieCoordY + j) == 'A')
        {
            life_ -= random_attack;
            if (life_ < 0)
            {
                life_ = 0;
            }
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies << " attacks the Alien." << endl;
            cout << "Alien receives a damage of " << random_attack << "." << endl;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (zombieCoordY - j > 0 && intro.getObject(zombieCoordX, zombieCoordY - j) == 'A')
        {
            life_ -= random_attack;
            if (life_ < 0)
            {
                life_ = 0;
            }
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies << " attacks the Alien." << endl;
            cout << "Alien receives a damage of " << random_attack << "." << endl;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (zombieCoordX + j < intro.getCol() && intro.getObject(zombieCoordX + j, zombieCoordY) == 'A')
        {
            life_ -= random_attack;
            if (life_ < 0)
            {
                life_ = 0;
            }
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies << " attacks the Alien." << endl;
            cout << "Alien receives a damage of " << random_attack << "." << endl;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (zombieCoordX - j > 0 && intro.getObject(zombieCoordX - j, zombieCoordY) == 'A')
        {
            life_ -= random_attack;
            if (life_ < 0)
            {
                life_ = 0;
            }
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies << " attacks the Alien." << endl;
            cout << "Alien receives a damage of " << random_attack << "." << endl;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
    }
    // if alien is not in range
    if (life_ > 0)
    {
        intro.displayGame();
        charAttri(intro);
        cout << "Zombie " << zombies << " ends its turn." << endl;
        pf::Pause();
        pf::ClearScreen();
    }
}

bool Alien::isMoveValid(int x, int y)
{
    Intro intro;
    if (!(intro.isObject(x, y)) || !(intro.isZombie(x, y)))
    {
        return true;
    }
    else
        return false;
}

void Alien::deadZombie(Intro &intro)
{
    int dead_count = 0;
    for (int i = 0; i < intro.getZombie(); i++)
    {
        if (random_life[i] == 0)
        {
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien has killed Zombie " << zombies[i] << "!" << endl;
            pf::Pause();
            pf::ClearScreen();
            // removing the zombie from the map along with its coordinates.
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombies.erase(zombies.begin() + i);
            random_life.erase(random_life.begin() + i);
            random_attack.erase(random_attack.begin() + i);
            random_range.erase(random_range.begin() + i);
            zombieCoordX.erase(zombieCoordX.begin() + i);
            zombieCoordY.erase(zombieCoordY.begin() + i);
            dead_count++;
        }
    }
    intro.setZombie(intro.getZombie() - dead_count);
}

void Alien::closestZombie(Intro &intro)
{
    int closest_zombie = -1;
    int closest_dis = INT_MAX;
    vector<int> coord_x = getCoordX(intro);
    vector<int> coord_y = getCoordY(intro);
    // distance between zombie and alien
    for (int i = 0; i < intro.getZombie(); i++)
    {
        int x_dis = abs(coord_x[i] - newAlienPosX(intro));
        int y_dis = abs(coord_y[i] - newAlienPosY(intro));
        int total_dis = x_dis + y_dis;
        if (total_dis < closest_dis)
        {
            closest_dis = total_dis;
            closest_zombie = i;
        }
    }
    // reducing the life of the closest zombie
    if (closest_zombie != -1)
    {
        random_life[closest_zombie] -= 10;
        // to make sure the zombies dont get negative life
        if (random_life[closest_zombie] <= 0)
        {
            random_life[closest_zombie] = 0;
        }
    }
}

void Alien::hitZombie(Intro &intro, int x, int y)
{
    for (int i = 0; i < intro.getZombie(); i++)
    {
        if (intro.getObject(x, y) == zombies[i])
        {
            random_life[i] -= attack_;
            if (random_life[i] <= 0)
            {
                random_life[i] = 0;
            }
            break;
        }
    }
}

void Alien::zombieMove(Intro &intro)
{
    for (int i = 0; i < intro.getZombie(); i++)
    {
        // if above is available, zombie  moves up
        if (zombieCoordY[i] + 1 < intro.getRows() + 1 && intro.getObject(zombieCoordX[i], zombieCoordY[i] + 1) == 'r')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved up by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        //  if below is avaibale, zombie moves down
        else if (zombieCoordY[i] - 1 > 0 && intro.getObject(zombieCoordX[i], zombieCoordY[i] - 1) == 'r')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved down by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if  right is  available, zombie moves right
        else if (zombieCoordX[i] + 1 < intro.getCol() && intro.getObject(zombieCoordX[i] + 1, zombieCoordY[i]) == 'r')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved right by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if left is avaiable, zombie moves left.
        else if (zombieCoordX[i] - 1 > 0 && intro.getObject(zombieCoordX[i] - 1, zombieCoordY[i]) == 'r')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved left by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        else if (zombieCoordY[i] + 1 < intro.getRows() + 1 && intro.getObject(zombieCoordX[i], zombieCoordY[i] + 1) == 'h')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved up by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        //  if below is avaibale, zombie moves down
        else if (zombieCoordY[i] - 1 > 0 && intro.getObject(zombieCoordX[i], zombieCoordY[i] - 1) == 'h')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved down by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if  right is  available, zombie moves right
        else if (zombieCoordX[i] + 1 < intro.getCol() && intro.getObject(zombieCoordX[i] + 1, zombieCoordY[i]) == 'h')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved right by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if left is avaiable, zombie moves left.
        else if (zombieCoordX[i] - 1 > 0 && intro.getObject(zombieCoordX[i] - 1, zombieCoordY[i]) == 'h')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved left by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        else if (zombieCoordY[i] + 1 < intro.getRows() + 1 && intro.getObject(zombieCoordX[i], zombieCoordY[i] + 1) == 'p')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved up by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        //  if below is avaibale, zombie moves down
        else if (zombieCoordY[i] - 1 > 0 && intro.getObject(zombieCoordX[i], zombieCoordY[i] - 1) == 'p')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved down by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if  right is  available, zombie moves right
        else if (zombieCoordX[i] + 1 < intro.getCol() && intro.getObject(zombieCoordX[i] + 1, zombieCoordY[i]) == 'p')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved right by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if left is avaiable, zombie moves left.
        else if (zombieCoordX[i] - 1 > 0 && intro.getObject(zombieCoordX[i] - 1, zombieCoordY[i]) == 'p')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved left by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        else if (zombieCoordY[i] + 1 < intro.getRows() + 1 && intro.getObject(zombieCoordX[i], zombieCoordY[i] + 1) == '<')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved up by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        //  if below is avaibale, zombie moves down
        else if (zombieCoordY[i] - 1 > 0 && intro.getObject(zombieCoordX[i], zombieCoordY[i] - 1) == '<')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved down by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if  right is  available, zombie moves right
        else if (zombieCoordX[i] + 1 < intro.getCol() && intro.getObject(zombieCoordX[i] + 1, zombieCoordY[i]) == '<')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved right by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if left is avaiable, zombie moves left.
        else if (zombieCoordX[i] - 1 > 0 && intro.getObject(zombieCoordX[i] - 1, zombieCoordY[i]) == '<')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved left by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        else if (zombieCoordY[i] + 1 < intro.getRows() + 1 && intro.getObject(zombieCoordX[i], zombieCoordY[i] + 1) == '>')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved up by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        //  if below is avaibale, zombie moves down
        else if (zombieCoordY[i] - 1 > 0 && intro.getObject(zombieCoordX[i], zombieCoordY[i] - 1) == '>')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved down by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if  right is  available, zombie moves right
        else if (zombieCoordX[i] + 1 < intro.getCol() && intro.getObject(zombieCoordX[i] + 1, zombieCoordY[i]) == '>')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved right by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if left is avaiable, zombie moves left.
        else if (zombieCoordX[i] - 1 > 0 && intro.getObject(zombieCoordX[i] - 1, zombieCoordY[i]) == '>')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved left by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        else if (zombieCoordY[i] + 1 < intro.getRows() + 1 && intro.getObject(zombieCoordX[i], zombieCoordY[i] + 1) == 'v')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved up by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        //  if below is avaibale, zombie moves down
        else if (zombieCoordY[i] - 1 > 0 && intro.getObject(zombieCoordX[i], zombieCoordY[i] - 1) == 'v')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved down by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if  right is  available, zombie moves right
        else if (zombieCoordX[i] + 1 < intro.getCol() && intro.getObject(zombieCoordX[i] + 1, zombieCoordY[i]) == 'v')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved right by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if left is avaiable, zombie moves left.
        else if (zombieCoordX[i] - 1 > 0 && intro.getObject(zombieCoordX[i] - 1, zombieCoordY[i]) == 'v')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved left by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        else if (zombieCoordY[i] + 1 < intro.getRows() + 1 && intro.getObject(zombieCoordX[i], zombieCoordY[i] + 1) == '^')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved up by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        //  if below is avaibale, zombie moves down
        else if (zombieCoordY[i] - 1 > 0 && intro.getObject(zombieCoordX[i], zombieCoordY[i] - 1) == '^')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved down by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if  right is  available, zombie moves right
        else if (zombieCoordX[i] + 1 < intro.getCol() && intro.getObject(zombieCoordX[i] + 1, zombieCoordY[i]) == '^')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved right by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if left is avaiable, zombie moves left.
        else if (zombieCoordX[i] - 1 > 0 && intro.getObject(zombieCoordX[i] - 1, zombieCoordY[i]) == '^')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved left by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        else if (zombieCoordY[i] + 1 < intro.getRows() + 1 && intro.getObject(zombieCoordX[i], zombieCoordY[i] + 1) == ' ')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved up by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        //  if below is avaibale, zombie moves down
        else if (zombieCoordY[i] - 1 > 0 && intro.getObject(zombieCoordX[i], zombieCoordY[i] - 1) == ' ')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordY[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved down by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if  right is  available, zombie moves right
        else if (zombieCoordX[i] + 1 < intro.getCol() && intro.getObject(zombieCoordX[i] + 1, zombieCoordY[i]) == ' ')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]++;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved right by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
        // if left is avaiable, zombie moves left.
        else if (zombieCoordX[i] - 1 > 0 && intro.getObject(zombieCoordX[i] - 1, zombieCoordY[i]) == ' ')
        {
            intro.setObject(zombieCoordX[i], zombieCoordY[i], ' ');
            zombieCoordX[i]--;
            intro.setObject(zombieCoordX[i], zombieCoordY[i], zombies[i]);
            intro.displayGame();
            charAttri(intro);
            cout << "Zombie " << zombies[i] << " have moved left by one." << endl;
            pf::Pause();
            pf::ClearScreen();
            zombieAttack(intro, zombies[i], zombieCoordX[i], zombieCoordY[i], random_range[i], random_attack[i]);
            pf::ClearScreen();
            continue;
        }
    }
}

vector<int> Alien::getCoordX(Intro &intro)
{
    return zombieCoordX;
}
vector<int> Alien::getCoordY(Intro &intro)
{
    return zombieCoordY;
}

void Alien::showHelp(Intro &intro)
{
    intro.displayGame();
    charAttri(intro);
    cout << endl;
    cout << "   Commands" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "1. up   "
         << "              - Move up." << endl;
    cout << "2. down "
         << "              - Move down." << endl;
    cout << "3. right"
         << "              - Move right." << endl;
    cout << "4. left "
         << "              - Move left." << endl;
    cout << "5. arrow"
         << "              - Change the direction of an arrow" << endl;
    cout << "6. help "
         << "              - Display commands for the user." << endl;
    cout << "7. save "
         << "              - Save the game." << endl;
    cout << "8. load "
         << "              - Load a game." << endl;
    cout << "9. quit "
         << "              - Quit the game." << endl;
    cout << "10. new "
         << "              - Start a new game." << endl;
    cout << "11. nuke "
         << "              - Nuke all zombies in the map." << endl;
}

void Alien::zombiePos(Intro &intro)
{
    srand(time(NULL));
    for (int i = 0; i < intro.getZombie(); i++)
    {
        int y = rand() % intro.getRows() + 1;
        int x = rand() % intro.getCol() + 1;
        while (intro.getObject(x, y) != ' ')
        {
            y = rand() % intro.getRows() + 1;
            x = rand() % intro.getCol() + 1;
        }
        zombieCoordX.push_back(x);
        zombieCoordY.push_back(y);
        intro.setObject(x, y, zombies[i]);
    }
}

void Alien::moveLeft(Intro &intro)
{
    while (true)
    {
        if (!(newAlienPosX(intro) > 1))
        {
            intro.displayGame();
            charAttri(intro);
            cout << "Alien hits the border." << endl;
            attack_ = 0;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (intro.isHealth(x_ - 1, y_))
        {
            life_ = life_ + 20;
            if (life_ > 100)
            {
                life_ = 100;
            }
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up a health pack" << endl;
            cout << "Alien life increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.isRock(x_ - 1, y_))
        {
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien stumbles upon a rock." << endl;
            attack_ = 0;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (intro.isPod(x_ - 1, y_))
        {
            intro.displayGame();
            charAttri(intro);
            closestZombie(intro);
            deadZombie(intro);
            cout << "Alien picked up a pod." << endl;
            cout << "The pod dealt 10 damage to the nearest zombie." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.isZombie(x_ - 1, y_))
        {
            intro.displayGame();
            charAttri(intro);
            hitZombie(intro, x_ - 1, y_);
            deadZombie(intro);
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien have damaged the zombie by " << attack_ << "." << endl;
            if (intro.isZombie(x_ - 1, y_))
            {
                attack_ = 0;
                cout << "However, the zombie is still alive." << endl;
                pf::Pause();
                pf::ClearScreen();
                break;
            }
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.getObject(x_ - 1, y_) == '>')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            x_--;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveRight(intro);
            break;
        }
        else if (intro.getObject(x_ - 1, y_) == '<')
        {
            attack_ = attack_ + 20;
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.getObject(x_ - 1, y_) == '^')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            x_--;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveUp(intro);
            break;
        }
        else if (intro.getObject(x_ - 1, y_) == 'v')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            x_--;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveDown(intro);
            break;
        }
        intro.setObject(x_, y_, '.');
        x_--;
        intro.setObject(x_, y_, alien_);
        intro.displayGame();
        charAttri(intro);
        pf::Pause();
        pf::ClearScreen();
    }
}

void Alien::moveRight(Intro &intro)
{
    while (true)
    {
        if (!(newAlienPosX(intro) < intro.getCol()))
        {
            intro.displayGame();
            charAttri(intro);
            cout << "Alien hits the border." << endl;
            attack_ = 0;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (intro.isRock(x_ + 1, y_))
        {
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien stumbles upon a rock." << endl;
            attack_ = 0;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (intro.isHealth(x_ + 1, y_))
        {
            life_ = life_ + 20;
            if (life_ > 100)
            {
                life_ = 100;
            }
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up a health pack" << endl;
            cout << "Alien life increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.isPod(x_ + 1, y_))
        {
            intro.displayGame();
            charAttri(intro);
            closestZombie(intro);
            deadZombie(intro);
            cout << "Alien picked up a pod." << endl;
            cout << "The pod dealt 10 damage to the nearest zombie." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.isZombie(x_ + 1, y_))
        {
            intro.displayGame();
            charAttri(intro);
            hitZombie(intro, x_ + 1, y_);
            deadZombie(intro);
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien have damaged the zombie by " << attack_ << "." << endl;
            if (intro.isZombie(x_ + 1, y_))
            {
                attack_ = 0;
                cout << "However, the zombie is still alive." << endl;
                pf::Pause();
                pf::ClearScreen();
                break;
            }
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.getObject(x_ + 1, y_) == '^')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            x_++;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveUp(intro);
            break;
        }
        else if (intro.getObject(x_ + 1, y_) == '>')
        {
            attack_ = attack_ + 20;
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.getObject(x_ + 1, y_) == 'v')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            x_++;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveDown(intro);
            break;
        }
        else if (intro.getObject(x_ + 1, y_) == '<')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            x_++;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveLeft(intro);
            break;
        }
        intro.setObject(x_, y_, '.');
        x_++;
        intro.setObject(x_, y_, alien_);
        intro.displayGame();
        charAttri(intro);
        pf::Pause();
        pf::ClearScreen();
    }
}
void Alien::moveUp(Intro &intro)
{
    while (true)
    {
        if (!(newAlienPosY(intro) < intro.getRows()))
        {
            intro.displayGame();
            charAttri(intro);
            cout << "Alien hits the border." << endl;
            attack_ = 0;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (intro.isRock(x_, y_ + 1))
        {
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien stumbles upon a rock." << endl;
            attack_ = 0;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (intro.isZombie(x_, y_ + 1))
        {
            intro.displayGame();
            charAttri(intro);
            hitZombie(intro, x_, y_ + 1);
            deadZombie(intro);
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien have damaged the zombie by " << attack_ << "." << endl;
            if (intro.isZombie(x_, y_ + 1))
            {
                attack_ = 0;
                cout << "However, the zombie is still alive." << endl;
                pf::Pause();
                pf::ClearScreen();
                break;
            }
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.isPod(x_, y_ + 1))
        {
            intro.displayGame();
            charAttri(intro);
            closestZombie(intro);
            deadZombie(intro);
            cout << "Alien picked up a pod." << endl;
            cout << "The pod dealt 10 damage to the nearest zombie." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.isHealth(x_, y_ + 1))
        {
            life_ = life_ + 20;
            if (life_ > 100)
            {
                life_ = 100;
            }
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up a health pack" << endl;
            cout << "Alien life increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.getObject(x_, y_ + 1) == 'v')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            y_++;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveDown(intro);
            break;
        }
        else if (intro.getObject(x_, y_ + 1) == '^')
        {
            attack_ = attack_ + 20;
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.getObject(x_, y_ + 1) == '<')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            y_++;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveLeft(intro);
            break;
        }
        else if (intro.getObject(x_, y_ + 1) == '>')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            y_++;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveRight(intro);
            break;
        }

        intro.setObject(x_, y_, '.');
        y_++;
        intro.setObject(x_, y_, alien_);
        intro.displayGame();
        charAttri(intro);
        pf::Pause();
        pf::ClearScreen();
    }
}
void Alien::moveDown(Intro &intro)
{
    while (true)
    {
        if (!(newAlienPosY(intro) > 1))
        {
            intro.displayGame();
            charAttri(intro);
            cout << "Alien hits the border." << endl;
            attack_ = 0;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (intro.isRock(x_, y_ - 1))
        {
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien stumbles upon a rock." << endl;
            attack_ = 0;
            pf::Pause();
            pf::ClearScreen();
            break;
        }
        else if (intro.isZombie(x_, y_ - 1))
        {
            intro.displayGame();
            charAttri(intro);
            hitZombie(intro, x_, y_ - 1);
            deadZombie(intro);
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
            cout << "Alien have damaged the zombie by " << attack_ << "." << endl;
            if (intro.isZombie(x_, y_ - 1))
            {
                attack_ = 0;
                cout << "However, the zombie is still alive." << endl;
                pf::Pause();
                pf::ClearScreen();
                break;
            }
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.isPod(x_, y_ - 1))
        {
            intro.displayGame();
            charAttri(intro);
            closestZombie(intro);
            deadZombie(intro);
            cout << "Alien picked up a pod." << endl;
            cout << "The pod dealt 10 damage to the nearest zombie." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.isHealth(x_, y_ - 1))
        {
            life_ = life_ + 20;
            if (life_ > 100)
            {
                life_ = 100;
            }
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up a health pack" << endl;
            cout << "Alien life increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.getObject(x_, y_ - 1) == '>')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            y_--;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveRight(intro);
            break;
        }
        else if (intro.getObject(x_, y_ - 1) == 'v')
        {
            attack_ = attack_ + 20;
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
        }
        else if (intro.getObject(x_, y_ - 1) == '<')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            y_--;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveLeft(intro);
            break;
        }
        else if (intro.getObject(x_, y_ - 1) == '^')
        {
            attack_ = attack_ + 20;
            intro.setObject(x_, y_, '.');
            y_--;
            intro.setObject(x_, y_, alien_);
            intro.displayGame();
            charAttri(intro);
            cout << "Alien picks up an arrow, and attack increased by 20." << endl;
            pf::Pause();
            pf::ClearScreen();
            moveUp(intro);
            break;
        }

        intro.setObject(x_, y_, '.');
        y_--;
        intro.setObject(x_, y_, alien_);
        intro.displayGame();
        charAttri(intro);
        pf::Pause();
        pf::ClearScreen();
    }
}

void Alien::charAttri(Intro &intro)
{
    cout << setw(8) << "Nuke:"
         << " " << intro.getNUKE() << endl;
    cout << setw(11) << "Stamina:"
         << " " << stamina_ << "/5" << endl;
    cout << setw(9) << "Alien "
         << "   : Life " << life_ << " attack " << attack_ << endl;
    for (int i = 0; i <= intro.getZombie() - 1; i++)
    {
        cout << setw(10) << "Zombie " << zombies[i] << " : Life " << random_life[i] << " attack " << random_attack[i] << " range : " << random_range[i] << endl;
    }
    cout << endl;
}

void Alien::randomAttri(Intro &intro)
{
    for (int i = 0; i <= intro.getZombie(); i++)
    {
        random_life.push_back(zombie_life[rand() % size(zombie_life)]);
        random_attack.push_back(zombie_attack[rand() % size(zombie_attack)]);
        random_range.push_back(zombie_range[rand() % size(zombie_range)]);
    }
}

int Alien::newAlienPosX(Intro &intro)
{
    return x_;
}
int Alien::newAlienPosY(Intro &intro)
{
    return y_;
}

void Alien::alienDisplay(Intro &intro, Alien &alien)
{
    intro.displayGame();
}

void Alien::alienUp(Intro &intro)
{
    moveUp(intro);
}
void Alien::alienLeft(Intro &intro)
{
    moveLeft(intro);
}
void Alien::alienRight(Intro &intro)
{
    moveRight(intro);
}
void Alien::alienDown(Intro &intro)
{
    moveDown(intro);
}

// when alien reaches boundary, when alien hits rock object, when he attack a zombie but the zombie lives the attack.

void Alien::move(Intro &intro)
{
    bool zombieTurn = false;
    char empty = ' ';
    dir_ = ' ';
    dir_.clear();
    cout << "command> ";
    cin >> dir_;
    cout << endl;
    pf::ClearScreen();
    if (dir_ == "up")
    {
        attack_ += 20;
        intro.displayGame();
        charAttri(intro);
        cout << "Alien gains " << attack_ << " attack in the first move." << endl;
        pf::Pause();
        pf::ClearScreen();
        dir_.clear();
        alienUp(intro);
        intro.removeTrail();
        zombieTurn = true;
    }

    else if (dir_ == "down")
    {
        attack_ += 20;
        intro.displayGame();
        charAttri(intro);
        cout << "Alien gains " << attack_ << " attack in the first move." << endl;
        pf::Pause();
        pf::ClearScreen();
        dir_.clear();
        alienDown(intro);
        intro.removeTrail();
        zombieTurn = true;
    }
    else if (dir_ == "left")
    {
        attack_ += 20;
        intro.displayGame();
        charAttri(intro);
        cout << "Alien gains " << attack_ << " attack in the first move." << endl;
        pf::Pause();
        pf::ClearScreen();
        dir_.clear();
        alienLeft(intro);
        intro.removeTrail();
        zombieTurn = true;
    }
    else if (dir_ == "right")
    {
        attack_ += 20;
        intro.displayGame();
        charAttri(intro);
        cout << "Alien gains " << attack_ << " attack in the first move." << endl;
        pf::Pause();
        pf::ClearScreen();
        dir_.clear();
        alienRight(intro);
        intro.removeTrail();
        zombieTurn = true;
    }
    else if (dir_ == "arrow")
    {
        dir_.clear();
        changeArrow(intro);
        pf::Pause();
        pf::ClearScreen();
        zombieTurn = false;
    }
    else if (dir_ == "help")
    {
        dir_.clear();
        showHelp(intro);
        pf::Pause();
        pf::ClearScreen();
        intro.displayGame();
        charAttri(intro);
        zombieTurn = false;
    }
    else if (dir_ == "save")
    {
        dir_.clear();
        intro.displayGame();
        charAttri(intro);
        saveGame(intro);
    }
    else if (dir_ == "load")
    {
        dir_.clear();
        intro.displayGame();
        charAttri(intro);
        loadGame(intro);
        cout << "Loading progress..." << endl;
        pf::Pause();
        pf::ClearScreen();
    }
    else if (dir_ == "quit")
    {
        dir_.clear();
        cout << "Are you sure you want to quit the game? (y/n) => ";
        cin >> dir_;
        transform(dir_.begin(), dir_.end(), dir_.begin(), ::tolower);
        cout << endl;
        if (dir_ == "y")
        {
            cout << "See you next time!" << endl;
            pf::Pause();
            pf::ClearScreen();
            dir_.clear();
            exit(0);
        }
        else if (dir_ == "n")
        {
            cout << "You may continue." << endl;
            dir_.clear();
            pf::Pause();
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
        }

        else
        {
            while (true)
            {
                cout << "Please enter a valid input. (y/n) => ";
                dir_.clear();
                cin >> dir_;
                transform(dir_.begin(), dir_.end(), dir_.begin(), ::tolower);
                cout << endl;
                if (dir_ == "y")
                {
                    cout << "See you next time!" << endl;
                    pf::Pause();
                    pf::ClearScreen();
                    dir_.clear();
                    exit(0);
                }
                else if (dir_ == "n")
                {
                    cout << "You may continue." << endl;

                    break;
                }
            }
        }
    }
    else if (dir_ == "nuke")
    {
        if (intro.isNUKE())
        {
            for (int i = 0; i < intro.getZombie(); i++)
            {
                random_life[i] -= 50;
                if (random_life[i] <= 0)
                {
                    random_life[i] = 0;
                    deadZombie(intro);
                }
            }

            intro.NUKE_--;
            intro.displayGame();
            charAttri(intro);
            cout << "Zombies are hit with a nuke! " << endl;
            pf::Pause();
            pf::ClearScreen();
            dir_.clear();
            zombieTurn = true;
        }
        else
        {

            intro.displayGame();
            charAttri(intro);
            cout << "There are no nukes left. " << endl;
            pf::Pause();
            pf::ClearScreen();
            dir_.clear();
        }
    }
    else if (dir_ == "new")
    {
        dir_.clear();
        cout << "Are you sure you want to start a new game? (y/n) => ";
        cin >> dir_;
        transform(dir_.begin(), dir_.end(), dir_.begin(), ::tolower);
        cout << endl;
        if (dir_ == "y")
        {

            pf::Pause();
            pf::ClearScreen();
            dir_.clear();
            resetGame(intro);
        }
        else if (dir_ == "n")
        {
            cout << "You may continue." << endl;
            dir_.clear();
            pf::Pause();
            pf::ClearScreen();
            intro.displayGame();
            charAttri(intro);
        }
        else
        {
            while (true)
            {
                cout << "Please enter a valid input. (y/n) => ";
                dir_.clear();
                cin >> dir_;
                transform(dir_.begin(), dir_.end(), dir_.begin(), ::tolower);
                cout << endl;
                if (dir_ == "y")
                {
                    cout << "See you next time!" << endl;
                    pf::Pause();
                    pf::ClearScreen();
                    dir_.clear();
                    resetGame(intro);
                    break;
                }
                else if (dir_ == "n")
                {
                    cout << "You may continue." << endl;
                    dir_.clear();
                    pf::Pause();
                    pf::ClearScreen();
                    intro.displayGame();
                    charAttri(intro);
                    break;
                }
            }
        }
    }

    else
    {
        intro.displayGame();
        charAttri(intro);
        cout << "Invalid input, type 'help' to see available commands." << endl;
        pf::Pause();
        pf::ClearScreen();
    }

    while (zombieTurn == true)
    {

        zombieMove(intro);
        result(intro);
        zombieTurn = false;
        stamina_ = 5;
        break;
    }
}

void Alien::changeArrow(Intro &intro)
{
    if (stamina_ > 0)
    {
        char object;
        dir_.clear();
        int row;
        int col;
        intro.displayGame();
        charAttri(intro);
        stamina_ -= 1;
        cout << "Enter rows => ";
        while (!(cin >> row) || (row <= 0) || (row > intro.getRows()))
        {
            cout << "Invalid input. Please enter a valid integer value => ";
            cin.clear();
            cin.ignore(INT_MAX, '\n'); // to clear input buffer
        }
        cout << endl;
        cout << "Enter column => ";
        while (!(cin >> col) || (col <= 0) || (col > intro.getCol()))
        {
            cout << "Invalid input. Please enter a valid integer value => ";
            cin.clear();
            cin.ignore(INT_MAX, '\n'); // to clear input buffer
        }
        cout << endl;
        cout << "Enter direction => ";
        cin >> dir_;
        while (dir_ != "left" && dir_ != "right" && dir_ != "up" && dir_ != "down")
        {
            cout << "Invalid input, please enter a valid direction => ";
            cin.clear();
            dir_.clear();
            cin.ignore(INT_MAX, '\n'); // to clear input buffer
            cin >> dir_;
        }
        cout << endl;
        // to find coordinates of the given values.
        object = intro.getObject(col, intro.getRows() - row + 1);
        if (dir_ == "left" && intro.isArrow(col, intro.getRows() - row + 1))
        {
            intro.setObject(col, intro.getRows() - row + 1, '<');
            cout << "Arrow " << object << " is now switched to <" << endl;
        }
        else if (dir_ == "right" && intro.isArrow(col, intro.getRows() - row + 1))
        {
            intro.setObject(col, intro.getRows() - row + 1, '>');
            cout << "Arrow " << object << " is now switched to >" << endl;
        }
        else if (dir_ == "up" && intro.isArrow(col, intro.getRows() - row + 1))
        {
            intro.setObject(col, intro.getRows() - row + 1, '^');
            cout << "Arrow " << object << " is now switched to ^" << endl;
        }
        else if (dir_ == "down" && intro.isArrow(col, intro.getRows() - row + 1))
        {
            intro.setObject(col, intro.getRows() - row + 1, 'v');
            cout << "Arrow " << object << " is now switched to v" << endl;
        }
        else
        {
            cout << "There are no arrows in that area." << endl;
        }
    }
    else
    {
        intro.displayGame();
        charAttri(intro);
        cout << "Alien ran out of stamina." << endl;
        dir_.clear();
        pf::ClearScreen;
        // pf::Pause;
    }
}

Alien::Alien(int life, int attack, int range, int stamina)
{
    life_ = life;
    attack_ = attack;
    range_ = range;
    stamina_ = stamina;
}

void Alien::alienPos(Intro &intro)
{
    alien_ = {'A'};
    y_ = (intro.getRows() + 1) / 2;
    x_ = (intro.getCol() + 1) / 2;
    intro.setObject(x_, y_, alien_);
}

bool Intro::isObject(int x, int y)
{
    return map_[rows_ - rows][col - 1] == 'A';
}

char Intro::getObject(int x, int y)
{
    return map_[rows_ - y][x - 1];
}

// this function requires modificiation, it doesn't work.
bool Intro::isZombie(int x, int y)
{
    if (map_[rows_ - y][x - 1] == '1')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '2')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '3')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '4')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '5')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '6')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '7')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '8')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Intro::setNUKE(int nuke)
{
    NUKE_ = nuke;
}

void Intro::setCol(int col)
{
    col_ = col;
}

void Intro::setRows(int row)
{
    rows_ = row;
}

void Intro::setZombies(int zombiess)
{
    zombie_ = zombiess;
}

void Intro::newBoard(Intro &intro, int rows, int col, int zombie, bool changed, int NUKE)
{
    changed_ = true;
    NUKE_ = NUKE;
    Alien alien;
    int n = 0;
    // to make sure the objects in the map does not change every alien movement.
    while (n < 1)
    {
        mapinit(rows, col, zombie);
        alien.randomAttri(intro);
        n++;
        break;
    }
    alien.zombiePos(intro);
    alien.alienPos(intro);
    while (true)
    {
        pf::ClearScreen();
        displayGame();
        alien.charAttri(intro);
        alien.move(intro);
    }
}

bool Intro::isHealth(int x, int y)
{
    return map_[rows_ - y][x - 1] == 'h';
}

bool Intro::isArrow(int x, int y)
{
    if (map_[rows_ - y][x - 1] == '>')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '<')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == '^')
    {
        return true;
    }
    else if (map_[rows_ - y][x - 1] == 'v')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Intro::setMap(const vector<vector<char>> &newMap)
{
    map_ = newMap;
}

void Intro::setZombie(int newZombie)
{
    zombie_ = newZombie;
}

bool Intro::isRock(int x, int y)
{
    return map_[rows_ - y][x - 1] == 'r';
}
bool Intro::isNUKE()
{
    if (NUKE_ > 0)
    {
        return true;
    }

    else
    {
        return false;
    }
}
bool Intro::isPod(int x, int y)
{
    return map_[rows_ - y][x - 1] == 'p';
}
int Intro::getNUKE() const
{
    return NUKE_;
}
bool Intro::getChanged() const
{
    return changed_;
}

int Intro::getZombie() const
{
    return zombie_;
}

int Intro::getCol() const
{
    return col_;
}

void Intro::init()
{
    rows_ = 5;
    col_ = 9;
    zombie_ = 1;
    NUKE_ = 1;
    changed_ = false;
    choice_ = "";
}

void Intro::removeTrail()
{
    Alien alien;
    char objects[] = {' ', 'p', 'h', 'h', '^', '<', '>', 'v', 'r'};
    int noObjects = size(objects);

    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < col_; j++)
        {
            int noObj = rand() % noObjects;
            if (map_[i][j] == '.')
            {
                setObject(j + 1, rows_ - i, objects[noObj]);
            }
        }
    }
}

int Intro::getRows() const
{
    return rows_;
}

void Intro::setObject(int col, int rows, char ch)
{
    map_[rows_ - rows][col - 1] = ch;
}

void Intro::mapinit(int rows, int col, int zombie)
{
    rows_ = rows;
    col_ = col;
    zombie_ = zombie;

    char objects[] = {' ', ' ', ' ', ' ', 'p', 'h', 'r', '^', '<', '>', 'v'};
    int noObjects = size(objects);
    // dynamic 2D array
    map_.resize(rows_); // create empty rows
    for (int i = 0; i < rows_; ++i)
    {
        map_[i].resize(col_); // resize each row
    }
    int noObj;
    // adding the objects into the map
    for (int i = 0; i < rows_; i++)
    {
        for (int j = 0; j < col_; j++)
        {
            noObj = rand() % noObjects; // generating random characters

            map_[i][j] = objects[noObj];
        }
    }
}

Intro::Intro(int rows, int col, int zombie, int NUKE, string choice, bool changed)
{
    rows_ = rows;
    col_ = col;
    zombie_ = zombie;
    NUKE_ = NUKE;
    choice_ = choice;
    changed_ = changed;
    mapinit(rows, col, zombie);
}

void Intro::displayGame()
{
    Alien alien;

    // to put the title in the middle part of the board.
    cout << setw((col_ + 2) / 2) << ". : Alien vs Zombie : ." << endl;
    for (int i = 0; i < rows_; i++)
    { // row is Y, col is X
        cout << "   ";
        for (int j = 0; j < col_; j++)
        {
            cout << "+---";
        }
        cout << "+" << endl;
        // row number diplaying
        cout << setw(2) << (i + 1);
        for (int j = 0; j < col_; j++)
        {
            cout << " | " << map_[i][j];
        }
        cout << " |" << endl;
    }
    cout << "   ";
    for (int j = 0; j < col_; j++)
    {
        cout << "+---";
    }
    cout << "+" << endl;

    cout << "             ";
    // column numbering displays
    for (int j = 0; j < col_; j++)
    {
        int n = (j + 1) / 10;
        cout << " "; // between
        if (n == 0)
        {
            cout << setw(3);
        }
        else
        {
            cout << n << "  ";
        }
    }
    cout << endl;
    cout << "    ";
    for (int j = 0; j < col_; j++)
    {
        cout << " " << (j + 1) % 10 << "  ";
    }
    cout << endl
         << endl;
}

vector<vector<char>> Intro::getMap()
{
    return map_;
}

void Intro::displayIntro()
{
    Intro intro;
    Alien alien;
    cout << "+--------------------------+" << endl;
    cout << "|  Default Game Settings   |" << endl;
    cout << "+--------------------------+" << endl;
    cout << endl;

    cout.width(5);
    cout << "Board Rows ";
    cout.width(6);
    cout << ":";
    cout.width(3);
    cout << rows_ << endl;

    cout.width(5);
    cout << "Board Columns ";
    cout.width(3);
    cout << ":";
    cout.width(3);
    cout << col_ << endl;

    cout.width(5);
    cout << "Zombie Count ";
    cout.width(4);
    cout << ":";
    cout.width(3);
    cout << zombie_ << endl;

    cout.width(5);
    cout << "Number of nukes ";
    cout.width(1);
    cout << ":";
    cout.width(3);
    cout << NUKE_ << endl;

    while (true)
    {

        cout << "Would you like to change the game settings (y/n)? => ";
        cin >> choice_;

        if (choice_ == "y" || choice_ == "Y")
        {
            choice_.clear();
            changeSettings();
        }

        else if ((choice_ != "y" && choice_ != "Y") && (choice_ != "n" && choice_ != "N"))
        {
            while (true)
            {
                choice_.clear();
                cout << "Invalid input, Please enter y/n => ";
                cin >> choice_;

                if (choice_ == "y" || choice_ == "Y")
                {
                    choice_.clear();
                    changeSettings();
                    break;
                }

                else if (choice_ == "n" || choice_ == "N")
                {
                    choice_.clear();
                    pf::ClearScreen();
                    displayGame();
                    break;
                }
            }
        }
        else if (choice_ == "n" || choice_ == "N")
        {
            choice_.clear();
            pf::ClearScreen();
            displayGame();
            break;
        }
        break;
    }
}

void Intro::changeSettings()
{
    Intro intro;
    Alien alien;
    pf::ClearScreen();

    if (choice_ == "n" || choice_ == "N")
    {
        return;
    }

    cout << "+--------------------------+" << endl;
    cout << "|  Board settings          |" << endl;
    cout << "+--------------------------+" << endl;
    cout << endl;

    cout << "Enter rows => ";
    while (!(cin >> rows_) || (rows_ % 2 == 0) || (rows_ <= 0) || rows_ >= 29) // Even positive integer
    {
        cout << "Invalid input. Please enter an odd integer value not more than or equal to 29 => ";
        cin.clear();
        cin.ignore(INT_MAX, '\n'); // to clear input buffer
    }

    cout << "Enter columns => ";
    while (!(cin >> col_) || (col_ % 2 == 0) || (col_ <= 0) || col_ >= 51) // Even positive integer
    {
        cout << "Invalid input. Please enter an odd integer value not more than  or equal to 51 => ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    cout << endl;
    cout << endl;

    cout << "Zombie Settings" << endl;
    cout << "-----------------" << endl;
    cout << "Enter the number of zombies => ";
    while (!(cin >> zombie_) || (zombie_ <= 0) || (zombie_ >= 10))
    {
        cout << "Invalid input. Please enter an  integer value or greater than 0 => ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
    cout << endl;
    cout << endl;
    cout << "Additional Settings" << endl;
    cout << "--------------------" << endl;
    cout << "Enter the number of nukes => ";
    while (!(cin >> NUKE_) || (NUKE_ < 0) || (NUKE_ >= 4))
    {
        cout << "Invalid input. The maximum number of nukes is 3 =>";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }

    cout << "Settings updated" << endl;
    pf::Pause();
    pf::ClearScreen();
    changed_ = true;
    intro.newBoard(intro, rows_, col_, zombie_, changed_, NUKE_);
}
