//---------------------------------------------------------------------------

#ifndef ConsoleLineH
#define ConsoleLineH

#include <iostream.h>
#include "Timer.h"

/**
 * The ConsoleLine object creates an interface for a single line of text
 * used in the UI classes (MapSelector, Console, and )
 * 
 */
class ConsoleLine {
    public:
        /**
         * Constructor that sets the fields in this object to their default values,
         * preparing the object for use.
         */
        ConsoleLine();

        /**
         * Constructor that sets the fields in this object to the parameters
         * given by the owning object.
         */
        ConsoleLine( string text, unsigned int color, Timer *time );

        /**
         * This method simply sets the fields in this object to the parameters
         * in this method call.
         */
        void setData( string text, unsigned int color, Timer *time );

        /**
         * This method sets the text field to the newText parameter.
         */
        void setText( string newText );

        /**
         * Returns field "text"
         */
        string getText();

        /**
         * Returns the line's colour for rendering with a D3D::Font.
         */
        unsigned int getColor();

        /**
         * Gets the alpha channel of this line's colour. This is generally used
         * in the Console object, where lines that are entered in the console
         * slowly disappear. This method is used to detect when they disappear,
         * so they can be properly deleted.
         */
        int getAlpha( Timer *time );

        /**
         * This method is used to change the alpha channel based on how long ago
         * this line was created. This functionality is used by the Console object,
         * where lines slowly disappear after they are entered.
         */
        void update( Timer *time );


    private:
        // The colour of the line
        unsigned int color;

        // The text within the line
        string text;

        // The time (in milliseconds) that this line was created (for disappearing text)
        unsigned int timeCreated;

};



//---------------------------------------------------------------------------
#endif
