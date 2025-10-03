// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include "atom.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static_assert(_HAPLO_ATOM_MAX == 6,
              "Updated HaploAtomType, maybe update haplo_atom_free");
void haplo_atom_free(HaploAtom atom)
{
  switch(atom.type)
  {
  case HAPLO_ATOM_STRING:
    free(atom.string);
    return;
  case HAPLO_ATOM_SYMBOL:
    free(atom.symbol);
    return;
  case HAPLO_ATOM_QUOTE:
    free(atom.quote);
    return;
  default:
    return;
  }
  return;
}

static_assert(_HAPLO_ATOM_MAX == 6,
              "Updated HaploAtomType, maybe update haplo_atom_deep_copy");
HaploAtom haplo_atom_deep_copy(HaploAtom atom)
{
  HaploAtom new_atom = {0};
  new_atom.type = atom.type;
  switch(atom.type)
  {
  case HAPLO_ATOM_STRING:
    new_atom.string = malloc(strlen(atom.string)+1);
    strcpy(new_atom.string, atom.string);
    break;
  case HAPLO_ATOM_SYMBOL:
    new_atom.symbol = malloc(strlen(atom.symbol)+1);
    strcpy(new_atom.symbol, atom.symbol);
    break;
  case HAPLO_ATOM_QUOTE:
    new_atom.quote = malloc(strlen(atom.quote)+1);
    strcpy(new_atom.quote, atom.quote);
    break;
  case HAPLO_ATOM_INTEGER:
    new_atom.integer = atom.integer;
    break;
  case HAPLO_ATOM_FLOAT:
    new_atom.floating_point = atom.floating_point;
    break;
  case HAPLO_ATOM_BOOL:
    new_atom.boolean = atom.boolean;
    break;
  default:
    new_atom.type = _HAPLO_ATOM_MAX;
    break;
  }
  return new_atom;
}

static_assert(_HAPLO_ATOM_MAX == 6,
              "Updated HaploAtomType, maybe update haplo_atom_free");
void haplo_atom_string(HaploAtom atom, char buf[HAPLO_ATOM_MAX_STRING_LEN])
{
  switch(atom.type)
  {
  case HAPLO_ATOM_STRING:
    sprintf(buf, "\"%s\"", atom.string);
    break;
  case HAPLO_ATOM_INTEGER:
    sprintf(buf, "%ld", atom.integer);
    break;
  case HAPLO_ATOM_FLOAT:
    sprintf(buf, "%f", atom.floating_point);
    break;
  case HAPLO_ATOM_BOOL:
    sprintf(buf, "%s", atom.boolean ? "true" : "false");
    break;
  case HAPLO_ATOM_QUOTE:
    sprintf(buf, "'%s", atom.quote);
    break;
  case HAPLO_ATOM_SYMBOL:
    sprintf(buf, "%s", atom.symbol);
    break;
  default:
    break;
  }
  return;
}
