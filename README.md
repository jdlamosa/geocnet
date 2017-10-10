# geocnet

Development :
- Developed in C/C++
- Time window - data agregation
- 3 dimensional matrix, with origin, destination and time
- topological properties:
  - Degree, Clustering Coefficient, diameter and betweenness

Tools:
- igraph to calculate measures
- python to communicate with database and create shapefile
- postgres to store network properties
- qgis to visualize the shapefile

Dependencies:
- lib Igraph - C
- PostgreSQL 9.5+
- PostGis 2.2
- Python 3 (numpy, configparser, os, sys, psycopg2, subprocess)
 
