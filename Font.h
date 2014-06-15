//---------------------------------------------------------------------------

#ifndef FontH
#define FontH

#include <DirectX/d3d9.h>
#include <DirectX/d3dx9.h>
#include <iostream.h>

// The Font object is directly associated with Direct3D, and is placed in the D3D
// namespace. It is now defined by the name D3D::Font.
namespace D3D {

    /**
     * The Font object is meant to load in a font and draw specified strings in
     * specified colours to the screen. It is used by the User Interface objects,
     * which include the Console, DrawingInfo, and MapSelector classes.
     */
    class Font {
        public:

            /**
             * Constructor that sets all pointers to null, preparing the object
             * for initialisation.
             */
            Font() {
                font = NULL;
            };

            /**
             * Destructor that simply tests to see if the Direct3D font has been
             * instantiated. If it has, then this method de-allocates it.
             */
            ~Font() {
                if ( font != NULL ) {
                    font->Release();
                    font = NULL;
                }
            };

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
            void init( LPDIRECT3DDEVICE9 device, int screenWidth, int screenHeight );

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
            void render( LPDIRECT3DDEVICE9 device, unsigned int color, int lineNum, unsigned int drawFlags );

            /**
             * This method controls the text string to be used in the drawing code.
             * So, for example, if you wanted to draw the strings "A" and "B", you
             * would call setText( "A" ) first, then render(). This draws the first
             * string. After that, you would call setText( "B" ), then render() again.
             * This would draw both text strings seperately. This functionality is
             * used by the UI classes (Console, MapSelector, and DrawingInfo) to
             * render their lines properly.
             */
            void setText( string newText );

        private:

            // The font size
            static const int LINE_HEIGHT = 16;

            // The DirectX font
            ID3DXFont *font;

            // The rendering text
            string text;

            // The screen dimensions
            int screenWidth;
            int screenHeight;

    };
};



//---------------------------------------------------------------------------
#endif
 