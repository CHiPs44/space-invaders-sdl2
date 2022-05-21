# Space Invaders 1978

## Inspiration

<https://blog.loadzero.com/blog/si78c/> / <https://github.com/loadzero/si78c>

<https://computerarcheology.com/Arcade/SpaceInvaders/Code.html>

## How to build

Tools & dependencies

```bash
sudo apt install build-essentials cmake libsdl2-dev libsdl2-image-dev
```

Building the game itself

```bash
mkdir build
cd build
cmake ..
make
```

To launch game from `build` directory:

```bash
ln -s ../assets assets
./space-invaders
```

## Keys

- [1] start 1 player game
- [left] move shift left
- [right] move shift right
- [space] fire

## TODO

- debug player fire
- aliens kills
- alien bombs
- alien victory
- shield demolition
- saucer management
- 2 player game

`EOF`
