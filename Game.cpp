#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector> // ADDED

using namespace std;

class GameImpl
{
public:
    GameImpl(int nRows, int nCols); // TESTED!
    int rows() const;   // TESTED!
    int cols() const;   // TESTED!
    bool isValid(Point p) const;    // written for us
    Point randomPoint() const;  // written for us
    bool addShip(int length, char symbol, string name); // TESTED!
    int nShips() const; // TESTED!
    int shipLength(int shipId) const; // TESTED!
    char shipSymbol(int shipId) const; // TESTED!
    string shipName(int shipId) const; // TESTED!
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);

private:
    int m_rows;
    int m_cols;
    struct ships
    {
        int m_shipLength;
        char m_shipSymbol;
        string m_shipName;
    };
    vector<ships> m_shipInfo; // now this is a vector of ship structs
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
    : m_rows(nRows), m_cols(nCols)  
{
}

int GameImpl::rows() const
{
    return m_rows; 
}

int GameImpl::cols() const
{
    return m_cols;  
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0 && p.r < rows() && p.c >= 0 && p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{

    const ships myCurrShip = { length, symbol, name };
    m_shipInfo.push_back(myCurrShip);
    
    return true;  
}

int GameImpl::nShips() const
{
    return m_shipInfo.size();
}

int GameImpl::shipLength(int shipId) const
{
    return m_shipInfo.at(shipId).m_shipLength;  
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_shipInfo.at(shipId).m_shipSymbol;
}

string GameImpl::shipName(int shipId) const
{
    return m_shipInfo.at(shipId).m_shipName; 
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    bool p1ShipsPlaced = p1->placeShips(b1);
    bool p2ShipsPlaced = p2->placeShips(b2);
    bool p1HitShot;
    bool p1destroyedP2Ship;
    int p2DestroyedShipId;
    bool p2HitShot;
    bool p2destroyedP1Ship;
    int p1DestroyedShipId;
    bool isValidShotP1 = true;
    bool isValidShotP2 = true;


    if (!p1ShipsPlaced || !p2ShipsPlaced)
    {
        return nullptr;
    }
    while (!b2.allShipsDestroyed() && !b1.allShipsDestroyed())
    {
        cout << p1->name() << "'s turn.  Board for " << p2->name() << ":" << endl; 
        if (p1->isHuman())    
        {
            b2.display(true);
        }
        else if (!p1->isHuman())
        {
            b2.display(false);
        }

        Point attackedPointOnB2 = p1->recommendAttack();
        isValidShotP1 = b2.attack(attackedPointOnB2, p1HitShot, p1destroyedP2Ship, p2DestroyedShipId);      
        p2->recordAttackByOpponent(attackedPointOnB2); // play() must call recordAttackByOpponent when attack made on this players board
        p1->recordAttackResult(attackedPointOnB2, isValidShotP1, p1HitShot, p1destroyedP2Ship, p2DestroyedShipId);
        if (!isValidShotP1 && p1->isHuman())
        {
            cout << p1->name() << " wasted a shot at (" << attackedPointOnB2.r << "," << attackedPointOnB2.c << ").\n";
        }
        else
        {
            string missedOrHit1;
            if (p1HitShot)
            {
                missedOrHit1 = "hit something";
            }
            if (!p1HitShot)
            {
                missedOrHit1 = "missed";
            }
            if (p1destroyedP2Ship)
            {
                missedOrHit1 = "destroyed the " + m_shipInfo.at(p2DestroyedShipId).m_shipName;
            }

            cout << p1->name() << " attacked (" << attackedPointOnB2.r << "," << attackedPointOnB2.c << ") and " << missedOrHit1 << ", resulting in:\n";

            if (p1->isHuman())      // display players 2 board (result of attack)
            {
                b2.display(true);
            }
            else if (!p1->isHuman())
            {
                b2.display(false);
            }
            if (b2.allShipsDestroyed())
            {
                cerr << p1->name() << " wins!/n";
                if (p2->isHuman())
                {
                    b1.display(false);
                }
                return p1;  
            }
        }
        if (shouldPause)        // press enter to continue    
        {
            cout << "Press enter to continue: ";
            cin.ignore(10000, '\n');
        }
        cout << p2->name() << "'s turn.  Board for " << p1->name() << ":" << endl;  
        if (p2->isHuman())          // display player 1's board
        {
            b1.display(true);
        }
        else if (!p2->isHuman())
        {
            b1.display(false);
        }
        Point attackedPointOnB1 = p2->recommendAttack();
        isValidShotP2 = b1.attack(attackedPointOnB1, p2HitShot, p2destroyedP1Ship, p1DestroyedShipId);      // player 2 attack player 1s board
        p1->recordAttackByOpponent(attackedPointOnB1);
        p2->recordAttackResult(attackedPointOnB1, isValidShotP2, p2HitShot, p2destroyedP1Ship, p1DestroyedShipId);
        if (!isValidShotP2 && p2->isHuman())
        {
            cout << p2->name() << " wasted a shot at (" << attackedPointOnB1.r << "," << attackedPointOnB1.c << ").\n";
        }
        else
        {
            string missedOrHit2;
            if (p2HitShot)
            {
                missedOrHit2 = "hit something";
            }
            if (!p2HitShot)
            {
                missedOrHit2 = "missed";
            }
            if (p2destroyedP1Ship)
            {
                missedOrHit2 = "destroyed the " + m_shipInfo.at(p1DestroyedShipId).m_shipName;
            }
            cout << p2->name() << " attacked (" << attackedPointOnB1.r << "," << attackedPointOnB1.c << ") and " << missedOrHit2 << ", resulting in:\n";

            if (p2->isHuman())      // display players 1 board (result of attack)
            {
                b1.display(true);
            }
            else if (!p2->isHuman())
            {
                b1.display(false);
            }
        }
        if (shouldPause)        // press enter to continue  
        {
            cout << "Press enter to continue: ";
            cin.ignore(10000, '\n');
        }
    }   // guarentees that p1 has lost by the end of the while loop
    if (b2.allShipsDestroyed()) // double check
    {
        cerr << p1->name() << " wins!/n";
        if (p2->isHuman())
        {
            b1.display(false);
        }
        return p1; 
    }
    if (b1.allShipsDestroyed())
    {
        cerr << p2->name() << " wins!/n";
        if (p1->isHuman())
        {
            b2.display(false);
        }
        return p2;
    }
    return p2;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1 || nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1 || nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows() && length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
            << endl;
        return false;
    }
    if (!isascii(symbol) || !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
            << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X' || symbol == '.' || symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
            << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr || p2 == nullptr || nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

