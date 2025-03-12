/** ---------------------------------------- LICENSE SECTION -------------------------------------------------------------------
 *
 * Copyright (c) 2019 Piotr Wojtowicz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */

This document defines following sections:
I. Introduction.
II. How to use the library.


---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
I. Introduction.
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------

This is an ANSI C library which defines 5 types of buffers:
1. Linear Buffers
2. Vendor Buffers
3. Vector Buffers
4. Tree Bufers
5. Ring Buffers

---------------------------------------------------------------------------------------------------
1. Linear Buffer - this is most obvious buffer type; simple bytes table.
uint8_t data[23];

---------------------------------------------------------------------------------------------------
2. Vendor buffer
Vendor buffer is an unknown buffer type.
Clasic linear buffer is a table of uint8_t bytes accessed by library using memcpy function.
Comparing to that, vendor buffer is treated as "seems to be" linear buffer but accessed by vendor-specific function memcpy-like.
This function pointer must be provided to each library function which accesses vendor buffer.
Vendor-specific memcpy like function must look like below:
Buff_Size_DT (*Buff_Vendor_Memcpy_HT) (const Buff_Memcpy_Params_XT *params);
It is function which takes pointer to structure with all needed parameters and returns size of data which copied (can copy less than was requested).
Parameter structure is:

typedef struct Buff_Memcpy_Params_eXtended_Tag
{
   // pointer to destination memory
   void        *dest;
   // pointer to source memory
   const void  *src;
   // offset in destination memory under which data shall be stored
   Buff_Size_DT dest_offset;
   // offset in source memory from which data shall be stored
   Buff_Size_DT src_offset;
   // size maximum size of data to be copied by Buff_Vendor_Memcpy_HT;
   // function can copy less - in that case no more calls will be executed, even if is_last_part in current call is BUFF_FALSE
   Buff_Size_DT size;
   // if BUFF_TRUE then this is last call of Buff_Vendor_Memcpy_HT needed to copy requested data buffer
   Buff_Bool_DT is_last_part;
}Buff_Memcpy_Params_XT;

When library function copies data to vendor buffer, there must be provided pointer to Buff_Vendor_Memcpy_HT type function
and pointer to vendor buffer. Vendor buffer pointer is provided as "dest" parameter of Buff_Memcpy_Params_XT structure.
Parameter "src" is in this case pointer to part of linear buffer from which data is copied and shall be used as normal linear buffer.
Parameter dest_offset shows offset in bytes in vendor destination buffer, however its use is up to vendor.
Parameter src_offset is offset in bytes and must be used together with src pointer.

When library function copies data from vendor buffer, there must be also provided pointer to Buff_Vendor_Memcpy_HT type function
and pointer to vendor buffer. Vendor buffer pointer is provided as "src" parameter of Buff_Memcpy_Params_XT structure.
Parameter "dest" is in this case pointer to part of linear buffer to which data is copied and shall be used as normal linear buffer.
Parameter src_offset shows offset in bytes in vendor source buffer, however its use is up to vendor.
Parameter dest_offset is offset in bytes and must be used together with dest pointer.

Parameter is_last_part shows if current call of vendor-memcpy function is last call for current operation or if library function will do another "memcpy" call.

Vendor buffers mechanism can be used for example for copying data to some peripheral of microprocessor. Very often peripherals have specific methods
for accessing it, like UARTs have usually single register from/to which data must be copied (under same address), sometimes logic is more complicated.
Vendor memcpy-like function implements in that case proper access to peripheral.

---------------------------------------------------------------------------------------------------
3. Vector buffers
Vector buffer is a table of pointers to linear buffers (Buff_Readable_Vector_XT / Buff_Writeable_Vector_XT)
which is some kind of alternative to simple data pointer. It is used for copying procedures (not as permanent storage container).
For example:
Vector buffer which is composed from 4 elements:
 v[0] = {0x12, 0x44, 0x01, 0xC7, 0x55};
 v[1] = {0x36, 0x21};
 v[2] = {0x72, 0x4C, 0x31};
 v[3] = {0x22};

 Represents one linear buffer:
 data = {0x12, 0x44, 0x01, 0xC7, 0x55, 0x36, 0x21, 0x72, 0x4C, 0x31, 0x22}
         |__________________________|  |________|  |______________|  |__|
                      |                     |             |            |
                    v[0]                  v[1]          v[2]         v[3]

These buffers are useful for communication libraries.
Let say, there is a library which is responsible for communication protocol; it must add some header and checksum to transmitted data.
Application provided pointer to data which must be sent. Communication library wants to add header (on the begining)
and checksum (at the end) and must execute "transmit" function (which writes data to transmitter) at one call.
For that reason, library must compose transmition packet in some temporary buffer by storing on the begining the header,
next copy user data and at the end checksum. Next, provide its pointer for transmit function. It consumes more RAM (temporary buffer).
Other solution is vector. Let say, transmit function is able to take vector instead of data pointer and compose linear data
from vector in transmit buffer. In this case our library prepares vector with 3 elements: v[0] = header; v[1] = user data; v[2] = checksum
and provides vector to transmit function.

There can be 2 types of vector buffers - readable vector (vector read only) and writeable vector (readable and writeable vector).
Single element of each type is defined as below:

typedef struct Buff_Writeable_Vector_eXtended_Tag
{
   void        *data;
   Buff_Size_DT size;
}Buff_Writeable_Vector_XT;

typedef struct Buff_Readable_Vector_eXtended_Tag
{
   const void  *data;
   Buff_Size_DT size;
}Buff_Readable_Vector_XT;

---------------------------------------------------------------------------------------------------
4. Tree buffers
Tree buffer is a table of pointers to vectors or other trees. For this reason tree buffer is some kind of recurseve buffer.
It is some kind of alternative to simple data pointer.
It is used for copying procedures (not as permanent storage container).

IN BELOW EXAMPLE THERE ARE USED TREE ELEMENTS ".v" AND ".t" ONLY TO SHOW IDEA. STRUCTURES USED BY LIBRARY HAVE FIELDS WITH DIFFERENT NAME.

For example:
 Tree buffer which is composed of: pointer to "before" tree, first vector of 2 elements, second vector of 2 elements and "after" tree:
 before[0].v[0]   = {0x03};
 before[0].v[1]   = {0xD3, 0xDD};

 t[0].t    = &before
 t[1].v[0] = {0x12}
 t[1].v[1] = {0x36, 0x21}
 t[2].v[0] = {0x72, 0x4C, 0x31}
 t[2].v[1] = {0x22}
 t[3].t    = &after;

 after_before[0].v[0]   = {0xB2};
 after_before[0].v[1]   = {0xC2, 0xC3, 0xC4, 0xC5};

 after[0].t    = &after_before;
 after[1].v[0] = {0x17, 0x40};
 after[2].t    = &after_after;

 after_after[0].v[0]   = {0x01};
 after_after[0].v[1]   = {0x02, 0x03};
 after_after[0].v[2]   = {0x04};
 after_after[1].t      = &after_after_after;

 after_after_after[0].v[0]   = {0xF1};

Represents one linear buffer:

                                                            data
                                                             |
                                                             V
  /---------------------------------------------------------------------------------------------------------------------------------\
  {0x03, 0xD3, 0xDD, 0x12, 0x36, 0x21, 0x72, 0x4C, 0x31, 0x22, 0xB2, 0xC2, 0xC3, 0xC4, 0xC5, 0x17, 0x40, 0x01, 0x02, 0x03, 0x04, 0xF1}
   |______________|  |______________|  |____________________|  |____________________________________________________________________|
          |                   |                  |                                               |
        t[0].t              t[1].v             t[2].v                                          t[3].t
          |                   |                  |                                               |
          V                   V                  V                                               V
   /--------------\  /--------------\  /--------------------\  /--------------------------------------------------------------------\
  {0x03, 0xD3, 0xDD}{0x12, 0x36, 0x21}{0x72, 0x4C, 0x31, 0x22}{0xB2, 0xC2, 0xC3, 0xC4, 0xC5, 0x17, 0x40, 0x01, 0x02, 0x03, 0x04, 0xF1}
   |______________|  |__|  |________|  |______________|  |__|  |__________________________|  |________|  |__________________________|
          |           |         |             |           |                  |                   |                    |
   before[0].v      v[0]      v[1]          v[0]        v[1]           after[0].t          after[1].v           after[2].t
          |                                                                  |                   |                    |
          V                                                                  V                   V                    V
   /--------------\                                            /--------------------------\  /--------\  /--------------------------\
  {0x03, 0xD3, 0xDD}                                          {0xB2, 0xC2, 0xC3, 0xC4, 0xC5}{0x17, 0x40}{0x01, 0x02, 0x03, 0x04, 0xF1}
   |__|  |________|                                            |__________________________|  |________|  |____________________|  |__|
    |        |                                                               |                   |               |                 |
  v[0]     v[1]                                                 after_before[0].v              v[0]  after_after[0].v  after_after[1].t
                                                                             |                                   |                 |
                                                                             V                                   V                 V
                                                               /--------------------------\              /--------------------\  /--\
                                                              {0xB2, 0xC2, 0xC3, 0xC4, 0xC5}            {0x01}{0x02, 0x03}{0x04}{0xF1}
                                                               |__|  |____________________|              |__|  |________|  |__|  |__|
                                                                |              |                          |        |        |      |
                                                              v[0]           v[1]                       v[0]     v[1]     v[2]     |
                                                                                                                 after_after_after[0].v
                                                                                                                                   |
                                                                                                                                   V
                                                                                                                                 /--\
                                                                                                                                {0xF1}
                                                                                                                                 |__|
                                                                                                                                   |
                                                                                                                                 v[0]

These buffers are useful for communication libraries.
Let say, there is a library which is responsible for communication protocol; it must add some header and checksum to transmitted data.
Communication library wants to add header (on the begining) and checksum (at the end) and must execute "transmit" function
(which writes data to transmitter) at one call.
In comparision to example described in vector explanation, Application doesnt provide linear buffer but vector.
For that reason, library must compose transmition vector, bigger than provided one, to first vector cell store pointer to header,
next copy cells from user data vector and at the end put checksum to last cell. Next, provide its pointer for transmit function.
Other solution is tree with 3 elements - add header to "tree[0].v" as pointer to one element vector,
next put user vector pointer to "tree[1].v" as second vector and at the end, add one element vector with checksum at "tree[2].v" pointer.
If data provided by user is already tree, then above example is same with one exception - "tree[1]" will point to sub-tree ("tree[1].t").

---------------------------------------------------------------------------------------------------
5. Ring Buffers
Ring buffer or sometimes called cyclic buffer is storage container. It is basing on linear buffer where user can store
any data. Type of the data and its interpretation is used-defined. Buffer only provides functions to write any number of bytes
(of course less than buffer size), read it (read with removing), peak it (read without removing), overwriting (modifying data already stored in buffer),
checking out/in (obtaining pointer of linear memory to be modified by user and after that saving result) and clearing (without reading).
Most important feature of this type of buffer is that data can be wrapped between the end and the begining of the buffer if during writing
end of buffer has been reached and on its begining there is free space (data from that location has been previously read).
This is fully invisible for used because write / peak / read / overwrite / remove / clear are encapsulated
(user doesn't operate directly on buffer's memory area but uses functions dedicated for it). If there is not enough free space
in the buffer during writing, this implementation of ring buffer may overwrite oldest data or not,
depends to definition BUFF_RING_ALLOW_OLD_DATA_OVERWRITE and write function parameter "overwrite_if_no_free_space".


---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
II. How to use the library.
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------

Library consists of 2 files: api/buff.h and imp/src/buff.c. These 2 files must be added to the project. Next, also buff_cfg.h must be created as buff.h includes it.
File buff_cfg.h defines which elements of library user wants to use, defines settings needed by library, etc.
Generally, buff.h and buff.c needs a lot of settings which must be provided by user. Most of these settings are pre-defined to enable all functions and
to compile correctly under gcc.
User must review them and configure properly. Below I will list all settings user must review. I will list them and describe groups of settings
and when / how it shall be re-defined.
I will not describe them in details in this file because all settings are described in details in buff.h.

---------------------------------------------------------------------------------------------------
1. Compiler dependant settings
This is a group of settings, predefined on the begining of buff.h to work properly with gcc. There are following definitions and macroes defined as below:
#define BUFF_FALSE                           false
#define BUFF_TRUE                            true
#define BUFF_BOOL_IS_TRUE(condition)                              (condition)
#define BUFF_BOOL_IS_FALSE(condition)                             (!(condition))
#define BUFF_LIKELY(expresion)                                    (expresion)
#define BUFF_UNLIKELY(expresion)                                  (expresion)
#define BUFF_MAKE_INVALID_PTR(type)                               ((type*)(NULL))
#define BUFF_MAKE_INVALID_HANDLER(type)                           ((type)(NULL))
#define BUFF_CHECK_PTR(type, ptr)                                 (BUFF_MAKE_INVALID_PTR(type) != (ptr))
#define BUFF_CHECK_HANDLER(type, handler)                         (BUFF_MAKE_INVALID_HANDLER(type) != (handler))
#define BUFF_COMPARE_PTRS(type1, ptr1, type2, ptr2)               (((type1*)(ptr1)) == ((type2*)(ptr2)))
#define BUFF_COMPARE_HANDLERS(type1, handler1, type2, handler2)   (((type1)(handler1)) == ((type2)(handler2)))

These definitions must be re-defined by user if doesn't work properly with compiler used in the project.
For detailed description what is the purpose of use for each of them please look into buff.h where it is defined.

---------------------------------------------------------------------------------------------------
2. Library debugging support
This section includes 3 macroes defined by default as empty macroes:
#define BUFF_ENTER_FUNC()
#define BUFF_EXIT_FUNC()
#define BUFF_DEBUG_RING(buf)

First 2 can be used to monitor entrance and exit of each function defined in library. Third is called additionally on every entrance and exit of functions accessing ring buffers,
providing as parameter pointer to the buffer which is assessed.
If user doesn't want to monitor entrances / exit of library functions, these macroes should remain defined as empty.

---------------------------------------------------------------------------------------------------
3. Library features enabling / disabling
This is a group of settings used to enable / disable group of library features. There are:
#define BUFF_FEATURE_DISABLED                false
#define BUFF_FEATURE_ENABLED                 true
#define BUFF_DEFAULT_FEATURES_STATE          BUFF_FEATURE_ENABLED
#define BUFF_SAVE_PROGRAM_MEMORY             BUFF_FEATURE_DISABLED
#define BUFF_USE_VENDOR_BUFFERS              BUFF_DEFAULT_FEATURES_STATE
#define BUFF_USE_VECTOR_BUFFERS              BUFF_DEFAULT_FEATURES_STATE
#define BUFF_USE_TREE_BUFFERS                BUFF_DEFAULT_FEATURES_STATE
#define BUFF_USE_RING_BUFFERS                BUFF_DEFAULT_FEATURES_STATE
#define BUFF_RING_ALLOW_OLD_DATA_OVERWRITE   BUFF_DEFAULT_FEATURES_STATE
#define BUFF_RING_USE_EXTENSIONS             BUFF_DEFAULT_FEATURES_STATE
#define BUFF_RING_USE_PROTECTED_EXTENSIONS   BUFF_DEFAULT_FEATURES_STATE
#define BUFF_RING_USE_PROTECTION             BUFF_FEATURE_ENABLED
#define BUFF_RING_USE_BUSY_SIZE_MONITORING   BUFF_FEATURE_ENABLED
#define BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED        BUFF_DEFAULT_FEATURES_STATE

First 2 must work properly with preprocessor checking:
#if(condition)

Third setting is default state for most of features. Almost all of settings are re-defined by default to BUFF_DEFAULT_FEATURES_STATE so if it is set to BUFF_FEATURE_ENABLED
then library is pre-configured to enable all features. BUFF_SAVE_PROGRAM_MEMORY says if library shall replace body of some functions by calling internally other which can replace it.
Next 4 definitions enable / disable most important groups of features: vector/vendor/tree/ring buffers.
Last 6 definitions are used only if ring buffers are enabled.

---------------------------------------------------------------------------------------------------
4. Multi thread / Multi-context operations protection
This section defines macroes used only for ring buffers and only if BUFF_RING_USE_PROTECTION setting is enabled:
#define BUFF_PROTECTION_DECLARE()
#define BUFF_PROTECTION_CREATE(buf)    true
#define BUFF_PROTECTION_DESTROY(buf)
#define BUFF_PROTECTION_INIT(buf)      true
#define BUFF_PROTECTION_DEINIT(buf)
#define BUFF_PROTECTION_LOCK(buf)
#define BUFF_PROTECTION_UNLOCK(buf)

These macroes are used to create, initialize, activate and deactivate protection mechanism used in case if user will access same ring buffers from different threads.
Usually it can be mutex or critical section, depends to user decision.
Is you are sure ring buffer will not be accessed in same time by different threads or if you will secure it externally, these macroes can remain empty
and BUFF_RING_USE_PROTECTION can be turned off.

---------------------------------------------------------------------------------------------------
5. Library types.
This section defines many types used by library, however user shall focus on 3:
#ifndef BUFF_BOOL_DT_EXTERNAL
typedef bool_t Buff_Bool_DT;
#endif
#ifndef BUFF_SIZE_DT_EXTERNAL
typedef size_t Buff_Size_DT;
#endif
#ifndef BUFF_NUM_ELEMS_DT_EXTERNAL
typedef size_t Buff_Num_Elems_DT;
#endif

These 3 types are most basic and most important, shall be reviewed by user and re-defined if needed.
Especially Buff_Bool_DT (not every compiler knows bool_t type which is used by default.

---------------------------------------------------------------------------------------------------
6. Interfaces.
This section includes prototypes of all functions defined by library. Before prototype of each function there is defined default setting of SW switch do enable / disable it, like on example below:
#ifndef BUFF_COPY_FROM_VECTOR_ENABLED
#define BUFF_COPY_FROM_VECTOR_ENABLED                    BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_COPY_FROM_VECTOR_ENABLED)
Buff_Size_DT Buff_Copy_From_Vector(
   void                            *dest,
   const Buff_Readable_Vector_XT   *src,
   Buff_Num_Elems_DT                src_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src);
#endif

By default all function SW switches are re-defined to BUFF_DEFAULT_FEATURES_STATE.
It gived 2 basic ways of enabling / disabling features:
 a) Leave default definition of BUFF_DEFAULT_FEATURES_STATE to BUFF_FEATURE_ENABLED. In this scenario library is fully enabled. Now we can disable specific functions or gorups of features (types of buffers using definitions from point 4). To turn OFF specific functions or groups, simply re-define its SW switch in buff_cfg.h to BUFF_FEATURE_DISABLED.
 b) Re-define BUFF_DEFAULT_FEATURES_STATE to BUFF_FEATURE_DISABLED. Now all features of library are turned OFF. To enable only these functions which we need, re-define its group switch (point 4) to BUFF_FEATURE_ENABLED and function SW switch to BUFF_FEATURE_ENABLED in buff_acfg.h

Following function interfaces there are defined:
#if(BUFF_USE_VECTOR_BUFFERS)
    Buff_Get_Readable_Vector_Data_Size
    Buff_Get_Writeable_Vector_Data_Size
    Buff_Copy_From_Vector
    #if(BUFF_USE_VENDOR_BUFFERS)
        Buff_Copy_From_Vector_Vendor
    #endif
    Buff_Copy_To_Vector
    #if(BUFF_USE_VENDOR_BUFFERS)
        Buff_Copy_To_Vector_Vendor
    #endif
    Buff_Copy_Vector_To_Vector
#endif

#if(BUFF_USE_TREE_BUFFERS)
    Buff_Get_Readable_Tree_Data_Size
    Buff_Get_Writeable_Tree_Data_Size
    Buff_Copy_From_Tree
    #if(BUFF_USE_VENDOR_BUFFERS)
        Buff_Copy_From_Tree_Vendor
    #endif
    Buff_Copy_To_Tree
    #if(BUFF_USE_VENDOR_BUFFERS)
        Buff_Copy_To_Tree_Vendor
    #endif
    Buff_Copy_Tree_To_Tree
#endif

#if(BUFF_USE_RING_BUFFERS)
    Buff_Ring_Init
    Buff_Ring_Deinit
    #if(BUFF_RING_USE_EXTENSIONS)
        Buff_Ring_Init_Extension
        Buff_Ring_Add_Extension
    #endif
    #if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
        Buff_Ring_Add_Protected_Extension
    #endif
    #if(BUFF_RING_USE_PROTECTION)
        Buff_Protection_Lock
        Buff_Protection_Unlock
    #endif
    BUFF_RING_IS_EMPTY
    Buff_Ring_Is_Empty
    BUFF_RING_IS_FULL
    Buff_Ring_Is_Full
    BUFF_RING_GET_SIZE
    Buff_Ring_Get_Size
    BUFF_RING_GET_BUSY_SIZE
    Buff_Ring_Get_Busy_Size
    #if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
        BUFF_RING_GET_MAX_BUSY_SIZE
        Buff_Ring_Get_Max_Busy_Size
    #endif
    BUFF_RING_GET_FREE_SIZE
    Buff_Ring_Get_Free_Size
    Buff_Ring_Get_Continous_Free_Size
    BUFF_RING_GET_CURRENT_POS
    Buff_Ring_Get_Current_Pos
    #if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
        Buff_Ring_Data_Check_Out
        Buff_Ring_Data_Check_In
    #endif
    Buff_Ring_Write
    #if(BUFF_USE_VENDOR_BUFFERS)
        Buff_Ring_Write_Vendor
    #endif
    #if(BUFF_USE_VECTOR_BUFFERS)
        Buff_Ring_Write_From_Vector
    #endif
    #if(BUFF_USE_TREE_BUFFERS)
        Buff_Ring_Write_From_Tree
    #endif
    Buff_Ring_OverWrite_If_Exist
    #if(BUFF_USE_VENDOR_BUFFERS)
        Buff_Ring_OverWrite_If_Exist_Vendor
    #endif
    Buff_Ring_Peak
    #if(BUFF_USE_VENDOR_BUFFERS)
        Buff_Ring_Peak_Vendor
    #endif
    #if(BUFF_USE_VECTOR_BUFFERS)
        Buff_Ring_Peak_To_Vector
    #endif
    #if(BUFF_USE_TREE_BUFFERS)
        Buff_Ring_Peak_To_Tree
    #endif
    Buff_Ring_Read
    #if(BUFF_USE_VENDOR_BUFFERS)
        Buff_Ring_Read_Vendor
    #endif
    #if(BUFF_USE_VECTOR_BUFFERS)
        Buff_Ring_Read_To_Vector
    #endif
    #if(BUFF_USE_TREE_BUFFERS)
        Buff_Ring_Read_To_Tree
    #endif
    Buff_Ring_To_Ring_Copy
    Buff_Ring_Remove
    Buff_Ring_Clear
#endif
