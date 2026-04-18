# BFS visualizer

A small tool that animates breadth-first search on a hand-made graph. Built with C++ and SFML 3.

![demo](gifs/demo.gif)

## What it does

Press space to start. From each visited node, points spawn and travel simultaneously toward its unvisited neighbors, edges are traced one parent at a time.

## Build

Requires SFML 3.x. On systems where `apt` provide SFML 2.x, install 3.x from source — see the [SFML docs](https://www.sfml-dev.org/tutorials/3.0/).

g++ src/main.cpp -o bfs -lsfml-graphics -lsfml-window -lsfml-system
./bfs

or open in VS code and run the build task.

## Notes

Built it to get a feel of SFML's render loop and also practice managing a dynamic list of moving points.