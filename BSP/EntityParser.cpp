//---------------------------------------------------------------------------

#pragma hdrstop

#include "EntityParser.h"


namespace Entity {

    //==========================================================
    //          ENTITY::LINE METHODS
    //==========================================================

    Line::Line() {
        identifier = NULL;
        value = NULL;
    };
    Line::~Line() {        free();    };
    void Line::parse( char *line ) {
        int identifierLength = tokenLength( line );        identifier = new char[ identifierLength + 1 ];        memcpy( identifier, line + 1, identifierLength );        char *valuePtr = line + identifierLength + 3;        int valueLength = tokenLength( valuePtr );        value = new char[ valueLength + 1 ];        memcpy( value, valuePtr + 1, valueLength );        identifier[ identifierLength ] = 0;        value[ valueLength ] = 0;    };    int Line::tokenLength( char *token ) {        // find the first quote ( " ), and start counting from there        while ( *token != '"' ) {            ++token;        }        ++token;        int length = 0;        char *charAt = token;        while ( *charAt != '"' ) {            // If the letter pointed to by charAt is NOT a quote, then charAt            // points to the next character, and the length is incremented.            ++charAt;            ++length;        }        return length;    };    void Line::free() {        if ( identifier != NULL ) {            delete[] identifier;            value = NULL;        }        if ( value != NULL ) {            delete[] value;            value = NULL;        }    };
    bool Line::identifierMatch( char *other ) {
        return strcmp( identifier, other ) == 0;

    };
    //==========================================================
    //          ENTITY::ENTITY METHODS
    //==========================================================

    Entity::Entity() {};
    Entity::~Entity() {        free();    };    // Returns a pointer to the start of the next bsp_entity declaration    char *Entity::parse( char *entity ) {        int quoteNum = 0;        while ( *entity != '}' ) {            if ( *entity == '"' ) {                quoteNum++;                if ( quoteNum % 4 == 1 ) {                    lines.resize( lines.size() + 1 );                    lines[ lines.size() - 1 ].parse( entity );                }            }            entity++;        }        return entity + 3;    };    char *Entity::getValue( char *identifier ) {        for ( unsigned int lineNum = 0; lineNum < lines.size(); ++lineNum ) {            if ( lines[ lineNum ].identifierMatch( identifier ) ) {                // The identifier was found!                return lines[ lineNum ].getValue();            }        }        return NULL;    };    bool Entity::isLight() {        char *value = getValue( "classname" );        if ( value == NULL ) {            return false;        }        if ( strcmp( "light", value ) == 0 ) {            return true;        }        return false;    };    Point3f Entity::getOrigin() {        char *value = getValue( "origin" );        Point3f origin;        origin.x = 0.0f;        origin.y = 0.0f;        origin.z = 0.0f;        if ( value == NULL ) {            return origin;        }        // parse the x, y, and z values        value = strtok( value, " " );        origin.x = ( float ) atoi( value );        value = strtok( NULL, " " );        origin.y = ( float ) atoi( value );        value = strtok( NULL, " " );        origin.z = ( float ) atoi( value );        return origin;    };    void Entity::free() {        for ( unsigned int i = 0; i < lines.size(); ++i ) {            lines[ i ].free();        }    };
    //==========================================================
    //          ENTITY::PARSER METHODS
    //==========================================================

    void Parser::load( BSP::Header *header, FILE *mapFile ) {
        char *entityLump = new char[ header->lump[ BSP_ENTITY_LUMP ].length ];

        fseek( mapFile, header->lump[ BSP_ENTITY_LUMP ].offset, 0 );
        fread( entityLump, 1, header->lump[ BSP_ENTITY_LUMP ].length, mapFile );

        char *entityAt = entityLump;
        while ( entityAt < entityLump + header->lump[ BSP_ENTITY_LUMP ].length ) {
            entities.push_back();

            entityAt = entities[ entities.size() - 1 ].parse( entityAt );
        }

        delete entityLump;
    };
    void Parser::unload() {        entities.resize( 0 );        lights.resize( 0 );    };

};


//---------------------------------------------------------------------------
#pragma package(smart_init)
