#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <random>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
    : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
    // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
    bool /* shotHit */, bool /* shipDestroyed */,
    int /* shipId */)                                    
{
    // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
    // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

bool getLineWithHOrVString(string& hOrVString)
{
    bool result(cin >> hOrVString);
    if (!result)
        cin.clear();  
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player   // PLAYER OR AWFULPLAYER?????????????????
{
public:
    HumanPlayer(string playerNm, const Game& g);
    virtual ~HumanPlayer();
    virtual bool isHuman() const;   // return true
    virtual bool placeShips(Board& b);      // DETERMINE WHICH OF THESE FXNS BELOW (IF ALL?) WILL KEEP
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

HumanPlayer::HumanPlayer(string playerNm, const Game& g)
    : Player(playerNm, g) 
{
}

HumanPlayer::~HumanPlayer()
{
}

bool HumanPlayer::isHuman() const
{
    return true;
}

bool HumanPlayer::placeShips(Board& b)
{
    string hOrV;
    Direction myDirection;
    int myRow;
    int myCol;
    string topOrLeft;
    Point rowColPoint;
    cout << this->name() << " must place " << this->game().nShips() << " ships.\n";
    b.display(false);
    for (int currShipId = 0; currShipId < this->game().nShips(); currShipId++)
    {
        cout << "Enter h or v for direction of " << this->game().shipName(currShipId) << " (length " << this->game().shipLength(currShipId) << "): ";
        getLineWithHOrVString(hOrV);
        while (hOrV != "h" && hOrV != "v")
        {
            cout << "Direction must be h or v.\n";
            cout << "Enter h or v for direction of " << this->game().shipName(currShipId) << " (length " << this->game().shipLength(currShipId) << "): ";
            getLineWithHOrVString(hOrV);
        }
        if (hOrV == "v")
        {
            myDirection = VERTICAL;
            topOrLeft = "topmost";
        }
        else if (hOrV == "h")
        {
            myDirection = HORIZONTAL;
            topOrLeft = "leftmost";
        }
        cout << "Enter row and column of " << topOrLeft << " cell (e.g., 3 5): ";

        bool validRowCol = getLineWithTwoIntegers(myRow, myCol);
        while (!validRowCol)
        {
            cout << "You must enter two integers.\n";
            cout << "Enter row and column of " << topOrLeft << " cell (e.g., 3 5): ";
            validRowCol = getLineWithTwoIntegers(myRow, myCol);
        }
        rowColPoint = { myRow, myCol };
        bool myShipCanBePlaced = b.placeShip(rowColPoint, currShipId, myDirection);
        while (!myShipCanBePlaced || !validRowCol)
        {
            if (!validRowCol)
            {
                //cerr << "myShipCanBePlaced bool is " << myShipCanBePlaced << ", where 0 is false" << endl;      // TEST
                //cerr << "it would be placing it at (" << myRow << ", " << myCol << ")\n";       // TEST
                cout << "You must enter two integers.\n";
                //cerr << "myShipCanBePlaced bool is " << myShipCanBePlaced << endl;          // TEST
                //cerr << "it would be placing it at (" << myRow << ", " << myCol << ")";     // TEST
            }
            else if (!myShipCanBePlaced)
            {
                //cerr << "myShipCanBePlaced bool is " << myShipCanBePlaced << ", where 0 is false" << endl;      // TEST
                //cerr << "it would be placing it at (" << myRow << ", " << myCol << ")\n";       // TEST
                cout << "The ship can not be placed there.\n";
                //cerr << "myShipCanBePlaced bool is " << myShipCanBePlaced << ", where 0 is false" << endl;      // TEST
                //cerr << "it would be placing it at (" << myRow << ", " << myCol << ")\n";       // TEST
            }
            cout << "Enter row and column of " << topOrLeft << " cell (e.g., 3 5): ";
            validRowCol = getLineWithTwoIntegers(myRow, myCol);
  //        cerr << "*******validRowCol bool is " << validRowCol << ", where 0 is false\n";
            if (validRowCol)
            {
                rowColPoint = { myRow, myCol };
                myShipCanBePlaced = b.placeShip(rowColPoint, currShipId, myDirection);
      //        cerr << "*******myShipcanBePlaced bool is " << myShipCanBePlaced << ", where 0 is false\n";
            }
        }
        if (currShipId != game().nShips() - 1)
        {
            b.display(false);
        }
    }
    return true; 
}

Point HumanPlayer::recommendAttack()
{
    int attackRow;
    int attackCol;
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    bool validRowCol = getLineWithTwoIntegers(attackRow, attackCol);
    while (!validRowCol)
    {
        cout << "You must enter two integers.\n";
        cout << "Enter the row and column to attack (e.g., 3 5): ";
        validRowCol = getLineWithTwoIntegers(attackRow, attackCol);
    }
    return Point(attackRow, attackCol);
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId) 
{
    // leave blank
}

void HumanPlayer::recordAttackByOpponent(Point p) 
{
    // leave blank
}





//*********************************************************************
//  MediocrePlayer
//*********************************************************************


class MediocrePlayer : public Player  
{
public:
    MediocrePlayer(string PlayerNm, const Game& g);
    virtual ~MediocrePlayer();
    bool placeShips(Board& b);      
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool isValidPointMed(Point p);
private:
    vector<Point> m_attackedPoints; // now this is a vector of ship structs
    bool m_state1;
    Point m_transitionPoint;
    int m_numAttackedPointsOnCross;
  
};

MediocrePlayer::MediocrePlayer(string PlayerNm, const Game& g)
    : Player(PlayerNm, g), m_state1(true), m_transitionPoint(Point(0, 0)), m_numAttackedPointsOnCross(0)
{   
}

MediocrePlayer::~MediocrePlayer()
{
    
}

bool MediocrePlayer::isValidPointMed(Point p)
{
    for (int i = 0; i < m_attackedPoints.size(); i++)
    {
        if (m_attackedPoints.at(i).r == p.r && m_attackedPoints.at(i).c == p.c)
        {
            return false;;
        }
    }
    return true;
}


bool placeShipsShipIdHelper(Board& b, const Game& g, int shipId)
{
    if (shipId >= g.nShips())
    {
        return true;
    }
    for (int currRow = 0; currRow < g.rows(); currRow++)
    {
        for (int currCol = 0; currCol < g.cols(); currCol++)
        {
            const Point placingPoint(currRow, currCol);
            if (b.placeShip(placingPoint, shipId, HORIZONTAL))
            {
                bool placedAll = placeShipsShipIdHelper(b, g, shipId + 1);
                if (placedAll)
                {
                    return true;
                }

                //Backtrack if you can't place every ship (its's impossible)
                b.unplaceShip(placingPoint, shipId, HORIZONTAL);
            }
            if (b.placeShip(placingPoint, shipId, VERTICAL))
            {
                bool placedAll = placeShipsShipIdHelper(b, g, shipId + 1);
                if (placedAll)
                {
                    return true;
                }
                //backtracking
                b.unplaceShip(placingPoint, shipId, VERTICAL);
            }
        }
    }
    return false;   // if it gets through the for loop, its impossible to place all ships so return false;
}


bool MediocrePlayer::placeShips(Board& b)
{
    for (int counter = 0; counter < 50; counter++)
    {
        b.block();
        if (placeShipsShipIdHelper(b, this->game(), 0))
        {
            b.unblock();
            return true;
        }
        b.clear();
    }
    return false;
}


Point MediocrePlayer::recommendAttack()
{
    // if in state 1           
    Point myRandomPoint = Point(randInt(this->game().rows()), randInt(this->game().cols()));
    int counterRightRows = 0;
    int counterLeftRows = 0;
    int counterDownCols = 0;
    int counterUpCols = 0;
    int totalCounter = 0;
    bool allPointsAttacked = true;

    if (m_state1)
    {
        bool validRandomPoint = false;
        while (validRandomPoint == false)
        {
            validRandomPoint = true;
            for (int i = 0; i < m_attackedPoints.size(); i++)
            {
                if (m_attackedPoints.at(i).r == myRandomPoint.r && m_attackedPoints.at(i).c == myRandomPoint.c)
                {
                    validRandomPoint = false;
                }
            }
            if (validRandomPoint != true)
            {
                myRandomPoint = Point(randInt(this->game().rows()), randInt(this->game().cols()));
            }
        }
    }
    else if (!m_state1)
    {
        bool validRandomPoint = false;
        while (validRandomPoint == false)
        {
            validRandomPoint = true;
            for (int i = 0; i < m_attackedPoints.size(); i++)
            {
                if (m_attackedPoints.at(i).r == myRandomPoint.r && m_attackedPoints.at(i).c == myRandomPoint.c)
                {
                    validRandomPoint = false;
                }
            }
            if (validRandomPoint != true)
            {
                myRandomPoint = Point(randInt(this->game().rows()), randInt(this->game().cols()));
            }
        }
        while (!(((myRandomPoint.r >= m_transitionPoint.r - 4) && (myRandomPoint.r <= m_transitionPoint.r + 4) && (myRandomPoint.c == m_transitionPoint.c)) ||
            ((myRandomPoint.c >= m_transitionPoint.c - 4) && (myRandomPoint.c <= m_transitionPoint.c + 4) && (myRandomPoint.r == m_transitionPoint.r)))
            && !(myRandomPoint.r == m_transitionPoint.r && myRandomPoint.c == m_transitionPoint.c))
        {
            myRandomPoint = Point(randInt(this->game().rows()), randInt(this->game().cols()));
            validRandomPoint = false;
            while (validRandomPoint == false/* && !m_exit*/)
            {
                m_state1 = true;
                validRandomPoint = true;
                for (int i = 0; i < m_attackedPoints.size(); i++)
                {
                    if (m_attackedPoints.at(i).r == myRandomPoint.r && m_attackedPoints.at(i).c == myRandomPoint.c)
                    {
                        validRandomPoint = false;
                    }
                }
                if (validRandomPoint != true)
                {
                    myRandomPoint = Point(randInt(this->game().rows()), randInt(this->game().cols()));
                }
            }
        }
    }
    m_attackedPoints.push_back(myRandomPoint);  
    return myRandomPoint; 
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if (!shotHit && m_state1/*|| !validShot*/)
    {
        m_state1 = true;
    }
    else if (shipDestroyed && shotHit)  // random attack got lucky and destroyed a ship from earlier
    {
        m_state1 = true;
  //      m_numAttackedPointsOnCross = 0;
    }
    else if (shotHit && !shipDestroyed && m_state1)
    {
        m_transitionPoint = p;
        m_state1 = false;
    }
}


void MediocrePlayer::recordAttackByOpponent(Point p)
{
    // this function does nothing
}



//*********************************************************************
//  GoodPlayer
//*********************************************************************

// TODO:  You need to replace this with a real class declaration and
//        implementation.
// typedef AwfulPlayer GoodPlayer;
class GoodPlayer : public Player   // PLAYER OR AWFULPLAYER?????????????????
{
public:
    GoodPlayer(string PlayerNm, const Game& g);
    virtual ~GoodPlayer();
    virtual bool placeShips(Board& b);      // DETERMINE WHICH OF THESE FXNS BELOW (IF ALL?) WILL KEEP
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool isValidPoint(Point p);
private:
    vector<Point> m_attackedPoints; 
    int m_goodState;
    int m_north;
    int m_east;
    int m_south;
    int m_west;
    string m_mostRecentDirection;
    Point m_transitionPoint;
    Point m_mostRecentlyAttackedPoint;
    //int m_numAttackedPointsOnCross;

};

GoodPlayer::GoodPlayer(string PlayerNm, const Game& g)
    : Player(PlayerNm, g), m_goodState(1), m_north(0), m_east(0), m_south(0), m_west(0), m_mostRecentDirection("none"), m_transitionPoint(Point(0, 0)), m_mostRecentlyAttackedPoint(Point(0,0))
{

}

GoodPlayer::~GoodPlayer()
{

}

bool GoodPlayer::placeShips(Board& b)
{
    for (int counter = 0; counter < 50; counter++)
    {
        b.block();
        if (placeShipsShipIdHelper(b, this->game(), 0))
        {
            b.unblock();
            return true;
        }
        b.clear();
    }
    //    cerr << "Couldn't place ships!!";
    return false;
}

bool GoodPlayer::isValidPoint(Point p)
{
    for (int i = 0; i < m_attackedPoints.size(); i++)
    {
        if (m_attackedPoints.at(i).r == p.r && m_attackedPoints.at(i).c == p.c)
        {
            return false;;
        }
    }
    return true;
}


Point GoodPlayer::recommendAttack()
{
    Point nextAttackPoint = Point(randInt(game().rows()), randInt(game().cols()));
    if (m_goodState == 1)       // attack rand point along checker
    {
        cerr << "I'm in State 1!!!!!!!!!!!\n";
        while (!(isValidPoint(nextAttackPoint) && ((nextAttackPoint.r % 2 == 0 && nextAttackPoint.c % 2 == 0) || (nextAttackPoint.r % 2 == 1 && nextAttackPoint.c % 2 == 1))))
        {
            nextAttackPoint = Point(randInt(this->game().rows()), randInt(this->game().cols()));
            if (m_attackedPoints.size() >= (game().rows() * game().cols())/2)
            {
                m_goodState = 5;
                cerr << "************Transiton to goodState 5 within if";
                nextAttackPoint = Point(randInt(game().rows()), randInt(game().cols()));
                while (!isValidPoint(nextAttackPoint))
                {
                    nextAttackPoint = Point(randInt(game().rows()), randInt(game().cols()));
                }
                return (nextAttackPoint);
            }
        }
        //}
    }
    if (m_goodState == 2)       // once hit a point on checker, attack clockwise in 4 directions around that point
    {
        cerr << "I'm in State 2!!!!!!!!!!!\n";
        if (m_north == 0 && m_transitionPoint.r > 0 && isValidPoint(Point(m_transitionPoint.r - 1, m_transitionPoint.c)))
        {
            nextAttackPoint = Point(m_transitionPoint.r - 1, m_transitionPoint.c);
            m_north = 1;
            m_mostRecentDirection = "north";
        }
        else if (m_east == 0 && m_transitionPoint.c < game().cols() - 1 && isValidPoint(Point(m_transitionPoint.r, m_transitionPoint.c + 1)))
        {
            nextAttackPoint = Point(m_transitionPoint.r, m_transitionPoint.c + 1);
            m_east = 1;
            m_mostRecentDirection = "east";
        }
        else if (m_south == 0 && m_transitionPoint.r < game().rows() - 1 && isValidPoint(Point(m_transitionPoint.r + 1, m_transitionPoint.c)))
        {
            nextAttackPoint = Point(m_transitionPoint.r + 1, m_transitionPoint.c);
            m_south = 1;
            m_mostRecentDirection = "south";
        }
        else if (m_west == 0 && m_transitionPoint.c > 0 && isValidPoint(Point(m_transitionPoint.r, m_transitionPoint.c - 1)))
        {
            nextAttackPoint = Point(m_transitionPoint.r, m_transitionPoint.c - 1);
            m_west = 1;
            m_mostRecentDirection = "west";
        }
    }
    if (m_goodState == 3)   // once hit a point when going clock wise (in state 2), go to state three and move only in that direction
    {
        cerr << "I'm in State 3!!!!!!!!!!!\n";
        if (m_north == 2 && m_mostRecentlyAttackedPoint.r > 0)
        {
            nextAttackPoint = { m_mostRecentlyAttackedPoint.r - 1, m_mostRecentlyAttackedPoint.c };
            if (!isValidPoint(nextAttackPoint))
            {
                m_goodState = 4;
            }
        }
        else if (m_east == 2 && m_mostRecentlyAttackedPoint.c < game().cols() - 1)
        {
            nextAttackPoint = { m_mostRecentlyAttackedPoint.r, m_mostRecentlyAttackedPoint.c + 1 };
            if (!isValidPoint(nextAttackPoint))
            {
                m_goodState = 4;
            }
        }
        else if (m_south == 2 && m_mostRecentlyAttackedPoint.r < game().rows() - 1)
        {
            nextAttackPoint = { m_mostRecentlyAttackedPoint.r + 1, m_mostRecentlyAttackedPoint.c };
            if (!isValidPoint(nextAttackPoint))
            {
                m_goodState = 4;
            }
        }
        else if (m_west == 2 && m_mostRecentlyAttackedPoint.c > 0)
        {
            nextAttackPoint = { m_mostRecentlyAttackedPoint.r, m_mostRecentlyAttackedPoint.c - 1 };
            if (!isValidPoint(nextAttackPoint))
            {
                m_goodState = 4;
            }
        }
        else    // if went off board, transiiton to moving in oppositite direction
        {
            m_goodState = 4;
            "***********Transition to goodState 5\n";
        }
    }
    if (m_goodState == 4)   // move in opposite direction of before bc either went off board or missed the next shot
    {
        cerr << "I'm in State 4!!!!!!!!!!!\n";
        if (m_north == 2 && m_mostRecentlyAttackedPoint.r < game().rows() - 1)  // go south (opposite direction of initial attack)
        {
            nextAttackPoint = { m_mostRecentlyAttackedPoint.r + 1, m_mostRecentlyAttackedPoint.c };
            if (!isValidPoint(nextAttackPoint))
            {
                m_goodState = 5;
            }
        }
        else if (m_east == 2 && m_mostRecentlyAttackedPoint.c > 0)  // go west
        {
            nextAttackPoint = { m_mostRecentlyAttackedPoint.r, m_mostRecentlyAttackedPoint.c - 1 };
            if (!isValidPoint(nextAttackPoint))
            {
                m_goodState = 5;
            }
        }
        else if (m_south == 2 && m_mostRecentlyAttackedPoint.r > 0) // go north
        {
            nextAttackPoint = { m_mostRecentlyAttackedPoint.r - 1, m_mostRecentlyAttackedPoint.c };
            if (!isValidPoint(nextAttackPoint))
            {
                m_goodState = 5;
            }
        }
        else if (m_west == 2 && m_mostRecentlyAttackedPoint.c < game().cols() - 1)  // go east
        {
            nextAttackPoint = { m_mostRecentlyAttackedPoint.r, m_mostRecentlyAttackedPoint.c + 1 };
            if (!isValidPoint(nextAttackPoint))
            {
                m_goodState = 5;
            }
        }
        else    // it did not destroy the ship -> edge case
        {
            m_goodState = 5;
            "***********Transition to goodState 5\n";
        }
    }
    if (m_goodState == 5)   // randomly check the board
    {
        cerr << "I'm in State 5!!!!!!!!!!!\n";
        m_north = 0;
        m_east = 0;
        m_south = 0;
        m_west = 0;
        m_mostRecentDirection = "none";
        bool validRandomPoint = false;

        // first try checker board method again about 50 times
        int i = 0;
        while (!(isValidPoint(nextAttackPoint) && ((nextAttackPoint.r % 2 == 0 && nextAttackPoint.c % 2 == 0) || (nextAttackPoint.r % 2 == 1 && nextAttackPoint.c % 2 == 1))) && i < 50)
        {
            nextAttackPoint = Point(randInt(this->game().rows()), randInt(this->game().cols()));
            i++;
        }

        while (!validRandomPoint)
        {
            validRandomPoint = true;
            for (int i = 0; i < m_attackedPoints.size(); i++)
            {
                if (m_attackedPoints.at(i).r == nextAttackPoint.r && m_attackedPoints.at(i).c == nextAttackPoint.c)
                {
                    validRandomPoint = false;
                }
            }
            if (validRandomPoint != true)
            {
                nextAttackPoint = Point(randInt(this->game().rows()), randInt(this->game().cols()));
            }
        }
    }
    m_attackedPoints.push_back(nextAttackPoint);
    m_mostRecentlyAttackedPoint = nextAttackPoint;
    return nextAttackPoint;
}


void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if (shotHit && !shipDestroyed && (m_goodState == 1 || m_goodState == 5))      // when m_north = 0, that direction has not been attacked yet, when = 1, have attacked in that direction, when == 2, have hit in that direction
    {
        m_transitionPoint = p;
        m_goodState = 2;
        cerr << "**********Transition to goodState 2\n";
    }
    else if (shotHit && !shipDestroyed && m_goodState == 2 && m_mostRecentDirection == "north" && m_north == 1)
    {
        m_goodState = 3;
        m_north = 2;
        cerr << "**********Transition to goodState 3\n";
    }
    else if (shotHit && !shipDestroyed && m_goodState == 2 && m_mostRecentDirection == "east" && m_east == 1)
    {
        m_goodState = 3;
        m_east = 2;
        cerr << "**********Transition to goodState 3\n";
    }
    else if (shotHit && !shipDestroyed && m_goodState == 2 && m_mostRecentDirection == "south" && m_south == 1)
    {
        m_goodState = 3;
        m_south = 2;
        cerr << "**********Transition to goodState 3\n";
    }
    else if (shotHit && !shipDestroyed && m_goodState == 2 && m_mostRecentDirection == "west" && m_west == 1)
    {
        m_goodState = 3;
        m_west = 2;
        cerr << "**********Transition to goodState 3\n";
    }
    else if (!shotHit && m_goodState == 3)
    {
        m_goodState = 4;
        m_mostRecentlyAttackedPoint = m_transitionPoint;
        cerr << "**********Transition to goodState 4\n";
    }
    else if (!shotHit && m_goodState == 4)
    {
        m_goodState = 5;
        cerr << "**********Transition to goodState 5\n";
    }
    else if (shipDestroyed)
    {
        m_goodState = 1;
        m_north = 0;
        m_east = 0;
        m_south = 0;
        m_west = 0;
        m_mostRecentDirection = "none";
        cerr << "**********Transition to goodState 1\n";
    }
}

void GoodPlayer::recordAttackByOpponent(Point p)
{
    // leave empty
}




//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };

    int pos;
    for (pos = 0; pos != sizeof(types) / sizeof(types[0]) &&
        type != types[pos]; pos++)
        ;
    switch (pos)
    {
    case 0:  return new HumanPlayer(nm, g);
    case 1:  return new AwfulPlayer(nm, g);
    case 2:  return new MediocrePlayer(nm, g);
    case 3:  return new GoodPlayer(nm, g);
    default: return nullptr;
    }
}
