#pragma once
#include <list>
#include <string>

#include <BWAPI/Interface.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Error.h>

#include <BWAPI/Filters.h>
#include <BWAPI/UnaryFilter.h>
#include <BWAPI/Input.h>
#include <BWAPI/CoordinateType.h>

#include <sstream>

namespace BWAPI
{
  // Forward Declarations
  class Bulletset;
  class Color;
  class Event;
  class Force;
  class Forceset;
  class GameType;
  class Player;
  class Playerset;
  class Race;
  class Region;
  class Regionset;
  class TechType;
  class Unit;
  class UnitCommand;
  class Unitset;
  class UpgradeType;

  /// The abstract Game class is implemented by BWAPI and is the primary means of obtaining all
  /// game state information from Starcraft Broodwar. Game state information includes all units,
  /// resources, players, forces, bullets, terrain, fog of war, regions, etc.
  class Game : public Interface<Game>
  {
  protected:
    virtual ~Game() {};
  public :
    /// Retrieves the set of all teams/forces. Forces are commonly seen in @UMS game types and
    /// some others such as @TvB and the team versions of game types.
    ///
    /// @returns Forceset containing all forces in the game.
    virtual const Forceset& getForces() const = 0;

    /// Retrieves the set of all players in the match. This includes the neutral player, which
    /// owns all the resources and critters by default.
    ///
    /// @returns Playerset containing all players in the game.
    virtual const Playerset& getPlayers() const = 0;

    /// Retrieves the set of all accessible units. If Flag::CompleteMapInformation is enabled,
    /// then the set also includes units that are not visible to the player.
    ///
    /// @note Units that are inside refineries are not included in this set.
    ///
    /// @returns Unitset containing all known units in the game.
    virtual const Unitset& getAllUnits() const = 0;

    /// Retrieves the set of all accessible @minerals in the game.
    ///
    /// @returns Unitset containing @minerals
    virtual const Unitset& getMinerals() const = 0;

    /// Retrieves the set of all accessible @geysers in the game.
    ///
    /// @returns Unitset containing @geysers
    virtual const Unitset& getGeysers() const = 0;

    /// Retrieves the set of all accessible neutral units in the game. This includes @minerals,
    /// @geysers, and @critters.
    ///
    /// @returns Unitset containing all neutral units.
    virtual const Unitset& getNeutralUnits() const = 0;

    /// Retrieves the set of all @minerals that were available at the beginning of the game.
    ///
    /// @note This set includes resources that have been mined out or are inaccessible.
    ///
    /// @returns Unitset containing static @minerals
    virtual const Unitset& getStaticMinerals() const = 0;

    /// Retrieves the set of all @geysers that were available at the beginning of the game.
    ///
    /// @note This set includes resources that are inaccessible.
    ///
    /// @returns Unitset containing static @geysers
    virtual const Unitset& getStaticGeysers() const = 0;

    /// Retrieves the set of all units owned by the neutral player (resources, critters, etc.)
    /// that were available at the beginning of the game.
    ///
    /// @note This set includes units that are inaccessible.
    ///
    /// @returns Unitset containing static neutral units
    virtual const Unitset& getStaticNeutralUnits() const = 0;

    /// Retrieves the set of all accessible bullets.
    ///
    /// @returns Bulletset containing all accessible Bullet objects.
    virtual const Bulletset& getBullets() const = 0;

    /// Retrieves the set of all accessible @Nuke dots.
    ///
    /// @note Nuke dots are the red dots painted by a @Ghost when using the nuclear strike ability.
    ///
    /// @returns Set of Positions giving the coordinates of nuke locations.
    virtual const Position::set& getNukeDots() const = 0;

    /// Retrieves the list of all unhandled game events.
    ///
    /// @returns std::list containing Event objects.
    virtual const std::list< Event >& getEvents() const = 0;

    /// Retrieves the Force interface object associated with a given identifier.
    ///
    /// @param forceID
    ///   The identifier for the Force object.
    ///
    /// @returns Force interface object mapped to the given \p forceID.
    /// @retval nullptr if the given identifier is invalid.
    virtual Force* getForce(int forceID) const = 0;

    /// Retrieves the Player interface object associated with a given identifier.
    ///
    /// @param playerID
    ///   The identifier for the Player object.
    ///
    /// @returns Player interface object mapped to the given \p playerID.
    /// @retval nullptr if the given identifier is invalid.
    virtual Player* getPlayer(int playerID) const = 0;

    /// Retrieves the Unit interface object associated with a given identifier.
    ///
    /// @param unitID
    ///   The identifier for the Unit object.
    ///
    /// @returns Unit interface object mapped to the given \p unitID.
    /// @retval nullptr if the given identifier is invalid.
    virtual Unit* getUnit(int unitID) const = 0;

    /// Retrieves a Unit interface object from a given unit index. The value given as an index
    /// maps directly to Broodwar's unit array index and matches the index found in replay files.
    /// In order to use this function, CompleteMapInformation must be enabled.
    ///
    /// @param unitIndex
    ///   The unitIndex to identify the Unit with. A valid index is 0 <= unitIndex & 0x7FF < 1700.
    ///
    /// @returns Unit interface object that matches the given \p unitIndex.
    /// @retval nullptr if the given index is invalid.
    virtual Unit* indexToUnit(int unitIndex) const = 0;

    /// Retrieves the Region interface object associated with a given identifier.
    ///
    /// @param regionID
    ///   The identifier for the Region object.
    ///
    /// @returns Region interface object mapped to the given \p regionID.
    /// @retval nullptr if the given ID is invalid.
    virtual Region* getRegion(int regionID) const = 0;

    /// Retrieves the @GameType of the current game.
    ///
    /// @returns GameType indicating the rules of the match.
    /// @see GameType
    virtual GameType getGameType() const = 0;

    /// Retrieves the current latency setting that the game is set to. Latency indicates the delay
    /// between issuing a command and having it processed.
    ///
    /// @returns The latency setting of the game, which is of Latency::Enum.
    /// @todo Change return type to Latency::Enum without breaking everything.
    virtual int getLatency() const = 0;

    /// Retrieves the number of logical frames since the beginning of the match. If the game is
    /// paused, then getFrameCount will not increase.
    ///
    /// @returns Number of logical frames that have elapsed since the game started as an integer.
    virtual int getFrameCount() const = 0;

    /// Retrieves the maximum number of logical frames that have been recorded in a replay. If
    /// the game is not a replay, then the value returned is undefined.
    ///
    /// @returns The number of logical frames that the replay contains.
    virtual int getReplayFrameCount() const = 0;

    /// Retrieves the logical frame rate of the game in frames per second (FPS).
    ///
    /// Example:
    /// @code
    ///   BWAPI::Broodwar->setLocalSpeed(0);
    ///   
    ///   // Log and display the best logical FPS seen in the game
    ///   static int bestFPS = 0;
    ///   bestFPS = std::max(bestFPS, BWAPI::Broodwar->getFPS());
    ///   BWAPI::Broodwar->drawTextScreen(BWAPI::Positions::Origin, "%cBest: %d GFPS\nCurrent: %d GFPS", BWAPI::Text::White, bestFPS, BWAPI::Broodwar->getFPS());
    /// @endcode
    /// @returns Logical frames per second that the game is currently running at as an integer.
    /// @see getAverageFPS
    virtual int getFPS() const = 0;

    /// Retrieves the average logical frame rate of the game in frames per second (FPS).
    ///
    /// @returns Average logical frames per second that the game is currently running at as a
    /// double.
    /// @see getFPS
    virtual double getAverageFPS() const = 0;

    /// Retrieves the position of the user's mouse on the screen, in Position coordinates.
    ///
    /// @returns Position indicating the location of the mouse.
    /// @retval Positions::Unknown if Flag::UserInput is disabled.
    virtual Position getMousePosition() const = 0;

    /// Retrieves the state of the given mouse button.
    ///
    /// @param button
    ///   A MouseButton enum member indicating which button on the mouse to check.
    ///
    /// @return A bool indicating the state of the given \p button. true if the button was pressed
    /// and false if it was not.
    /// @retval false always if Flag::UserInput is disabled.
    virtual bool getMouseState(MouseButton button) const = 0;

    /// Retrieves the state of the given keyboard key.
    ///
    /// @param key
    ///   A Key enum member indicating which key on the keyboard to check.
    ///
    /// @return A bool indicating the state of the given \p key. true if the key was pressed
    /// and false if it was not.
    /// @retval false always if Flag::UserInput is disabled.
    virtual bool getKeyState(Key key) const = 0;

    /// Retrieves the top left position of the viewport from the top left corner of the map, in
    /// pixels.
    ///
    /// @returns Position containing the coordinates of the top left corner of the game's viewport.
    /// @retval Positions::Unknown always if Flag::UserInput is disabled.
    /// @see setScreenPosition
    virtual BWAPI::Position getScreenPosition() const = 0;

    /// Moves the top left corner of the viewport to the provided position relative to the map's
    /// origin (top left (0,0)).
    ///
    /// @param x
    ///   The x coordinate to move the screen to, in pixels.
    /// @param y
    ///   The y coordinate to move the screen to, in pixels.
    /// @see getScreenPosition
    virtual void setScreenPosition(int x, int y) = 0;
    /// @overload
    void setScreenPosition(BWAPI::Position p);

    /// Pings the minimap at the given position. Minimap pings are visible to allied players.
    ///
    /// @param x
    ///   The x coordinate to ping at, in pixels, from the map's origin (left).
    /// @param y
    ///   The y coordinate to ping at, in pixels, from the map's origin (top).
    virtual void pingMinimap(int x, int y) = 0;
    /// @overload
    void pingMinimap(BWAPI::Position p);

    /// Checks if the state of the given flag is enabled or not.
    ///
    /// @note Flags may only be enabled at the start of the match during the AIModule::onStart
    /// callback.
    ///
    /// @param flag
    ///   The Flag::Enum entry describing the flag's effects on BWAPI.
    /// @returns true if the given \p flag is enabled, false if the flag is disabled.
    /// @todo Take Flag::Enum as parameter instead of int
    virtual bool isFlagEnabled(int flag) const = 0;

    /// Enables the state of a given flag.
    ///
    /// @note Flags may only be enabled at the start of the match during the AIModule::onStart
    /// callback.
    ///
    /// @param flag
    ///   The Flag::Enum entry describing the flag's effects on BWAPI.
    /// @todo Take Flag::Enum as parameter instead of int
    virtual void enableFlag(int flag) = 0;

    /** Returns the set of accessible units that are on the given build tile. */
    Unitset getUnitsOnTile(int tileX, int tileY, const UnitFilter &pred = nullptr) const;
    /// @overload
    Unitset getUnitsOnTile(BWAPI::TilePosition tile, const UnitFilter &pred = nullptr) const;

    /** Returns the set of accessible units that are in or overlapping the given rectangle. */
    virtual Unitset getUnitsInRectangle(int left, int top, int right, int bottom, const UnitFilter &pred = nullptr) const = 0;
    /// @overload
    Unitset getUnitsInRectangle(BWAPI::Position topLeft, BWAPI::Position bottomRight, const UnitFilter &pred = nullptr) const;

    /** Returns the set of accessible units within or overlapping a circle at the given point with the given radius. */
    Unitset getUnitsInRadius(BWAPI::Position center, int radius, const UnitFilter &pred = nullptr) const;

    /// Retrieves the closest unit to center that matches the criteria of the callback pred within
    /// an optional radius.
    ///
    /// @param center
    ///   The position to start searching for the closest unit.
    /// @param pred (optional)
    ///   The UnitFilter predicate to determine which units should be included. This includes
    ///   all units by default.
    /// @param radius (optional)
    ///   The radius to search in. If omitted, the entire map will be searched.
    ///
    /// @returns The desired unit that is closest to center.
    /// @retval nullptr If a suitable unit was not found.
    ///
    /// @see getBestUnit, UnitFilter
    Unit *getClosestUnit(Position center, const UnitFilter &pred = nullptr, int radius = 999999) const;

    /// Retrieves the closest unit to center that matches the criteria of the callback pred within
    /// an optional rectangle.
    ///
    /// @param center
    ///   The position to start searching for the closest unit.
    /// @param pred (optional)
    ///   The UnitFilter predicate to determine which units should be included. This includes
    ///   all units by default.
    /// @param left (optional)
    ///   The left position of the rectangle. This value is 0 by default.
    /// @param top (optional)
    ///   The top position of the rectangle. This value is 0 by default.
    /// @param right (optional)
    ///   The right position of the rectangle. This value includes the entire map width by default.
    /// @param bottom (optional)
    ///   The bottom position of the rectangle. This value includes the entire map height by default.
    ///
    /// @see UnitFilter
    virtual Unit *getClosestUnitInRectangle(Position center, const UnitFilter &pred = nullptr, int left = 0, int top = 0, int right = 999999, int bottom = 999999) const = 0;

    /// Compares all units with pred to determine which of them is the best. All units are checked.
    /// If center and radius are specified, then it will check all units that are within the radius
    /// of the position.
    ///
    /// @param best
    ///   A BestUnitFilter that determines which parameters should be considered when calculating
    ///   which units are better than others.
    /// @param pred
    ///   A UnitFilter that determines which units to include in calculations.
    /// @param center (optional)
    ///   The position to use in the search. If omitted, then the entire map is searched.
    /// @param radius (optional)
    ///   The distance from \p center to search for units. If omitted, then the entire map is
    ///   searched.
    /// 
    /// @returns The desired unit that best matches the given criteria.
    /// @retval nullptr if a suitable unit was not found.
    ///
    /// @see getClosestUnit, BestUnitFilter, UnitFilter
    virtual Unit *getBestUnit(const BestUnitFilter &best, const UnitFilter &pred, Position center = Positions::Origin, int radius = 999999) const = 0;

    /// Returns the last error that was set using setLastError. If a function call in BWAPI has
    /// failed, you can use this function to retrieve the reason it failed.
    ///
    /// @returns Error type containing the reason for failure.
    ///
    /// @see setLastError, Errors
    virtual Error getLastError() const = 0;

    /// Sets the last error so that future calls to getLastError will return the value that was
    /// set.
    ///
    /// @param e (optional)
    ///   The error code to set. If omitted, then the last error will be cleared.
    ///
    /// @retval true If the type passed was Errors::None, clearing the last error.
    /// @retval false If any other error type was passed.
    /// @see getLastError, Errors
    virtual bool setLastError(BWAPI::Error e = Errors::None) const = 0;

    /// Retrieves the width of the map in build tile units.
    ///
    /// @returns Width of the map in tiles.
    /// @see mapHeight
    virtual int mapWidth() const = 0;

    /// Retrieves the height of the map in build tile units.
    ///
    /// @returns Height of the map in tiles.
    /// @see mapHeight
    virtual int mapHeight() const = 0;

    /// Retrieves the file name of the currently loaded map.
    /// @TODO: Note on campaign files.
    ///
    /// @returns Map file name as std::string object.
    ///
    /// @see mapPathName, mapName
    virtual std::string mapFileName() const = 0;

    /// Retrieves the full path name of the currently loaded map.
    /// @TODO: Note on campaign files.
    ///
    /// @returns Map file name as std::string object.
    ///
    /// @see mapFileName, mapName
    virtual std::string mapPathName() const = 0;

    /// Retrieves the title of the currently loaded map.
    ///
    /// @returns Map title as std::string object.
    ///
    /// @see mapFileName, mapPathName
    virtual std::string mapName() const = 0;

    /// Calculates the SHA-1 hash of the currently loaded map file.
    ///
    /// @returns std::string object containing SHA-1 hash.
    ///
    /// @note Campaign maps will return a hash of their internal map chunk components(.chk), while
    /// standard maps will return a hash of their entire map archive (.scm,.scx).
    /// @TODO: Note on replays.
    virtual std::string mapHash() const = 0;

    /** Returns true if the specified walk tile is walkable. The values of x and y are in walk tile
     * coordinates (different from build tile coordinates). Note that this just uses the static map data.
     * You will also need to make sure no ground units are on the coresponding build tile to see if its
     * currently walkable. To do this, see unitsOnTile. */
    virtual bool isWalkable(int walkX, int walkY) const = 0;
    /// @overload
    bool isWalkable(BWAPI::WalkPosition position) const;

    /// Returns the ground height at the given tile position.
    ///
    /// @param tileX
    ///   X position to query, in tiles
    /// @param tileY
    ///   Y position to query, in tiles
    ///
    /// @returns The tile height as an integer. Possible values are:
    ///     - 0: Low ground
    ///     - 1: Low ground doodad
    ///     - 2: High ground
    ///     - 3: High ground doodad
    ///     - 4: Very high ground
    ///     - 5: Very high ground doodad
    ///     .
    virtual int  getGroundHeight(int tileX, int tileY) const = 0;
    /// @overload
    int  getGroundHeight(TilePosition position) const;

    /** Returns true if the specified build tile is buildable. Note that this just uses the static map data.
     * You will also need to make sure no ground units on the tile to see if its currently buildable. To do
     * this, see unitsOnTile. */
    virtual bool isBuildable(int tileX, int tileY, bool includeBuildings = false) const = 0;
    /// @overload
    bool isBuildable(TilePosition position, bool includeBuildings = false) const;

    /** Returns true if the specified build tile is visible. If the tile is concealed by fog of war, the
     * function will return false. */
    virtual bool isVisible(int tileX, int tileY) const = 0;
    /// @overload
    bool isVisible(TilePosition position) const;

    /// Checks if a given tile position has been explored by the player. An explored tile position
    /// indicates that the player has seen the location at some point in the match, partially
    /// revealing the fog of war for the remainder of the match.
    ///
    /// @param tileX
    ///   The x tile coordinate to check.
    /// @param tileY
    ///   The y tile coordinate to check.
    /// 
    /// @retval true If the player has explored the given tile position (partially revealed fog).
    /// @retval false If the tile position was never explored (completely black fog).
    ///
    /// @see isVisible
    virtual bool isExplored(int tileX, int tileY) const = 0;
    /// @overload
    bool isExplored(TilePosition position) const;

    /// Checks if the given tile position has @Zerg creep on it.
    ///
    /// @param tileX
    ///   The x tile coordinate to check.
    /// @param tileY
    ///   The y tile coordinate to check.
    ///
    /// @retval true If the given tile has creep on it.
    /// @retval false If the given tile does not have creep, or if it is concealed by the fog of war.
    virtual bool hasCreep(int tileX, int tileY) const = 0;
    /// @overload
    bool hasCreep(TilePosition position) const;

    /// Checks if the given pixel position is powered by an owned @Protoss_Pylon for an optional
    /// unit type.
    ///
    /// @param x
    ///   The x pixel coordinate to check.
    /// @param y
    ///   The y pixel coordinate to check.
    /// @param unitType (optional)
    ///   Checks if the given UnitType requires power or not. If ommitted, then it will assume
    ///   that the position requires power for any unit type.
    ///
    /// @retval true if the type at the given position will have power.
    /// @retval false if the type at the given position will be unpowered.
    virtual bool hasPowerPrecise(int x, int y, UnitType unitType = UnitTypes::None ) const = 0;
    /// @overload
    bool hasPowerPrecise(Position position, UnitType unitType = UnitTypes::None) const;

    /// Checks if the given tile position if powered by an owned @Protoss_Pylon for an optional
    /// unit type.
    ///
    /// @param tileX
    ///   The x tile coordinate to check.
    /// @param tileY
    ///   The y tile coordinate to check.
    /// @param unitType (optional)
    ///   Checks if the given UnitType will be powered if placed at the given tile position. If
    ///   omitted, then only the immediate tile position is checked for power, and the function
    ///   will assume that the location requires power for any unit type.
    ///   
    /// @retval true if the type at the given tile position will receive power.
    /// @retval false if the type will be unpowered at the given tile position.
    bool hasPower(int tileX, int tileY, UnitType unitType = UnitTypes::None) const;
    /// @overload
    bool hasPower(TilePosition position, UnitType unitType = UnitTypes::None) const;
    /// @overload
    bool hasPower(int tileX, int tileY, int tileWidth, int tileHeight, UnitType unitType = UnitTypes::None) const;
    /// @overload
    bool hasPower(TilePosition position, int tileWidth, int tileHeight, UnitType unitType = UnitTypes::None) const;

    /** Returns true if the given unit type can be built at the given build tile position. Note the tile
     * position specifies the top left tile of the building. If builder is not null, the unit will be
     * discarded when determining whether or not any ground units are blocking the build location. */
    virtual bool canBuildHere(TilePosition position, UnitType type, const Unit *builder = nullptr, bool checkExplored = false) = 0;

    /** Returns true if the AI player has enough resources, supply, tech, and required units in order to
     * make the given unit type. If builder is not null, canMake will return true only if the builder unit
     * can build the given unit type. */
    virtual bool canMake(UnitType type, const Unit *builder = nullptr) const = 0;

    /** Returns true if the AI player has enough resources required to research the given tech type. If unit
     * is not null, canResearch will return true only if the given unit can research the given tech type. */
    virtual bool canResearch(TechType type, const Unit *unit = nullptr, bool checkCanIssueCommandType = true) = 0;

    /** Returns true if the AI player has enough resources required to upgrade the given upgrade type. If
     * unit is not null, canUpgrade will return true only if the given unit can upgrade the given upgrade
     * type. */
    virtual bool canUpgrade(UpgradeType type, const Unit *unit = nullptr, bool checkCanIssueCommandType = true) = 0;

    /** Returns the set of starting locations for the given map. To determine the starting location for the
     * players in the current match, see Player::getStartLocation. */
    virtual const TilePosition::set& getStartLocations() const = 0;

    /** Prints text on the screen. Text is not sent to other players in multiplayer games. */
    virtual void vPrintf(const char *format, va_list arg) = 0;
    ///
    void printf(const char *format, ...);

    /** Sends text to other players - as if it were entered in chat. In single player games and replays,
     * this will just print the text on the screen. If the game is a single player match and not a replay,
     * then this function can be used to execute cheat codes, i.e. Broodwar->sendText("show me the money"). */
    void sendText(const char *format, ...);
    ///
    void vSendText(const char *format, va_list arg);

    /// 
    void sendTextEx(bool toAllies, const char *format, ...);
    ///
    virtual void vSendTextEx(bool toAllies, const char *format, va_list arg) = 0;

    /** Used to change the race while in a lobby. Note that there is no onLobbyEnter callback yet, so this
     * function cannot be used at this time. */
    virtual void changeRace(Race race) = 0;

    /** Returns true if Broodwar is in a game. Returns false for lobby and menu screens */
    virtual bool isInGame() const = 0;

    /** Returns true if Broodwar is in a multiplayer game. Returns false for single player games and
     * replays. */
    virtual bool isMultiplayer() const = 0;

    /** Returns true if Broodwar is in a BNet multiplayer game.
    */
    virtual bool isBattleNet() const = 0;

    /** Returns true if Broodwar is paused. If the game is paused, Game::getFrameCount will continue to
     * increase and AIModule::onFrame will still be called while paused. */
    virtual bool isPaused() const = 0;

    /** Returns true if Broodwar is in a replay. */
    virtual bool isReplay() const = 0;

    /** Used to start the game while in a lobby. Note that there is no onLobbyEnter callback yet, so this
     * function cannot be used at this time. */
    virtual void startGame() = 0;

    /// Pauses the game. While paused, Game::getFrameCount will not increase, but AIModule::onFrame
    /// will still be called.
    /// @see resumeGame
    virtual void pauseGame() = 0;

    /// Resumes the game from a paused state.
    /// @see pauseGame
    virtual void resumeGame() = 0;

    /// Leaves the current game by surrendering and enters the post-game statistics/score screen.
    virtual void leaveGame() = 0;

    /// Restarts the match. Works the same as if the match was restarted from the in-game menu
    /// (F10). This option is only available in single player games.
    /// @todo return a bool indicating success, document error code for invalid state
    virtual void restartGame() = 0;

    /** Sets the speed of the game to the given number. Lower numbers are faster. 0 is the fastest speed
     * StarCraft can handle (which is about as fast as the fastest speed you can view a replay at). Any
     * negative value will reset the speed to the StarCraft default. */
    virtual void setLocalSpeed(int speed) = 0;

    /** Issues a command to a group of units */
    virtual bool issueCommand(const Unitset& units, UnitCommand command) = 0;

    /// Retrieves the set of units that are currently selected by the user outside of BWAPI. This
    /// function requires that Flag::UserInput be enabled.
    ///
    /// @returns A Unitset containing the user's selected units. If Flag::UserInput is disabled, 
    /// then this set is always empty.
    ///
    /// @see enableFlag
    virtual const Unitset& getSelectedUnits() const = 0;

    /// Retrieves the player object that BWAPI is controlling. 
    ///
    /// @returns Pointer to Player interface object representing the current player.
    /// @retval nullptr if the current game is a replay.
    ///
    /// Example usage
    /// @code
    ///   void ExampleAIModule::onStart()
    ///   {
    ///     BWAPI::Broodwar->sendText("Hello, my name is %s.", BWAPI::Broodwar->self()->getName().c_str());
    ///   }
    /// @endcode
    virtual Player* self() const = 0;

    /** Returns a pointer to the enemy player. If there is more than one enemy, this returns a pointer to
     * just one enemy (see enemies to get all enemies still in game). In replays this will
     * return nullptr. */
    virtual Player* enemy() const = 0;

    /// Retrieves a pointer to the Player interface object representing the neutral player. The
    /// neutral player owns all the resources and critters on the map by default.
    ///
    /// @returns Player interface indicating the neutral player.
    virtual Player* neutral() const = 0;

    /** Returns a set of all the ally players that have not left or been defeated. Does not include self. */
    virtual Playerset& allies() = 0;

    /** Returns a set of all the enemy players that have not left or been defeated. */
    virtual Playerset& enemies() = 0;

    /** Returns a set of all the observer players that have not left. */
    virtual Playerset& observers() = 0;

    /// Sets the size of the text for all calls to drawText following this one.
    ///
    /// @param size (optional)
    ///   The size of the text. This value can be from 0 to 3 inclusive. A higher value indicates
    ///   a larger text size. If this value is omitted or invalid, then it will use a default
    ///   value of 1.
    virtual void setTextSize(int size = 1) = 0;

    /// Draws text on the screen at the given coordinates. Text can be drawn in different colors
    /// or formatted using the Text::Enum members.
    ///
    /// @param ctype
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// @param x
    ///   The x coordinate, in pixels, relative to ctype.
    /// @param y
    ///   The y coordinate, in pixels, relative to ctype.
    /// @param format
    ///   The string formatting portion. This is the same as printf style formatting.
    /// @param arg
    ///   Arglist containing the intermediate list of arguments to format before finally sending
    ///   the string to Broodwar.
    virtual void vDrawText(CoordinateType::Enum ctype, int x, int y, const char *format, va_list arg) = 0;
    /// @overload
    void drawText(CoordinateType::Enum ctype, int x, int y, const char *format, ...);
    /// @overload
    void drawTextMap(int x, int y, const char *format, ...);
    /// @overload
    void drawTextMap(Position p, const char *format, ...);
    /// @overload
    void drawTextMouse(int x, int y, const char *format, ...);
    /// @overload
    void drawTextMouse(Position p, const char *format, ...);
    /// @overload
    void drawTextScreen(int x, int y, const char *format, ...);
    /// @overload
    void drawTextScreen(Position p, const char *format, ...);

    /// Draws a rectangle on the screen with the given color.
    ///
    /// @param ctype
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// @param left
    ///   The x coordinate, in pixels, relative to ctype, of the left edge of the rectangle.
    /// @param top
    ///   The y coordinate, in pixels, relative to ctype, of the top edge of the rectangle.
    /// @param right
    ///   The x coordinate, in pixels, relative to ctype, of the right edge of the rectangle.
    /// @param bottom
    ///   The y coordinate, in pixels, relative to ctype, of the bottom edge of the rectangle.
    /// @param color
    ///   The color of the rectangle.
    /// @param isSolid (optional)
    ///   If true, then the shape will be filled and drawn as a solid, otherwise it will be drawn
    ///   as an outline. If omitted, this value will default to false.
    virtual void drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;
    /// @overload
    void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxMap(Position leftTop, Position rightBottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxMouse(Position leftTop, Position rightBottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    /// @overload
    void drawBoxScreen(Position leftTop, Position rightBottom, Color color, bool isSolid = false);

    /// Draws a triangle on the screen with the given color.
    ///
    /// @param ctype
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// @param ax
    ///   The x coordinate, in pixels, relative to ctype, of the first point.
    /// @param ay
    ///   The y coordinate, in pixels, relative to ctype, of the first point.
    /// @param bx
    ///   The x coordinate, in pixels, relative to ctype, of the second point.
    /// @param by
    ///   The y coordinate, in pixels, relative to ctype, of the second point.
    /// @param cx
    ///   The x coordinate, in pixels, relative to ctype, of the third point.
    /// @param cy
    ///   The y coordinate, in pixels, relative to ctype, of the third point.
    /// @param color
    ///   The color of the triangle.
    /// @param isSolid (optional)
    ///   If true, then the shape will be filled and drawn as a solid, otherwise it will be drawn
    ///   as an outline. If omitted, this value will default to false.
    virtual void drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;
    /// @overload
    void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleMap(Position a, Position b, Position c, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleMouse(Position a, Position b, Position c, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    /// @overload
    void drawTriangleScreen(Position a, Position b, Position c, Color color, bool isSolid = false);

    /// Draws a circle on the screen with the given color.
    ///
    /// @param ctype
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// @param x
    ///   The x coordinate, in pixels, relative to ctype.
    /// @param y
    ///   The y coordinate, in pixels, relative to ctype.
    /// @param radius
    ///   The radius of the circle, in pixels.
    /// @param color
    ///   The color of the circle.
    /// @param isSolid (optional)
    ///   If true, then the shape will be filled and drawn as a solid, otherwise it will be drawn
    ///   as an outline. If omitted, this value will default to false.
    virtual void drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid = false) = 0;
    /// @overload
    void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleMap(Position p, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleMouse(Position p, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);
    /// @overload
    void drawCircleScreen(Position p, int radius, Color color, bool isSolid = false);

    /// Draws an ellipse on the screen with the given color.
    ///
    /// @param ctype
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// @param x
    ///   The x coordinate, in pixels, relative to ctype.
    /// @param y
    ///   The y coordinate, in pixels, relative to ctype.
    /// @param xrad
    ///   The x radius of the ellipse, in pixels.
    /// @param yrad
    ///   The y radius of the ellipse, in pixels.
    /// @param color
    ///   The color of the ellipse.
    /// @param isSolid (optional)
    ///   If true, then the shape will be filled and drawn as a solid, otherwise it will be drawn
    ///   as an outline. If omitted, this value will default to false.
    virtual void drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;
    /// @overload
    void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseMap(Position p, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseMouse(Position p, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    /// @overload
    void drawEllipseScreen(Position p, int xrad, int yrad, Color color, bool isSolid = false);

    /// Draws a dot on the map or screen with a given color.
    ///
    /// @param ctype
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// @param x
    ///   The x coordinate, in pixels, relative to ctype.
    /// @param y
    ///   The y coordinate, in pixels, relative to ctype.
    /// @param color
    ///   The color of the dot.
    virtual void drawDot(CoordinateType::Enum ctype, int x, int y, Color color) = 0;
    /// @overload
    void drawDotMap(int x, int y, Color color);
    /// @overload
    void drawDotMap(Position p, Color color);
    /// @overload
    void drawDotMouse(int x, int y, Color color);
    /// @overload
    void drawDotMouse(Position p, Color color);
    /// @overload
    void drawDotScreen(int x, int y, Color color);
    /// @overload
    void drawDotScreen(Position p, Color color);

    /// Draws a line on the map or screen with a given color.
    ///
    /// @param ctype
    ///   The coordinate type. Indicates the relative position to draw the shape.
    /// @param x1
    ///   The starting x coordinate, in pixels, relative to ctype.
    /// @param y1
    ///   The starting y coordinate, in pixels, relative to ctype.
    /// @param x2
    ///   The ending x coordinate, in pixels, relative to ctype.
    /// @param y2
    ///   The ending y coordinate, in pixels, relative to ctype.
    /// @param color
    ///   The color of the line.
    virtual void drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color) = 0;
    /// @overload
    void drawLineMap(int x1, int y1, int x2, int y2, Color color);
    /// @overload
    void drawLineMap(Position a, Position b, Color color);
    /// @overload
    void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
    /// @overload
    void drawLineMouse(Position a, Position b, Color color);
    /// @overload
    void drawLineScreen(int x1, int y1, int x2, int y2, Color color);
    /// @overload
    void drawLineScreen(Position a, Position b, Color color);

    /// Retrieves the maximum delay, in number of frames, between a command being issued and the
    /// command being executed by Broodwar.
    ///
    /// @note In Broodwar, latency is used to keep the game synchronized between players without
    /// introducing lag.
    ///
    /// @returns Difference in frames between commands being sent and executed.
    /// @see getLatencyTime, getRemainingLatencyFrames
    virtual int getLatencyFrames() const = 0;

    /// Retrieves the maximum delay, in milliseconds, between a command being issued and the
    /// command being executed by Broodwar.
    ///
    /// @returns Difference in milliseconds between commands being sent and executed.
    /// @see getLatencyFrames, getRemainingLatencyTime
    virtual int getLatencyTime() const = 0;

    /// Retrieves the number of frames it will take before a command sent in the current frame
    /// will be executed by the game.
    ///
    /// @returns Number of frames until a command is executed if it were sent in the current
    /// frame.
    /// @see getRemainingLatencyTime, getLatencyFrames
    virtual int getRemainingLatencyFrames() const = 0;
    
    /// Retrieves the number of milliseconds it will take before a command sent in the current
    /// frame will be executed by Broodwar.
    ///
    /// @returns Amount of time, in milliseconds, until a command is executed if it were sent in
    /// the current frame.
    virtual int getRemainingLatencyTime() const = 0;

    /// Retrieves the current revision of BWAPI.
    ///
    /// @returns The revision number of the current BWAPI interface.
    ///
    /// @threadsafe
    virtual int getRevision() const = 0;

    /// Retrieves the debug state of the BWAPI build.
    ///
    /// @returns true if the BWAPI module is a DEBUG build, and false if it is a RELEASE build.
    ///
    /// @threadsafe
    virtual bool isDebug() const = 0;

    /// Checks the state of latency compensation.
    ///
    /// @returns true if latency compensation is enabled, false if it is disabled.
    /// @see setLatCom
    virtual bool isLatComEnabled() const = 0;

    /// Changes the state of latency compensation. Latency compensation modifies the state of
    /// BWAPI's representation of units to reflect the implications of issuing a command
    /// immediately after the command was performed, instead of waiting consecutive frames for the
    /// results. Latency compensation is enabled by default.
    ///
    /// @param isEnabled
    ///   Set whether the latency compensation feature will be enabled (true) or disabled (false).
    ///
    /// @see isLatComEnabled.
    virtual void setLatCom(bool isEnabled) = 0;

    /// Checks if the GUI is enabled. The GUI includes all drawing functions of BWAPI, as well
    /// as screen updates from Broodwar.
    ///
    /// @retval true If the GUI is enabled, and everything is visible
    /// @retval false If the GUI is disabled and drawing functions are rejected
    ///
    /// @see setGUI
    virtual bool isGUIEnabled() const = 0;

    /// Sets the rendering state of the Starcraft GUI. This typically gives Starcraft a very
    /// low graphical frame rate and disables all drawing functionality in BWAPI.
    ///
    /// @param enabled
    ///   A boolean value that determines the state of the GUI. Passing false to this function
    ///   will disable the GUI, and true will enable it.
    ///
    /// Example Usage:
    /// @code
    ///   void ExampleAIModule::onStart()
    ///   {   // Make our bot run thousands of games as fast as possible!
    ///     Broodwar->setLocalSpeed(0);
    ///     Broodwar->setGUI(false);
    ///   }
    /// @endcode
    ///
    /// @see isGUIEnabled
    virtual void setGUI(bool enabled) = 0;

    /// Retrieves the Starcraft instance number recorded by BWAPI to identify which Starcraft
    /// instance an AI module belongs to. This only applies to users running multiple instances
    /// of Starcraft.
    ///
    /// @returns
    ///   An integer value representing the instance number.
    ///
    /// @threadsafe
    virtual int getInstanceNumber() const = 0;

    /// Retrieves the Actions Per Minute (APM) that the bot is producing.
    ///
    /// @param includeSelects (optional)
    ///   If true, the return value will include selections as individual commands, otherwise
    ///   it will exclude selections. This value is false by default.
    ///
    /// @returns The number of actions that the bot has executed per minute, on average.
    virtual int getAPM(bool includeSelects = false) const = 0;

    /// Changes the map to the one specified. Once restarted, the game will load the map that was
    /// provided. Changes do not take effect unless the game is restarted.
    ///
    /// @param mapFileName
    ///   A string containing the path and file name to the desired map.
    ///
    /// @retval true if the function succeeded and has changed the map.
    /// @retval false if the function failed, does not have permission from the tournament module,
    ///               failed to find the map specified, or received an invalid parameter.
    virtual bool setMap(const char *mapFileName) = 0;
    /// @overload
    bool setMap(const std::string &mapFileName);

    /// Sets the number of graphical frames for every logical frame. This allows the game to run
    /// more logical frames per graphical frame, increasing the speed at which the game runs.
    ///
    /// @param frameSkip
    ///   Number of graphical frames per logical frame. If this value is 0 or less, then it will
    ///   default to 1.
    virtual void setFrameSkip(int frameSkip) = 0;

    /// Checks if there is a path from source to destination. This only checks if the source
    /// position is connected to the destination position. This function does not check if all 
    /// units can actually travel from source to destination. Because of this limitation, it has
    /// an O(1) complexity, and cases where this limitation hinders gameplay is uncommon at best.
    /// 
    /// @param source
    ///   The source position.
    /// @param destination
    ///   The destination position.
    ///
    /// @retval true if there is a path between the two positions
    /// @retval false if there is no path
    virtual bool hasPath(Position source, Position destination) const = 0;

    /// Sets the alliance state of the current player with the target player.
    ///
    /// @param player
    ///   The target player to set alliance with.
    /// @param allied (optional)
    ///   If true, the current player will ally the target player. If false, the current player
    ///   will make the target player an enemy. This value is true by default.
    /// @param alliedVictory (optional)
    ///   Sets the state of "allied victory". If true, the game will end in a victory if all
    ///   allied players have eliminated their opponents. Otherwise, the game will only end if
    ///   no other players are remaining in the game. This value is true by default.
    virtual bool setAlliance(BWAPI::Player *player, bool allied = true, bool alliedVictory = true) = 0;

    /// In a game, this function sets the vision of the current BWAPI player with the target
    /// player. In a replay, this function toggles the visibility of the target player.
    ///
    /// @param player
    ///   The target player to toggle vision.
    /// @param enabled (optional)
    ///   The vision state. If true, and in a game, the current player will enable shared vision
    ///   with the target player, otherwise it will unshare vision. If in a replay, the vision
    ///   of the target player will be shown, otherwise the target player will be hidden. This
    ///   value is true by default.
    virtual bool setVision(BWAPI::Player *player, bool enabled = true) = 0;

    /// Retrieves current amount of time in seconds that the game has elapsed.
    ///
    /// @returns Time, in seconds, that the game has elapsed as an integer.
    virtual int  elapsedTime() const = 0;

    /// Sets the command optimization level. Command optimization is a feature in BWAPI that tries
    /// to reduce the APM of the bot by grouping or eliminating unnecessary game actions. For
    /// example, suppose the bot told 24 @Zerglings to @Burrow. At command optimization level 0,
    /// BWAPI is designed to select each Zergling to burrow individually, which costs 48 actions.
    /// With command optimization level 1, it can perform the same behaviour using only 4 actions.
    /// The command optimizer also reduces the amount of bytes used for each action if it can
    /// express the same action using a different command. For example, Right_Click uses less
    /// bytes than Move.
    ///
    /// @param level
    ///   An integer representation of the aggressiveness for which commands are optimized. A
    ///   lower level means less optimization, and a higher level means more optimization. The
    ///   values for level are as follows:
    ///     - 0: No optimization.
    ///     - 1: Some optimization.
    ///       - Is not detected as a hack.
    ///       - Does not alter behaviour.
    ///       - Units performing the following actions are grouped and ordered 12 at a time:
    ///         - Attack_Unit
    ///         - Morph (@Larva only)
    ///         - Hold_Position
    ///         - Stop
    ///         - Follow
    ///         - Gather
    ///         - Return_Cargo
    ///         - Repair
    ///         - Burrow
    ///         - Unburrow
    ///         - Cloak
    ///         - Decloak
    ///         - Siege
    ///         - Unsiege
    ///         - Right_Click_Unit
    ///         - Halt_Construction
    ///         - Cancel_Train (@Carrier and @Reaver only)
    ///         - Cancel_Train_Slot (@Carrier and @Reaver only)
    ///         - Cancel_Morph (for non-buildings only)
    ///         - Use_Tech
    ///         - Use_Tech_Unit
    ///         .
    ///       - The following order transformations are applied to allow better grouping:
    ///         - Attack_Unit becomes Right_Click_Unit if the target is an enemy
    ///         - Move becomes Right_Click_Position
    ///         - Gather becomes Right_Click_Unit if the target contains resources
    ///         - Set_Rally_Position becomes Right_Click_Position for buildings
    ///         - Set_Rally_Unit becomes Right_Click_Unit for buildings
    ///         - Use_Tech_Unit with Infestation becomes Right_Click_Unit if the target is valid
    ///         .
    ///       .
    ///     - 2: More optimization by grouping structures.
    ///       - Includes the optimizations made by all previous levels.
    ///       - May be detected as a hack by some replay utilities.
    ///       - Does not alter behaviour.
    ///       - Units performing the following actions are grouped and ordered 12 at a time:
    ///         - Attack_Unit (@Turrets, @Photon_Cannons, @Sunkens, @Spores)
    ///         - Train
    ///         - Morph
    ///         - Set_Rally_Unit
    ///         - Lift
    ///         - Cancel_Construction
    ///         - Cancel_Addon
    ///         - Cancel_Train
    ///         - Cancel_Train_Slot
    ///         - Cancel_Morph
    ///         - Cancel_Research
    ///         - Cancel_Upgrade
    ///         .
    ///       .
    ///     - 3: Extensive optimization 
    ///       - Includes the optimizations made by all previous levels.
    ///       - Units may behave or move differently than expected.
    ///       - Units performing the following actions are grouped and ordered 12 at a time:
    ///         - Attack_Move
    ///         - Set_Rally_Position
    ///         - Move
    ///         - Patrol
    ///         - Unload_All
    ///         - Unload_All_Position
    ///         - Right_Click_Position
    ///         - Use_Tech_Position
    ///         .
    ///       .
    ///     - 4: Aggressive optimization
    ///       - Includes the optimizations made by all previous levels.
    ///       - Positions used in commands will be rounded to multiples of 32.
    ///       - @High_Templar and @Dark_Templar that merge into @Archons will be grouped and may
    ///         choose a different target to merge with. It will not merge with a target that
    ///         wasn't included.
    ///       .
    ///     .
    ///
    virtual void setCommandOptimizationLevel(int level) = 0;

    /// Returns the remaining countdown time. The countdown timer is used in @CTF and @UMS game
    /// types.
    ///
    /// @code
    ///   void ExampleAIModule::onStart()
    ///   {
    ///     if ( Broodwar->getGameType() == GameTypes::Capture_The_Flag || Broodwar->getGameType() == GameTypes::Team_Capture_The_Flag )
    ///     {
    ///       Broodwar->registerEvent([](Game*){ Broodwar->sendText("Try to find my flag!"); },   // action
    ///                               [](Game*){ return Broodwar->countdownTimer() == 0; },       // condition
    ///                               1);                                                         // times to run (once)
    ///     }
    ///   }
    /// @endcode
    ///
    /// @returns Integer containing the time (in game seconds) on the countdown timer.
    virtual int countdownTimer() const = 0;

    /// Retrieves the set of all regions on the map.
    ///
    /// @returns Regionset containing all map regions.
    virtual const Regionset &getAllRegions() const = 0;

    /// Retrieves the region at a given position.
    ///
    /// @param x
    ///   The x coordinate, in pixels.
    /// @param y
    ///   The y coordinate, in pixels.
    ///
    /// @returns Pointer to the Region interface at the given position.
    virtual BWAPI::Region *getRegionAt(int x, int y) const = 0;
    /// @overload
    BWAPI::Region *getRegionAt(BWAPI::Position position) const;

    /// Retrieves the amount of time (in milliseconds) that has elapsed when running the last AI
    /// module callback. This is used by tournament modules to penalize AI modules that use too
    /// much processing time.
    ///
    /// @retval 0 When called from an AI module.
    /// @returns Time in milliseconds spent in last AI module call.
    virtual int getLastEventTime() const = 0;

    /// Sets the state of the fog of war when watching a replay.
    ///
    /// @param reveal (optional)
    ///   The state of the reveal all flag. If false, all fog of war will be enabled. If true,
    ///   then the fog of war will be revealed. It is true by default.
    virtual bool setRevealAll(bool reveal = true) = 0;

    /// Retrieves a basic build position just as the default Computer AI would. This allows users
    /// to find simple build locations without relying on external libraries.
    ///
    /// @param type
    ///   A valid UnitType representing the unit type to accomodate space for.
    /// @param desiredPosition
    ///   A valid TilePosition containing the desired placement position.
    /// @param maxRange (optional)
    ///   The maximum distance (in tiles) to build from \p desiredPosition.
    /// @param creep (optional)
    ///   A special boolean value that changes the behaviour of @Creep_Colony placement.
    ///
    /// @retval TilePositions::Invalid If a build location could not be found within \p maxRange.
    /// @returns
    ///   A TilePosition containing the location that the structure should be constructed at.
    TilePosition getBuildLocation(UnitType type, TilePosition desiredPosition, int maxRange = 64, bool creep = false) const;

    /// Calculates the damage received for a given player. It can be understood as the damage from
    /// \p fromType to \p toType. Does not include shields in calculation. Includes upgrades if
    /// players are provided.
    ///
    /// @param fromType
    ///   The unit type that will be dealing the damage.
    /// @param toType
    ///   The unit type that will be receiving the damage.
    /// @param fromPlayer (optional)
    ///   The player owner of the given type that will be dealing the damage. If omitted, then
    ///   no player will be used to calculate the upgrades for \p fromType.
    /// @param toPlayer (optional)
    ///   The player owner of the type that will be receiving the damage. If omitted, then this
    ///   parameter will default to Broodwar->self().
    ///
    /// @returns The amount of damage that fromType would deal to toType.
    /// @see getDamageTo
    int getDamageFrom(UnitType fromType, UnitType toType, Player *fromPlayer = nullptr, Player *toPlayer = nullptr) const;

    /// Calculates the damage dealt for a given player. It can be understood as the damage to
    /// \p toType from \p fromType. Does not include shields in calculation. Includes upgrades if
    /// players are provided.
    ///
    /// @note This function is nearly the same as #getDamageFrom. The only difference is that
    /// the last parameter is intended to default to Broodwar->self().
    ///
    /// @param toType
    ///   The unit type that will be receiving the damage.
    /// @param fromType
    ///   The unit type that will be dealing the damage.
    /// @param toPlayer (optional)
    ///   The player owner of the type that will be receiving the damage. If omitted, then
    ///   no player will be used to calculate the upgrades for \p toType.
    /// @param fromPlayer (optional)
    ///   The player owner of the given type that will be dealing the damage. If omitted, then
    ///   this parameter will default to Broodwar->self().
    ///
    /// @returns The amount of damage that fromType would deal to toType.
    /// @see getDamageFrom
    int getDamageTo(UnitType toType, UnitType fromType, Player *toPlayer = nullptr, Player *fromPlayer = nullptr) const;
  };

  extern Game *BroodwarPtr;

  /// Broodwar wrapper
  class GameWrapper
  {
  private:
    std::ostringstream ss;
  public:
    /// definition of ostream_manipulator type for convenience
    typedef std::ostream& (*ostream_manipulator)(std::ostream&);

    /// Member access operator to retain the original Broodwar-> behaviour.
    Game *operator ->() const;

    /// Output stream operator for printing text to Broodwar. Using this operator invokes
    /// Game::printf when a newline character is encountered.
    template < class T >
    GameWrapper &operator <<(const T &in)
    {
      // Pass whatever into the stream
      ss << in;
      return *this;
    };
    /// @overload
    GameWrapper &operator <<( const ostream_manipulator &fn );    
    
    /// Flushes the Broodwar stream, printing all text in the stream to the screen.
    void flush();
  };

  extern GameWrapper Broodwar;

}

