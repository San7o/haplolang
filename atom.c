// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "atom.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

_Static_assert(_HAPLO_ATOM_MAX == 6,
              "Updated HaploAtomType, maybe update haplo_atom_free");
void haplo_atom_free(HaploAtom *atom)
{
  switch(atom->type)
  {
  case HAPLO_ATOM_STRING:
    free(atom->value.string);
    atom->value.string = NULL;
    return;
  case HAPLO_ATOM_SYMBOL:
    free(atom->value.symbol);
    atom->value.symbol = NULL;
    return;
  case HAPLO_ATOM_QUOTE:
    free(atom->value.quote);
    atom->value.quote = NULL;
    return;
  default:
    return;
  }
  return;
}

_Static_assert(_HAPLO_ATOM_MAX == 6,
              "Updated HaploAtomType, maybe update haplo_atom_deep_copy");
HaploAtom haplo_atom_deep_copy(HaploAtom atom)
{
  HaploAtom new_atom = {0};
  new_atom.type = atom.type;
  switch(atom.type)
  {
  case HAPLO_ATOM_STRING:
    new_atom.value.string = malloc(strlen(atom.value.string)+1);
    strcpy(new_atom.value.string, atom.value.string);
    break;
  case HAPLO_ATOM_SYMBOL:
    new_atom.value.symbol = malloc(strlen(atom.value.symbol)+1);
    strcpy(new_atom.value.symbol, atom.value.symbol);
    break;
  case HAPLO_ATOM_QUOTE:
    new_atom.value.quote = malloc(strlen(atom.value.quote)+1);
    strcpy(new_atom.value.quote, atom.value.quote);
    break;
  case HAPLO_ATOM_INTEGER:
    new_atom.value.integer = atom.value.integer;
    break;
  case HAPLO_ATOM_FLOAT:
    new_atom.value.floating_point = atom.value.floating_point;
    break;
  case HAPLO_ATOM_BOOL:
    new_atom.value.boolean = atom.value.boolean;
    break;
  default:
    new_atom.type = _HAPLO_ATOM_MAX;
    break;
  }
  return new_atom;
}

_Static_assert(_HAPLO_ATOM_MAX == 6,
              "Updated HaploAtomType, maybe update haplo_atom_free");
void haplo_atom_string(HaploAtom atom, char buf[HAPLO_ATOM_MAX_STRING_LEN])
{
  switch(atom.type)
  {
  case HAPLO_ATOM_STRING:
    sprintf(buf, "\"%s\"", atom.value.string);
    break;
  case HAPLO_ATOM_INTEGER:
    sprintf(buf, "%ld", atom.value.integer);
    break;
  case HAPLO_ATOM_FLOAT:
    sprintf(buf, "%f", atom.value.floating_point);
    break;
  case HAPLO_ATOM_BOOL:
    sprintf(buf, "%s", atom.value.boolean ? "true" : "false");
    break;
  case HAPLO_ATOM_QUOTE:
    sprintf(buf, "'%s", atom.value.quote);
    break;
  case HAPLO_ATOM_SYMBOL:
    sprintf(buf, "%s", atom.value.symbol);
    break;
  default:
    break;
  }
  return;
}
