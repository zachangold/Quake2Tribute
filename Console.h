//---------------------------------------------------------------------------

#ifndef ConsoleH
#define ConsoleH

#include "BSPMap.h"
#include "D3DContext.h"
#include "Timer.h"


#include "ConsoleLine.h"

/**
 * The user can interact with the game engine in a few ways, one of which being
 * the game console. The console acts like a console in DOS, except this console
 * sends messages to the game engine, and the game engine can send messages back
 * to the console. For now, the only commands the user can give to the console are
 * "map <mapname>", which loads the map with file name the same as <mapname>, and
 * "showmaps", which lists all of the valid map names. The console takes care of
 * parsing those commands, and then printing console messages to the screen.
 */
class Console {
    public:

        /**
         * Constructor that sets all of the Console's values to their defaults,
         * preparing that console for later use.
         */
        Console();

        /**
         * Destructor that deletes any memory allocated by the Console object
         */
        ~Console();

        /**
         * init() prepares the Console for rendering and handling input.
         * d3dContext: The Direct3D Context object used by the Engine.
         */
        void init( D3DContext *d3dContext, int screenWidth, int screenHeight );

        /**
         * render() method draws the console to the screen. It also takes care of
         * making the Console lines fade out and disappear, de-allocating them
         * when they become invisible.
         */
        void render();

        /**
         * clearScreen() simply deletes all of the console's lines, "cleaning" the
         * console part of the screen.
         */
        void clearScreen();

        /**
         * printMessage() adds a line to the end of the lines array, with parameter
         * message as its text, coloured with parameter color.
         */
        void printMessage( string message, unsigned int color );

        /**
         * Appends parameter c to the end of the input string. This is used for
         * when the console has focus and the user presses a key - the letter that
         * the user input appears at the end of the input string.
         */
        void addInputChar( char c );

        /**
         * deleteChar() removes the last character of the input string. This is
         * used when the user presses the backspace key.
         */
        void deleteChar();

        /**
         * executeInputCommand() executes the input string as a command - this is
         * done when the user presses the Enter key.
         * The return value is the type of command that the user input. For now,
         * this can only be a new map, the command to show all maps, or an unknown
         * command, and the engine can respond accordingly. When the "map<mapname>"
         * command is used, the map name can be accessed by calling getMapName().
         */
        int executeInputCommand();

        /**
         * loadMap() creates a BSP map with file name the same as parameter fName.
         * The BSP Map reports its loading progress to the Console while it is loading.
         * Parameter camera's position is set to the spawning point in the map.
         */
        BSPMap *loadMap( string fName, Camera *camera );

        /**
         * unloadMap() unloads parameter map, and the map reports its unloading
         * progress to the console while it is unloading.
         */
        void unloadMap( BSPMap *map );

        /**
         * Returns the map name from the last console command. This is called
         * when the COMMAND_NEWMAP value is returned from the previous call to
         * executeInputCommand().
         */
        char *getMapName();


        /**
         * This boolean is set to true if the console receives keyboard input,
         * false otherwise.
         */
        bool hasFocus;


        // The command from the user was not a valid command
        static const int COMMAND_UNKNOWN = 0;

        // The command from the user follows "map <mapname>"
        static const int COMMAND_NEWMAP = 1;

        // The command from the user was "showmaps"
        static const int COMMAND_SHOWMAPS = 2;

        // The maximum number of lines the console can contain.
        static const int MAX_CONSOLE_LINES = 40;

    private:

        // The output lines of the console
        vector< ConsoleLine * > lines;

        // The line that is being input by the user only draws if hasFocus == true
        string inputLine;

        // The Direct3D font
        D3D::Font *font;

        // The Direct3D Context object used by the Engine
        D3DContext *d3dContext;

        // The dimensions of the screen
        int screenWidth, screenHeight;

        // The timer object for console lines that gradually fade away and disappear
        Timer time;

        // an unsigned integer for the "flickering" effect of the underscore (_)
        //  at the end of the input line.
        unsigned int textFlicker;

};






//---------------------------------------------------------------------------
#endif
 