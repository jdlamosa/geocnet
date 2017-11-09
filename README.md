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

Installation Guide in portuguese and english: 
- https://docs.google.com/document/d/1SoRU4vb75tS_yNZXZWe_uC28VIWQGbFv3kmXCIl5Cjk/edit?usp=sharing
 
