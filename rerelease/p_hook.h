// FasterQuake2 mod

/*============================================================================

	This file is part of Lithium II Mod for Quake II
	Copyright (C) 1997, 1998, 1999, 2010 Matthew A. Ayres

	Lithium II Mod is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Lithium II Mod is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Lithium II Mod.  If not, see <http://www.gnu.org/licenses/>.

	Quake II is a trademark of Id Software, Inc.

	Code by Matt "WhiteFang" Ayres, matt@lithium.com

============================================================================*/

#pragma once

void Hook_InitGame(void);
void Hook_PlayerDie(edict_t *attacker, edict_t *self);
void Hook_Think(edict_t *self);
edict_t* Hook_Start(edict_t *ent);
void Hook_Reset(edict_t *rhook);
bool Hook_Check(edict_t *self);
void Hook_Service(edict_t *self);
void Hook_Track(edict_t *self);
void Hook_Touch(edict_t *self, edict_t *other, const trace_t &tr, bool other_touching_self);
void Hook_Fire(edict_t *owner, vec3_t start, vec3_t forward);
void Weapon_Hook_Fire(edict_t *ent);
void Weapon_Hook(edict_t *ent);
