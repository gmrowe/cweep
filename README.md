# mines

A minesweeper clone in C. The graphical layer is handled by raylib.
The project is strictly a learning exercise. Should get some decent C knowlege
out of it.

![Current state of game](./mines_screenshot.png)

# Build

To build the project, run make

``` shell
make
```

This should build the `mines` executable. Then just run it:

``` shell
./mines
```

# Controls
 - Mouse Right-click | Keyboard 'M'     = Flag/Unflag suspected mine
 - Mouse Left-click  | Keyboard 'Spave' = Reveal cell

# TODO

[X] Enable mines remaining counter \
[X] Enable timekeeping \
[X] Enable first guess safety \
[ ] Chord reveal all neigbors \
[ ] Show all unmarked mines if loss \
[ ] Show incorrecly marked mines if loss \
[ ] Show animation on win \
[ ] Add actual flag image to cells when marking \
[ ] In game restart \
[ ] In game difficulty option \
[ ] High scores \
