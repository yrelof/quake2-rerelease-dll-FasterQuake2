// FasterFps mod

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

// Offhand laser grappling hook
//
// Code originally from Orange 2 Mod

#include "g_local.h"

// variables named "grapple" instead of "hook" for uniformity with previous version of the FasterFps mod
cvar_t *ff_grapple_sky;
cvar_t *ff_grapple_damage;
cvar_t *ff_grapple_initdamage;
cvar_t *ff_grapple_maxdamage;
cvar_t *ff_grapple_delay;

void Hook_InitGame(void)
{
	ff_grapple_sky = gi.cvar("ff_grapple_sky", "0", CVAR_NOFLAGS);
	ff_grapple_damage = gi.cvar("ff_grapple_damage", "1", CVAR_NOFLAGS);
	ff_grapple_initdamage = gi.cvar("ff_grapple_initdamage", "1", CVAR_NOFLAGS);
	ff_grapple_maxdamage = gi.cvar("ff_grapple_maxdamage", "1", CVAR_NOFLAGS);
	ff_grapple_delay = gi.cvar("ff_grapple_delay", "0.0", CVAR_NOFLAGS);
}

void Hook_PlayerDie(edict_t *attacker, edict_t *self)
{
	Hook_Reset(self->client->hook);
}

// reset the hook.  pull all entities out of the world and reset
// the clients weapon state
void Hook_Reset(edict_t *rhook)
{
	if (!rhook)
		return;

	// start with NULL pointer checks
	if (rhook->owner && rhook->owner->client)
	{
		// client's hook is no longer out
		rhook->owner->client->hook_out = false;
		rhook->owner->client->hook_on = false;
		rhook->owner->client->hook = NULL;
		//	   rhook->owner->client->ps.pmove.pm_flags &= ~PMF_NO_PREDICTION;
	}

	// this should always be true and free the laser beam
	if (rhook->laser)
		G_FreeEdict(rhook->laser);

	// delete ourself
	G_FreeEdict(rhook);
};

// resets the hook if it needs to be
bool Hook_Check(edict_t *self)
{
	if (!self->enemy || !self->owner)
	{
		Hook_Reset(self);
		return true;
	}

	// drop the hook if either party dies/leaves the game/etc.
	if ((!self->enemy->inuse) || (!self->owner->inuse) ||
		(self->enemy->client && self->enemy->health <= 0) ||
		(self->owner->health <= 0))
	{
		Hook_Reset(self);
		return true;
	}

	// drop the hook if player lets go of button
	// and has the hook as current weapon
	if (!((self->owner->client->latched_buttons | self->owner->client->buttons) & BUTTON_ATTACK)
		&& (strcmp(self->owner->client->pers.weapon->pickup_name, "Hook") == 0))
	{
		Hook_Reset(self);
		return true;
	}

	return false;
}

void Hook_Service(edict_t *self)
{
	vec3_t	hook_dir;

	// if hook should be dropped, just return
	if (Hook_Check(self))
		return;

	// give the client some velocity ...
	if (self->enemy->client)
		hook_dir = self->enemy->s.origin - self->owner->s.origin;
	else
		hook_dir = self->s.origin - self->owner->s.origin;

	hook_dir.normalize();
	self->owner->velocity = hook_dir * g_grapple_pull_speed->value;

	//	SV_AddGravity(self->owner);
}

// keeps the invisible hook entity on hook->enemy (can be world or an entity)
THINK (Hook_Track) (edict_t *self) -> void
{
	vec3_t normal;

	// if hook should be dropped, just return
	if (Hook_Check(self))
		return;

	// bring the pAiN!
	if (self->enemy->client)
	{
		// move the hook along with the player.  It's invisible, but
		// we need this to make the sound come from the right spot

		if (self->owner->client->ff_grapple_damage >= ff_grapple_maxdamage->value)
		{
			Hook_Reset(self);
			return;
		}

		gi.unlinkentity(self);
		self->s.origin = self->enemy->s.origin;
		gi.linkentity(self);

		normal = self->enemy->s.origin - self->owner->s.origin;

		T_Damage(self->enemy, self, self->owner, vec3_origin, self->enemy->s.origin, normal, ff_grapple_damage->value, 0, DAMAGE_NO_KNOCKBACK, MOD_HOOK);

		self->owner->client->ff_grapple_damage += ff_grapple_damage->value;
	}
	else
	{
		// If the hook is not attached to the player, constantly copy
		// copy the target's velocity. Velocity copying DOES NOT work properly
		// for a hooked client. 
		self->velocity = self->enemy->velocity;
	}

	self->nextthink = level.time + 100_ms;
}

// the hook has hit something.  what could it be?
TOUCH(Hook_Touch) (edict_t *self, edict_t *other, const trace_t &tr, bool other_touching_self) -> void
{
	vec3_t dir, normal;

	// ignore hitting the person who launched us
	if (other == self->owner)
		return;

	if (!self->owner || !self->owner->client)
		return;

	// ignore hitting items/projectiles/etc.
	if (other->solid == SOLID_NOT || other->solid == SOLID_TRIGGER || other->movetype == MOVETYPE_FLYMISSILE)
		return;

	if (tr.surface && (tr.surface->flags & SURF_SKY) && !ff_grapple_sky->value)
	{
		Hook_Reset(self);
		return;
	}
	
	if (other->client) 		// we hit a player	
	{
		// ignore hitting a teammate
		if (OnSameTeam(other, self->owner))
			return;

		// we hit an enemy, so do a bit of damage
		dir = self->owner->s.origin - other->s.origin;
		normal = other->s.origin - self->owner->s.origin;

		if (self->owner->client->ff_grapple_damage >= ff_grapple_maxdamage->value) {
			Hook_Reset(self);
			return;
		}

		if (ff_grapple_maxdamage->value >= ff_grapple_initdamage->value)
			T_Damage(other, self, self->owner, dir, self->s.origin, normal, ff_grapple_initdamage->value, ff_grapple_initdamage->value, DAMAGE_NONE, MOD_HOOK);

		self->owner->client->ff_grapple_damage += ff_grapple_initdamage->value;
	}
	else     // we hit something thats not a player
	{					
		// if we can hurt it, then do a bit of damage
		if (other->takedamage) {
			dir = self->owner->s.origin - other->s.origin;
			normal = other->s.origin - self->owner->s.origin;
			T_Damage(other, self, self->owner, dir, self->s.origin, normal, ff_grapple_damage->value, ff_grapple_damage->value, DAMAGE_NONE, MOD_UNKNOWN);

			self->owner->client->ff_grapple_damage += ff_grapple_initdamage->value;
		}
		// stop moving
		self->velocity = vec3_t{ 0,0,0 };

		// gi.sound() doesnt work because the origin of an entity with no model is not 
		// transmitted to clients or something.  hoped this would be fixed in Q2 ...
		gi.positioned_sound(self->s.origin, self, CHAN_WEAPON, gi.soundindex("flyer/Flyatck2.wav"), 1, ATTN_NORM, 0);
	}

	// remember who/what we hit, must be set before Hook_Check() is called
	self->enemy = other;

	// if hook should be dropped, just return
	if (Hook_Check(self))
		return;

	// we are now anchored
	self->owner->client->hook_on = true;
	//	self->owner->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION;

	// keep up with that thing
	self->think = Hook_Track;
	self->nextthink = level.time + 100_ms;

	self->solid = SOLID_NOT;

	self->owner->hook_time = level.time;
}

// move the two ends of the laser beam to the proper positions
THINK(Hook_Think) (edict_t *self) -> void
{
	vec3_t forward, right, offset, start;
	vec3_t dir;   // Kyper - Lithium port - remove forward and right?

	// stupid check for NULL pointers ...
	if (!(self && self->owner && self->owner->owner && self->owner->owner->client))
	{
		gi.Com_Error("Hook_Think: error, reload your savegame to re-enabled grapple\n");
		G_FreeEdict(self);
		return;
	}

	// put start position into start
	AngleVectors(self->owner->owner->client->v_angle, forward, right, NULL);   // Kyper - Lithium port - remove forward and right?
	offset = vec3_t{ 24, 8, -8 };  // Kyper - Lithium port - changed "ent->viewheight - 8" to "-8" following example in p_weapon.cpp
	P_ProjectSource(self->owner->owner, self->owner->owner->client->v_angle, offset, start, dir);

	// move the two ends
	self->s.origin = start;
	self->s.old_origin = self->owner->s.origin;

	gi.linkentity(self);

	// set up to go again
	self->nextthink = level.time + FRAME_TIME_S;
}

// create a laser and return a pointer to it
edict_t *Hook_Start(edict_t *ent)
{
	edict_t *self;

	self = G_Spawn();
	self->movetype = MOVETYPE_NONE;
	self->solid = SOLID_NOT;
	self->s.renderfx |= RF_BEAM | RF_TRANSLUCENT;
	self->s.modelindex = 1;			// must be non-zero
	self->owner = ent;

	// set the beam diameter
	self->s.frame = 4;

	// set the color
	self->s.skinnum = 0xf0f0f0f0;  // red

	//if (ctf->value && ctf_coloredhook->value && ent->owner->client->resp.ctf_team == 2)
	if ((ctf->integer || teamplay->integer) && ent->owner->client->resp.ctf_team == CTF_TEAM2)   // Kyper - Lithium port - forget the option, just give Blue team blue...
		self->s.skinnum = 0xf1f1f1f1;  // blue

	self->think = Hook_Think;

	self->mins = vec3_t{ -8, -8, -8 };
	self->maxs = vec3_t{ 8, 8, 8 };
	gi.linkentity(self);

	self->spawnflags |= SPAWNFLAG_LASER_ZAP | SPAWNFLAG_LASER_ON;
	self->svflags &= ~SVF_NOCLIENT;
	Hook_Think(self);

	return self;
}

// creates the invisible hook entity and sends it on its way
// attaches a laser to it
void Hook_Fire(edict_t *owner, vec3_t start, vec3_t forward) {
	edict_t *hook;
	trace_t tr;

	hook = G_Spawn();
	hook->movetype = MOVETYPE_FLYMISSILE;
	hook->solid = SOLID_BBOX;
	hook->clipmask = MASK_SHOT;
	hook->owner = owner;			// this hook belongs to me
	owner->client->hook = hook;		// this is my hook
	hook->classname = "hook";		// this is a hook

	hook->s.angles = vectoangles(forward);
	hook->velocity = forward * g_grapple_fly_speed->value;

	hook->touch = Hook_Touch;

	hook->think = G_FreeEdict;
	hook->nextthink = level.time + 5_sec;

	gi.setmodel(hook, "");
	
	hook->s.origin = start;
	hook->s.old_origin = hook->s.origin;

	hook->mins = { 0, 0, 0 };
	hook->maxs = { 0, 0, 0 };

	// start up the laser
	hook->laser = Hook_Start(hook);

	// put it in the world
	gi.linkentity(hook);

	// from id's code.   // Kyper - Lithium port - now from the remaster code!
	tr = gi.traceline(owner->s.origin, hook->s.origin, hook, MASK_SHOT);
	if (tr.fraction < 1.0f)
	{
		hook->s.origin = hook->s.origin + (forward * -10);
		hook->touch(hook, tr.ent, tr, false);
	}
}

// a call has been made to fire the hook
void Weapon_Hook_Fire(edict_t *ent)
{
	vec3_t forward, right;
	vec3_t start;
	vec3_t offset;
	vec3_t dir;      // Kyper - Lithium port - replaces forward?

	if (ent->client->hook_out)
		return;

	// don't allow the client to fire the hook too rapidly
	if (level.time < gtime_t::from_sec(ent->client->last_hook_time) + gtime_t::from_sec(ff_grapple_delay->value))
		return;

	ent->client->last_hook_time = level.time.seconds();

	ent->client->hook_out = true;
	ent->client->ff_grapple_damage = 0;

	// calculate start position and forward direction
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	offset = vec3_t{ 24, 8, -8 };    // Kyper - Lithium port - changed "ent->viewheight - 8" to "-8" following example in p_weapon.cpp
	
	P_ProjectSource(ent, ent->client->v_angle, offset, start, dir);

	// kick back??
	P_AddWeaponKick(ent, ent->client->v_forward * -2, { -1.f, 0.f, 0.f });

	// actually launch the hook off
	Hook_Fire(ent, start, dir);

	gi.sound(ent, CHAN_WEAPON, gi.soundindex("flyer/Flyatck3.wav"), 1, ATTN_NORM, 0);

	PlayerNoise(ent, start, PNOISE_WEAPON);
}

// boring service routine
void Weapon_Hook(edict_t *ent)
{
	static int pause_frames[] = { 19, 32, 0 };
	static int fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_Hook_Fire);
}