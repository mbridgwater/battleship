# battleship
Implement a battleship game in C++ that can be played by two human players or against virtual players, ranging in skill-level.

REPORT:
5/18/21

Description of Design
The main data structures that I utilized in this project were base and derived classes which was the premise of the project as well as vectors to which I sometimes added structs and other types of data. My project focuses a lot around the game class, as that is where I store information about my ships. Within the private interface of the game class, I created a struct of data type ship which stored the length, symbol, and name of the ship. Another private member variable that I stored was a vector of data type ships, which is how I would access the characteristics of my ships throughout the project. Moving on to the board class, I stored a two dimensional array, initialized to ‘.’, which would act as my physical board that will be displayed during games. In the Player.h and Player.cpp file, inheritance starts to come into play. In my HumanPlayer class, I stored no private member variables as access to the game reference within the Player class was sufficient to solve the tasks. On the other hand, the MediocrePlayer class stores several member variables as it needs private members to recommend attacks (which the human player would do them self, not the code). In my MedicorePlayer class, I stored a vector as a private member that tracked already attacked points by the mediocre player. I also had private boolean, point, and int data types to help me solve algorithms for attacking. Finally, for my GoodPlayer class, I stored similar private members, including a vector of previously attacked points, several int types, and other trivial types.

Prose Description of Good Player’s Strategies
	My good player’s strategy for placing ships uses a recursive helper function. Note that the mediocre player and good player place ships using the same method as I did not find that it affected the outcome of how much the Good Player beat the Mediocre player when trying to place ships in different methods. In the recursive helper function, our base case is to see if the shipId we passed in is larger than or equal to the total number of ships, in which case, all ships have been placed and we return true. Otherwise, we create a double for loop to traverse through the board and for both the vertical and horizontal directions we recursively call the placeShipsShipIdHelper until all ships have been placed, in which case we return true. Within these if statements, we unplace the ships if we were not able to place all and return true so that it can attempt to place the ship in a fully new pattern. If we get through the for loops without returning true, then we were not able to place the ships and our function returns false.
	My good player’s strategy for recommending moves is essentially broken down into five states, which uses private member variables of GoodPlayer to keep track of which strategy GoodPlayer is pursuing at that moment. My recordAttackResult function will set the member variables depending on whether the ship has been hit, destroyed, and depending on the state we were previously in. My first state attacks a random point along a checkered pattern (essentially, I only attack points that have even rows and even columns or odd rows and odd columns). Look to Figure 1 for an example of what a checkered pattern looks like on the board:
o . o . o . o
. o . o . o .
o . o . o . o
. o . o . o .
Figure 1: Image of checker pattern board 
GoodPlayer transitions out of state 1 once a ship has been hit, moving into state 2. In state 2, the strategy is to attack each point directly around the already hit ship segment in a clockwise fashion (labeled as the transition point). Once another ship segment has been hit, the strategy transitions into state 3. In state 3, the strategy is to continue attacking in the direction of the already hit ships until we destroy the ship or miss a shot. If we destroy the ship, we move back to state 1 and randomly attack points on the checkered pattern. If we miss our shot, however, we transition into state 4, where we now attack in the opposite direction that we were just attacking (i.e. if we were attacking north of the transition point in state 3, we now attack south of the transition point in state 4). Ideally, this will lead to the enemy's ship being destroyed. However, if not, we have hit an edge case which leads to us pursuing state 5. In state 5, we essentially just attack randomly; though, note that we first attempt to attack randomly along the checkered pattern 50 times until we transition to attacking randomly all together to make the GoodPlayers strategy even better. Before returning, this function pushes the point we are about to recommend onto the previously attacked points vector and sets some other variables, and then we return the point we want to recommend. I found that after running 1000 tests, the Good Player beat the Mediocre Player nearly 80% of the time. 






Pseudocode for Non-trivial Algorithms including bool MediocrePlayer::placeShips(Board& b), Point MediocrePlayer::recommendAttack(), and Point GoodPlayer::recommendAttack()
bool MediocrePlayer::placeShips(Board& b)
repeat 50 times
	block the player’s board
	if our place ship helper returns true and ships were placed
		unblock our board
		return true
	clear our board
return false if we were never able to place ships

bool placeShipsShipIdHelper(Board& b, const Game& g, int shipId)
if our shipID is larger than or equal to the total number of ships to be placed
	return true
for all the rows in the game
	for all the columns in the game
		if we were able to place the ship on the board horizontally
			recursively call place ship helper with one added to shipId
			if recursive call is true
				return true
			unplace the ship to backtrack if needed
		if we were able to place the ship on the board vertically
			recursively call place ship helper with one added to shipId
			if recursive call is true
				return true
			unplace the ship to backtrack if needed
return false

Point GoodPlayer::recommendAttack()
* Note that a valid point is defined as being on the board and not already being attacked

if in state 1
	while the point is valid and follows our checkered pattern
		create a random point
		if we have attacked a number of points that indicates that the checker pattern is full
			move to state 5
			generate a new valid random point
if in state 2
	write four if/else if statements depending on the direction that we are attacking 
		recommend the next attack point to be in that direction
		increment our member variables to keep track of already going in that direction
if in state 3
	write four else/else if statements to recommend the next attack point in that direction
	increment member variables to keep track of going in that direction
	else
		transition to state 4
if in state 4
	write four if statements to move in the opposite direction of those of state 3
		recommend the next valid point in that opposite direction
	else
		move to state 5 to catch edge cases
if in state 5
	attempt to place 50 times along the checker pattern
	if we don’t find a point to attack on the checker pattern, attack a completely random valid point
push our recommended point onto the previous attacked points vector
reset our most recently attacked point
return our point
