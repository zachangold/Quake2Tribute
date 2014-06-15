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

    void Line::parse( char *line ) {
        int identifierLength = tokenLength( line );
    bool Line::identifierMatch( char *other ) {
        return strcmp( identifier, other ) == 0;

    };
    //==========================================================
    //          ENTITY::ENTITY METHODS
    //==========================================================

    Entity::Entity() {};
    Entity::~Entity() {
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


};


//---------------------------------------------------------------------------
#pragma package(smart_init)