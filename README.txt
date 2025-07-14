Kirby's Adventure Game
1. Student Details
Keren Zaig 329878656
Issachar Welt 021891163

2. General Explanation
This project is a 2D platformer game inspired by the classic "Kirby's Adventure". It is built using C++ and the SFML multimedia library.

The player controls the main character, Kirby, through various levels. The core gameplay involves navigating platforms, avoiding hazards, and interacting with enemies. Kirby can walk, jump, swim, and inhale enemies. Inhaling certain enemies grants Kirby special abilities, such as the Spark attack. The game also features various power-ups that provide temporary or permanent benefits, like health restoration, extra lives, and speed boosts. The objective is to navigate through each level and reach the exit door to proceed. The game includes a multi-level structure, a Heads-Up Display (HUD) for player stats, and a menu system.

3. Design
The program's architecture is built upon several key design patterns to ensure flexibility, scalability, and separation of concerns.

GameController: This is the central class of the game. It owns the main game loop, the game window, and the primary data structures holding all game objects (m_kirby, m_fixedObjects, m_enemies). It is responsible for orchestrating the main phases of each frame: handling events, updating all objects, checking for collisions, and drawing everything to the screen. It also manages the game state (e.g., switching between the main menu and gameplay).

GameObject Hierarchy:

GameObject: An abstract base class for every entity in the game world. It defines a common interface for updating, drawing, and handling collisions. It uses Double Dispatch for collision handling, allowing specific object types to react to each other correctly.

MovingObject: Inherits from GameObject and adds properties for movement, such as speed and direction. Kirby and Enemy are derived from this class.

FixedObject: Inherits from GameObject and represents all non-moving entities, such as Wall, Floor, PowerUp, and Door.

State Pattern (Kirby):

Kirby's behavior is managed by a State pattern. The abstract KirbyState class defines the interface for all possible states (e.g., KirbyStandingState, KirbyJumpingState, KirbySwimmingState). The Kirby class holds a std::unique_ptr<KirbyState> to its current state. Input handling and physics updates are delegated to the current state object. This cleanly separates the logic for each of Kirby's behaviors and makes it easy to transition between them (e.g., from KirbyWalkingState to KirbyJumpingState when the jump key is pressed).

Strategy Pattern (Enemies):

Enemy behavior is made modular using the Strategy pattern. Each Enemy object is composed of a MoveBehavior, AttackBehavior, and CollisionBehavior. These are abstract base classes, and concrete strategies (e.g., PatrolMove, FlyingMove, SparkAttack) can be assigned to an enemy at runtime. This allows for creating diverse enemies with different combinations of behaviors without creating a large number of enemy subclasses.

Command Pattern (Power-ups):

The power-up system uses the Command pattern. The abstract PowerUpCommand class defines an interface with execute() and undo() methods. Concrete commands like SpeedBoostCommand and HealthCommand encapsulate a specific action. When Kirby collects a PowerUp object, a corresponding command is created and passed to the PowerUpManager. The manager executes the command and, for timed effects, holds onto it to call undo() when the duration expires. This decouples the power-up item from the logic of its effect.

Factory Pattern (Level Loading):

The GameObjectFactory is used to create game objects dynamically during level loading. The WorldMap class reads a collision map image pixel by pixel. Each color corresponds to a specific game object type. The factory maintains a std::map of sf::Color to a creation function (lambda). When WorldMap encounters a color, it asks the factory to create the associated object. This makes the level-loading process highly extensible; new object types can be added by simply registering them with the factory, without any changes to the WorldMap or GameController.

Singleton Pattern (Resource Management):

The ResourceManager is a Singleton that manages loading and storing game assets like textures, fonts, and sounds. This ensures that each resource is loaded only once and can be accessed globally via a shared pointer, preventing unnecessary memory usage and disk I/O.

4. File List
Core System
main.cpp: The entry point of the program. Initializes GameController and starts the game loop.

GameController.h/.cpp: The central class that manages the game loop, states, objects, and core logic.

GameState.h: Abstract base class for the game's states (e.g., Main Menu, Playing).

MainMenuState.h/.cpp: Implements the main menu screen and its logic.

PlayingState.h/.cpp: Implements the main gameplay state, managing level progression.

Level.h/.cpp: Manages the data for a single level, including its objects and background.

WorldMap.h/.cpp: Responsible for loading game objects from a collision map image and drawing the level background.

GameObjectFactory.h/.cpp: Implements the Factory pattern for creating GameObject instances from color codes.

ResourceManager.h/.cpp: Singleton class for managing game assets (textures, fonts).

MusicManager.h/.cpp: Manages loading and playing background music.

HUD.h/.cpp: Manages the Heads-Up Display, showing player health, lives, and score.

Animator.h/.cpp: Manages sprite sheet animations for game objects.

SpriteSheet.h/.cpp: A utility class to easily draw sprites from a larger sprite sheet image.

GlobalSizes.h: Contains global constants for screen dimensions, physics, etc.

Game Objects (GameObj/)
GameObject.h/.cpp: Abstract base class for all game entities.

MovingObject.h/.cpp: Base class for objects that can move (Kirby, Enemy).

FixedObject.h/.cpp: Base class for static objects (Wall, Floor, PowerUp).

Kirby.h/.cpp: The player character class. Manages its own state machine and power-ups.

Enemy.h/.cpp: The enemy class. Uses strategy patterns for its behaviors.

PowerUp.h/.cpp: Base class for collectible power-up items.

HealthPowerUp.h/.cpp, LifePowerUp.h/.cpp, SpeedPowerUp.h/.cpp: Concrete power-up items.

Wall.h/.cpp, Floor.h/.cpp, Spike.h/.cpp, Door.h/.cpp, Exit.h/.cpp, Water.h/.cpp: Concrete static game objects.

Kirby States (States/KirbyStates/)
KirbyState.h: Abstract base class for all of Kirby's states.

KirbyStandingState.h/.cpp, KirbyWalkingState.h/.cpp, KirbyJumpingState.h/.cpp, KirbyFallingState.h/.cpp, KirbySwimmingState.h/.cpp, KirbySparkAttackState.h/.cpp, KirbyWaterAttackState.h/.cpp: Concrete implementations of Kirby's various behavioral states.

Enemy Behaviors (Behaviors/)
MoveBehavior.h: Abstract base class for enemy movement strategies.

PatrolMove.h, FlyingMove.h, HoppingMove.h: Concrete movement strategies.

AttackBehavior.h/.cpp: Abstract base class for enemy attack strategies.

SimpleAttack.h, SparkAttack.h/.cpp: Concrete attack strategies.

CollisionBehavior.h/.cpp: Abstract base class for enemy collision strategies.

IgnoreWalls.h: A concrete collision strategy.

Power-Up Commands (Commands/)
PowerUpCommand.h: Abstract base class for power-up effects.

HealthCommand.h, LifeCommand.h, SpeedBoostCommand.h, SparkCommand.h: Concrete command classes that encapsulate specific power-up effects.

PowerUpManager.h/.cpp: Manages the lifecycle of active power-up commands.

5. Main Data Structures
std::vector<std::unique_ptr<GameObject>> m_fixedObjects (in GameController): This vector holds all the static objects in the current level. std::unique_ptr is used to manage the memory of the objects automatically.

std::vector<std::unique_ptr<Enemy>> m_enemies (in GameController): This vector holds all the enemy objects.

std::map<sf::Color, CreateFunction, ColorComparator> (in GameObjectFactory): This map is the core of the Factory pattern. It maps a specific sf::Color to a lambda function that knows how to create the corresponding GameObject. This allows for data-driven object creation.

std::vector<ActivePowerUpEffect> m_activeEffects (in PowerUpManager): A vector of structs, where each struct contains a unique_ptr to a PowerUpCommand and an sf::Clock to track its duration. This allows the manager to apply and later undo timed effects.

6. Noteworthy Algorithms
Double Dispatch for Collision Handling: Collision detection starts with a simple bounding box check. When a collision is confirmed, a double dispatch mechanism is used to determine the specific types of the two colliding objects.

Level Loading from Collision Map: The WorldMap::loadObjectsFromFile function implements a data-driven approach to level design. It iterates over every pixel of a special PNG file ("collision map"). The color of each pixel determines which game object to create at that location, using the GameObjectFactory. This decouples the level design from the game's code, allowing levels to be created and modified by simply editing an image.

Vertical Camera Snapping: The camera in GameController::updateView follows Kirby smoothly on the X-axis but is "snapped" on the Y-axis. The level is treated as being divided into vertical sections. The camera's Y position is locked to the center of the section Kirby is currently in. This prevents the player from seeing areas above or below them and emulates the camera behavior of many classic platformers.

7. Known Bugs
When more than Twizzy is spawned in a level, they can have collision problems.
Animations sometimes don't update immediately.
Kirby can sometimes clip through the floor when going up hills.

8. Other Notes
This design approach makes it straightforward to add new enemies, power-ups, abilities, and level objects in the future.

There are some rules to follow when making a new level such as
	1) Backround texture must be called Level[num].png and Collision map must be called Level[num]Collisions.png
	2) Door pairs cannot be the same color as any of the factory registered objects
	3) Water areas must be marked with a pixel in the top left corner and in the bottom right corner
	4) There can only be one exit per level

Because of time constraints some aspects of the game were cut, such as a final boss and more powers for Kirby,
but we made the design expandable for the potential of future updates.

