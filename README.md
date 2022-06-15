# Space Invaders 1978 - SDL2

## Inspiration

Original Space Invaders from 1978:

- emulated with original ROM files: <https://blog.loadzero.com/blog/si78c/> / <https://github.com/loadzero/si78c>
- original Z80 source code with comments: <https://computerarcheology.com/Arcade/SpaceInvaders/Code.html>
- another emulation with ROM files: https://github.com/superzazu/invaders

Screenshots where taken from the amazing Javascript "video" player embedded in loadzero's website.

Graphical assets were drawn manually with GIMP, Pinta and Drawing, neither of them being a good tool to edit such tiny bitmaps.

## Getting started

### Install tools & dependencies

With Ubuntu 22.04 / 20.04 (should work with other Debian derivatives, too):

```bash
sudo apt install git build-essential cmake libsdl2-dev libsdl2-image-dev
```

### Initialize the local repository

```bash
cd <source-root>
git clone <project>
git submodule init
git submodule update
```

NB: thanks to <https://gitlab.com/aminosbh/sdl2-cmake-modules> for the SDL2 cmake integration!

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
- `[NEXT]` next level for player 1 or 2 (TODO)

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

- [x] player move [GAME]
- [x] player fire [GAME]
- [x] aliens move [GAME]
- [ ] aliens kills [GAME]
- [ ] alien bombs [GAME]
- [ ] player kill by bombs [GAME]
- [ ] alien victory [GAME]
- [ ] player victory [GAME]
- [ ] shield demolition (by player & aliens) [GAME]
- [ ] saucer management (frequency & score) [GAME]
- [ ] sounds (background, fire, bombs (?), kills/explosions, saucer, ...) [GAME]
- [ ] next level after victory [NEXT]
- [ ] 2 player game
- [ ] horizontal color stripes (adhesive tape style)
- [ ] high score load / save
- [ ] flash 0000 score for 1+ seconds on [PLAY] scene
- [ ] animations from original game on home screen [HOME]

### Ideas

- [ ] function keys instead of 1, 2 and others?
- [ ] level counter?
- [ ] pause game?
- [ ] credits (scene or pop-up with semi-graphic chars)?
- [ ] backgrounds/borders around 4/3 screen?
- [ ] buttons + mouse for commands / settings (within borders)?
- [ ] fullscreen mode?
- [ ] DEB/RPM packages (with OpenSUSE build service)?

`EOF`
