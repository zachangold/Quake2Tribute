//---------------------------------------------------------------------------

#ifndef EntityParserH
#define EntityParserH

#include "StandardHeaders.h"
#pragma hdrstop

#include "BSPCommon.h"
#include "Light.h"

namespace Entity {
    class Line {
        public:            Line();            ~Line();            void parse( char *line );            int tokenLength( char *token );            void free();            bool identifierMatch( char *other );            char *getValue() {                return value;            };        private:            char *identifier;            char *value;    };    class Entity {        public:            Entity();            ~Entity();            char *parse( char *entity );            char *getValue( char *identifier );            bool isLight();            Point3f getOrigin();            void free();        private:            vector< Line > lines;    };    class Parser {        public:            Parser() {};            ~Parser() {                unload();            };            void load( BSP::Header *header, FILE *mapFile );            void unload();        private:            vector< Entity > entities;            vector< Light > lights;    };};

//---------------------------------------------------------------------------
#endif
