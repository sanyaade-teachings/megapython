
import sys
import thread

class A:
    def __init__(self):
#        print "Hello __init__!"
        sys.print_vm_state()
        self.x = 0
#        return self

    def foo(self):
        print "x=",self.x

del sys.registerCallback
del sys.time
del sys.puts
del sys.exit
del thread.yieldThread
del thread.critLock
del thread.critUnlock

print "hello world!, heap:",sys.heap()

def bar():
    print "Hello space!"

a = A()
print "got here"
sys.print_vm_state()
b = A()
print "got here2"

#b.x=2
a.foo()
thread.runInThread(bar)
#b.foo()
