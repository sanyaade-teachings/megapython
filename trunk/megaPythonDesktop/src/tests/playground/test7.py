
#from sys import print_vm_state
import sys
#import thread
#print "hi"

class A:
    def __init__(self):
        self.x = 0

    def foo(self):
        print "x=",self.x

def bar():
    print "Hello space!"

#print_vm_state()
#del print_vm_state
#del sys

a = A()
#print "got here"
#sys.print_vm_state() # manual GC here fixes the problems...the auto part isn't working.  What's different?
b = A()
#sys.print_vm_state()
#print "got here2"

b.x=2
a.foo()
#thread.runInThread(bar)
b.foo()

#bar()
#x = [7,8,9]
#y = (x[0] + 8,)
#print "x=",x," y=",y



#print "hello world!, heap:",sys.heap()

c = A()
#print "got here"
d = A()
#print "got here2"

d.x=2
c.foo()
#thread.runInThread(bar)
d.foo()
