#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H
#include "../game/gameobject.h"

Mesh load_obj(const char *filename);
Material *parse_mtl(const char *filename);

#endif
