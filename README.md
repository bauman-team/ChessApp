# Game chess for windows pc.

### For frontend used libraries [TGUI](https://tgui.net/), [SFML](https://www.sfml-dev.org/).

### In master branch 
- Good interface created
- Two players game is working (include time mode)
- AI game is working (include time mode)

### In develop branch
- Playing mode with NN (Nerual Network)
- Improving side menu
- Add pawn transform
- Add scaling
- Improving interface with statistics
- Game completion menu when someone won
- Choose started figures positions of game

## Installing:
- Project will be cross-platform, now tested Windows and Linux (Ubuntu).
- For building project needs Git, CMake and compiler.

```powershell
git clone https://github.com/bauman-team/ChessApp.git ChessApp
cd ChessApp
mkdir build
cd build
cmake ..
cmake --build .
```

- On windows need:

Put to the executable file directory this files of shared libraries:
```powershell
sfml-graphics
sfml-window
sfml-audio
sfml-system
tgui
```