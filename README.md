# Space Invaders 1978

## Inspiration

- <https://blog.loadzero.com/blog/si78c/> / <https://github.com/loadzero/si78c>
- <https://computerarcheology.com/Arcade/SpaceInvaders/Code.html>

## How to build

### Installing tools & dependencies

```bash
sudo apt install build-essentials cmake libsdl2-dev libsdl2-image-dev
```

### Building the game itself from project's root

```bash
mkdir build
cd build
cmake ..
make
```

### Launching game from `build` directory

```bash
ln -s ../assets assets
./space-invaders
```

## Scenes

- BOOT startup, displays random chars for 1+ second
- HOME menu with score for each invader
- PLAY intro for player 1 or 2
- GAME game itself for player 1 or 2
- OVER player loose (TODO)

## Keys

| Key | Description  |
| --: | :----------- |
| [1] | start 1 player game|
| [2] | start 2 players game (TODO)|
| [left arrow] | move ship left|
| [right arrow] | move ship right |
| [space] | fire |
| [escape] | quit |
| [c] | insert coin / add credit |
| [r] | restart to boot scene |
| [+ (numpad)] |increase speed |
| [- (numpad)] |decrease speed |
| [enter (numpad)] | change screenshot |
| [g] | toogle grid |
| [d] | toogle debug |
| [s] | toogle screenshots |

## TODO

- [X] debug player fire
- [X] hide/show debug
- [_] hide/show screenshots (SDL window reset)
- [_] aliens kills
- [_] alien bombs
- [_] ship kill
- [_] alien victory
- [_] player victory & next level (level counter?)
- [_] shield demolition (by player & aliens)
- [_] saucer management (frequency & score)
- [_] high score load / save
- [_] 2 player game
- [_] sounds (background, fire, bombs?, kills, saucer, ...)
- [_] credits (scene or pop-up with semi-graphic chars)?
- [_] animations from original game on home scren?
- [_] backgrounds/borders arounds 4/3 screen?
- [_] buttons + mouse for commands / settings?
- [_] ...

`EOF`
