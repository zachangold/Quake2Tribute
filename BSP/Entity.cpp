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
     * Constructor that nulls out the pointers in the object, preparing     * it for later.     */    Line::Line() {
        identifier = NULL;
        value = NULL;
    };
    /**     * Destructor that makes sure that all memory is de-allocated     */    Line::~Line() {        free();    };

    /**
     * Parser that loads in the line pointed to by parameter line, storing     * the identifier and value in the according fields.     */    void Line::parse( char *line ) {
        // get the length of the identifier token, and store it in the identifier field        int identifierLength = tokenLength( line );        identifier = new char[ identifierLength + 1 ];        memcpy( identifier, line + 1, identifierLength );        // Read in the value, and store it in the value field        char *valuePtr = line + identifierLength + 3;        int valueLength = tokenLength( valuePtr );        value = new char[ valueLength + 1 ];        memcpy( value, valuePtr + 1, valueLength );        // Terminate the identifier and value strings with the null character,        // showing where the end of the string is.        identifier[ identifierLength ] = 0;        value[ valueLength ] = 0;    };    /**     * Function that returns the length (in characters) of the token pointed     * to by parameter token. A token is just a "word", and is separated by     * double quotes ( " )     */    int Line::tokenLength( char *token ) {        // find the first quote ( " ), and start counting from there        while ( *token != '"' ) ++token;                ++token;        int length = 0;        char *charAt = token;        // Find the length of the string before the next quote (")        while ( *charAt != '"' ) {            // If the letter pointed to by charAt is NOT a quote, then charAt            // points to the next character, and the length is incremented.            ++charAt;            ++length;        }        // Return the length of the token        return length;    };    /**     * Method that deallocates the identifier and the value character strings.     */    void Line::free() {        if ( identifier != NULL ) {            delete[] identifier;            identifier = NULL;        }        if ( value != NULL ) {            delete[] value;            value = NULL;        }    };
    
    /**
     * Tests to see if two identifiers are the same. (The first identifier is     * stored in field identifier, and the second is the parameter other)     */    bool Line::identifierMatch( char *other ) {
        return strcmp( identifier, other ) == 0;

    };
    //==========================================================
    //          ENTITY::ENTITY METHODS
    //==========================================================

    /**
     * Destructor makes sure that the lines have been deallocated     */    Entity::~Entity() {
        free();    };    /**     * Parses an entire entity declaration, creating entity lines as it     * goes along. Returns a pointer to the next entity to be loaded.     */    char *Entity::parse( char *entity ) {        // Every 4 quotes is a new line        int quoteNum = 0;        // while the Entity declaration is not over,        while ( *entity != '}' ) {            // if that character is a quote (")            if ( *entity == '"' ) {                // increment the quote number                quoteNum++;                // if this is the beginning quote in an entity line,                if ( quoteNum % 4 == 1 ) {                    // add a new line and load it in                    lines.push_back();                    lines[ lines.size() - 1 ] = new Line();                    lines[ lines.size() - 1 ]->parse( entity );                }            }            // move to the next character in the entity declaration.            entity++;        }        // Return a pointer to the next entity        return entity + 3;    };    /**     * Returns the value of the line that has the same identifier as     * the identifier parameter     */    char *Entity::getValue( char *identifier ) {        // go through each line        for ( unsigned int lineNum = 0; lineNum < lines.size(); ++lineNum ) {            // if the identifier of that line is a match,            if ( lines[ lineNum ]->identifierMatch( identifier ) ) {                // The identifier was found!                return lines[ lineNum ]->getValue();            }        }        // Otherwise, if no match was found, return null        return NULL;    };    /**     * Verifies if this entity is a light or not. Lights are handled in     * a special way to assist in world lighting.     */    bool Entity::isLight() {        // get the classname        char *value = getValue( "classname" );        // If no classname was found, it is not a light        if ( value == NULL ) {            return false;        }        // if the classname is "light", then it is a light        if ( strcmp( "light", value ) == 0 ) {            return true;        }        // Otherwise, it is not a light        return false;    };    /**     * Returns the position of an entity in the form of a Point3f     */    Point3f Entity::getOrigin() {        // The value of the location of the object        char *value = getValue( "origin" );        // The return value        Point3f origin;        origin.x = 0.0f;        origin.y = 0.0f;        origin.z = 0.0f;        // if there was no definite origin, then return (0, 0, 0)        if ( value == NULL ) {            return origin;        }        // parse the x, y, and z values from value        value = strtok( value, " " );        origin.x = ( float ) atof( value ) * BSP::MAP_SCALE;        value = strtok( NULL, " " );        origin.y = ( float ) atof( value ) * BSP::MAP_SCALE;        value = strtok( NULL, " " );        origin.z = ( float ) atof( value ) * BSP::MAP_SCALE;        // Return the values that were found        return origin;    };    /**     * Returns the colour of an entity (usually a light) in the form of a Point3f     */    Point3f Entity::getColor() {        // Get the colour of this entity        char *value = getValue( "_color" );        // Default colour value is (1.0, 1.0, 1.0), for white light        Point3f color;        color.x = 1.0f;        color.y = 1.0f;        color.z = 1.0f;        // If there was no definite colour, return white        if ( value == NULL ) {            return color;        }        // parse the colour from value        value = strtok( value, " " );        color.x = ( float ) atof( value );        value = strtok( NULL, " " );        color.y = ( float ) atof( value );        value = strtok( NULL, " " );        color.z = ( float ) atof( value );        // Return the colour of the entity.        return color;    };    /**     * Deletes all of the memory allocated by this Entity object.     */    void Entity::free() {        for ( unsigned int i = 0; i < lines.size(); ++i ) {            delete lines[ i ];        }    };
    //==========================================================
    //          ENTITY::LIGHT METHODS
    //==========================================================
    /**
     * Loads in a light from the entity pointed to by parameter entity     */    void Light::load( Entity *entity ) {        // Create the light object
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
    /**     * Enables this light with DirectX     */    void Light::setEnableState( LPDIRECT3DDEVICE9 device, int lightNum ) {        light->enable( device, lightNum );    };    /**     * Returns the distance from point pos. This is for enabling the closest     * lights to a point.     */    float Light::getDistFromPoint( Point3f pos ) {        // Just use pythagorean theorem for the distance        return sqrt( ( pos.x - light->getLight()->Position.x ) * ( pos.x - light->getLight()->Position.x ) +                     ( pos.y - light->getLight()->Position.y ) * ( pos.y - light->getLight()->Position.y ) +                     ( pos.z - light->getLight()->Position.z ) * ( pos.z - light->getLight()->Position.z ) );    };


    //==========================================================
    //          ENTITY::MONSTER METHODS
    //==========================================================



    //==========================================================
    //          ENTITY::PARSER METHODS
    //==========================================================


    /**
     * load() method loads in all entity data from the BSP map file.     */    void Parser::load( BSP::Header *header, FILE *mapFile ) {

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
    /**     * unload() method deletes all entity data that was created by load()     */    void Parser::unload() {        // Delete the entities        for ( unsigned int i = 0; i < entities.size(); ++i ) {            delete entities[ i ];        }        entities.resize( 0 );        for ( unsigned int i = 0; i < monsters.size(); ++i ) {            delete monsters[ i ];        }        monsters.resize( 0 );        // Delete the entity lights        for ( unsigned int i = 0; i < lights.size(); ++i ) {            delete lights[ i ];        }        lights.resize( 0 );    };

    /**
     * enableLights() method enables the eight closest lights     * to parameter pos     */    void Parser::enableLights( LPDIRECT3DDEVICE9 device, Point3f pos ) {
        // The maximum number of lights to be enabled        const int NUM_LIGHTS = 8;        // The enabled lights        struct {            Light *light;            float dist;        } enabledLights[ NUM_LIGHTS ];        memset( enabledLights, 0, ( sizeof( Light * ) + sizeof( float ) ) * NUM_LIGHTS );        // Set the first light to the first light in the light array        enabledLights[ 0 ].light = lights[ 0 ];        enabledLights[ 0 ].dist = lights[ 0 ]->getDistFromPoint( pos );        // For each light that exists,        for ( unsigned int lightSet = 0; lightSet < lights.size(); ++lightSet ) {            // Go through each of the lights that have already been enabled            for ( int i = 0; i < NUM_LIGHTS; ++i ) {                // If that light has not been set yet, set it to this light                if ( enabledLights[ i ].light == NULL ) {                    enabledLights[ i ].light = lights[ lightSet ];                    enabledLights[ i ].dist = lights[ lightSet ]->getDistFromPoint( pos );                    break;                } else if ( enabledLights[ i ].dist > lights[ lightSet ]->getDistFromPoint( pos ) ) {                    // Or, if this light is closer than the one that has already been set,                    // insert the light in that position in the array.                    for ( int shift = NUM_LIGHTS - 1; shift > i; --shift ) {                        enabledLights[ shift ] = enabledLights[ shift - 1 ];                    }                                        enabledLights[ i ].light = lights[ lightSet ];                    enabledLights[ i ].dist = lights[ lightSet ]->getDistFromPoint( pos );                    break;                }            }        }        // Enable the lights that are closest to Point "pos"        for ( int i = 0; i < NUM_LIGHTS; ++i ) {            enabledLights[ i ].light->setEnableState( device, i );        }    };    /**     * Returns the name of the skybox, found with the first entity.     */    char *Parser::getSkyBoxName() {        return entities[ 0 ]->getValue( "sky" );    };    /**     * Sets the position of the camera to the player's spawn point     */    void Parser::setCameraPos( Camera *camera ) {        Point3f origin;        unsigned int entityNum;        // Go through each entity until one is found that has the "player start"        // classname        for ( entityNum = 0; entityNum < entities.size(); ++entityNum ) {            if ( strcmp( entities[ entityNum ]->getValue( "classname" ), "info_player_start" ) == 0 ) {                break;            }        }        // From the entity we just found, set the camera's location to the origin        // of that entity.        origin = entities[ entityNum ]->getOrigin();        camera->pos->x = -origin.y;        camera->pos->y = -origin.z;        camera->pos->z = origin.x;    };    MonsterInfo monsterInfo[ NUM_MONSTER_TYPES ] = {        {"Q2/models/monsters/infantry", },        {},        {}    };};


//---------------------------------------------------------------------------
#pragma package(smart_init)
