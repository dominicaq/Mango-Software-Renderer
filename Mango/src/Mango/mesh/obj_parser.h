#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H
#include "../game/gameobject.h"

Mesh load_obj(const char *fpath, const char *fname);
Material **parse_mtl(const char* fpath, const char *fname, int *num_materials);

#endif
