Do you want to see your geolocalized complex network with topological properties? Now you can! Using the geocnet tool you can visualize your complex Networks as a shapefile of nodes and a shapefile of edges in a GIS. 

# geocnet

Development :
- Developed in C
- topological properties:
  - Degree, Clustering Coefficient, Diameter and Betweenness

Tools used:
- igraph to calculate measures
- python to communicate with database and create shapefile
- postgres to store network properties
- qgis to visualize the shapefile

Dependencies:
- lib Igraph - C
- PostgreSQL 9.5+
- PostGis 2.2
- Python 3 (numpy, configparser, os, sys, psycopg2, subprocess)

Installation Guide in portuguese and english: geoCnet.pdf

INPUT: 
- A shapefile of the nodes with a mandatory column of type integer named id, representing the id's of the nodes. All the characteristics of the polygons/points will be associated to their respective points as attributes. To create a shapefile for the nodes - using any GIS software, it is necessary to create a point type shapefile for the nodes, with the nodes at their given position.

- Weighted adjacency matrix representing the connections between these nodes.

OUTPUT: 
- A line type shapefile representing the edges of the network. The point-type-shapefile and the line-type-shapefile will have topological attributes of nodes and edges respectively.
 
