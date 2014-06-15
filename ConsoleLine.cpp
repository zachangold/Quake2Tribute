//---------------------------------------------------------------------------

#pragma hdrstop

#include "ConsoleLine.h"


/**
 * Constructor that sets the fields in this object to their default values,
 * preparing the object for use.
 */
ConsoleLine::ConsoleLine() {
    color = 0;
    timeCreated = 0;
    text = "";
};


/**
 * Constructor that sets the fields in this object to the parameters
 * given by the owning object.
 */
ConsoleLine::ConsoleLine( string text, unsigned int color, Timer *time ) {
    this->text = text;
    this->color = color;

    if ( time != NULL ) {
        timeCreated = time->getTimeMillis();
    } else {
        timeCreated = 0;
    }
};


/**
 * This method simply sets the fields in this object to the parameters
 * in this method call.
 */
void ConsoleLine::setData( string text, unsigned int color, Timer *time ) {
    this->text = text;
    this->color = color;
    if ( time != NULL ) {
        timeCreated = time->getTimeMillis();
    } else {
        timeCreated = 0;
    }
};


/**
 * This method sets the text field to the newText parameter.
 */
void ConsoleLine::setText( string newText ) {
    text = newText;
};


/**
 * Returns field "text"
 */
string ConsoleLine::getText() {
    return text;
};


/**
 * Returns the line's colour for rendering with a D3D::Font.
 */
unsigned int ConsoleLine::getColor() {
    return color;
};


/**
 * Gets the alpha channel of this line's colour. This is generally used
 * in the Console object, where lines that are entered in the console
 * slowly disappear. This method is used to detect when they disappear,
 * so they can be properly deleted.
 */
int ConsoleLine::getAlpha( Timer *time ) {
    unsigned int msPassed = time->getTimeMillis() - timeCreated;
    return 2.0 - ( ( float ) msPassed ) / 2000.0;
};


/**
 * This method is used to change the alpha channel based on how long ago
 * this line was created. This functionality is used by the Console object,
 * where lines slowly disappear after they are entered.
 */
void ConsoleLine::update( Timer *time ) {
    // Start by turning off all of the alpha bits:
    color &= ( ~( 255 << 24 ) );

    // calculate the alpha channel of the colour.
    unsigned int msPassed = time->getTimeMillis() - timeCreated;
    float alpha = 4.0 - ( ( float ) msPassed ) / 1500.0;

    // make sure the alpha channel is within 0.0 to 1.0
    if ( alpha < 0.0 ) {
        alpha = 0.0;
    } else if ( alpha > 1.0 ) {
        alpha = 1.0;
    }

    // modify the colour so the alpha channel is correct
    unsigned int cAlpha = ( ( unsigned char ) ( alpha * 255.0 ) ) << 24;
    color |= cAlpha;
};



//---------------------------------------------------------------------------
#pragma package(smart_init)
