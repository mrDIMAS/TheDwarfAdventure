# The Dwarf Adventure

2D sandbox adventure RPG with destructible and generated world.

## Story
You are a dwarf and you live in a deserted kingdom. For a long time your people lived in wealth and prosperity, but on some day a huge chunk of mysterious energetic crystals was found. These crystal were called Arkalith, they were used to create amazing machines and powerful weapons that allowed the kingdom to seize neighboring kingdoms. Your kingdom fell into decay after the discovery of the mysterious Black creature deep underground in the thick of the arkalith. The ancient creature immediately incinerated the dwarves who were mining this sinister mineral. After a while the kingdom was filled with deadly creatures from other worlds. Royal forces were not prepared for this strike - dwarves dying everyday since then, not so many of them can survive. Discovery of Arkalith also affected ordinary creatures - bees, frogs, bats - even ordinary bee became huge and dangerous. Swords and axes became useless, creatures from other worlds could be destroyed only by the energy of the arkalith. You is the only hope for your kingdom!

## Screenshots
- Main menu
![Main menu](/release_screenshots/menu.png?raw=true "Main Menu")
- Editor
![Editor](/release_screenshots/editor.png?raw=true "Editor")
- Underground settlement
![Underground settlement](/release_screenshots/underground_settlement.png?raw=true "Underground settlement")
![Underground settlement 2](/release_screenshots/underground_settlement2.png?raw=true "Underground settlement 2")
- Caves
![Caves](/release_screenshots/caves.png?raw=true "Caves")
![Caves 2](/release_screenshots/caves2.png?raw=true "Caves 2")
- Flying islands
![Flying islands](/release_screenshots/flying_islands.png?raw=true "Flying islands")
- Settlement
![Settlement](/release_screenshots/settlement.png?raw=true "Settlement")

## Gameplay video
- Soon

## Supported languages
- Russian
- English (soon) 

Any help is appreciated!

## Questions and answers
### Can I win the game?
No. Making of story still in progress. 

### I'm stuck, how can I get out
Use Construction gun to create ladder from soil that you mined by pickaxe.

### Is there some console/debug menu?
Yes. Just press tilde `~`

## System requirements
- 2 core processor with SSE2 support
- 1 Gb RAM
- Videocard with 512 Mb VRAM with OpengGL 3.2 support
- 80 Mb disk space + 2 Gb for save files

## How to build

First of all you need C++17 compiler. I'm using GCC 8.2 on Linux and Visual Studio 2015 (with std::experimental) on Windows.

### Dependencies
Game depends on SFML and GLEW libraries.

### Windows
Use `projects/DwarfDig.sln` on Visual Studio 2015 or higher.

### Linux 
I'm using CodeLite IDE, but you can use generated makefile from `codeliteproj`

### Mac
Unfortunately I didn't tested game on Mac and never build it for Mac. But you can try.

