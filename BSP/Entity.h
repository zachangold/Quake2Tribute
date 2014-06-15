//---------------------------------------------------------------------------

#ifndef EntityH
#define EntityH


#include "BSPCommon.h"
#include "BSPTree.h"
#include "Light.h"

#define NUM_MONSTER_TYPES 3
typedef struct {
    char *dirName;
    char *skinName;
} MonsterInfo;

/**
 * The following set of classes are all based on loading in the entity lump of
 * a BSP File, and are therefore put into the Entity namespace. All of these
 * classes must be accessed by using "Entity::" as a prefix to the classname.
 */
namespace Entity {

    //MonsterInfo monsterInfo[ NUM_MONSTER_TYPES ];

    /**
     * The entity Line class is a simple entry in the entity lump.
     * An entry consists of two tokens: the first is an identifier, and the
     * second is its value.
     */
    class Line {
        public:

//---------------------------------------------------------------------------
#endif