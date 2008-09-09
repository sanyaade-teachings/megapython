/*
 * PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
 * Copyright 2002 Dean Hall
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __CLASS_H__
#define __CLASS_H__

/**
 * Class Object Type
 *
 * Class object type header.
 *
 * Log
 * ---
 *
 * 2008/01/21   First.
 */

/***************************************************************
 * Constants
 **************************************************************/


/***************************************************************
 * Macros
 **************************************************************/


/***************************************************************
 * Types
 **************************************************************/

/**
 * Class obj
 *
 * Contains attr dict, parent class tuple, and name
 */
typedef struct PmClass_s
{
    /** Object descriptor */
    PmObjDesc_t od;

    /** Name */
    pPmString_t name;

    /** Attrs dict */
    pPmDict_t attrs;

    /** Parent class names */
    pPmTuple_t bases;
} PmClass_t,
 *pPmClass_t;

/**
 * Method obj
 *
 * Marks something as a method (i.e. has implicit self argument)
 * Includes function, and a pointer to the self class
 */
typedef struct PmMethod_s
{
    /** Object descriptor */
    PmObjDesc_t od;

    /** Function */
    pPmFunc_t func;

    /** Parent attrs */
    pPmClass_t self;
} PmMethod_t,
 *pPmMethod_t;


/***************************************************************
 * Prototypes
 **************************************************************/

/**
 * Creates a new Class obj.  Populates with given params.
 *
 * @param   name Name of the class
 * @param   attrs Dictionary of attributes for the class
 * @param   bases Tuple of names of parent classes
 * @param   r_pobj Pointer to struct where class will be stored
 * @return  Return status
 */
PmReturn_t class_create(pPmString_t name, pPmDict_t attrs, pPmTuple_t bases, pPmObj_t *r_pobj);

/**
 * Creates a new Method obj.  Populates with given params.
 *
 * @param   func Function that the method calls
 * @param   self Class that the method belongs to
 * @param   r_pobj Pointer to struct where method will be stored
 * @return  Return status
 */
PmReturn_t class_newMethod(pPmFunc_t func, pPmClass_t self, pPmObj_t *r_pobj);

/**
 * Creates a new Class Instance obj.
 *
 * @param   pclass Class to create an instance of
 * @param   r_pobj Pointer to class instance
 * @return  Return status
 */
PmReturn_t class_newInstance(pPmClass_t pclass, pPmObj_t *r_pobj);

#endif /* __CLASS_H__ */
