# Licenta UAIC
## Introduction
This is the project used for the demonstrations of my bachelor's thesis, which was discussing the topic of impoverished areas and why many remain like that. It is a visual representation of the distribution of resources between communities with different levels of development.

## How it works
The project consists of a map made of 5150 hexagons, displayed in a 100 x 50 grid. These have a type of biome assigned to them (one of the following: ocean, sea, plains, hills, mountains). They make up the foundation of the map, on which are placed multiple cities. These cities then collect resources from the tiles closest to them (by pressing V we display a Voronoi Diagram which shows the areas of control of each city). The resources are then distributed among cities, using a formula which causes cities with more roads connected to them to receive more resources than ones with less connections. This is used to visualize the phenomenon where people usually go to live and add value to places which can provide more opportunities (for example people from a small village are more eager to go and work/add value in a big city).

## Algorithms
Perlin Noise - for terrain generation

Fortune's algorithm - for drawing the Voronoi Diagram

Dijkstra's algorithm - for making the roads between cities

## Technologies used
Programming language: C++

Graphic library: SFML
