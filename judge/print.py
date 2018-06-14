#!/usr/bin/python
# -*- coding: utf-8 -*-

VISUAL = True

import sys
import os
import getopt
if VISUAL:
    from PIL import Image
    from PIL import ImageDraw

##########################################

def readBoard(fname):
    try:
        f=open(fname)
        (X,Y) = f.readline().split()
        X = int(X)
        Y = int(Y)
        S = int(f.readline())
        src = []
        for i in range(S):
            (x,y) = f.readline().split()
            src.append((int(x),int(y)))
        # end for
        H = int(f.readline())
        home = []
        for i in range(H):
            (x,y) = f.readline().split()
            home.append((int(x),int(y)))
        # end for
        B = int(f.readline())
        blok = []
        for i in range(B):
            (x,y) = f.readline().split()
            blok.append((int(x),int(y)))
        # end for
        dane = f.read().strip('\n')
    except:
        err(1)
    # end try
    return (X,Y,src,home,blok,dane)
# end def

##########################################

def readPipe(fname):
    try:
        f=open(fname)
        L = int(f.readline())
        pipe = []
        for i in range(L):
            (x,y,typ) = f.readline().split()
            pipe.append((int(x),int(y),typ))
        # end for
    except:
        err(2)
    # end try
    return pipe
# end def

##########################################

def loadImages():
    imgl = ["B","G","S","H","I1","I2","L1","L2","L3","L4","T1","T2","T3","T4","X"]
    imgs = {}
    for i in imgl: imgs[i] = Image.open("img/"+i+".png")
    return imgs
# end def

##########################################

t0 = ['I1','L2','L3','T2','T3','T4','X','H','S']
t1 = ['I2','L3','L4','T1','T3','T4','X','H','S']
t2 = ['I1','L1','L4','T1','T2','T4','X','H','S']
t3 = ['I2','L1','L2','T1','T2','T3','X','H','S']

w0 = ['I1','L2','L3','T2','T3','T4','X']
w1 = ['I2','L3','L4','T1','T3','T4','X']
w2 = ['I1','L1','L4','T1','T2','T4','X']
w3 = ['I2','L1','L2','T1','T2','T3','X']


n0 = ['I2','L1','L4','T1','G','B','H','S']
n1 = ['I1','L1','L2','T2','G','B','H','S']
n2 = ['I2','L2','L3','T3','G','B','H','S']
n3 = ['I1','L3','L4','T4','G','B','H','S']

rule = { 'I1' : ( t0,n1,t2,n3 ),
         'I2' : ( n0,t1,n2,t3 ),
         'L1' : ( t0,t1,n2,n3 ),
         'L2' : ( n0,t1,t2,n3 ),
         'L3' : ( n0,n1,t2,t3 ),
         'L4' : ( t0,n1,n2,t3 ),
         'T1' : ( t0,t1,n2,t3 ),
         'T2' : ( t0,t1,t2,n3 ),
         'T3' : ( n0,t1,t2,t3 ),
         'T4' : ( t0,n1,t2,t3 ),
         'X'  : ( t0,t1,t2,t3 ) }

##########################################

def drawBoard(fname,t,p):

    if not VISUAL: return

    X = t[0]
    Y = t[1]
    src = t[2]
    home = t[3]
    blok = t[4]

    imgs = loadImages()

    boardImg = Image.new( "RGB", (20*X,20*Y) )
    draw = ImageDraw.Draw( boardImg )

    for y in range(Y):
        for x in range(X):
            boardImg.paste( imgs["G"], (x*20,y*20) )

    for (x,y) in src: boardImg.paste( imgs["S"], (x*20,y*20) )
    for (x,y) in home: boardImg.paste( imgs["H"], (x*20,y*20) )
    for (x,y) in blok: boardImg.paste( imgs["B"], (x*20,y*20) )

    for (x,y,typ) in p: boardImg.paste( imgs[typ], (x*20,y*20) )

    boardImg.save(fname)
    os.system("open "+fname)

# end def

##########################################

def checkPipe(t,p):
    X = t[0]
    Y = t[1]
    src = t[2]
    home = t[3]
    blok = t[4]

    dane = t[5]
    #  print src
    #  print home
    #  print blok
    #  print dane
    #  print p

    elem = ['I1','I2','L1','L2','L3','L4','T1','T2','T3','T4','X']

    pl = {}
    for y in range(Y):
        for x in range(X):
            pl[(x,y)]='G'

    # mur dookoła krainy
    for y in range(Y): pl[(-1,y)]=pl[(X,y)]='B'
    for x in range(X): pl[(x,-1)]=pl[(x,Y)]='B'

    for (x,y) in src: pl[(x,y)]='S'
    for (x,y) in home: pl[(x,y)]='H'
    for (x,y) in blok: pl[(x,y)]='B'

    for (x,y,typ) in p:
        pl[(x,y)] = typ
        # kolejność budowania (warunek złagodzony w stosunku do tematu!!!)
        ok=0
        if pl[(x,y-1)] in elem+["S"]: ok=1
        if pl[(x+1,y)] in elem+["S"]: ok=1
        if pl[(x,y+1)] in elem+["S"]: ok=1
        if pl[(x-1,y)] in elem+["S"]: ok=1
        if ok==0: err(11)

    # kontrola długości
    if len(p)>len(dane): err(3)

    # czy rurociąg nie wychodzi poza planszę
    for (x,y,typ) in p:
        if x<0 or x>=X or y<0 or y>=Y: err(9)

    # czy rurociąg nie whodzi na samego siebie
    for i in range(len(p)):
        for j in range(i+1,len(p)):
            if p[i][0]==p[j][0] and p[i][1]==p[j][1]: err(10)

    # kontrola typu elementów
    for (x,y,typ) in p:
        if not typ in elem: err(4)

    # kontrola zgodności elementów rurociągu z danymi
    for i in range(len(p)) :
        if p[i][2][0]!=dane[i]: err(5)
        # end for

    # czy rurociąg tylko na pustych polach
    for (x,y,typ) in p:
        if (x,y) in src+home+blok: err(6)
    # end for

    # czy sąsiednie elementy pasują
    for (x,y,typ) in p:
        if VISUAL: print(x,y,typ)
        if not pl[(x,y-1)] in rule[typ][0] : err(7)
        if not pl[(x+1,y)] in rule[typ][1] : err(7)
        if not pl[(x,y+1)] in rule[typ][2] : err(7)
        if not pl[(x-1,y)] in rule[typ][3] : err(7)
    # end for

    # czy każdy dom ma wodę
    for (x,y) in home:
        licz=0
        if pl[(x,y-1)] in w0: licz+=1
        if pl[(x+1,y)] in w1: licz+=1
        if pl[(x,y+1)] in w2: licz+=1
        if pl[(x-1,y)] in w3: licz+=1
        if licz!=1: err(8)
        # end for


# end def

##########################################

def err(n):
    error = { 1:"bledny format pliku wejsciowego",
              2:"bledny format pliku wyjsciowego",
              3:"za dużo elementów rurociagu",
              4:"niezany element rurociagu",
              5:"niezgodny element rurociagu",
              6:"błędne prowadzenie rurociągu",
              7:"błędne dopasowanie kolejnych elementów",
              8:"dom bez wody!",
              9:"rurociąg poza planszą",
              10:"rurociąg wchodzi sam na siebie",
              11:"zła kolejność budowy",
              12:"nieznany błąd"
              }
    print("Error",n,":",error[n])

    exit(n)
# end def

##########################################

try:
    if len(sys.argv)==2:
        finame=sys.argv[1]
        t = readBoard(finame)
        drawBoard(finame.split('.')[0]+".png",t,[])
    elif len(sys.argv)==3:
        finame=sys.argv[1]
        t = readBoard(finame)
        foname=sys.argv[2]
        p = readPipe(foname)
        # checkPipe(t,p)
        drawBoard(finame.split('.')[0]+".png",t,p)
    else:
        print("Usage: ./judge.py <file.in> [ <file.out> ]")
except SystemExit as e:
    raise e
except:
    err(12)
# end if

print("OK")
sys.exit(0)

##########################################
