#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier") &&
        g.addShip(4, 'B', "battleship") &&
        g.addShip(3, 'D', "destroyer") &&
        g.addShip(3, 'S', "submarine") &&
        g.addShip(2, 'P', "patrol boat");
}

struct TestShip {
    int id;
    int length;
    char symbol;
    std::string name;
};

std::vector<TestShip> add_test_ships(Game& g) {
    const int NUM_SHIPS =
        g.cols() - 1; // Longest ship will be shorter than the number of columns,
                      // to ensure there's water for testing purposes
    std::vector<TestShip> test_ships;

    for (int i = 0; i < NUM_SHIPS; i++) {
        const TestShip temp{ i, i + 1, static_cast<char>('A' + i),
                            "valid " + std::to_string(i) };
        test_ships.push_back(temp);
        assert(g.addShip(test_ships[i].length, test_ships[i].symbol,
            test_ships[i].name));
    }

    return test_ships;
}

// `Game` class tests
void game_tests() {
    Game g{ MAXROWS, MAXCOLS };

    // `const` methods
    //
    assert(g.rows() == MAXROWS);
    assert(g.cols() == MAXCOLS);
    assert(g.isValid(Point{ 0, 0 }));
    assert(g.isValid(Point{ MAXROWS - 1, MAXCOLS - 1 }));

    // `Game::addShip()
    //
    // 10 valid test_ships
    std::vector<TestShip> test_ships = add_test_ships(g);
    assert(g.nShips() == 9);

    // `Game::shipLength()`, `Game::shipSymbol()`, `Game::shipName()`
    //
    for (const TestShip& ship : test_ships) {
        assert(g.shipLength(ship.id) == ship.length);
        assert(g.shipSymbol(ship.id) == ship.symbol);
        assert(g.shipName(ship.id) == ship.name);
    }

    // `Game::play()`
    // TOOO

    std::cerr << "Passed `Game` test cases " << std::endl;
}



// `Board` class tests
void board_tests() {
    Game g{ MAXROWS, MAXCOLS };

    std::vector<TestShip> test_ships = add_test_ships(g);

    Board b{ g };

    b.block();
    b.clear();
    b.display(false);
    std::cerr << "The above board should be cleared." << std::endl;

    // `Board::placeShip()`
    //
    assert(g.nShips() == 9);
    assert(!b.placeShip(Point{ 0, 0 }, -1, HORIZONTAL)); // Invalid ID
    assert(!b.placeShip(Point{ 0, MAXCOLS - 1 }, test_ships.at(1).id,
        HORIZONTAL)); // Invalid direction, ship of length 2

    for (const TestShip& ship : test_ships) {
        assert(b.placeShip(Point{ ship.id, 0 }, ship.id, HORIZONTAL));
        std::cerr << "Placed ship with id " << ship.id << std::endl;
    }
    b.display(false);
    std::cerr << "The above board should display nine horizontal ships stacked "
        "top-down"
        << std::endl;

    assert(!b.placeShip(Point{ 0, 0 }, test_ships.at(0).id,
        HORIZONTAL)); // Cannot overlap
    assert(!b.placeShip(Point{ 9, 0 }, test_ships.at(0).id,
        HORIZONTAL)); // Cannot have duplicate ships

// `Board::unplaceShips()`
//
    assert(!b.unplaceShip(Point{ 0, 0 }, -1, HORIZONTAL));         // Invalid ID
    assert(!b.unplaceShip(Point{ 0, 0 }, g.nShips(), HORIZONTAL)); // Invalid ID
    for (const TestShip& ship : test_ships) {
        if (ship.id != 0) {
            assert(!b.unplaceShip(
                Point{ ship.id, 0 }, ship.id,
                VERTICAL)); // Does not contain ship at indicated locations
        }
        assert(!b.unplaceShip(
            Point{ ship.id, 1 }, ship.id,
            HORIZONTAL)); // Does not contain ship at indicated locations
        assert(b.unplaceShip(Point{ ship.id, 0 }, ship.id, HORIZONTAL));
        std::cerr << "Unplaced ship with id " << ship.id << std::endl;
    }
    assert(!b.allShipsDestroyed()); // Unplacing ships does not destroy them
    b.display(false);
    std::cerr << "The above board should contain no ships." << std::endl;

    // `Board::attack()`
    //
    
    b.clear();
    for (const TestShip& ship : test_ships) {
        assert(b.placeShip(Point{ ship.id, 0 }, ship.id, HORIZONTAL));
    }
    
    for (const TestShip& ship : test_ships) {
        // Set to values that don't make sense for a failed hit
        bool hit = true;
        bool destroyed = true;
        int ship_id = ship.id;
        assert(!b.attack(Point{ MAXROWS, MAXCOLS }, hit, destroyed, // I THINK MY CODE IS FAILING AROUND HERE BC POINT IS INVALID -> HOW DO I PREVENT THIS??????
            ship_id)); // Invalid point
        assert(b.attack(Point{ ship.id, MAXCOLS - 1 }, hit, destroyed,
            ship_id)); // Missed hit
    

        assert(!hit);
        assert(!destroyed);
        assert(ship_id == -1);    

        for (int i = 0; i < ship.length - 1; i++) {
            // Set to values that don't make sense for a successful hit
            hit = false;
            destroyed = true;

            assert(b.attack(Point{ ship.id, i }, hit, destroyed, ship_id));
            assert(hit);
            assert((ship.id == 0) ? destroyed : !destroyed);
            // assert(if shipId == 0, destroyed == true; else, destroyed == false)
        }

        // Set to values that don't make sense for a successful hit
        hit = false;
        destroyed = true;
        ship_id = -1;

        assert(!b.attack(Point{ ship.id, ship.length - 2 }, hit, destroyed,
            ship_id)); // Attack on previouly attacked location

        b.attack(Point{ ship.id, ship.length - 1 }, hit, destroyed, ship_id);
        assert(hit);
        assert(destroyed);
        assert(ship_id == ship.id);

        std::cerr << "Destroyed ship with id " << ship.id << std::endl; 
    }
    assert(b.allShipsDestroyed());
    

    std::cerr << "Passed `Board` test cases " << std::endl;
}


int main()
{



    //------------------------Testing void BoardImpl::clear(), void BoardImpl::block(), void BoardImpl::unblock() BELOW----------------------------//
    //Game test1G(4, 5);
    //Board test1B(test1G);
    //cerr << "Before blocking: \n";
    //test1B.display(false);
    //cerr << endl << endl;
    //test1B.block();                         // testing block
    //cerr << "After blocking half: \n";
    //test1B.display(false);
    //cerr << endl << endl;
    //test1B.unblock();                       // testing unblock
    //cerr << "After unblocking the blocked ones: \n";
    //test1B.display(false);
    //cerr << endl << endl;
    //test1B.block();                         // testing block
    //cerr << "After blocking half: \n";
    //test1B.display(false);
    //cerr << endl << endl;
    //test1B.clear();                       // testing clear
    //cerr << "After clearing the board: \n";
    //test1B.display(false);
    //assert(test1G.rows() == 4);
    //assert(test1G.cols() == 5);
    //cerr << endl << endl;
    //------------------------Testing void BoardImpl::clear(), void BoardImpl::block(), void BoardImpl::unblock() ABOVE----------------------------//





    //--------------------------------Testing bool GameImpl::addShip(int length, char symbol, string name), int GameImpl::nShips() const,------------------------------//
    // ----------------------- int shipLength(int shipId) const, char shipSymbol(int shipId) const, string shipName(int shipId) const;  BELOW----------------------------//
    //Game test2G(5, 5);
    //Board test2B(test2G);
    //assert(test2G.rows() == 5);
    //assert(test2G.cols() == 5);
    //assert(test2G.addShip(6, 'e', "name!") == false);
    //assert(test2G.addShip(3, 'a', "myName") == true);
    //assert(test2G.shipLength(0) == 3);
    //assert(test2G.shipName(0) == "myName");
    //assert(test2G.shipSymbol(0) == 'a');
    //assert(test2G.nShips() == 1);
    //assert(test2G.addShip(3, 'b', "myName2") == true);
    //assert(test2G.nShips() == 2);
    //assert(test2G.shipLength(1) == 3);
    //assert(test2G.shipName(1) == "myName2");
    //assert(test2G.shipSymbol(1) == 'b');
    //assert(test2G.addShip(4, 'b', "myName3") == false);
    //assert(test2G.addShip(5, 'c', "myName2") == true);
    //assert(test2G.nShips() == 3);
    //assert(test2G.shipLength(2) == 5);
    //assert(test2G.shipName(2) == "myName2");
    //assert(test2G.shipSymbol(2) == 'c');
    //cerr << endl << "The following board should be empty: \n";
    //test2B.display(false);
    //cerr << endl;
    ////--------------------------------Testing bool GameImpl::addShip(int length, char symbol, string name), int GameImpl::nShips() const,------------------------------//
    // ----------------------- int shipLength(int shipId) const, char shipSymbol(int shipId) const, string shipName(int shipId) const;  ABOVE----------------------------//




    //------------------------Testing bool placeShip(Point topOrLeft, int shipId, Direction dir) and bool unplaceShip(Point topOrLeft, int shipId, Direction dir) BELOW----------------------------//
    //Game test3G(10, 10);  
    //Point firstpoint1(0, 0);
    //Point lastpoint2(9, 9);
    //Point point3(2, 3);
    //Point point4(6, 5);
    //Point point5(0, 5);
    //assert(test3G.addShip(3, 'a', "name0") == true);    //first placing ships in vector; shipIds at end of name
    //assert(test3G.addShip(1, 'b', "name1") == true);
    //assert(test3G.addShip(3, 'c', "name2") == true);
    //assert(test3G.addShip(5, 'd', "name3") == true);
    //assert(test3G.addShip(6, 'e', "name4") == true);
    //assert(test3G.addShip(8, 'f', "name5") == true);
    //assert(test3G.addShip(2, 'g', "name6") == true);
    //assert(test3G.addShip(9, 'h', "name7") == true);
    //assert(test3G.addShip(10, 'i', "name8") == true);
    //assert(test3G.addShip(4, 'j', "name9") == true);
    //assert(test3G.addShip(0, 'k', "name10Fail") == false);
    //assert(test3G.nShips() == 10);
    //Board test3B(test3G);
    //cerr << endl << endl << "The following board should be empty:\n";       // now placing on board
    //test3B.display(false);
    //assert(test3B.placeShip(firstpoint1, -9, HORIZONTAL) == false); // placing a negtive/invalid shipId
    //assert(test3B.placeShip(firstpoint1, 10, HORIZONTAL) == false); // placing a too large/invalid shipId
    //assert(test3B.placeShip(point3, 8, HORIZONTAL) == false); // placing ship partly outside of board
    //assert(test3B.placeShip(firstpoint1, 8, HORIZONTAL) == true); // sucessfully placing ship horizontally across entire row 0
    //cerr << endl << endl << "The following board should have a ship (symbol i) place filling the entire first row:\n";
    //cerr << endl;
    //test3B.display(false);
    //cerr << endl;
    //assert(test3B.placeShip(firstpoint1, 3, VERTICAL) == false); // overlapping another ship vertically
    //assert(test3B.placeShip(point5, 6, HORIZONTAL) == false);   // overlapping another ship horizontally
    //assert(test3B.placeShip(point4, 9, VERTICAL) == true);  // should place a ship vertically in column 5 from row 6-9
    //cerr << endl << endl << "The following board should have 2 ships placed (symbols i and j) filling the entire first row and vertically in column 5 from row 6-9:\n";
    //cerr << endl;
    //test3B.display(false);
    //cerr << endl << endl;
    //test3B.display(false);
    //assert(test3B.placeShip(point3, 9, VERTICAL) == false); // cannot place a ship of the same shipId
    //assert(test3B.unplaceShip(point4, 9, VERTICAL) == true);
    //assert(test3B.placeShip(point3, 9, VERTICAL) == true); // can place a ship w same ID once you have unplaced it
    //cerr << endl << endl << "Once you unplace ship w symbol j, you can repl ace it to a new point.\n";
    //cerr << "The following board should have 2 ships placed(symbols i and j) filling the entire first row and vertically in column 3 from row 2 - 5:\n";
    //cerr << endl;
    //test3B.display(false);
    //cerr << endl << endl;
    //assert(test3B.placeShip(lastpoint2, 1, HORIZONTAL) == true);
    //assert(test3B.placeShip(point4, 2, VERTICAL) == true);
    //assert(test3B.placeShip(Point(4, 0), 4, VERTICAL) == true);
    //cerr << "The following board should have 5 ships placed(symbols b, c, e, i, and j):\n";
    //cerr << endl;
    //test3B.display(false);
    //cerr << endl << endl;
    //assert(test3B.unplaceShip(point4, 0, VERTICAL) == false);   //invalid shipId bc this has not been placed
    //assert(test3B.unplaceShip(point3, 0, HORIZONTAL) == false); //invalid shipId bc this has not been placed
    //assert(test3B.unplaceShip(point3, -1, HORIZONTAL) == false); //invalid shipId bc negative
    //assert(test3B.unplaceShip(point3, 10, HORIZONTAL) == false); //invalid shipId bc larger than last index
    //assert(test3B.unplaceShip(point4, 6, VERTICAL) == false);   // wrong location of this shipId
    //assert(test3B.unplaceShip(Point(6, 6), 9, VERTICAL) == false);  // not the most left position of this ship
    //assert(test3B.unplaceShip(firstpoint1, 8, HORIZONTAL) == true); // unplace every ship from here and below:
    //assert(test3B.unplaceShip(point3, 9, VERTICAL) == true);
    //assert(test3B.unplaceShip(lastpoint2, 1, HORIZONTAL) == true); 
    //assert(test3B.unplaceShip(point4, 2, VERTICAL) == true);
    //assert(test3B.unplaceShip(Point(4, 0), 4, VERTICAL) == true);
    //assert(test3B.unplaceShip(point4, 2, VERTICAL) == false);   // can't double unplace a ship
    //cerr << "After unplacing every ship, the following board should be empty:\n";
    //cerr << endl;
    //test3B.display(false);
    //cerr << endl << endl;
    //------------------------Testing bool placeShip(Point topOrLeft, int shipId, Direction dir) and bool unplaceShip(Point topOrLeft, int shipId, Direction dir) ABOVE----------------------------//






    //------------------------Testing void BoardImpl::display(bool shotsOnly) const, bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId), bool BoardImpl::allShipsDestroyed() const BELOW----------------------------//
    //Game test4G(10, 10);
    //assert(test4G.addShip(1, 'a', "name0") == true);    //first placing ships in vector; shipIds at end of name
    //assert(test4G.addShip(2, 'b', "name1") == true);
    //assert(test4G.addShip(3, 'c', "name2") == true);
    //assert(test4G.addShip(4, 'd', "name3") == true);
    //assert(test4G.addShip(5, 'e', "name4") == true);
    //assert(test4G.addShip(6, 'f', "name5") == true);
    //assert(test4G.addShip(7, 'g', "name6") == true);
    //assert(test4G.addShip(8, 'h', "name7") == true);
    //assert(test4G.addShip(9, 'i', "name8") == true);
    //assert(test4G.addShip(10,'j', "name9") == true);
    //assert(test4G.nShips() == 10);
    //Board test4B(test4G);
    //assert(test4B.placeShip(Point(0, 0), 0, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 1), 1, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 2), 2, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 3), 3, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 4), 4, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 5), 5, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 6), 6, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 7), 7, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 8), 8, VERTICAL) == true);
    //assert(test4B.placeShip(Point(0, 9), 9, VERTICAL) == true);
    //cerr << "After placing every ship, the following board should have all ships placed vertically, length incremented by 1, row 0 should be completely filled w each diff ship:\n";
    //cerr << endl;
    //test4B.display(false);
    //cerr << endl << endl;
    //cerr << "Now display shots only; board should be empty:\n";
    //cerr << endl;
    //test4B.display(true);
    //cerr << endl << endl;
    //cerr << "Now we begin attacking:";
    //bool myShotHit;
    //bool myShipDestroyed;
    //int myShipId = 0;
    //assert(!test4B.attack(Point(10, 0), myShotHit, myShipDestroyed, myShipId) && !myShotHit && !myShipDestroyed && myShipId == -1);  // outside of board
    //assert(!test4B.attack(Point(0, -1), myShotHit, myShipDestroyed, myShipId) && !myShotHit && !myShipDestroyed && myShipId == -1);  // outside of board
    //assert(test4B.attack(Point(2, 0), myShotHit, myShipDestroyed, myShipId) && !myShotHit && !myShipDestroyed && myShipId == -2);  // attack that hits the water
    //cerr << endl << endl;
    //cerr << "First attack - water!\n";
    //cerr << "There should be an o at Point(2,0); shotsOnly = false\n";
    //cerr << endl;
    //test4B.display(false);
    //cerr << endl << endl;
    //cerr << "There should be an o at Point(2,0); shotsOnly = true\n";
    //cerr << endl;
    //test4B.display(true);
    //cerr << endl << endl;
    //assert(!test4B.attack(Point(2, 0), myShotHit, myShipDestroyed, myShipId) && !myShotHit && !myShipDestroyed && myShipId == -1);  // already attacked this position in the water
    //assert(test4B.attack(Point(0, 0), myShotHit, myShipDestroyed, myShipId) && myShotHit && myShipDestroyed && myShipId == 0);  // completely destroying boat at Point(0, 0) since its of length 1, shipId updated to 0 (happened to already be that)
    //assert(test4B.attack(Point(2, 3), myShotHit, myShipDestroyed, myShipId) && myShotHit == true && myShipDestroyed == false && myShipId == 0);  // attacking boat at Point(2, 3)
    //assert(test4B.attack(Point(9, 9), myShotHit, myShipDestroyed, myShipId) && myShotHit && !myShipDestroyed && myShipId == 0);  // attacking boat at Point(9, 9)
    //assert(test4B.attack(Point(0, 3), myShotHit, myShipDestroyed, myShipId) && myShotHit && !myShipDestroyed && myShipId == 0);  // completely destroying boat at Point(0, 0) since its of length 1
    //assert(test4B.attack(Point(1, 3), myShotHit, myShipDestroyed, myShipId) && myShotHit && !myShipDestroyed && myShipId == 0);
    //assert(test4B.attack(Point(3, 3), myShotHit, myShipDestroyed, myShipId) && myShotHit && myShipDestroyed && myShipId == 3);
    //cerr << endl << endl;
    //assert(!test4B.attack(Point(3, 3), myShotHit, myShipDestroyed, myShipId) && !myShotHit && !myShipDestroyed && myShipId == -1);   // SHOULD myShipDestroyed BE FALSE????????????????????????????????
    //cerr << endl << endl;
    //cerr << "There should be an o at Point(2,0) and multiple X's; two ships destroyed; shotsOnly = false\n";
    //cerr << endl;
    //test4B.display(false);
    //cerr << endl << endl;
    //cerr << "There should be an o at Point(2,0) and multiple X's; two ships destroyed; shotsOnly = true\n";
    //cerr << endl;
    //test4B.display(true);
    //cerr << endl << endl;

    ////testing bool allShipsDestroyed() const
    //assert(!test4B.allShipsDestroyed());
    //assert(test4B.attack(Point(0, 1), myShotHit, myShipDestroyed, myShipId));   // destroying all ships
    //assert(test4B.attack(Point(1, 1), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(0, 2), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(1, 2), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(2, 2), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(0, 4), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(1, 4), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(2, 4), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(3, 4), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(4, 4), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(0, 5), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(1, 5), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(2, 5), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(3, 5), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(4, 5), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(5, 5), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(0, 6), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(1, 6), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(2, 6), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(3, 6), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(4, 6), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(5, 6), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(6, 6), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(0, 7), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(1, 7), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(2, 7), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(3, 7), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(4, 7), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(5, 7), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(6, 7), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(7, 7), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(0, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(1, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(2, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(3, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(4, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(5, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(6, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(7, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(8, 8), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(0, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(1, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(2, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(3, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(4, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(5, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(6, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(7, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(8, 9), myShotHit, myShipDestroyed, myShipId));
    //assert(test4B.attack(Point(9, 0), myShotHit, myShipDestroyed, myShipId) && !myShotHit && !myShipDestroyed && myShipId == -2);
    //cerr << "There should be just X's and o's aka all ships should be destroyed; shotsOnly = false\n";
    //cerr << endl;
    //test4B.display(false);
    //cerr << endl << endl;
    //cerr << "There should be just X's and o's aka all ships should be destroyed; shotsOnly = true\n";
    //cerr << endl;
    //test4B.display(true);
    //assert(test4B.allShipsDestroyed());
    //cerr << endl << endl;
    //------------------------Testing void BoardImpl::display(bool shotsOnly) const, bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId), bool BoardImpl::allShipsDestroyed() const ABOVE----------------------------//

    



    //------------------------Testing Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause) below-------------------------------------------------//
    //Game test5G(10, 10);
    ////Player* test5P1 = AwfulPlayer("Chloe", test5G);
    ////Player* test5P2 = AwfulPlayer("Chloe", test5G);
    //addStandardShips(test5G);
    //Board test5B1(test5G);
    //Board test5B2(test5G);
    //Player* p1 = createPlayer("awful", "Awful Audrey", test5G);
    //Player* p2 = createPlayer("awful", "Awful Mimi", test5G);
    //test5G.play(p1, p2, true);
    //------------------------Testing Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause) above-------------------------------------------------//
    

    //------------------------Testing bool HumanPlayer::placeShips(Board& b) and Human Class below--------------------------------------------------------------------------------------//
    //Game test6G(10, 10);
    ////Player* test6P1 = AwfulPlayer("Chloe", test5G);
    ////Player* test6P2 = AwfulPlayer("Chloe", test5G);
    //addStandardShips(test6G);
    //Board test6B1(test6G);
    //Board test6B2(test6G);
    //Player* p1test6 = createPlayer("awful", "Awful Mimi", test6G);
    //Player* p2test6 = createPlayer("human", "Human Missy", test6G);
    //test6G.play(p1test6, p2test6, true);





    //Game test7G(10, 10);
    //addStandardShips(test7G);
    //Board test7B1(test7G);
    //Board test7B2(test7G);
    //Player* p1Test7 = createPlayer("mediocre", "missy", test7G);
    //Player* p2Test7 = createPlayer("mediocre", "chloe", test7G);
    //for (int i = 0; i < 50; i++)
    //{
    //    assert(p1Test7->placeShips(test7B1));
    //    test7B1.display(false);
    //}

    // human vs mediocre player

    Game test8G(10, 10);
    //Player* test6P1 = AwfulPlayer("Chloe", test5G);
    //Player* test6P2 = AwfulPlayer("Chloe", test5G);
    addStandardShips(test8G);
    Board test8B1(test8G);
    Board test8B2(test8G);
    Player* p1test8 = createPlayer("mediocre", "Mediocre Mimi", test8G);
    Player* p2test8 = createPlayer("human", "Human Missy", test8G);
    test8G.play(p1test8, p2test8, true);

    //int counter = 0;
    //for (int i = 0; i < 1000; i++)
    //{
    //    Game test9G(10, 10);
    //    //  test9G.addShip(2, 'a', "myAShip");
    //    //  test9G.addShip(3, 'b', "myBShip");
    //    //  test9G.addShip(4, 'c', "myCShip");
    //    ////  test9G.addShip(4, 'd', "myDShip");
    //    //  test9G.addShip(6, 'e', "myEShip");
    //    addStandardShips(test9G);
    //    Board test9B1(test9G);
    //    Board test9B2(test9G);
    //    Player* p1Test9 = createPlayer("good", "Good P1", test9G);
    //    Player* p2Test9 = createPlayer("mediocre", "Mediocre P2", test9G);
    //    if (test9G.play(p1Test9, p2Test9, false) == p1Test9)
    //    {
    //        counter++;
    //    }
    //    delete p1Test9;
    //    delete p2Test9;
    //}
    //cerr << endl << endl;
    //cerr << "My good player won " << counter << " out of 1000 times!\n";

    //cerr << endl;
    cerr << "YAY! TEST CASES PASSED :))\n";
    return 0;
}



/*

const int NTRIALS = 10;

cout << "Select one of these choices for an example of the game:" << endl;
cout << "  1.  A mini-game between two mediocre players" << endl;
cout << "  2.  A mediocre player against a human player" << endl;
cout << "  3.  A " << NTRIALS
    << "-game match between a mediocre and an awful player, with no pauses"
    << endl;
cout << "Enter your choice: ";
string line;
getline(cin, line);
if (line.empty())
{
    cout << "You did not enter a choice" << endl;
}
else if (line[0] == '1')
{
    Game g(2, 3);
    g.addShip(2, 'R', "rowboat");
    Player* p1 = createPlayer("mediocre", "Popeye", g);
    Player* p2 = createPlayer("mediocre", "Bluto", g);
    cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
    g.play(p1, p2);
    delete p1;
    delete p2;
}
else if (line[0] == '2')
{
    Game g(10, 10);
    addStandardShips(g);
    Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
    Player* p2 = createPlayer("human", "Shuman the Human", g);
    g.play(p1, p2);
    delete p1;
    delete p2;
}
else if (line[0] == '3')
{
    int nMediocreWins = 0;

    for (int k = 1; k <= NTRIALS; k++)
    {
        cout << "============================= Game " << k
            << " =============================" << endl;
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("awful", "Awful Audrey", g);
        Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
        Player* winner = (k % 2 == 1 ?
            g.play(p1, p2, false) : g.play(p2, p1, false));
        if (winner == p2)
            nMediocreWins++;
        delete p1;
        delete p2;
    }
    cout << "The mediocre player won " << nMediocreWins << " out of "
        << NTRIALS << " games." << endl;
    // We'd expect a mediocre player to win most of the games against
    // an awful player.  Similarly, a good player should outperform
    // a mediocre player.
}
else
{
    cout << "That's not one of the choices." << endl;
}
*/