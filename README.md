# Space Invaders 1978 - SDL2

## Inspiration

Original Space Invaders from 1978:

- either emulated with ROM files: cf. <https://blog.loadzero.com/blog/si78c/> & <https://github.com/loadzero/si78c>
- or original Z80 code: <https://computerarcheology.com/Arcade/SpaceInvaders/Code.html>

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

- BOOT startup, displays random chars for 1+ second
- HOME menu with score for each invader
- PLAY intro for player 1 or 2
- GAME game itself for player 1 or 2
- OVER player loose (TODO)

## Keys

| Key               | Description                       |
| ----------------: | :-------------------------------- |
| [1]               | start 1 player game               |
| [2]               | start 2 players game (TODO)       |
| [Left arrow]      | move ship left                    |
| [Right arrow]     | move ship right                   |
| [space]           | fire                              |
| [escape]          | quit                              |
| [c]               | insert coin / add credit          |
| [r]               | reboot (restart to boot scene)    |
| [+ (numpad)]      | increase speed                    |
| [- (numpad)]      | decrease speed                    |
| [g]               | toogle grid                       |
| [d]               | toogle debug                      |
| [s]               | toogle screenshots                |
| [enter (numpad)]  | change screenshot                 |

## TODO

### UX/UI

- [x] debug player fire
- [x] hide/show debug
- [x] hide/show screenshots

### Game

- [ ] aliens kills
- [ ] alien bombs
- [ ] ship kill
- [ ] alien victory
- [ ] player victory & next level (level counter?)
- [ ] shield demolition (by player & aliens)
- [ ] saucer management (frequency & score)
- [ ] 2 player game
- [ ] sounds (background, fire, bombs (?), kills/explosions, saucer, ...)
- [ ] horizontal color stripes (adhesive tape style)
- [ ] high score load / save
- [ ] animations from original game on home screen

### Ideas

- [ ] pause game?
- [ ] credits (scene or pop-up with semi-graphic chars)?
- [ ] backgrounds/borders around 4/3 screen?
- [ ] buttons + mouse for commands / settings (within borders)?
- [ ] fullscreen mode?
- [ ] DEB/RPM packages (with OpenSUSE build service)?

`EOF`
