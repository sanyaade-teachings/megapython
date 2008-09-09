# PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
# Copyright 2007 David Greenberg
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
# Provides PyMite's threading stuff
#
# USAGE
# -----
#
# import threading
#
# LOG
# ---
# 2007/01/17    Created.
#

#### FUNCS

#
# Runs the given function in a thread sharing the current global namespace
#
def spawn(f):
    """__NATIVE__
    PmReturn_t retval;
    pPmObj_t pf;

    /* If wrong number of args, raise TypeError */
    if (NATIVE_GET_NUM_ARGS() != 1)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    /* If arg is not a function, raise TypeError */
    pf = NATIVE_GET_LOCAL(0);
    if (OBJ_GET_TYPE(pf) != OBJ_TYPE_FXN
        && OBJ_GET_TYPE(pf) != OBJ_TYPE_MTH)
    {
        PM_RAISE(retval, PM_RET_EX_TYPE);
        return retval;
    }

    retval = interp_addThread((pPmFunc_t)pf);
    return retval;
    """
    pass

#
# Yields the current thread to allow another thread to run
#
def wait():
    """__NATIVE__
    /* Set the reschedule flag to true */
    VM_SET_RESCHEDULE(1);
    return PM_RET_OK;
    """
    pass

def lock():
    """__NATIVE__
    /* Set the atomic flag to true */
    VM_SET_ATOMIC(1);
    return PM_RET_OK;
    """
    pass

def unlock():
    """__NATIVE__
    /* Set the atmoic flag to false */
    VM_SET_ATOMIC(0);
    return PM_RET_OK;
    """
    pass


# :mode=c:
