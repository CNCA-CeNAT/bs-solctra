# coding: utf-8
import os
path = os.getcwd()
arr = []
arr1 = []
carp="/RBSSolctra"
direccion=path+carp
carp1="/RBSSolctra1"
direccion1=path+carp1

for files in next(os.walk(direccion))[2]:
    arr.append(direccion+"/"+files)

for files in next(os.walk(direccion))[2]:
    arr1.append(direccion1+"/"+files)

headers='x, y, z'

tam=0
for files in arr:
    file = open(files, "r+")
    filenew = open(arr1[tam], "w")
    filenew.write(headers + '\n')
    for line in file:
        linea=line.replace("\t", ", ")
        filenew.write(linea)
    tam+=1
    filenew.close()
