# US Map Route Planner

## Overview 

This project represents a map of the United States highway network as a graph with vertices (equivalent to cities or intersections), edges (highways or road segments), and edge weights (physical distance in miles between two connected vertices). Within the project, there are two main parts: GraphProcessor, which handles graph data and calculating the shortest path for a user to take, and GraphDemo, which allows for user interaction and route visualization. 

## Detailed Overview

1) The program takes a .graph file that defines vertices, edges, and edge weight as an input
2) The user selects two cities in the US using name or coordinates 
3) Using Dijkstra's Algorithm, the algorithm finds the shortest distance between the two vertices closest to the two destinations 
4) After computing the path, the program outputs the total distance, total execution time, and draws the route on a map of the United States using the Visualize class

## Skills Used/Key Features

- <big>**Graph Data Structures**</big> Uses graphs and weighted edges to simulate calculating the shortest route on a map
- <big>**Dijkstras Algorithm**</big> An algorithm used to calculate the shortest path between nodes in a weighted graph. Uses a priority queue to explore the closet unexplored node. For each adjacent node, calculate its distance from start via current, if new distance shorter, update and mark the node as visited. Repeat until shortest distance found
- <big>**Visualization**</big> Displays the calculated route on top of a static, real-world US map 
- <big>**Performance Optimization**</big> Uses priority queues and processing alogrithms to maximize performance 

## File Purposes

*Algorithm Files*
- <big>**GraphProcessor.java**</big> Contains methods to intialize the graph, locate the nearest points, and compute the shortest path and total route distance
- <big>**Point.java**</big> Defines the Point object, which represents a geographic location, contains latitude and longitude
- <big>**Visualize.java**</big> Defines methods used to display calculated routes over a map using the StdDraw library

*Other Files*
- <big>**TestSimpleGraphProcessor.java**</big> Uses a small test dataset to verify nearest point calculation, route computation, and connectivity
- <big>**TestUSGraphProcessor.java**</big> Larger dataset to evaluate performance on real world cities found on a US map, test shortest, medium, and longest routes, checks error handling
- <big>**StdDraw.java**</big> Utility class commonly used for drawing graphics 

## How to Run Program/Inputs 

- Compile and run GraphDemo 
- Follow the on-scree instructions to select two cities, either by name or latitude/longitude 

## Outputs

- The program will draw the shortest route over a real world map, and print the total distance (in miles) and program execution time

## Next Steps 

- Factor in road speeds and not only distances
- Give alternate routes - fastest vs shortest route
- Add a method to avoid dead ends, ie if a route is heading towards the ocean 