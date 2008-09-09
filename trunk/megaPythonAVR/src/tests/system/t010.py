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
# Regression Test for Issue #10
#
# Create sequence_getSubscript()
#
# The new function is used in BINARY_SUBSCRIPT,
# so this tests the four supported subscript statements
#

# Test string subscript
s = "test"
assert s[0] == 't'
assert s[-2] == 's'

# Test tuple subscript
t = (4, 2)
assert t[1] == 2
assert t[-2] == 4

# Test list subscript
l = [2, 5, 6]
assert l[2] == 6
assert l[-1] == 6

# Test dict subscript
d = {0:0, 1:'t', "2":2}
assert d[0] == 0
assert d[1] == 't'
assert d["2"] == 2

#
# UNPACK_SEQUENCE also uses seq_getSubscript()
#
a,b,c = (1,2,3)
assert a == 1
assert b == 2
assert c == 3

c,b,a = [2,5,6]
assert c == 2
assert b == 5
assert a == 6

#
# #59: Improve bytecode UNPACK_SEQUENCE
#
a,b,c = "256"
assert a == "2"
assert b == "5"
assert c == "6"
