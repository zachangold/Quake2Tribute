//---------------------------------------------------------------------------

#pragma hdrstop

#include "Entity.h"


/**
 * The Entity-related classes are placed in the Entity namespace
 */
namespace Entity {

    //==========================================================
    //          ENTITY::LINE METHODS
    //==========================================================

    /**
     * Constructor that nulls out the pointers in the object, preparing
        identifier = NULL;
        value = NULL;
    };


    /**
     * Parser that loads in the line pointed to by parameter line, storing
        // get the length of the identifier token, and store it in the identifier field
    
    /**
     * Tests to see if two identifiers are the same. (The first identifier is
        return strcmp( identifier, other ) == 0;

    };
    //==========================================================
    //          ENTITY::ENTITY METHODS
    //==========================================================

    /**
     * Destructor makes sure that the lines have been deallocated
        free();
    //==========================================================
    //          ENTITY::LIGHT METHODS
    //==========================================================
    /**
     * Loads in a light from the entity pointed to by parameter entity
        light = new D3D::Light();
        light->reset();

        D3DLIGHT9 *d3dlight = light->getLight();

        // Get the light's colour and position
        Point3f origin = entity->getOrigin();
        Point3f color = entity->getColor();

        // Transfer the light's position to DirectX coordinates
        d3dlight->Position = D3DXVECTOR3( origin.y, origin.z, -origin.x );

        // set up all of the light's properties
        d3dlight->Type = D3DLIGHT_POINT;    // make the light type 'point light'
        d3dlight->Diffuse = D3DXCOLOR( color.x, color.y, color.z, 1.0f );    // set the light's color

        // Set up light attenuation (objects that are farther away a lit less)
        d3dlight->Attenuation1 = 0.015 / BSP::MAP_SCALE;

        // Limit how far this light extends
        d3dlight->Range = 600.0f * BSP::MAP_SCALE;
    };



    //==========================================================
    //          ENTITY::MONSTER METHODS
    //==========================================================



    //==========================================================
    //          ENTITY::PARSER METHODS
    //==========================================================


    /**
     * load() method loads in all entity data from the BSP map file.

        // Allocate memory for the entity lump and then load it in
        char *entityLump = new char[ header->lump[ BSP_ENTITY_LUMP ].length ];
        fseek( mapFile, header->lump[ BSP_ENTITY_LUMP ].offset, 0 );
        fread( entityLump, 1, header->lump[ BSP_ENTITY_LUMP ].length, mapFile );

        // load in entities until the end of the entity data is reached
        char *entityAt = entityLump;
        while ( entityAt < entityLump + header->lump[ BSP_ENTITY_LUMP ].length ) {

            // Create an entity
            entities.push_back( new Entity() );

            int entNum = entities.size() - 1;

            // Parse that entity
            entityAt = entities[ entNum ]->parse( entityAt );


            // If that entity is a light, create an Entity::Light and store it
            // in the lights array
            if ( entities[ entNum ]->isLight() ) {
                lights.push_back( new Light() );
                lights[ lights.size() - 1 ]->load( entities[ entNum ] );
            } else if ( strContains( entities[ entNum ]->getValue( "classname" ), "monster_" ) ) {
                monsters.push_back( new Monster() );
                monsters[ monsters.size() - 1 ]->init( entities[ entNum ] );
            }
        }

        // delete the memory allocated for the entity lump.
        delete[] entityLump;
    };


    /**
     * enableLights() method enables the eight closest lights
        // The maximum number of lights to be enabled


//---------------------------------------------------------------------------
#pragma package(smart_init)