# Licenta UAIC
## Introduction
This is the project used for the demonstrations of my bachelor's thesis, which was discussing the topic of impoverished areas and why many remain like that. It is a visual representation of the distribution of resources between communities with different levels of development.

## How it works
The project consists of a map made of 5150 hexagons displayed in a 100 x 50 grid, each having a type of biome assigned. Then 8 cities are placed randomly on the map, all valid roads are traced between them and have the surounding tiles attributed to them.

## Algorithms
Perlin Noise - for terrain generation

Fortune's algorithm - for drawing the Voronoi Diagram

Dijkstra's algorithm - for making the roads between cities

## Technologies used
Programming language: C++

Graphic library: SFML
