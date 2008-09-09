import sys

def f1():
    print "callback!"

sys.registerCallback(0,f1)
