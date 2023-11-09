# Faster Fps: Quake II Remastered mod for faster gameplay in solo

## Summary

Easily rush levels, instant weapon switch, grapple, efficient auto-save.

## Gameplay overview

With this mod you can play fast in the existing campaigns, you rush through the levels, almost like a speedrunner, but you don't need to know the levels and you can kill everyone.  
Your weapons are more powerful, and you takes less damage, you easily kill loads of monsters.  

Use only Super Shootgun, Machine Gun, Rocket Launcher and Rail Gun.  
You get them directly on a new game, and all ammo items are still useful because they give you a small amount of ammo for those main weapons.

Weapon switch is now really instantaneous, enjoy chaining kills with really distinct weapons.  

Fly with a fast grapple, and rocket jump without taking self damage.  

Almost never create a savegame, the mod has an auto-save every 30 seconds on different slots.  

Use compass when you have any doubt, you're here to run and shoot fast, not to block on closed doors.  
Ignore items, no need to worry about the good timing for their use.  

Start a new game in hard mode, to have more monsters, the game will still be easy with your overpowered weapons.  

If you want some changes for your personal taste of gameplay, you can, almost all changes of this mod are easily customizable with configuration variables. Also, you still have other weapons, with ammo and more damage.  

## Demo video

https://youtu.be/5SviiIMcPBk

## Installation

Unzip the contents into "%USERPROFILE%\Saved Games\Nightdive Studios\Quake II\" (%USERPROFILE% gets you something like that: "C:\Users\MY_USER_NAME\").  
You should have a "fasterfps" folder in there, alongside the existing "baseq2", with "game_x64.dll" inside.  

Launch the game with the command line parameter added:  
```
+set game fasterfps
```
In Steam, Gog and Epic you can customize the launch options in the properties of the game.  

Or you can create on new manual shortcut on the game, example:  
```
C:\MyGames\QuakeII\ptah_Shipping_Playfab_EGS_x64.exe +set game fasterfps
```

While at it, you can also add the "-skipmovies" command line for a faster start of the game.  

Less convenient alternative to the command line: start the game, open the console, enter "game fasterfps".  

## Features

- When you switch weapons, it's really instantaneous, because it's frustrating to have to wait for the shoot cooldown before switching, now you can chain rocket and rail shoot for example.  
Variable: ff_fastest_weapon_switch, 0 or 1, default value is 1  
The g_instant_weapon_switch and g_quick_weapon_switch existed outside of this mod, but it's not instantaneous at all.  

- The mod has an auto-save every 30 seconds on different slots. Every game should have this feature.  
Variables:  
  - ff_autosave_enabled, 0 or 1, default value is 1  
  - ff_autosave_interval_in_seconds, default value is 30  
  - ff_autosave_slot_count, default value is 10  

- The player does 3x more damage, and receive 0.7x less damage, because I think it's more fun to shoot many monsters than one big, while rushing through levels.  
It's designed for new game with the hard difficulty choice.  
Variables:  
  - ff_player_damage_sent_multiplier, default value is 3.0  
  - ff_player_damage_received_multiplier, default value is 0.7  

- When you start a new game you already have the main weapons: Super Shootgun, Machine Gun, Rocket Launcher and Rail Gun, because I think that other weapons are similar or less interesting.  
Variables:  
  - ff_give_main_weapons_at_start, 0 or 1, default value is 1  
  - ff_start_ammo_shootgun, default value is 20  
  - ff_start_ammo_machinegun, default value is 60  
  - ff_start_ammo_rocket, default value is 10  
  - ff_start_ammo_railgun, default value is 10  

- Any ammo item you pick gives some ammo for the main weapons.  
For example, if you pick one rocket ammo item, you'll get 2 shootgun cells, 4 machine gun bullets, 0.5 rockets, 0.4 railgun slugs.  
0.5 means that you have to pick 2 ammo items to get 1 rocket.  
If you pick an ammo item for a weapon that is not one the 4 main weapons, you also get this weapon ammo in addition of the 4 others.  
This behavior is necessary for the non-main weapons ammo items to still be useful, and to have ammo for the main weapons at the first levels.  
Variables:  
  - ff_extra_ammo_shootgun, default value is 2  
  - ff_extra_ammo_machinegun, default value is 4   
  - ff_extra_ammo_rocket, default value is 0.5  
  - ff_extra_ammo_railgun, default value is 0.4  

- The player doesn't receive damage from its own weapon, so you can abuse rocket jumps.  
Rocket jump power is configurable by a variable, but it is not changed from de vanilla game.  
Variables:  
  - ff_self_damage, 0 or 1, default value is 0  
  - ff_rocket_jump_power, default value is 1600  

- When you start a new game you have the grapple, and its launch speed is increased.  
Variables:  
  - g_grapple_fly_speed, the variable is already in the game without this mod, default value is changed to 3000 (it was 650), a higher value has no effect  
  - g_grapple_pull_speed, the variable is already in the game without this mod, default value is 650  

- The rocket area damage radius is 1.5x larger, for rockets launched by the player.  
Variable: ff_player_rocket_radius_scale, default value is 1.5  

- The bullets from the machine gun have no spread, because I like when my weapon shoots where I aim.  

- Barrels always explode immediately, like in original Quake 2. Imported from the Unmaster mod.  
Variable: ff_barrel_delay, 0 or 1, default value is 0  

- When you click on the weapon hotkey of the already selected weapon, it doesn't cycle to another weapon, because it's unnecessary and annoying in this mod.  
Variable: ff_weapon_chains, 0 or 1, default value is 0  

- The following changes are done because I prefer to quickly kill many dumb monsters.  

- Monsters will not duck projectiles, like in original Quake 2. Imported from the Unmaster mod.  
Variable: ff_monster_duck, 0 or 1, default value is 0  

- Monsters will not strafe to avoid grenades, like in original Quake 2. Imported from the Unmaster mod.  
Variable: ff_monster_sidestep, 0 or 1, default value is 0  

- Monsters stop shooting when losing sight. Imported from the Unmaster mod.  
Variable: ff_monster_blindfire, 0 or 1, default value is 0  

- Monsters can't see you when you're behind them and tell each other that you're there. Imported from the Unmaster mod.  
Variable: ff_monster_hyperaware, 0 or 1, default value is 0  

- Monsters stay on the platforms/floors they spawn on, like in original Quake 2. Imported from the Unmaster mod.  
Variable: ff_monster_walkjump, 0 or 1, default value is 0  

## Key binding

The key binding I use for this mod, just as a suggestion and to show important bindings:  
- R D F G: up, left, down, right (not WASD/ZQSD to have space for additional keys on the left)
- Z: compass (W in qwerty)  
- S: grapple  
- T: super shootgun  
- E: machine gun  
- H: rocket launcher  
- B: rail gun (to press with the thumb)  
- 4: blaster (default key, very rarely used, but can still be useful if you really are out of ammo)  
- A: items wheel (rarely used)(Q in qwerty)  
- Q: weapons wheel (very rarely used)(A in qwerty)  

My keybinding.cfg file: https://raw.githubusercontent.com/yrelof/quake2-rerelease-dll-fasterfps/main/keybinds.cfg  
Right click on the link, save link target in your "%USERPROFILE%\Saved Games\Nightdive Studios\Quake II\fasterfps" directory.

## Other information

Source code: https://github.com/yrelof/quake2-rerelease-dll-fasterfps  

Mod coded for the "Update 1" version of Quake 2 Remastered (from 2023-10-03).  

The mod also works with the additional campaigns and with the Coop.

I recommend to start a new game in hard difficulty, to have a lot of monsters.  
Save games should be compatible anyway if you copy them to the mod folder (baseq2\save to fasterfps\save), but you will not get automatically the main weapons and the grapple.  

If you want an easier difficulty (notably for "The Reckoning" campaign), change the ff_player_damage_received_multiplier variable instead of changing the difficulty of a new game, to still have a lot a monsters.

To customize mod variables, edit the "settings.cfg" file in the mod directory.

Some changes come from the Unmaster mod, they are specified in the "Features" section. Link: https://github.com/nickgovier/Q2unmaster  