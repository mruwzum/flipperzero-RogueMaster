# Flipcraft

A real first-person voxel survival sandbox, rendered in true 3D on the 128x64 one-bit screen. Walk through a procedurally generated world, chop trees, dig for coal and iron, smelt ingots and glass, craft tools at a crafting table, build with placed blocks, stash loot in chests, tame a wolf and try to outlive creepers and your own dynamite.

Flipcraft is an independent project by ApertureFox Technology. It is written from scratch for the Flipper Zero: a software rasterizer with a depth buffer, cached chunk meshes and an SD-card streaming world that never has to fit in memory at once.

## Worlds

Every world grows from a text seed in four sizes, from 128x128 up to 1024x1024 blocks: forests, deserts, ravines, ore veins and one house with a stocked chest somewhere on the map. The same seed produces the same terrain at any size. The world, your inventory, health and the contents of every chest and furnace are saved on the SD card as a single .fcw file, and any number of worlds can be created, renamed, inspected and deleted from the menu.

Each world carries its own settings, chosen at creation and editable later:

- **Gamemode** - Survival, Hardmode (death deletes the world) or Creative (infinite blocks from a picker, no damage, no drops).
- **Terrain** - Normal (hills, forests, deserts, ravines), Flat (a level plain five courses deep, with trees and a house), Superflat (the same plain, completely bare) or Woods (normal relief, no deserts, twice the trees). Chosen at creation.
- **Mobs** - on or off.
- **Draw distance** - Far renders the whole chunk ring around you, Near renders only the chunk you stand in and is the lightest mode in both RAM and time.

## Gameplay

- 17 block types, 3x3 crafting, a furnace fueled by coal or wood, chests that keep their contents.
- Pickaxes, axes, shovels and swords in wood, stone and iron tiers, plus shears and dynamite.
- Sheep, wolves, creepers and bees. Wolves hunt sheep and creepers on their own and can be tamed with two apples; a tamed wolf guards you and never bites. A creeper stalks you, swells for two seconds and blows a 3x3x3 crater. Bees drop saplings.
- Dynamite is placed like a block and lit with a short press; adjacent charges chain-react.
- Eight hearts, fall damage, apples to heal. In Survival you respawn where you died; in Hardmode you do not.

## Controls

In the world:

- **Up / Down** - walk forward / backward
- **Left / Right** - turn
- **OK + Up / Down** - look up / down
- **OK + Left / Right** - previous / next hotbar slot (in Creative: scroll the block palette)
- **OK short** - place a block, use the crafting table, furnace or chest, light dynamite, or hit the creature in front of you
- **OK long** - mine or break the targeted block; on a wild wolf, tame it (costs two apples)
- **Back short** - jump
- **Back long** - open the inventory (in Creative: the block picker)
- **OK + Back** - save and quit to the menu

Creatures are forgiving to aim at: anything near the crosshair counts. Blocks need the crosshair itself.

In the inventory, crafting, furnace and chest screens:

- **Arrows** - move the cursor
- **OK** - pick up a stack, put it down, or take the crafted or smelted output
- **OK + arrow** - drop one item from the held stack into each slot you cross
- **Back** - close

In the menu the arrows move, OK confirms and Back returns.
