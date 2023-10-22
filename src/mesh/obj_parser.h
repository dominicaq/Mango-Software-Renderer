#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"

Mesh *load_obj_mesh(const char *filename);

#endif
