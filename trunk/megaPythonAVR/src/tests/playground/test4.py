# PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
# Copyright 2002 Dean Hall
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

#
# Interactive Test 065
# Tests plat module's putb and getb functions
#

import sys

#y = {}
#y.foo = "bar"
#y.fizz = "buzz"

#x = ['h','e','l','l','o',' ','d','u','d','e','!']
x = list("hello dude!")
#y = ['l','a','d','y']
y=x[-5:-1]
y=map(chr,y)
print y

#del x
z = list("newly defined")
a=z[-7:]
print map(chr,a)
#del y
#sys.puts(map(ord,list(z)))
#sys.puts('\n')
#x.append('a')
#x.append('b')
#x.append('c')
#x.append('\n')
#sys.puts(x)


