#include <iostream>
#include <cmath>
#include <sstream>
#include <cstdlib>

using namespace std;

/*
 * Comp Sci 1511A
 * Final Project: Zork Clone
 * Matt Doebler, Gavin Hoffman, Ryan Koetter, John (JP) Paulson, Harper Wittmers
 * 5 December 2023
 */



/*
 * A collection of static methods and fields used to manage the inventories
 * of the player and the rooms, which are stored as arrays of 10 booleans.
 */
class Items {
public:
    static int itemInt(string item);
    static string itemsList[];
    static string itemDescriptions[];
    static string itemsString(bool inv[]);
};

// The name of each item; index is the number associated with each item in inventories
string Items::itemsList[] = {"Landing Gear Part", "Warp Drive Part", "Engine Part",
                             "Rifle", "Scope", "Machete", "Shovel",
                             "Explosive Powder", "Gas Filter", "Canister"};

// The description of each item obtained when inspecting
string Items::itemDescriptions[] = {"A component of your ship’s landing gear; one of the three parts needed to repair your ship.",
                                    "A component of your ship’s warp drive; one of the three parts needed to repair your ship.",
                                    "A component of your ship’s engine; one of the three parts needed to repair your ship.",
                                    "A standard-issue combat rifle. Powerful, but very hard to aim without a scope.\n(Equipped as long as it is in your inventory)",
                                    "A scope for your rifle.\n(Equipped as long as it and the rifle are in your inventory)",
                                    "A machete; intended to clear foliage, but can also be used as a melee weapon.\n(Equipped as long as it is in your inventory)",
                                    "A shovel you can use to dig with.",
                                    "A highly volatile powder that explodes when used.",
                                    "An improved filter for your spacesuit that protects it from highly volatile gasses.\n(Equipped as long as it is in your inventory)",
                                    "A very heavy metal canister. While in your backpack, you use twice as much oxygen when moving."};

/**
 * Takes in a string representing an item and returns that item's number.
 * Only uses the first word in an item's name, i.e. "landing" is a valid way to
 * get 0 (the index of the landing gear part), but "gear part" is not.
 * "powder" and "filter" can also be used to get the indices of the explosive
 * powder and gas filter, respectively.
 *
 * @param item A string representing the item's name
 * @return The number of the item, or -1 if no valid item is found
 */
int Items::itemInt(string item) {
    string itemFirst;
    istringstream itemStream(item);
    getline(itemStream, itemFirst, ' ');

    if(itemFirst == "landing" || itemFirst == "Landing") {
        return 0;
    } else if(itemFirst == "warp" || itemFirst == "Warp") {
        return 1;
    } else if(itemFirst == "engine" || itemFirst == "Engine") {
        return 2;
    } else if(itemFirst == "rifle" || itemFirst == "Rifle") {
        return 3;
    } else if(itemFirst == "scope" || itemFirst == "Scope") {
        return 4;
    } else if(itemFirst == "machete" || itemFirst == "Machete") {
        return 5;
    } else if(itemFirst == "shovel" || itemFirst == "Shovel") {
        return 6;
    } else if(itemFirst == "explosive" || itemFirst == "Explosive" ||
              itemFirst == "powder" || itemFirst == "Powder") {
        return 7;
    } else if(itemFirst == "gas" || itemFirst == "Gas" ||
              itemFirst == "filter" || itemFirst == "Filter") {
        return 8;
    } else if(itemFirst == "canister" || itemFirst == "Canister") {
        return 9;
    }

    return -1;
}

/**
 * Lists all items in the given inventory.
 *
 * @param inv An array of 10 booleans
 * @return A string of all items in the inventory, or "no items" if there are none.
 */
string Items::itemsString(bool inv[]) {
    int numFound = 0;
    int listed = 0;
    string str = "";

    for(int i = 0; i < 10; i++) {
        if(inv[i]) {
            numFound++;
        }
    }

    for(int i = 0; i < 10; i++) {
        if(inv[i]) {
            if(listed > 0 && numFound > 2) {
                str += ", ";
            }
            listed++;
            if(listed == numFound && numFound > 1) {
                if(numFound == 2) {
                    str += " ";
                }
                str += "and ";
            }
            if(i == 7) {
                str += "some ";
            } else if(i == 2) {
                str += "an ";
            } else {
                str += "a ";
            }
            str += Items::itemsList[i];
        }
    }

    if(numFound == 0) {
        str = "no items";
    }

    return str;
}

/*
 * A class representing a monster to fight.
 */
class Monster {
public:
    Monster();
    Monster(int startingHealth, string name, string description, string deathMessage);
    void attack(int damage);
    bool exists; // True iff there is a living monster in a room
    string getName();
    string getDescription();
private:
    int health; // The health of this monster
    string monsterName; // The name of this monster
    string monsterDescription; // A description of this monster
    string monsterDeathMessage; // Printed when this monster dies
};

/*
 * A class representing a room in the game.
 */
class Room {
public:
    Room(string name, string description, char mapType, int w, int h);
    Room();
    void setConnection(int direction, Room *otherRoom);
    Room* getConnection(int direction);
    string getName();
    string getRoomData();
    bool roomItems[10]; // The room's inventory; each entry corresponds to one item (see Items class)
    void printRoomMap();
    bool canDig; // True if the player can take the dig action here, false otherwise
    bool canExplode; // True if the player can use the explosive powder here
    bool hasMonster();
    Monster monster; // The monster that is in this room
private:
    Room *connections[4]; // A list of rooms connected to this room; entry 0 is to the north, 1 to the east, etc.
    string roomName; // The name of this room
    string roomDescription; // The description for this room
    char printType; // When rendering the room on a map, what shape it will be
    // R for rectangle, C for circle, D for diamond, O for wide ellipse, P for tall ellipse
    int width; // The width of this room when printed out
    int height; // The height of this room when printed out
};

/*
 * A class representing the map and interface between Room and Game classes
 */
class Map {
public:
    Map();
    bool go(int direction);
    void printMap();
    Room getCurrentPosition();
    bool dig();
    bool explode();
    void drop(int itemIndex);
    bool grab(int itemIndex);
    bool monsterHere();
    void speak(string sentence);
    void attack(int damage);
    bool repair();
private:
    Room *currentPosition; // The room the player is currently in
    Room roomList[12]; // List of all rooms
};

/*
 * Class representing current game state. Acts as a class for the player, which doesn't have a dedicated class
 */
class Game {
public:
    Game();
    void run();
    void move(int direction);
    void parseInput(string command);
    void rangedAttack();
    void meleeAttack();
    void menu();
    static void instructions();
private:
    Map map; // The map, which also keeps track of the player's current position
    int health; // The player's health/oxygen
    bool inventory[10]; // The player's inventory (see Items class and roomItems field in Room class)
    bool win; // Set to true if the player has achieved the victory condition
    bool quit; // Set to true if the player quits from the in-game menu
    bool exploded; // Set to true if the player achieves the special explosion loss condition
};

/**
 * No argument constructor for Monster.
 */
Monster::Monster() {
    health = 0;
    monsterName = "";
    monsterDescription = "";
    monsterDeathMessage = "";
    exists = false;
}

/**
 * Constructor for Monster class.
 *
 * @param startingHealth How much health the monster begins with
 * @param name Name of the monster
 * @param description Description of the monster
 * @param deathMessage Message printed when monster dies
 */
Monster::Monster(int startingHealth, string name, string description, string deathMessage) {
    health = startingHealth;
    monsterName = name;
    monsterDescription = description;
    monsterDeathMessage = deathMessage;
    exists = true;
}

/**
 * @return monsterName field
 */
string Monster::getName() {
    return monsterName;
}

/**
 * @return monsterDescription field
 */
string Monster::getDescription() {
    return monsterDescription;
}

/**
 * Deals damage to the monster. If their health goes below 0, prints the death message and
 * sets exists to false.
 *
 * @param damage Amount by which monster's health is decreased
 */
void Monster::attack(int damage) {
    health -= damage;
    if(health <= 0) {
        exists = false;
        cout << monsterDeathMessage << "\n";
    }
}

/**
 * No argument constructor for Room class.
 */
Room::Room() {
    roomName = "";
    roomDescription = "";
    printType = ' ';
    width = 0;
    height = 0;
    canDig = false;
    canExplode = false;
}

/**
 * Constructor for Room class.
 *
 * @param name Name of the room
 * @param description Description of the room
 * @param mapType Shape of the room when printed
 * @param w Width of the room
 * @param h Height of the room
 */
Room::Room(string name, string description, char mapType, int w, int h) {
    roomName = name;
    roomDescription = description;
    for(int i = 0; i < 4; i++) {
        connections[i] = nullptr;
    }
    printType = mapType;
    width = w;
    height = h;
    canDig = false;
    canExplode = false;
    for(int i = 0; i < 10; i++) {
        roomItems[i] = false;
    }
}

/**
 * @return roomName field
 */
string Room::getName() {
    return roomName;
}

/**
 * Creates a string giving the room's description, the items in the room,
 * any monsters in the room, and indicating whether the room can be dug or
 * exploded.
 *
 * @return A string describing the room
 */
string Room::getRoomData() {
    string roomData = "" + roomDescription + "\n";
    roomData += "You see " + Items::itemsString(roomItems) + " in the room.\n";
    if(hasMonster()) {
        roomData += monster.getDescription() + "\n";
    }
    if(canDig) {
        roomData += "There is something sticking out of the dirt. You could dig it out with the right tool.\n";
    }
    if(canExplode) {
        roomData += "There is something wedged underneath a cracked rock, but the rock is too heavy to move.\n";
    }

    return roomData;
}

/**
 * Sets an element in the connections array to point to a different room
 *
 * @param direction 0 for north, 1 for east, 2 for south, 3 for west
 * @param otherRoom A pointer to the room which will be connected to this room
 */
void Room::setConnection(int direction, Room *otherRoom) {
    connections[direction] = otherRoom;
}

/**
 * Returns the room connected to this room in the specified direction.
 *
 * @param direction 0 for north, etc.
 * @return Pointer to the room in this direction
 */
Room *Room::getConnection(int direction) {
    return connections[direction];
}

/**
 * Prints a visual representation of this room to output, including any connections.
 */
void Room::printRoomMap() {
    string boundary; // string to be printed when this is the edge of the room
    string space; // string to be printed when not at the edge of the room
    int offset = 0; // if there is a connection to the west, the whole room must be offset to clear space
    // so that it can be displayed properly
    if(connections[3] != nullptr) {
        offset = (connections[3]->getName()).length() + 4;
    }
    switch(printType) {
        case 'D': // If the room's shape is a diamond
            boundary = " * ";
            space = "   ";
            break;
        case 'C': // If it is a circle
        case 'R': // If it is a rectangle
            boundary = "***";
            space = "   ";
            break;
        case 'O': // If it is a long ellipsoid
            boundary = "* * *";
            space = "     ";
            break;
        default: // If it is a tall ellipsoid, or something else
            boundary = "*";
            space = " ";
    }

    if(connections[0] != nullptr) { // If there is a connection to the north
        for(int y = 0; y < 2; y++) {
            for(int x = 0; x < offset; x++) {
                cout << " ";
            }
            for(int x = 0; x < (width / 2); x++) {
                cout << space;
            }
            if(y == 0) {
                cout << connections[0]->getName();
            } else {
                cout << " | ";
            }
            cout << "\n";
        }
    }

    for(int y = 0; y < height; y++) { // Going line by line
        if(offset > 0) { // i.e. if there is a connection to the west
            if(y == (height / 2)) { // If halfway down the screen, display connection to west
                cout << connections[3]->getName() << " -- ";
            } else { // Pad space to left
                for(int x = 0; x < offset; x++) {
                    cout << " ";
                }
            }
        }
        int size = height / 2;
        double root;
        for(int x = 0; x < width; x++) {
            switch(printType) {
                case 'C': // For circle or either type of ellipsoid
                case 'O': // Ellipsoids are treated as circles and just get wider or taller by using
                case 'P': // longer and shorter strings, respectively
                    root = sqrt(pow(x - size, 2) + pow(y - size, 2));
                    if(root < (size + 0.5) && root > (size - 0.5)) {
                        cout << boundary;
                    } else {
                        cout << space;
                    }
                    break;
                case 'R': // For rectangle
                    if(x == 0 || x == (width - 1) || y == 0 || y == (height - 1)) {
                        cout << boundary;
                    } else {
                        cout << space;
                    }
                    break;
                case 'D': // For diamond
                    if((abs(x - size) + abs(y - size)) == size) {
                        cout << boundary;
                    } else {
                        cout << space;
                    }
            }
        }
        if(y == (height / 2) && connections[1] != nullptr) {
            // If halfway down and there is connection to east, display it
            cout << " -- " << connections[1]->getName();
        }
        cout << "\n";
    }

    if(connections[2] != nullptr) { // If there is connection to south, display it
        for(int y = 0; y < 2; y++) {
            for(int x = 0; x < offset; x++) {
                cout << " ";
            }
            for(int x = 0; x < (width / 2); x++) {
                cout << space;
            }
            if(y == 1) {
                cout << connections[2]->getName();
            } else {
                cout << " | ";
            }
            cout << "\n";
        }
    }
}

/**
 * @return Whether there is a monster in this room
 */
bool Room::hasMonster() {
    return monster.exists;
}

/**
 * Constructor for Map class. Default constructor works fine, since game always starts the same way
 */
Map::Map() {
    // Creates each room
    Room spaceship("Spaceship wreckage", "The wreckage of your spaceship", 'D', 11, 11);
    Room desert("Desert", "The desert north of your spaceship", 'R', 7, 7);
    Room hills("Hills", "The hills east of your spaceship", 'O', 5, 5);
    Room forestEdge("Forest's edge", "The edge of the forest", 'R', 5, 9);
    Room swamp("Swamp", "The swamp west of your spaceship", 'R', 7, 7);

    Room ancientBuilding("Ancient building", "A gateway into an ancient alien city", 'C', 9, 9);
    Room ancientRuins("Ancient ruins", "The ruins of an ancient alien city", 'R', 5, 5);

    Room mountainPath("Mountain path", "A narrow traversable path through the otherwise impenetrable mountains", 'R', 9, 5);
    Room mountains("Mountains", "Deep in the mountains, at the end of the mountain path", 'D', 5, 5);

    Room forest("Forest", "A forest of alien trees", 'C', 7, 7);
    Room grove("Grove", "A small grove deep in the forest", 'O', 7, 7);

    Room lake("Lake", "A pristine lake surrounded by alien flora", 'P', 7, 7);

    // Adds each room to roomList array and sets connections
    roomList[0] = spaceship;
    roomList[1] = desert;
    roomList[0].setConnection(0, &roomList[1]);
    roomList[1].setConnection(2, &roomList[0]);
    roomList[2] = hills;
    roomList[0].setConnection(1, &roomList[2]);
    roomList[2].setConnection(3, &roomList[0]);
    roomList[3] = forestEdge;
    roomList[0].setConnection(2, &roomList[3]);
    roomList[3].setConnection(0, &roomList[0]);
    roomList[4] = swamp;
    roomList[0].setConnection(3, &roomList[4]);
    roomList[4].setConnection(1, &roomList[0]);

    roomList[5] = ancientBuilding;
    roomList[1].setConnection(0, &roomList[5]);
    roomList[5].setConnection(2, &roomList[1]);
    roomList[6] = ancientRuins;
    roomList[5].setConnection(1, &roomList[6]);
    roomList[6].setConnection(3, &roomList[5]);

    roomList[7] = mountainPath;
    roomList[2].setConnection(1, &roomList[7]);
    roomList[7].setConnection(3, &roomList[2]);
    roomList[8] = mountains;
    roomList[7].setConnection(1, &roomList[8]);
    roomList[8].setConnection(3, &roomList[7]);

    roomList[9] = forest;
    roomList[3].setConnection(2, &roomList[9]);
    roomList[9].setConnection(0, &roomList[3]);
    roomList[10] = grove;
    roomList[9].setConnection(2, &roomList[10]);
    roomList[10].setConnection(0, &roomList[9]);

    roomList[11] = lake;
    roomList[4].setConnection(3, &roomList[11]);
    roomList[11].setConnection(1, &roomList[4]);

    // Sets currentPosition to spaceship room
    currentPosition = &roomList[0];

    // Sets initial places of items and which rooms can be dug and exploded
    roomList[0].roomItems[3] = true;
    roomList[1].roomItems[9] = true;
    roomList[2].roomItems[8] = true;
    roomList[4].canExplode = true;
    roomList[5].roomItems[7] = true;
    roomList[6].roomItems[2] = true;
    roomList[8].roomItems[1] = true;
    roomList[9].roomItems[6] = true;
    roomList[10].canDig = true;
    roomList[11].roomItems[5] = true;

    // Creates Sphinx and places it in ancient building
    Monster sphinx(180, "Sphinx", "A Sphinx guards the passage to the east. It asks you "
                                  "\"What is a Martian's favorite candy?\" (To answer, use the "
                                  "'say' or 'talk' command, followed by your answer in all lower case.)",
                   "The Sphinx vanishes into mist...");
    roomList[5].monster = sphinx;

    // Creates hostile monster and places it in mountain path
    Monster hostileMonster(100, "Hostile Monster", "A hostile monster is running towards you!\n"
                                                   "You can't go to the east while it's here!\n",
                           "The monster keels over and dies.\n");
    roomList[7].monster = hostileMonster;


}

/**
 * Determines whether the player is able to move in this direction, and if they can,
 * updates currentPosition.
 *
 * @param direction 0 for north, etc.
 * @return true if player can move in this direction, false otherwise
 */
bool Map::go(int direction) {
    if(currentPosition->getConnection(direction) == nullptr) { // If room has no valid connection
        cout << "There's nothing in this direction!\n";
        return false;
    }

    if(direction == 1 && currentPosition->hasMonster()) { // Player can't move east if there is a monster in the room
        // While there's no particular design reason for this,
        // the map is designed so that the monsters always block paths to the east
        cout << "Your path is blocked by a monster!\n";
        return false;
    }

    Room *nextPosition = currentPosition->getConnection(direction);
    currentPosition = nextPosition;
    return true;
}

/**
 * Prints the room map and description for the current room.
 */
void Map::printMap() {
    currentPosition->printRoomMap();
    cout << currentPosition->getRoomData();
}

/**
 * @return The Room object that currentPosition points to
 */
Room Map::getCurrentPosition() {
    return *currentPosition;
}

/**
 * If the player can dig in the current room, updates canDig to false and adds the landing
 * gear part to the room's inventory.
 *
 * @return true if the player successfully digs up the landing gear part
 */
bool Map::dig() {
    if(currentPosition->canDig) {
        currentPosition->canDig = false;
        currentPosition->roomItems[0] = true;
        cout << "You find a warp drive part buried in the dirt.\n";
        return true;
    } else {
        cout << "Can't dig here!\n";
        return false;
    }
}

/**
 * If the player can use the explosive powder in the current room, updates canExplode
 * to false and adds the scope to the room's inventory.
 *
 * @return true if the player successfully blows up the rock and gets the scope
 */
bool Map::explode() {
    if(currentPosition->canExplode) {
        currentPosition->canExplode = false;
        currentPosition->roomItems[4] = true;
        cout << "You blow up the rock and find a scope for your rifle.\n";
        return true;
    } else {
        cout << "Nothing to use the powder on here!\n";
        return false;
    }
}

/**
 * Adds the specified item to the room's inventory
 *
 * @param itemIndex Index of the item to drop
 */
void Map::drop(int itemIndex) {
    currentPosition->roomItems[itemIndex] = true;
    cout << "You drop the " << Items::itemsList[itemIndex] << ".\n";
}

/**
 * If specified item is in the room, removes it from the room's inventory and returns true.
 *
 * @param itemIndex Index of the item to take
 * @return true if the item could be successfully removed from the room
 */
bool Map::grab(int itemIndex) {
    if(currentPosition->roomItems[itemIndex]) {
        currentPosition->roomItems[itemIndex] = false;
        cout << "You pick up the " << Items::itemsList[itemIndex] << ".\n";
        return true;
    }

    cout << "There's no such item in this room!\n";
    return false;
}

/**
 * @return true if current room has a monster in it
 */
bool Map::monsterHere() {
    return currentPosition->hasMonster();
}

/**
 * Called if the player uses the speak command. Only use is to answer the Sphinx's riddle.
 * No effect besides text output if Sphinx not in the room or if the answer is wrong.
 * Removes the Sphinx from the room (by attacking it for its entire health bar)
 * if riddle is answered correctly.
 *
 * @param sentence Answer to the riddle, or phrase to be spoken
 */
void Map::speak(string sentence) {
    if(!monsterHere() || currentPosition->monster.getName() != "Sphinx") {
        cout << "Nobody responds...\n";
    } else {
        // Requires the answer to be in all lower case
        if(sentence == "a mars bar" || sentence == "mars bar" || sentence == "mars bars") {
            currentPosition->monster.attack(180);
        } else {
            cout << "The Sphinx remains still...\n";
        }
    }
}

/**
 * Calls the attack function on the monster in this room.
 *
 * @param damage Damage done to monster
 */
void Map::attack(int damage) {
    currentPosition->monster.attack(damage);
}

/**
 * Simply returns whether the player is in the spaceship.
 * Used to ensure that the player is in the right place before achieving
 * the victory condition.
 *
 * @return true if the current room is the spaceship, false otherwise
 */
bool Map::repair() {
    return (currentPosition->getName() == "Spaceship wreckage");
}

/**
 * Constructor for game. No argument constructor works fine, since game is always initialized the same.
 */
Game::Game() {
    health = 40;
    win = false;
    quit = false;
    exploded = false;
    for(int i = 0; i < 10; i++) {
        inventory[i] = false;
    }
}

/**
 * Runs the game.
 */
void Game::run() {
    cout << "You wake up on an alien planet among the ruins of your spaceship.\n"
            "You appear to be the only survivor. You need to repair the ship and escape\n"
            "with your life. To do this, you will need to find a landing gear component,\n"
            "a warp drive component, and an engine component, which were scattered somewhere\n"
            "on the planet when the ship crashed. You will then need to return to the ship and\n"
            "repair it (by using any of the components with the use command).\n"
            "You can open the in-game menu at any time with the menu command\n"
            "see a list of instructions or quit (progress is not saved).\n\n"
            "You have only 40 hours of oxygen remaining.\n"
            "Moving will use up 1 hour of your oxygen. If you should come to any harm, your\n"
            "suit's medkit will heal you, but the chemical reactions it uses need oxygen,\n"
            "which it will take from your tank.\nGood luck!\n\n";

    map.printMap();
    string command;

    do {
        cout << "You have " << health << " hours of oxygen remaining.\n";
        getline(cin, command);
        parseInput(command);
    } while(health > 0 && !win && !quit && !exploded);
    // Game continues until health runs out or a special condition is achieved

    if(win) { // If player wins
        cout << "You repair your spaceship and leave the planet.\nCongratulations! You have won!\n";
    } else if(exploded) { // If player explodes
        cout << "You have exploded and lost.\n";
    } else if(!quit) { // If player quits, no message should be printed. Only other path is if they ran out of health
        cout << "You have run out of oxygen and lost.\n";
    }
}

/**
 * Parses the command input by the player and updates the game state.
 * Saves the command in instruction, and any additional arguments in argument
 *
 * @param command Command input by the player
 */
void Game::parseInput(string command) {
    istringstream commandStream(command);
    string instruction; // Determines what the player wants to do
    string argument;
    int itemArg; // If the input requires an item, this is assigned to that item's number
    getline(commandStream, instruction, ' ');
    getline(commandStream, argument);
    if(instruction == "go" || instruction == "Go" || instruction == "walk" || instruction == "Walk" ||
       instruction == "run" || instruction == "Run") { // If player wants to move
        switch (argument.at(0)) { // Only parses the first character after the command
            case 'N':
            case 'n':
                move(0);
                break;
            case 'E':
            case 'e':
                move(1);
                break;
            case 'S':
            case 's':
                move(2);
                break;
            case 'W':
            case 'w':
                move(3);
                break;
            default:
                cout << "Not a valid direction\n";
        }
    } else if(instruction == "grab" || instruction == "Grab") { // If the player picks an item up
        itemArg = Items::itemInt(argument);
        if (itemArg == -1 || inventory[itemArg]) { // If player inputs an invalid item
            cout << "There's no such item in this room!\n";
        } else { // If the item can be picked up from the room, adds it to inventory
            inventory[itemArg] = map.grab(itemArg);
        }
    } else if(instruction == "drop" || instruction == "Drop") { // If the player drops an item
        itemArg = Items::itemInt(argument);
        if(itemArg == -1 || !inventory[itemArg]) {
            // If the player inputs an invalid item or does not have the item they want to drop
            cout << "You have no such item!\n";
        } else {
            map.drop(itemArg);
            inventory[itemArg] = false;
        }
    } else if(instruction == "inspect" || instruction == "Inspect") {
        // If the player wants to get an item's description
        itemArg = Items::itemInt(argument);
        if(itemArg == -1 || !inventory[itemArg]) {
            cout << "You have no such item!\n";
        } else {
            cout << Items::itemDescriptions[itemArg] << "\n";
        }
    } else if(instruction == "backpack" || instruction == "Backpack") {
        // If the player wants to view their inventory
        cout << "You have " << Items::itemsString(inventory) << " in your backpack.\n";
    } else if(instruction == "view" || instruction == "View") {
        // If the player wants to view the map
        map.printMap();
    } else if(instruction == "shoot" || instruction == "Shoot") {
        // If the player wants to make a ranged attack
        rangedAttack();
    } else if(instruction == "attack" || instruction == "Attack") {
        // If the player wants to make a melee attack
        meleeAttack();
    } else if(instruction == "say" || instruction == "Say" ||
              instruction == "talk" || instruction == "Talk") {
        // If the player wishes to say something
        map.speak(argument);
    } else if(instruction == "dig" || instruction == "Dig") {
        // If the player wants to dig
        if (inventory[6]) { // If the player has a shovel
            if(map.dig()) {
                inventory[0] = map.grab(0);
            }
        } else {
            cout << "You have nothing to dig with!";
        }
    } else if(instruction == "use" || instruction == "Use") {
        // If the player wants to use an item
        itemArg = Items::itemInt(argument);
        if (itemArg == -1 || !inventory[itemArg]) {
            cout << "You have no such item!\n";
        } else {
            switch (itemArg) {
                case 0: // If player uses a spaceship part, checks that they have
                case 1: // all parts and are at the spaceship.
                case 2: // If so, they win
                    if(!inventory[0] || !inventory[1] || !inventory[2] || !map.repair()) {
                        cout << "You need all 3 spaceship parts in order to repair, and you must be "
                                "at the spaceship wreckage.\n";
                    } else {
                        win = true;
                    }
                    break;
                case 3: // Using the rifle does the same thing as the shoot command
                    rangedAttack();
                    break;
                case 5: // Using the machete does the same thing as the attack command
                    meleeAttack();
                    break;
                case 6: // Using the shovel does the same thing as the dig command
                    if(map.dig()) {
                        inventory[0] = map.grab(0);
                    }
                    break;
                case 7: // If the player uses the explosive powder
                    if(map.explode()) { // If the player is in a valid room, reveals the scope
                        inventory[4] = map.grab(4); // Picks up scope
                        cout << "The explosion reacts with the alien atmosphere\n"
                                "and creates a cloud of poison gas!\n";
                        if (!inventory[8]) {
                            // Player achieves the special exploded loss condition if they
                            // don't have the gas filter
                            cout << "It corrodes the filter in your spacesuit!\n";
                            exploded = true;
                        }
                    }
                    break;
                default:
                    cout << "No way to use this item here!\n";
            }
        }
    } else if(instruction == "menu" || instruction == "Menu") {
        // If the player wants to open the menu
        menu();
    } else { // If the player doesn't input a valid command
        cout << "Command not found!\n";
    }

}

/**
 * Opens the in-game menu, which allows the player to continue, see instructions, or quit the game.
 */
void Game::menu() {
    int choice;

    do {
        cout << "Input 0 to continue, 1 to see a list of game inputs, and 2 to quit to main menu:\n";
        cin >> choice;
        while (choice < 0 || choice > 2) {
            cout << "Invalid choice.\n";
            cout << "Input 0 to continue,\n1 to see a list of game inputs,\n 2 to quit to main menu:\n";
            cin >> choice;
        }

        if (choice == 2) {
            quit = true;
        } else if (choice == 1) {
            instructions();
        }
    } while(choice == 1);

    string clear;
    getline(cin, clear);
}

/**
 * Attempts to make the player move to a different room. If the move is valid,
 * moves them, decreases their oxygen, and prints the map and room info of the new room.
 *
 * @param direction The direction the player will move
 */
void Game::move(int direction) {
    if(map.go(direction)) {
        health--; // player loses oxygen
        if(inventory[9]) { // If the player has the canister troll item, loses another oxygen
            health--;
        }
        cout << "You move to the " << map.getCurrentPosition().getName() << "\n";
        map.printMap();
    }
}

/**
 * Makes a ranged attack if the player has the rifle and there is a valid monster to attack.
 * If so, determines whether it hits/crits and the amount of damage done.
 */
void Game::rangedAttack() {
    if(!inventory[3]) { // If the player doesn't have the rifle
        cout << "You have no ranged weapon!\n";
        return;
    } else if(!map.monsterHere()) { // If there is no monster to attack
        cout << "No valid target!\n";
        return;
    }

    int aim;
    int damage;
    aim = rand() % 10 + 1; // Random int from 1-10
    damage = rand() % 15 + 5; // Random int from 5-20

    if(!inventory[4]) { // If the player doesn't have the scope
        if(aim < 6) { // 1-5 (50% chance) is a miss
            cout << "You missed!\n";
        } else if(aim == 10) { // 10 (10% chance) is a critical (flat 25 damage)
            cout << "A critical! 25 damage!\n";
            map.attack(25);
        } else { // 6-9 (40% chance) is a normal hit
            cout << "You hit for " << damage << " damage!\n";
            map.attack(damage);
        }
    } else { // If the player has the scope
        if(aim > 7) { // 8-10 (30% chance) is a critical
            cout << "A critical! 25 damage!\n";
            map.attack(25);
        } else { // 1-7 (70% chance) is a normal hit
            cout << "You hit for " << damage << " damage!\n";
            map.attack(damage);
        }
    }

    if(map.monsterHere()) { // If the monster is still alive after being hit, deals damage to the player
        cout << "The monster hits you back!\n"
                "Your medkit heals you, but uses 1 hour of oxygen!\n";
        health--;
    }
}

/**
 * Makes a melee attack using the machete if the player has it, otherwise using fists and
 * determines the amount of damage done.
 */
void Game::meleeAttack() {
    if(!map.monsterHere()) { // If there's no monster in the room
        cout << "No valid target!\n";
    }

    int damage;

    cout << "You hit the monster with your ";
    if(inventory[5]) { // If the player has the machete, deals 5-15 damage
        cout << "machete ";
        damage = rand() % 10 + 5;
    } else { // If the player does not have the machete, deals 1-5 damage
        cout << "fists ";
        damage = rand() % 5 + 1;
    }

    cout << "for " << damage << " damage!\n";
    map.attack(damage);

    if(map.monsterHere()) { // If the monster is still alive after being hit, deals damage to the player
        cout << "The monster hits you back!\n"
                "Your medkit heals you, but uses 1 hour of oxygen!\n";
        health--;
    }
}

/**
 * Static method that prints the list of possible commands.
 */
void Game::instructions() {
    cout << "go [north/east/south/west]: Move in the specified direction.\n"
            "\t(walk and run also valid)\n"
            "grab [item]: Pick up the specified item.\n"
            "drop [item]: Removes the specified item from your inventory and places it in your current room.\n"
            "inspect [item]: Gives a description of the specified item (must be in your inventory).\n"
            "backpack: Lists all items in your inventory.\n"
            "view: Shows the map and gives a description of your current room.\n"
            "shoot: Delivers an attack if there is a monster in the room (you must have a ranged weapon).\n"
            "attack: Delivers a melee attack if there is a monster in the room\n"
            "\t(will use a melee weapon if you have one, otherwise will use your fists).\n"
            "say [phrase]: Says the specified phrase to whoever is nearby. (talk is also valid)\n"
            "use [item]: Uses the specified item (must be in your inventory).\n"
            "\tUsing any spaceship part will allow you to repair your spaceship\n"
            "\tif you have all of them and are at the spaceship.\n"
            "menu: Opens the in-game menu.\n";

}

void mainMenu();

int main() {

    mainMenu();

    return 0;
}

/**
 * Game's main menu. Allows the player to play the game, view in-game instructions,
 * or quit the program.
 */
void mainMenu() {
    cout << "Welcome to our Zork clone, Planet!\n";
    int play;

    do {
        cout << "Input 0 to play, 1 to view the game instructions, and 2 to quit:\n";
        cin >> play;
        while(play < 0 || play > 2) {
            cout << "Invalid choice.\n";
            cout << "Input 0 to play, 1 to view the game instructions, and 2 to quit:\n";
        }

        string clear;
        getline(cin, clear);

        if(play == 0) {
            Game game;
            game.run();
        } else if(play == 1) {
            Game::instructions();
        }
    } while(play != 2);

    cout << "Goodbye!";
}