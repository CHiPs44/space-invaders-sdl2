# Space Invaders 1978 - SDL2

## Inspiration

Original Space Invaders from 1978:

- emulated with original ROM files: <https://blog.loadzero.com/blog/si78c/> / <https://github.com/loadzero/si78c>
- original Z80 source code with comments: <https://computerarcheology.com/Arcade/SpaceInvaders/Code.html>

Screenshots where taken from the amazing Javascript "video" player embedded in loadzero's website.

Graphical assets were drawn manually with GIMP, Pinta and Drawing, neither of them being a good tool to edit such tiny bitmaps.

## Getting started

### Install tools & dependencies

```bash
sudo apt install build-essentials cmake libsdl2-dev libsdl2-image-dev
```

### Initialize the local repository

```bash
cd <source-root>
git clone <project>
git submodule init
```

### Build the game

```bash
cd <project-root>
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

- `[BOOT]` startup, displays random chars for 1+ second
- `[HOME]` menu with score for each invader
- `[PLAY]` intro for player 1 or 2
- `[GAME]` game itself for player 1 or 2
- `[OVER]` player(s) loose (TODO)

## Keys

|              Key | Description                    |
| ---------------: | :----------------------------- |
|              [1] | start 1 player game            |
|              [2] | start 2 players game (TODO)    |
|     [left arrow] | move ship left                 |
|    [right arrow] | move ship right                |
|          [space] | fire                           |
|         [escape] | quit                           |
|              [c] | insert coin / add credit       |
|              [r] | reboot (restart to boot scene) |
|     [+ (numpad)] | increase speed                 |
|     [- (numpad)] | decrease speed                 |
|              [g] | toogle grid                    |
|              [d] | toogle debug                   |
|              [s] | toogle screenshots             |
| [enter (numpad)] | change screenshot              |

## TODO

### UX/UI

- [x] 4/3 landscape screen with borders instead of original 8/7 portrait aspect ratio
- [x] hide/show debug
- [x] hide/show screenshots

### Game

- [x] player move
- [x] player fire
- [x] aliens move
- [ ] aliens kills
- [ ] alien bombs
- [ ] player kill by bombs
- [ ] alien victory
- [ ] player victory & next level
- [ ] shield demolition (by player & aliens)
- [ ] saucer management (frequency & score)
- [ ] 2 player game
- [ ] sounds (background, fire, bombs (?), kills/explosions, saucer, ...)
- [ ] horizontal color stripes (adhesive tape style)
- [ ] high score load / save
- [ ] animations from original game on home screen

### Ideas

- [ ] level counter?
- [ ] pause game?
- [ ] credits (scene or pop-up with semi-graphic chars)?
- [ ] backgrounds/borders around 4/3 screen?
- [ ] buttons + mouse for commands / settings (within borders)?
- [ ] fullscreen mode?
- [ ] DEB/RPM packages (with OpenSUSE build service)?

`EOF`
