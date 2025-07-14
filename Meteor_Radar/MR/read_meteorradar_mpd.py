#!/usr/bin/env python
import numpy as np
import array,datetime

def read_meteorradar_mpd(filename, headerlength=None, verbose=None):

    if headerlength==None: headerlength=29

    datain = []
    try:
        f=open(filename,'r')
    except FileNotFoundError:
        return datain

    a=f.readlines()

    b = a[0].split()
    if b[-1]=='2.1':
        headerlength=27
    elif float(b[-1])<=2:
        headerlength=21
    else:
        headerlength=29

    if (verbose!=None)&(verbose!=0):
        print(' ... Reading input from mpd file: '+filename)
        print(' ')
        for j in range(headerlength):
            print(a[j][:-1])

    for i in range(headerlength,len(a)):
        try:
            datain.append([datetime.datetime(int(a[i][:5]),int(a[i][6:8]),int(a[i][9:11]),\
                  hour=int(a[i][11:14]),minute=int(a[i][15:17]),second=int(a[i][18:20]), \
                  microsecond=int(float(a[i][20:24])*1000000)),a[i][25:30], \
                  float(a[i][31:36]),float(a[i][37:43]),float(a[i][44:51]), \
                  float(a[i][52:58]),float(a[i][59:65]),float(a[i][66:73]), \
                  int(a[i][74:76]),float(a[i][77:84]),int(a[i][85:91]), \
                  int(a[i][92:100]),float(a[i][101:108]),float(a[i][109:114]), \
                  float(a[i][115:120]),float(a[i][121:])])
        except ValueError:
            continue

    f.close() 

    if (verbose!=None)&(verbose!=0):
        print(' ... Total data counts: '+str(len(datain)))

    return datain
