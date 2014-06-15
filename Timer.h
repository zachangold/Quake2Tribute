//---------------------------------------------------------------------------

#ifndef TimerH
#define TimerH

#include <windows.h>


/**
 * This class is made to add a layer of abstraction to the windows timing calls.
 * The only method that is used on a regular basis is the getTimeMillis() method,
 * which returns the system time in milliseconds.
 */
class Timer {
    public:

        /**
         * Constructor that sets the class members to 0
         */
        Timer() {
            memset( &startTime, 0, sizeof( SYSTEMTIME ) );
            memset( &currentTime, 0, sizeof( SYSTEMTIME ) );
        };

        /**
         * Unused
         */
        void start();

        /**
         * Returns the system time in milliseconds
         */
        unsigned int getTimeMillis();

    private:
        // The current time (updated when start() or getTimeMillis() are called)
        SYSTEMTIME currentTime;

        // This is unused
        SYSTEMTIME startTime;

};

//---------------------------------------------------------------------------
#endif
