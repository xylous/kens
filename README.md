# kens

A snake game for the terminal.

## Getting started

### Requirements

For building:

- GNU make
- GCC

For running:

- ncurses

### Installation

Get the project directory and build with `make`. A `kens` binary should be made
available after compiling is over.

```
git clone https://github.com/xylous/kens kens
cd kens
make
```

### Usage

The basic snake game rules: use arrow keys to move the snake head around. Eat
fruit to expand. If you overlap your snake, you lose. If you fill in the entire
grid, you win. BUT: going through corners wraps you around to the other side.

You have a 20-by-20 grid to work with, for a total of 400 squares.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[GPLv3](./LICENSE)
