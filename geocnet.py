#Developed by Jessica Domingues (jessica.dominguess@gmail.com)

import numpy
import configparser
import os
import sys
import psycopg2
import subprocess

#--Variaveis--#

config = configparser.ConfigParser()
py_path = os.path.dirname(os.path.abspath(__file__))
app_path = py_path.replace('py','')
config.readfp(open(os.path.join(py_path,'defaults.cfg')))



matrix = config.get('PATH','matrix')
properties = config.get('PATH','properties')

#---('DB','port')
hostDB = config.get('DB','host')
userDB = config.get('DB','user')
passDB = config.get('DB','pasw')
nameDB = config.get('DB','name')
portDB = config.get('DB','port')
SRID = config.get('DB','srid')

matrix = config.get('PATH','matrix')
properties = config.get('PATH','properties')
file_points = config.get('PATH','points')
file_lines = config.get('PATH','lines')
points_out = config.get('PATH','points_out')

n_row = 54
n_col = 54
n_times = 1

class Data:
  
  def __init__(self):
    self.conn = None

            
  def leitura_dados(self):
    f = numpy.loadtxt(matrix)
    a = f.reshape((1,54,54))
    return a 
    
  def get_diameter(self):
    p = open(properties,'r')
    data  = p.readlines()
    data = [x.strip('\n') for x in data] 
    count_lines = sum(1 for line in data)
    
    diameter_list = []
    
    for i in range(1,count_lines,16):
      diameter_list.append(data[i])
      diameter_list = [d.replace('\n','') for d in diameter_list]
      diameter = numpy.array(diameter_list,dtype='int32')
   
    return diameter
  
  def get_avg_clustering(self):
    p = open(properties,'r')
    data  = p.readlines()
    data = [x.strip('\n') for x in data] 
    
    count_lines = sum(1 for line in data)
    
    avg_clustering_list = []
    
    for i in range(3,count_lines,16):
      avg_clustering_list.append(data[i])
      avg_clustering = numpy.array(avg_clustering_list, dtype='float64' )

    return avg_clustering
  
  def get_clustering(self):
    f = open(properties,'r')
    data = f.readlines()
    data = [x.strip('\n') for x in data] 
    clustering_list = []
    
    count_lines = sum(1 for line in data)
    
    for i in range(5,count_lines,16):
      clustering_list.append(data[i].strip().split(' '))
      aux = numpy.array(clustering_list, dtype='float64' )
    
    clustering =  aux.reshape((n_times,n_row))
    return clustering 

  def get_avgDegree(self):
    p = open(properties,'r')
    data  = p.readlines()
    data = [x.strip('\n') for x in data] 
    
    count_lines = sum(1 for line in data)
    
    avg_degree_list = []
    
    for i in range(7,count_lines,16):
      avg_degree_list.append(data[i])
      avgDegree = numpy.array(avg_degree_list, dtype='float64' )
   
    return avgDegree


  def get_degreeAll(self):
    f = open(properties,'r')
    data = f.readlines()
    data = [x.strip('\n') for x in data] 
    degree_all_list = []
    
    count_lines = sum(1 for line in data)
    
    for i in range(9,count_lines,16):
      degree_all_list.append(data[i].strip().split(' '))
      aux = numpy.array(degree_all_list, dtype='float64' )
    
    degree_all =  aux.reshape((n_times,n_row))
    return degree_all  
  
  def get_degreeIn(self):
    f = open(properties,'r')
    data = f.readlines()
    data = [x.strip('\n') for x in data] 
    degree_in_list = []
    
    count_lines = sum(1 for line in data)
    
    for i in range(11,count_lines,16):
      degree_in_list.append(data[i].strip().split(' '))
      aux = numpy.array(degree_in_list, dtype='float64' )
    
    degree_in =  aux.reshape((n_times,n_row))
    return degree_in 

  def get_degreeOut(self):
    f = open(properties,'r')
    data = f.readlines()
    data = [x.strip('\n') for x in data] 
    degree_out_list = []
    
    count_lines = sum(1 for line in data)
    
    for i in range(13,count_lines,16):
      degree_out_list.append(data[i].strip().split(' '))
      aux = numpy.array(degree_out_list, dtype='float64' )
    
    degree_out =  aux.reshape((n_times,n_row))
    return degree_out 
  
  def get_betweenness(self):
    f = open(properties,'r')
    data = f.readlines()
    data = [x.strip('\n') for x in data] 
    betweenness_list = []
    
    count_lines = sum(1 for line in data)
    
    for i in range(15,count_lines,16):
      betweenness_list.append(data[i].strip().split(' '))
      aux = numpy.array(betweenness_list, dtype='float64' )
    
    betweenness =  aux.reshape((n_times,n_row))
    return betweenness  
    
class ConnectPoints:
  def __init__(self, fluxo):
    self.create_database()
    self.create_main_table()
    self.import_shapefile()
    self.create_connections(fluxo)
    self.save_shapefile()
  
  
  def create_database(self):
    os.environ['PGPASSWORD'] = passDB
    cont=subprocess.check_output('psql -lqt -h ' + hostDB + ' -U ' + userDB + ' -w | cut -d \| -f 1 | grep ' + nameDB + ' | wc -l', shell=True).decode('ascii').strip()
    if cont == '0':
        os.system('psql -h '+hostDB+' -p '+portDB+' -U '+userDB+' -w -c "create database '+nameDB+'"')
        conn = psycopg2.connect("host="+hostDB+" port="+portDB+" dbname="+nameDB+" user="+userDB+" password="+passDB)
        cur = conn.cursor()
        cur.execute('create extension postgis')
        conn.commit()
        cur.close()
        conn.close()
    self.conn = psycopg2.connect("host="+hostDB+" port="+portDB+" dbname="+nameDB+" user="+userDB+" password="+passDB)
   
  def create_main_table(self):
    cur = self.conn.cursor()
    
    cur.execute('DROP TABLE IF EXISTS ml_pontos')
    self.conn.commit()
    
    cur.execute('DROP TABLE IF EXISTS ml_linhas')
    self.conn.commit()
 
    cur.execute('CREATE TABLE ml_linhas (gid serial, \
      origin smallint, \
      destination smallint, \
      geom geometry(linestring,4326), \
      weight numeric, \
      CONSTRAINT ml_linhas_pkey PRIMARY KEY (gid))')
        
    self.conn.commit()
    cur.close()
  
  def import_shapefile(self):
    os.environ['PGPASSWORD'] = passDB
    os.system('shp2pgsql -s '+SRID+' -d -I -W "latin1" '+file_points+' public.ml_pontos | psql -h '+hostDB+' -p '+portDB+' -d '+nameDB+' -U '+userDB+' -w')
    cur = self.conn.cursor()
    cur.execute('alter table ml_pontos add column degree_avg_all smallint DEFAULT 0')
    cur.execute('alter table ml_pontos add column degree_all smallint DEFAULT 0')
    cur.execute('alter table ml_pontos add column degree_in smallint DEFAULT 0')
    cur.execute('alter table ml_pontos add column degree_out smallint DEFAULT 0')
    cur.execute('alter table ml_pontos add column clustering numeric(6,2) DEFAULT 0.0')
    cur.execute('alter table ml_pontos add column cluster_avg numeric(6,2) DEFAULT 0.0')
    cur.execute('alter table ml_pontos add column diameter smallint DEFAULT 0')
    cur.execute('alter table ml_pontos add column betweenness numeric(6,2) DEFAULT 0.0')

    self.conn.commit()      
    cur.close()
  
  def create_connections(self, fluxo):
    cur = self.conn.cursor()

    lines = fluxo
    i = 0
    for line in lines:
      #print(line)
      i = i + 1
      j = 0
      for cols in line:
       j = j + 1
       if cols != 0:
          cur.execute('insert into ml_linhas (origin, destination, geom, weight) values (%s,%s,st_makeline((select geom from ml_pontos where id = %s),(select geom from ml_pontos where id = %s)),%s)',(i,j,i,j,cols))
    self.conn.commit()
 
  def save_shapefile(self):
    os.system('pgsql2shp -h '+hostDB+' -p '+portDB+' -u '+userDB+' -P '+passDB+' -f '+file_lines+' '+nameDB+' "select * from ml_linhas;"')
  
  def save_shapefile_points(self):
    os.system('pgsql2shp -h '+hostDB+' -p '+portDB+' -u '+userDB+' -P '+passDB+' -f '+points_out+' '+nameDB+' "select * from ml_pontos;"')
  
  
class Graph:
  def __init__(self, array, table):
    self.conn = None
    self.conn = psycopg2.connect("host="+hostDB+" port="+portDB+" dbname="+nameDB+" user="+userDB+" password="+passDB)
 
 
    if table == 'diameter':
      self.update_diameter(array,table)
    
    elif table == 'cluster_avg':
      self.update_avg(array,table)
    
    elif table == 'degree_avg_all':
      self.update_avg(array,table)
    
    else:
      self.update_2D(array,table) 
   
   
   
  def update_2D(self, array, table):
    cur = self.conn.cursor()
    i = 0
    for value in array:
      i = i + 1 
      cur.execute('update ml_pontos set '+table+' = %s where id = %s',(value,i))
    self.conn.commit()
    
  def update_diameter(self, array, table):
    cur = self.conn.cursor()
    i = 0
    value = int(array[0])
    cur.execute('update ml_pontos set '+table+' = %s where id > %s',(value, i))
   
    self.conn.commit()
       
  def update_avg(self, array, table):
    cur = self.conn.cursor()
    i = 0
    value = array[0]
    cur.execute('update ml_pontos set '+table+' = %s where id > %s',(value, i))
   
    self.conn.commit()
    
    
if __name__ == '__main__':
    l = Data()
    data = l.leitura_dados()
    

    diameter = l.get_diameter()
    avg_c = l.get_avg_clustering()
    cluster = l.get_clustering()
    avg_d = l.get_avgDegree()
    degreeAll = l.get_degreeAll()
    degreeIn = l.get_degreeIn()
    degreeOut = l.get_degreeOut()
    betweenness = l.get_betweenness()
    
    lp = ConnectPoints(data[0])
      
    Graph(diameter,'diameter')
    Graph(avg_c,'cluster_avg')
    Graph(avg_d,'degree_avg_all')
    Graph(cluster[0],'clustering')
    Graph(degreeAll[0],'degree_all')
    Graph(degreeIn[0],'degree_in')
    Graph(degreeOut[0],'degree_out')
    Graph(betweenness[0],'betweenness')
    
    lp.save_shapefile_points()
      
