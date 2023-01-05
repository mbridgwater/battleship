#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);  // TESTED!
    void clear();  // TESTED!
    void block();  // TESTED!
    void unblock();  // TESTED!
    bool placeShip(Point topOrLeft, int shipId, Direction dir); // TESTED!
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir); // TESTED!
    void display(bool shotsOnly) const; // TESTED!
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);  // TESTED!
    bool allShipsDestroyed() const; // TESTED!

private:
    const Game& m_game; // given
    char m_board[MAXROWS][MAXCOLS]; // added
};

BoardImpl::BoardImpl(const Game& g)
    : m_game(g)
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_board[r][c] = '.';
        }
    }
    
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_board[r][c] = '.';
        }
    }
}

void BoardImpl::block()
{
    int numBlocked = (m_game.rows() * m_game.cols()) / 2;
    int i = 0;
    while (i < numBlocked)
    {
        int randRow = randInt(m_game.rows());
        int randCol = randInt(m_game.cols());
        if (m_board[randRow][randCol] != '\n')
        {
            m_board[randRow][randCol] = '\n';
            i++;
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if (m_board[r][c] == '\n')
            {
                m_board[r][c] = '.';
            }
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    int trackerCol = topOrLeft.c;
    int trackerRow = topOrLeft.r;
    if (shipId >= m_game.nShips() || shipId < 0)    // checking if shipId is invalid (1)
    {
        return false;
    }
    if (topOrLeft.r < 0 || topOrLeft.c < 0)     // checking if ship is partly or fully outside the board (2)
    {
        return false;
    }
    if (dir == VERTICAL && (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())) // checking if ship is partly or fully outside the board (2)
    {
        return false;
    }
    if (dir == HORIZONTAL && (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())) // checking if ship is partly or fully outside the board (2)
    {
        return false;
    }
    for (int i = 0; i < m_game.shipLength(shipId); i++) // checking if the ship would overlap an already placed ship (3) or a position put there by the clock function (4)
    {
        if (dir == HORIZONTAL)
        {
            if (m_board[topOrLeft.r][trackerCol] != '.')
            {
                return false;
            }
            trackerCol++;
        }
        if (dir == VERTICAL)
        {
            if (m_board[trackerRow][topOrLeft.c] != '.')
            {
                return false;
            }
            trackerRow++;
        }
    }
    for (int currR = 0; currR < m_game.rows(); currR++)     // checking that the ship with that shipId is not already placed on the board (5)
    {
        for (int currC = 0; currC < m_game.cols(); currC++)
        {
            if (m_board[currR][currC] == m_game.shipSymbol(shipId))
            {

                return false;
            }
        }
    }
    if (dir == HORIZONTAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_board[topOrLeft.r][topOrLeft.c + i] = m_game.shipSymbol(shipId);
        }
    }
    else // dir == VERTICAL
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            m_board[topOrLeft.r + i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
    }
 //   m_shipsOnBoard.at(shipId).m_shipPlaced = true; 
    return true; 
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId >= m_game.nShips() || shipId < 0)    // checking if shipId is invalid (1)
    {
        return false;
    }
    if (topOrLeft.r < 0 || topOrLeft.c < 0)     // checking if ship is partly or fully outside the board, i.e. double checking ship at indicated location (2)
    {
        return false;
    }
    if (dir == VERTICAL && (topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())) // checking if ship is partly or fully outside the board, i.e. double checking ship at indicated location (2)
    {
        return false;
    }
    if (dir == HORIZONTAL && (topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())) // checking if ship is partly or fully outside the board, i.e. double checking ship at indicated location (2)
    {
        return false;
    }
    if (dir == HORIZONTAL)
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (m_board[topOrLeft.r][topOrLeft.c + i] != m_game.shipSymbol(shipId))
            {
                return false;
            }
        }
        for (int a = 0; a < m_game.shipLength(shipId); a++)
        {
            m_board[topOrLeft.r][topOrLeft.c + a] = '.';  
        }
    }
    else // dir == VERTICAL
    {
        for (int i = 0; i < m_game.shipLength(shipId); i++)
        {
            if (m_board[topOrLeft.r + i][topOrLeft.c] != m_game.shipSymbol(shipId))
            {
                return false;
            }
        }
        for (int a = 0; a < m_game.shipLength(shipId); a++)
        {
            m_board[topOrLeft.r + a][topOrLeft.c] = '.';   
        }
    }
    return true;
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for (int a = 0; a < m_game.cols(); a++) // printing the column numbers
    {
        cout << a;
    }
    cout << endl;
    if (!shotsOnly)  // if shots only is true, update all symbols to be 'a'
    {
        for (int r = 0; r < m_game.rows(); r++)
        {
            cout << r << " ";
            for (int c = 0; c < m_game.cols(); c++)
            {
                cout << m_board[r][c];   
            }
            cout << endl;
        }
    }
    else
    {
        for (int r1 = 0; r1 < m_game.rows(); r1++)  // loop through the 2D array, printing; stop when we have a char that we want to print differently
        {
            cout << r1 << " ";
            for (int c1 = 0; c1 < m_game.cols(); c1++)
            {
                if (m_board[r1][c1] == '.' || m_board[r1][c1] == 'X' || m_board[r1][c1] == 'o') // any other char that it could be? perhaps newline? --> use isprint to avoid this, unecessary?
                {
                    cout << m_board[r1][c1];    // prints '.', 'X', and 'o'
                }
                else
                {
                    cout << '.';            // prints '.' instead of characters
                }
            }
            cout << endl;
        }
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    
    if (p.r < 0 || p.c < 0 || p.r >= m_game.rows() || p.c >= m_game.cols())     // checking if ship is partly or fully outside the board, i.e. double checking ship at indicated location (2)
    {
        shotHit = false;
        shipDestroyed = false;
        shipId = -1;            // Set shipId to -1 if attack is invalid
        return false;
    }
    if (m_board[p.r][p.c] == 'X' || m_board[p.r][p.c] == 'o')
    {
        shotHit = false;
        shipDestroyed = false;
        shipId = -1;            // Set shipId to -1 if you hit water or if attack is invalid
        return false;
    }
    char myTempShipSymbol = m_board[p.r][p.c];  // storing the ships symbol for later use
    if (m_board[p.r][p.c] != '.' && isprint(m_board[p.r][p.c])) 
    {
        shotHit = true;
        m_board[p.r][p.c] = 'X';
    }
    else   // its water
    {
        m_board[p.r][p.c] = 'o';
        shotHit = false;
        shipDestroyed = false;
        shipId = -2;            // Set shipId to -2 if you hit water 
        return true;
    }
    shipDestroyed = true;
    for (int r1 = 0; r1 < m_game.rows(); r1++)  // loop through the 2D array, and see if we can find same symbol anywhere
    {
        for (int c1 = 0; c1 < m_game.cols(); c1++)
        {
            if (m_board[r1][c1] == myTempShipSymbol)
            {
                shipDestroyed = false; 
            }
        }
    }
    if (shipDestroyed)
    {
        int counter = 0;
        while (m_game.shipSymbol(counter) != myTempShipSymbol)
        {
            counter++;
        }
        shipId = counter;
    }
    else
    {
        int counter = 0;
        while (m_game.shipSymbol(counter) != myTempShipSymbol)
        {
            counter++;
        }
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const   // if this true, player who was attacking won game!
{
    bool myDestroyedBool = false;
    for (int r1 = 0; r1 < m_game.rows(); r1++)  // loop through the 2D array, and see if we can find same symbol anywhere
    {
        for (int c1 = 0; c1 < m_game.cols(); c1++)
        {
            if (m_board[r1][c1] == 'X')     // to make sure that we have actually destroyed something on this board
            {
                myDestroyedBool = true;
            }
            if (m_board[r1][c1] != 'X' && m_board[r1][c1] != 'o' && m_board[r1][c1] != '.')
            {
                return false;
            }
        }
    }
    return myDestroyedBool; 
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
