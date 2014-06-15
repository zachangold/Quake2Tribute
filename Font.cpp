//---------------------------------------------------------------------------

#pragma hdrstop

#include "Font.h"


namespace D3D {

    /**
     * init() method creates a font with Direct3D. It also takes the screen's
     * width and height in case the font is to be drawn at the bottom or right
     * sides of the screen (This is decided in the drawFlags parameter of
     * render()). After this is called, the font can be used to draw text
     * to the screen with the setText() and render() methods.
     *
     * device: a link to the Direct3D device object. The call to instantiate
     *      the Direct3D font requires this link to the device in order
     *      to operate properly.
     * screenWidth, screenHeight: These parameters declare the screen size.
     *      If the font is used at the bottom or right sides of the screen,
     *
     */
    void Font::init( LPDIRECT3DDEVICE9 device, int screenWidth, int screenHeight ) {
        HRESULT hr;

        // Create the font
        hr = D3DXCreateFont( device,     //D3D Device
                             LINE_HEIGHT,               //Font height
                             0,                //Font width
                             FW_NORMAL,        //Font Weight
                             1,                //MipLevels
                             false,            //Italic
                             DEFAULT_CHARSET,  //CharSet
                             OUT_DEFAULT_PRECIS, //OutputPrecision
                             ANTIALIASED_QUALITY, //Quality
                             DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
                             "Courier New",          //pFacename,
                             &font );         //ppFont

        // remember the screen width and height in case this font is used to draw
        // at the bottom or right sides of the screen
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

    };

    
    /**
     * This method controls the text string to be used in the drawing code.
     * So, for example, if you wanted to draw the strings "A" and "B", you
     * would call setText( "A" ) first, then render(). This draws the first
     * string. After that, you would call setText( "B" ), then render() again.
     * This would draw both text strings seperately. This functionality is
     * used by the UI classes (Console, MapSelector, and DrawingInfo) to
     * render their lines properly.
     */
    void Font::setText( string newText ) {
        text = newText;
    };


    /**
     * This method renders the text from the field "text". The "text" field
     * is set in the setText() method.
     * device: A link to the Direct3D device to draw to.
     * color: The colour of the text to be drawn. This field is set by the
     *      return value of the D3DXCOLOR() function.
     * lineNum: The offset (in lines) of this line from the "start" line.
     *      The start line is at the bottom of the screen if the DT_BOTTOM
     *      bit is on in parameter drawFlags. Otherwise, the start line
     *      is at the top of the screen.
     * drawFlags: A 32-bit-long piece of data specifying how the text is
     *      to be drawn. For example, one might use drawFlags |= DT_BOTTOM
     *      to specify that the text should be drawn at the bottom of the
     *      screen.
     */
    void Font::render( LPDIRECT3DDEVICE9 device, unsigned int color, int lineNum, unsigned int drawFlags ) {
        RECT font_rect;

        if ( drawFlags & DT_BOTTOM ) {
            // Draw from the bottom of the screen
            SetRect( &font_rect, 0, 0, screenWidth, screenHeight - lineNum * LINE_HEIGHT );
        } else {
            // Draw from the top of the screen
            SetRect( &font_rect, 0, lineNum * LINE_HEIGHT, screenWidth, screenHeight );
        }

        // render the text
        font->DrawText( NULL,        //pSprite
                        text.c_str(),  //pString
                        -1,          //Count
                        &font_rect,  //pRect
                        drawFlags,//Format,
                        color ); //Color

    };

};


//---------------------------------------------------------------------------
#pragma package(smart_init)
