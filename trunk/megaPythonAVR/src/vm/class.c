/*
 * PyMite - A flyweight Python interpreter for 8-bit microcontrollers and more.
 * Copyright 2007 David Greenberg
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

#undef __FILE_ID__
#define __FILE_ID__ 0x18

/**
 * Class Object Type
 *
 * Class object type operations.
 *
 * Log
 * ---
 *
 * 2008/01/21   First
 */

/***************************************************************
 * Includes
 **************************************************************/

#include "pm.h"


/***************************************************************
 * Functions
 **************************************************************/

PmReturn_t
class_create(pPmString_t name, pPmDict_t attrs, pPmTuple_t bases, pPmObj_t *r_pobj)
{
    PmReturn_t retval = PM_RET_OK;
    pPmClass_t pclass = C_NULL;

    /* Allocate a class */
    retval = heap_getChunk(sizeof(PmClass_t), (uint8_t **)r_pobj);
    PM_RETURN_IF_ERROR(retval);

    /* Set class type, empty the contents */
    pclass = (pPmClass_t)*r_pobj;
    OBJ_SET_TYPE(pclass, OBJ_TYPE_CLO);
    pclass->name = name;
    pclass->attrs = attrs;
    pclass->bases = bases;

    return retval;
}

PmReturn_t
class_newMethod(pPmFunc_t func, pPmClass_t self, pPmObj_t *r_pobj)
{
    PmReturn_t retval = PM_RET_OK;
    pPmMethod_t pmeth = C_NULL;

    /* Allocate method */
    retval = heap_getChunk(sizeof(PmMethod_t), (uint8_t **)r_pobj);
    PM_RETURN_IF_ERROR(retval);

    /* Set class type, empty the contents */
    pmeth = (pPmMethod_t)*r_pobj;
    OBJ_SET_TYPE(pmeth, OBJ_TYPE_MTH);
    pmeth->func = func;
    pmeth->self = self;

    return retval;
}

PmReturn_t
class_newInstance(const pPmClass_t pclass, pPmObj_t *r_pobj)
{
#if 1
    uint8_t* initstr = (uint8_t*)"__init__";
#endif
    PmReturn_t retval = PM_RET_OK;
    pPmObj_t pobj;
    pPmClass_t pinstance;
    pPmObj_t pkey;
    pPmObj_t pval;
    int16_t i;
    
    /* don't garbage collect while we're working */
    gVmGlobal.nativeframe.nf_active = C_TRUE;

    /* the instance is modeled off of it's type */
    retval = class_create(pclass->name, pclass->attrs, pclass->bases, r_pobj);
    PM_RETURN_IF_ERROR(retval);
    pinstance = (pPmClass_t)*r_pobj;
    OBJ_SET_TYPE(pinstance, OBJ_TYPE_CLI);

    /* we need a new dict for the class's namespace */
    retval = dict_new(&pobj);
    PM_RETURN_IF_ERROR(retval);
    pinstance->attrs = (pPmDict_t)pobj;

#if 0
    /* make guarantees that the class was created correctly */
    /* not really useful, b/c these are guarenteed to be true */
    C_ASSERT(OBJ_GET_TYPE(pinstance) == OBJ_TYPE_CLI);
    C_ASSERT(OBJ_GET_TYPE(pinstance->attrs) == OBJ_TYPE_DIC);
    C_ASSERT(OBJ_GET_TYPE(pinstance->name) == OBJ_TYPE_STR);
    C_ASSERT(OBJ_GET_TYPE(pinstance->bases) == OBJ_TYPE_TUP);
#endif

    /* We must convert all functions to methods here.
     * We will iterate through every value in the attribute
     * dict, looking for functions that should be converted
     * to methods.
     *
     * pkey is the key (func name), pval is the function, and pobj is the method
     */
    for (i = 0; i < (pclass->attrs->d_keys->sl_length); i++)
    {
        retval = seglist_getItem(pclass->attrs->d_keys, i, &pkey); /* Get a key */
        PM_RETURN_IF_ERROR(retval);

        retval = seglist_getItem(pclass->attrs->d_vals, i, &pval); /* Get a val */
        PM_RETURN_IF_ERROR(retval);

        if (OBJ_GET_TYPE(pval) == OBJ_TYPE_FXN)
        {
            retval = class_newMethod((pPmFunc_t)pval, pinstance, &pobj);
            PM_RETURN_IF_ERROR(retval);

            C_ASSERT(pinstance->attrs != pclass->attrs);

            retval = dict_setItem((pPmObj_t)pinstance->attrs, pkey, pobj);
            PM_RETURN_IF_ERROR(retval);
        }
        /* Assume that shared objects remain */
        else
        {
            C_ASSERT(OBJ_GET_TYPE(pval) != OBJ_TYPE_FXN);
            retval = dict_setItem((pPmObj_t)pinstance->attrs, pkey, pval);
            PM_RETURN_IF_ERROR(retval);
        }
    }
#if 1
    /* Run the class's initializer, if it has one */
    retval = string_new((uint8_t const **)&initstr, &pkey);
    PM_RETURN_IF_ERROR(retval);
    retval = dict_getItem((pPmObj_t)pinstance->attrs, pkey, &pval);
    if (retval == PM_RET_OK)
    {
        /* Push the object, so it'll be TOS when __init__ returns */
        PM_PUSH((pPmObj_t)pinstance);
        /* Push the __init__, call it with no args (the self arg is implicit and added automagically) */
        C_ASSERT(OBJ_GET_TYPE(pval) == OBJ_TYPE_MTH);
        PM_PUSH(pval);
        /* The first arg means how many arguments are on the stack, the second means to not push the return value */
        interp_callFunction(0, 1);
    }
    /* Doesn't matter, it's fine anyway */
    else
    {
        retval = PM_RET_OK;
    }
#endif

    /* Re-enable GC */
    gVmGlobal.nativeframe.nf_active = C_FALSE;

    return retval;
}
