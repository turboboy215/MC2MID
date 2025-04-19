# MC2MID

## Mark Cooksey (NES/GB/GBC/GG/SMS) to MIDI converter

This tool converts music from NES, Game Boy and Game Boy Color, Game Gear, and Master System games using Mark Cooksey's sound engine to MIDI format.

It works with ROM images. To use it, you must specify the name of the ROM followed by the number of the bank containing the sound data (in hex).
For games that contain multiple banks of music (usually 2; Earthworm Jim has 3), you must run the program multiple times specifying where each different bank is located. However, in order to prevent files from being overwritten, the MIDI files from the previous bank must either be moved to a separate folder or renamed.

Note that for many games, there are "empty" tracks (usually the first or last track). This is normal.

To specify the system, use an additional command "GB", "NES", or "GG".

Examples:
* MC2MID "Aladdin (U) (Beta) [S].gb" B
* MC2MID "Earthworm Jim - Menace 2 the Galaxy (U) [C][!].gbc" 5
* MC2MID "Earthworm Jim - Menace 2 the Galaxy (U) [C][!].gbc" 6
* MC2MID "Earthworm Jim - Menace 2 the Galaxy (U) [C][!].gbc" 7
* MC2MID "Dragon's Lair - The Legend (E) [!].gb" 1
* MC2MID "Aladdin (E) [!].nes" 2 NES
* MC2MID "Robocop versus The Terminator (U) [!].gg" 20 GG

This tool was based on my own reverse-engineering of the sound engine, partially based on disassembly of Aladdin's sound code.

Also included is another program, MC2TXT, which prints out information about the song data from each game. This is essentially a prototype of MC2MID.

Supported games:

Game Boy (Color):
  * The Addams Family
  * Aladdin (GB)
  * Army Men: Air Combat
  * Carl Lewis Athletics 2000
  * Commander Keen
  * Cool Hand
  * Cool Spot
  * Cool World
  * Dennis
  * Dr. Franken
  * Dr. Franken II
  * Dracula: Crazy Vampire
  * Dragon's Lair: The Legend
  * Earthworm Jim: Menace 2 the Galaxy
  * F-15 Strike Eagle
  * Ferrari: Grand Prix Challenge
  * The Fidgetts
  * Flipper & Lopaka
  * Gex 3: Deep Cover Gecko
  * Gold and Glory: The Road to El Dorado
  * Gremlins Unleashed
  * Hype: The Time Quest
  * Indiana Jones and the Last Crusade
  * Jimmy Connors Tennis/Yannick Noah Tennis
  * Joe & Mac: Caveman Ninja
  * John Madden Football
  * Jungle Strike
  * Kirikou
  * Laura
  * Lemmings 2: The Tribes
  * Looney Tunes 2: Tazmanian Devil in Island Chase
  * Men in Black 2: The Series
  * Papyrus
  * Pinocchio
  * Pitfall: Beyond the Jungle
  * Resident Evil (prototypes)
  * Robin Hood
  * Sensible Soccer: European Champions
  * Snow White and the Seven Dwarfs
  * Speedy Gonzales
  * Spirou: The Robot Invasion
  * Star Hawk
  * Star Wars
  * Star Wars: The Empire Strikes Back
  * Super Off-Road
  * Top Gun: Fire Storm
  * Ultimate Fighting Championship
  * VIP
  * Les Visiteurs
  * Woody Woodpecker
  * World Cup Striker/Soccer

NES:
  * The Adventures of Dr. Franken (prototype)
  * Aladdin
  * Dragon's Lair
  * Indiana Jones and the Last Crusade (UBI Soft)
  * Jimmy Connors Tennis
  * Joe & Mac: Caveman Ninja
  * Prince of Persia

Game Gear/Master System:
 * F-15 Strike Eagle
 * Lemmings 2: The Tribes
 * Poker Faced Paul's Blackjack
 * Poker Faced Paul's Gin
 * Poker Faced Paul's Poker
 * Poker Faced Paul's Solitaire
 * RoboCop vs. The Terminator

## To do:
  * Panning support
  * Support for other versions of the sound engine (Game Boy Advance)
  * GBS and NSF (and SGC?) file support
