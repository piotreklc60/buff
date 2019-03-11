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

#ifndef BUFF_H_
#define BUFF_H_

#include "buff_acfg.h"

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY CONFIGURATION ----------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/* ----------------------------------------- COMPILER DEPENDANT SETTINGS ------------------------------------------------------- */


#ifndef BUFF_FALSE
/**
 * @brief Definition of boolean value "false" possible to be stored in variable type Buff_Bool_DT and to be checked by macroes
 * BUFF_BOOL_IS_TRUE and BUFF_BOOL_IS_FALSE.
 */
#define BUFF_FALSE                           false
#endif

#ifndef BUFF_TRUE
/**
 * @brief Definition of boolean value "true" possible to be stored in variable type Buff_Bool_DT and to be checked by macroes
 * BUFF_BOOL_IS_TRUE and BUFF_BOOL_IS_FALSE.
 */
#define BUFF_TRUE                            true
#endif

/**
 * Macros
 */

#ifndef BUFF_BOOL_IS_TRUE
/**
 * @brief Macro used to check if condition has boolean value true; used for condition checking.
 * Macro has been defined to check Buff_Bool_DT variable in case if for some compiler comparision must have some different syntax.
 * Must work properly with below example:
 *
 * Buff_Bool_DT state = BUFF_TRUE;
 *
 * if(BUFF_BOOL_IS_TRUE(state))
 * {
 *    printf("condition result is true");
 * }
 *
 * @result true if condition has boolean value true; false otherwise
 *
 * @param condition condition to be checked
 */
#define BUFF_BOOL_IS_TRUE(condition)                              (condition)
#endif

#ifndef BUFF_BOOL_IS_FALSE
/**
 * @brief Macro used to check if condition has boolean value false; used for condition checking.
 * Macro has been defined to check Buff_Bool_DT variable in case if for some compiler comparision must have some different syntax.
 * Must work properly with below example:
 *
 * Buff_Bool_DT state = BUFF_FALSE;
 *
 * if(BUFF_BOOL_IS_FALSE(state))
 * {
 *    printf("condition result is false");
 * }
 *
 * @result true if condition has boolean value false; false otherwise
 *
 * @param condition condition to be checked
 */
#define BUFF_BOOL_IS_FALSE(condition)                             (!(condition))
#endif

#ifndef BUFF_LIKELY
/**
 * @brief Macro used to suggest compiler that condition usually happens. Result might have influence
 * to behavior of program after compilation, for example work faster.
 * For GCC this macro can be redefined to:
 * #define BUFF_LIKELY(expresion)                                    __builtin_expect ((expresion), 1)
 *
 * @param expression condition which usually happens.
 */
#define BUFF_LIKELY(expresion)                                    (expresion)
#endif

#ifndef BUFF_UNLIKELY
/**
 * @brief Macro used to suggest compiler that condition usually doesn't happen. Result might have influence
 * to behavior of program after compilation, for example work faster.
 * For GCC this macro can be redefined to:
 * #define BUFF_LIKELY(expresion)                                    __builtin_expect ((expresion), 0)
 *
 * @param expression condition which usually doesn't happen.
 */
#define BUFF_UNLIKELY(expresion)                                  (expresion)
#endif

#ifndef BUFF_MAKE_INVALID_PTR
/**
 * Creates invalid pointer with specific type.
 * In practise, this macro maps NULL pointer to specific type to avoid compiler warnings.
 *
 * @result invalid pointer
 *
 * @param type type of invalid pointer to be returned
 */
#define BUFF_MAKE_INVALID_PTR(type)                               ((type*)(NULL))
#endif

#ifndef BUFF_MAKE_INVALID_HANDLER
/**
 * Creates invalid handler (pointer to function) with specific type.
 * In practise, this macro maps NULL handler to specific type to avoid compiler warnings.
 *
 * @result invalid handler
 *
 * @param type type of invalid handler to be returned
 */
#define BUFF_MAKE_INVALID_HANDLER(type)                           ((type)(NULL))
#endif

#ifndef BUFF_CHECK_PTR
/**
 * @brief Macro used to check if data pointer is valid.
 *
 * @result true if pointer is valid, false otherwise
 *
 * @param type of pointer to be checked
 * @param ptr pointer to be checked
 */
#define BUFF_CHECK_PTR(type, ptr)                                 (BUFF_MAKE_INVALID_PTR(type) != (ptr))
#endif

#ifndef BUFF_CHECK_HANDLER
/**
 * @brief Macro used to check if handler (pointer to function) is valid.
 *
 * @result true if handler is valid, false otherwise
 *
 * @param type of handler to be checked
 * @param handler handler to be checked
 */
#define BUFF_CHECK_HANDLER(type, handler)                         (BUFF_MAKE_INVALID_HANDLER(type) != (handler))
#endif

#ifndef BUFF_COMPARE_PTRS
/**
 * @brief Macro used to check if pointers are equal (if two examined pointers point to the same address).
 *
 * @result true if pointers are equal
 *
 * @param type1 of pointer 1 to be compared
 * @param ptr1 pointer 1 to be compared
 * @param type2 of pointer 2 to be compared
 * @param ptr2 pointer 2 to be compared
 */
#define BUFF_COMPARE_PTRS(type1, ptr1, type2, ptr2)               (((type1*)(ptr1)) == ((type2*)(ptr2)))
#endif

#ifndef BUFF_COMPARE_HANDLERS
/**
 * @brief Macro used to check if handlers (pointers to functions) are equal (if two examined handlers point to the same function).
 *
 * @result true if handlers are equal
 *
 * @param type1 of handler 1 to be compared
 * @param ptr1 handler 1 to be compared
 * @param type2 of handler 2 to be compared
 * @param ptr2 handler 2 to be compared
 */
#define BUFF_COMPARE_HANDLERS(type1, handler1, type2, handler2)   (((type1)(handler1)) == ((type2)(handler2)))
#endif


/* ----------------------------------------- LIBRARY DEBUGGING SUPPORT --------------------------------------------------------- */


#ifndef BUFF_ENTER_FUNC
/**
 * @brief Macro called on every entrance of any function from this library. Normally used as empty macro.
 * Can be redefined if for example there is needed to debug library.
 */
#define BUFF_ENTER_FUNC()
#endif

#ifndef BUFF_EXIT_FUNC
/**
 * @brief Macro called on every exit of any function from this library. Normally used as empty macro.
 * Can be redefined if for example there is needed to debug library.
 */
#define BUFF_EXIT_FUNC()
#endif

#ifndef BUFF_DEBUG_RING
/**
 * @brief Macro called on every entrance and exit of functions which access ring buffer. Normally used as empty macro.
 * Can be redefined if for example there is needed to debug library.
 */
#define BUFF_DEBUG_RING(buf)
#endif


/* ----------------------------------------- LIBRARY FEATURES ENABLING/DISABLING ----------------------------------------------- */


/**
 * Constants
 */

#ifndef BUFF_FEATURE_DISABLED
/**
 * @brief Definition used to disable specific features of library.
 * This definition must be properly recognized by preprocessor checking: #if(condition)
 */
#define BUFF_FEATURE_DISABLED                false
#endif

#ifndef BUFF_FEATURE_ENABLED
/**
 * @brief Definition used to enable specific features of library.
 * This definition must be properly recognized by preprocessor checking: #if(condition)
 */
#define BUFF_FEATURE_ENABLED                 true
#endif

#ifndef BUFF_DEFAULT_FEATURES_STATE
#define BUFF_DEFAULT_FEATURES_STATE          BUFF_FEATURE_ENABLED
#endif

#ifndef BUFF_SAVE_PROGRAM_MEMORY
/**
 * If defined to BUFF_FEATURE_ENABLED then library will be optimized to consume as less program memory as possible,
 * for example functions to copy from/to linear buffer will be replaced internally by functions for copying
 * from/to vendor buffer (as vendor "memcpy" will be used special internal function which emulates memcpy),
 * functions for copying from/to vector buffer will be replaced internally by functions for copying from/to tree buffers, etc.
 */
#define BUFF_SAVE_PROGRAM_MEMORY             BUFF_FEATURE_DISABLED
#endif

#ifndef BUFF_USE_VENDOR_BUFFERS
/**
 * @brief Definition to enable / disable support for vendor buffers. If defined to BUFF_FEATURE_ENABLED then all function used to copy
 * data from / to vendor-specific buffers will be enabled.
 * Vendor buffer is an unknown buffer type.
 * Clasic linear buffer is a table of uint8_t bytes accessed by library using memcpy function.
 * Comparing to that, vendor buffer is treated as "seems to be" linear buffer but accessed by vendor-specific function memcpy-like.
 * This function pointer must be provided to each library function which accesses vendor buffer.
 * Vendor-specific memcpy like function must look like below:
 * Buff_Size_DT (*Buff_Vendor_Memcpy_HT) (const Buff_Memcpy_Params_XT *params);
 * It is function which takes pointer to structure with all needed parameters and returns size of data which copied (can copy less than was requested).
 * Parameter structure is:
 *
 * typedef struct Buff_Memcpy_Params_eXtended_Tag
 * {
 *    // pointer to destination memory
 *    void        *dest;
 *    // pointer to source memory
 *    const void  *src;
 *    // offset in destination memory under which data shall be stored
 *    Buff_Size_DT dest_offset;
 *    // offset in source memory from which data shall be stored
 *    Buff_Size_DT src_offset;
 *    // size maximum size of data to be copied by Buff_Vendor_Memcpy_HT;
 *    // function can copy less - in that case no more calls will be executed, even if is_last_part in current call is BUFF_FALSE
 *    Buff_Size_DT size;
 *    // if BUFF_TRUE then this is last call of Buff_Vendor_Memcpy_HT needed to copy requested data buffer
 *    Buff_Bool_DT is_last_part;
 * }Buff_Memcpy_Params_XT;
 *
 * When library function copies data to vendor buffer, there must be provided pointer to Buff_Vendor_Memcpy_HT type function
 * and pointer to vendor buffer. Vendor buffer pointer is provided as "dest" parameter of Buff_Memcpy_Params_XT structure.
 * Parameter "src" is in this case pointer to part of linear buffer from which data is copied and shall be used as normal linear buffer.
 * Parameter dest_offset shows offset in bytes in vendor destination buffer, however its use is up to vendor.
 * Parameter src_offset is offset in bytes and must be used together with src pointer.
 *
 * When library function copies data from vendor buffer, there must be also provided pointer to Buff_Vendor_Memcpy_HT type function
 * and pointer to vendor buffer. Vendor buffer pointer is provided as "src" parameter of Buff_Memcpy_Params_XT structure.
 * Parameter "dest" is in this case pointer to part of linear buffer to which data is copied and shall be used as normal linear buffer.
 * Parameter src_offset shows offset in bytes in vendor source buffer, however its use is up to vendor.
 * Parameter dest_offset is offset in bytes and must be used together with dest pointer.
 *
 * Parameter is_last_part shows if current call of vendor-memcpy function is last call for current operation or if library function will do another "memcpy" call.
 *
 * Vendor buffers mechanism can be used for example for copying data to some peripheral of microprocessor. Very often peripherals have specific methods
 * for accessing it, like UARTs have usually single register from/to which data must be copied (under same address), sometimes logic is more complicated.
 * Vendor memcpy-like function implements in that case proper access to peripheral.
 */
#define BUFF_USE_VENDOR_BUFFERS              BUFF_DEFAULT_FEATURES_STATE
#endif

#ifndef BUFF_USE_VECTOR_BUFFERS
/**
 * @brief Definition to enable / disable support for vector buffers. If defined to BUFF_FEATURE_ENABLED then all function used to copy
 * data from / to vector buffers will be enabled.
 * Vector buffer is a table of pointers to linear buffers (Buff_Readable_Vector_XT / Buff_Writeable_Vector_XT)
 * which is some kind of alternative to simple data pointer. It is used for copying procedures (not as permanent storage container).
 * For example:
 * Vector buffer which is composed from 4 elements:
 * v[0] = {0x12, 0x44, 0x01, 0xC7, 0x55};
 * v[1] = {0x36, 0x21};
 * v[2] = {0x72, 0x4C, 0x31};
 * v[3] = {0x22};
 *
 * Represents one linear buffer:
 * data = {0x12, 0x44, 0x01, 0xC7, 0x55, 0x36, 0x21, 0x72, 0x4C, 0x31, 0x22}
 *         |__________________________|  |________|  |______________|  |__|
 *                      |                     |             |            |
 *                    v[0]                  v[1]          v[2]         v[3]
 *
 * These buffers are useful for communication libraries.
 * Let say, there is a library which is responsible for communication protocol; it must add some header and checksum to transmitted data.
 * Application provided pointer to data which must be sent. Communication library wants to add header (on the begining)
 * and checksum (at the end) and must execute "transmit" function (which writes data to transmitter) at one call.
 * For that reason, library must compose transmition packet in some temporary buffer by storing on the begining the header,
 * next copy user data and at the end checksum. Next, provide its pointer for transmit function. It consumes more RAM (temporary buffer).
 * Other solution is vector. Let say, transmit function is able to take vector instead of data pointer and compose linear data
 * from vector in transmit buffer. In this case our library prepares vector with 3 elements: v[0] = header; v[1] = user data; v[2] = checksum
 * and provides vector to transmit function.
 */
#define BUFF_USE_VECTOR_BUFFERS              BUFF_DEFAULT_FEATURES_STATE
#endif

#ifndef BUFF_USE_TREE_BUFFERS
/**
 * @brief Definition to enable / disable support for tree buffers. If defined to BUFF_FEATURE_ENABLED then all function used to copy
 * data from / to tree buffers will be enabled.
 * Tree buffer is a table of pointers to vectors or other trees. For this reason tree buffer is some kind of recurseve buffer.
 * It is some kind of alternative to simple data pointer.
 * It is used for copying procedures (not as permanent storage container).
 *
 * IN BELOW EXAMPLE THERE ARE USED TREE ELEMENTS ".v" AND ".t" ONLY TO SHOW IDEA. STRUCTURES USED BY LIBRARY HAVE FIELDS WITH DIFFERENT NAME.
 *
 * For example:
 * Tree buffer which is composed of: pointer to "before" tree, first vector of 2 elements, second vector of 2 elements "after" tree  and third vector of one element:
 * before[0].v[0]   = {0x03, 0x45};
 * before[0].v[1]   = {0xD3, 0xDD, 0xDF};
 *
 * t[0].t    = &before
 * t[1].v[0] = {0x12, 0x44, 0x01, 0xC7, 0x55}
 * t[1].v[1] = {0x36, 0x21}
 * t[2].v[0] = {0x72, 0x4C, 0x31}
 * t[2].v[1] = {0x22}
 * t[3].t    = &after;
 * t[4].v[0] = {0x7C, 0xE5}
 *
 * after_before[0].v[0]   = {0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7};
 * after_before[0].v[1]   = {0xC2, 0xC3, 0xC4, 0xC5};
 *
 * after[0].t    = &after_before;
 * after[1].v[0] = {0x17, 0x40};
 * after[2].t    = &after_after;
 *
 * after_after[0].v[0]   = {0x01};
 * after_after[0].v[1]   = {0x02, 0x03, 0x04};
 * after_after[0].v[2]   = {0x05};
 * after_after[1].t      = &after_after_after;
 *
 * after_after_after[0].v[0]   = {0xF1, 0xF2, 0xF3};
 * after_after_after[0].v[1]   = {0xF4, 0xF5, 0xF6, 0xF7, 0xF8};
 *
 * Represents one linear buffer:
 *
 *                                                                                                                                 data
 *                                                                                                                                   |
 *                                                                                                                                   V
 *  /---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\
 *  {0x03, 0x45, 0xD3, 0xDD, 0xDF, 0x12, 0x44, 0x01, 0xC7, 0x55, 0x36, 0x21, 0x72, 0x4C, 0x31, 0x22, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xC2, 0xC3, 0xC4, 0xC5, 0x17, 0x40, 0x01, 0x02, 0x03, 0x04, 0x05, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0x7C, 0xE5}
 *   |__________________________|  |______________________________________|  |____________________|  |__________________________________________________________________________________________________________________________________________________|  |________|
 *                |                                  |                                 |                                                                                 |                                                                                      |
 *              t[0].t                             t[1].v                            t[2].v                                                                            t[3].t                                                                                 t[4].v
 *                |                                  |                                 |                                                                                 |                                                                                      |
 *                V                                  V                                 V                                                                                 V                                                                                      V
 *   /--------------------------\  /--------------------------------------\  /--------------------\  /--------------------------------------------------------------------------------------------------------------------------------------------------\  /--------\
 *  {0x03, 0x45, 0xD3, 0xDD, 0xDF}{0x12, 0x44, 0x01, 0xC7, 0x55, 0x36, 0x21}{0x72, 0x4C, 0x31, 0x22}{0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xC2, 0xC3, 0xC4, 0xC5, 0x17, 0x40, 0x01, 0x02, 0x03, 0x04, 0x05, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8}{0x7C, 0xE5}
 *   |__________________________|  |__________________________|  |________|  |______________|  |__|  |________________________________________________________|  |________|  |__________________________________________________________________________|  |________|
 *                |                             |                     |             |           |                                 |                                  |                                           |                                              |
 *         before[0].v                        v[0]                  v[1]          v[0]        v[1]                          after[0].t                         after[1].v                                  after[2].t                                         v[0]
 *                |                                                                                                               |                                  |                                           |
 *                V                                                                                                               V                                  V                                           V
 *   /--------------------------\                                                                    /--------------------------------------------------------\  /--------\  /--------------------------------------------------------------------------\
 *  {0x03, 0x45, 0xD3, 0xDD, 0xDF}                                                                  {0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xC2, 0xC3, 0xC4, 0xC5}{0x17, 0x40}{0x01, 0x02, 0x03, 0x04, 0x05, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8}
 *   |________|  |______________|                                                                    |________________________________________________________|  |________|  |__________________________|  |____________________________________________|
 *       |              |                                                                                                         |                                  |                     |                                     |
 *     v[0]           v[1]                                                                                           after_before[0].v                             v[0]        after_after[0].v                      after_after[1].t
 *                                                                                                                                |                                                        |                                     |
 *                                                                                                                                V                                                        V                                     V
 *                                                                                                   /--------------------------------------------------------\              /--------------------------\  /--------------------------------------------\
 *                                                                                                  {0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xC2, 0xC3, 0xC4, 0xC5}            {0x01}{0x02, 0x03, 0x04}{0x05}{0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8}
 *                                                                                                   |________________________________|  |____________________|              |__|  |______________|  |__|  |____________________________________________|
 *                                                                                                                   |                             |                          |           |           |                          |
 *                                                                                                                 v[0]                          v[1]                       v[0]        v[1]        v[2]       after_after_after[0].v
 *                                                                                                                                                                                                                               |
 *                                                                                                                                                                                                                               V
 *                                                                                                                                                                                                         /--------------------------------------------\
 *                                                                                                                                                                                                        {0xF1, 0xF2, 0xF3}{0xF4, 0xF5, 0xF6, 0xF7, 0xF8}
 *                                                                                                                                                                                                         |______________|  |__________________________|
 *                                                                                                                                                                                                                 |                      |
 *                                                                                                                                                                                                               v[0]                   v[1]
 *
 * These buffers are useful for communication libraries.
 * Let say, there is a library which is responsible for communication protocol; it must add some header and checksum to transmitted data.
 * Communication library wants to add header (on the begining) and checksum (at the end) and must execute "transmit" function
 * (which writes data to transmitter) at one call.
 * In comparision to example described in vector explanation, Application doesnt provide linear buffer but vector.
 * For that reason, library must compose transmition vector, bigger than provided one, to first vector cell store pointer to header,
 * next copy cells from user data vector and at the end put checksum to last cell. Next, provide its pointer for transmit function.
 * Other solution is tree with 3 elements - add header to "tree[0].v" as pointer to one element vector,
 * next put user vector pointer to "tree[1].v" as second vector and at the end, add one element vector with checksum at "tree[2].v" pointer.
 * If data provided by user is already tree, then above example is same with one exception - "tree[1]" will point to sub-tree ("tree[1].t").
 */
#define BUFF_USE_TREE_BUFFERS                BUFF_DEFAULT_FEATURES_STATE
#endif

#ifndef BUFF_USE_RING_BUFFERS
/**
 * @brief Definition to enable / disable support for ring buffers. If defined to BUFF_FEATURE_ENABLED then all function used to copy
 * data from / to ring buffers will be enabled.
 * Ring buffer or sometimes called cyclic buffer is storage container. It is composed from linear buffer where user can store
 * any data. Type of the data and its interpretation is used-defined. Buffer only provides functions to write any number of bytes
 * (of course less than buffer size), read it (read with removing), peak it (read without removing), overwriting (modifying data already stored in buffer),
 * checking out/in (obtaining pointer of linear memory to be modified by user and after that saving result) and clearing (without reading).
 * Most important feature of this type of buffer is that data can be wrapped between the end and the begining of the buffer if during writing
 * end of buffer has been reached and on its begining there is free space (data from that location has been previously read).
 * This is fully invisible for used because write / peak / read / overwrite / remove / clear are encapsulated
 * (user doesn't operate directly on buffer's memory area but uses functions dedicated for it). If there is not enough free space
 * in the buffer during writing, this implementation of ring buffer may overwrite oldest data or not,
  * depends to definition BUFF_RING_ALLOW_OLD_DATA_OVERWRITE and write function parameter "overwrite_if_no_free_space".
 */
#define BUFF_USE_RING_BUFFERS                BUFF_DEFAULT_FEATURES_STATE
#endif

#ifndef BUFF_RING_ALLOW_OLD_DATA_OVERWRITE
/**
 * @brief Definition to enable / disable possibility to overwrite oldest data in ring buffer by write functions.
 * If this definition is set to BUFF_FEATURE_ENABLED then parameter "overwrite_if_no_free_space" of write functions determines
 * if oldest data shall be overwritten in case if there is not enough space in ring buffer. If defined to BUFF_FEATURE_DISABLED
 * then oldest data is NEVER overwritten and parameter "overwrite_if_no_free_space" of write functions is not used.
 */
#define BUFF_RING_ALLOW_OLD_DATA_OVERWRITE   BUFF_DEFAULT_FEATURES_STATE
#endif

#ifndef BUFF_RING_USE_EXTENSIONS
/**
 * @brief Definition to enable / disable ring buffer extensions.
 * Extension is a function which is called after buffer context modification.
 * There can be: read, write, overwrite and remove extensions. These extensions are called after buffer protection unlock.
 */
#define BUFF_RING_USE_EXTENSIONS             BUFF_DEFAULT_FEATURES_STATE
#endif

#ifndef BUFF_RING_USE_PROTECTED_EXTENSIONS
/**
 * @brief Definition to enable / disable ring buffer protected extensions.
 * Extension is a function which is called after buffer context modification.
 * There can be: read, write, overwrite and remove extensions. These extensions are called before buffer protection unlock.
 */
#define BUFF_RING_USE_PROTECTED_EXTENSIONS   BUFF_DEFAULT_FEATURES_STATE
#endif

#ifndef BUFF_RING_USE_PROTECTION
/**
 * @brief Definition used to enable / disable protection for ring buffers.
 * If defined to BUFF_FEATURE_ENABLED then for ach function which accesses ring buffer
 * there is enabled protection mechanism on entrance and disabled on exit.
 */
#define BUFF_RING_USE_PROTECTION             BUFF_FEATURE_ENABLED
#endif

#ifndef BUFF_RING_USE_BUSY_SIZE_MONITORING
/**
 * @brief Definition used to enable / disable functionality to monitor maximum usage of buffer.
 */
#define BUFF_RING_USE_BUSY_SIZE_MONITORING   BUFF_FEATURE_ENABLED
#endif

#ifndef BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED
/**
 * If defined to BUFF_FEATURE_ENABLED then functions Buff_Ring_Data_Check_Out and Buff_Ring_Data_Check_In will be enabled (exist).
 * This definition is placed in this section because it is used not only for functions enabling/disabling but also for structure
 * Buff_Ring_XT definition.
 */
#define BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED        BUFF_DEFAULT_FEATURES_STATE
#endif


/**
 * ----------------------------------------- MULTI-THREAD / MULTI-CONTEXT OPERATIONS PROTECTION --------------------------------
 *
 * This section contains macroes used to declare, initialize and manage protection mechanisms. These mechanisms are used
 * to protect operations on global data against multiple access in same time from different threads and from interruptions as well.
 * Used only for ring buffers.
 */


#ifndef BUFF_PROTECTION_DECLARE
/**
 * @brief Macro used to declare proper objects in ring buffer structure which are needed for protection mechanism.
 */
#define BUFF_PROTECTION_DECLARE()
#endif

#ifndef BUFF_PROTECTION_CREATE
/**
 * @brief Macro used to create dynamically proper objects (if needed) on ring buffer initialization
 *
 * @return true if creating objects succeeded, flase otherwise
 */
#define BUFF_PROTECTION_CREATE(buf)    true
#endif

#ifndef BUFF_PROTECTION_DESTROY
/**
 * @brief Macro used to destroy dynamic objects (if used) on ring buffer deinitialization
 */
#define BUFF_PROTECTION_DESTROY(buf)
#endif

#ifndef BUFF_PROTECTION_INIT
/**
 * @brief Macro used to initialize ring buffer protection mechanism on buffer initialization.
 * Called only if BUFF_PROTECTION_CREATE succeedes (returns true).
 *
 * @return true if initialization succeeded, flase otherwise
 */
#define BUFF_PROTECTION_INIT(buf)      true
#endif

#ifndef BUFF_PROTECTION_DEINIT
/**
 * @brief Macro used to deinitialize ring buffer protection mechanism on buffer deinitialization.
 * Caled BEFORE macro BUFF_PROTECTION_DESTROY.
 */
#define BUFF_PROTECTION_DEINIT(buf)
#endif

#ifndef BUFF_PROTECTION_LOCK
/**
 * @brief Macro used to activate lock mechanism on ring buffer. From this moment entity which called it has exclusive right
 * to access the buffer. No other thread / interruption will be able to access the buffer
 * until caller of BUFF_PROTECTION_LOCK calls BUFF_PROTECTION_UNLOCK.
 * For OS without memory protection usually critical section is used as protection mechanism which is best solution.
 * For OS with memory protections where buffer can be accessed only inside single process Mutex can be used to protect the buffer.
 */
#define BUFF_PROTECTION_LOCK(buf)
#endif

#ifndef BUFF_PROTECTION_UNLOCK
/**
 * @brief Macro used to deactivate lock mechanism on ring buffer. From this moment everyone will be able to access the buffer.
 * For OS without memory protection usually critical section is used as protection mechanism which is best solution.
 * For OS with memory protections where buffer can be accessed only inside single process Mutex can be used to protect the buffer.
 */
#define BUFF_PROTECTION_UNLOCK(buf)
#endif



/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY CONFIGURATION ------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY TYPES ------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

#ifndef BUFF_BOOL_DT_EXTERNAL
/**
 * @brief Type of boolean variable. If bool_t name is unknown type for used compiler, shall be externally re-defined
 * to some other type able to keep boolean state: false(BUFF_FALSE) and true(BUFF_TRUE).
 */
typedef bool_t Buff_Bool_DT;
#endif

#ifndef BUFF_SIZE_DT_EXTERNAL
/**
 * @brief Type of variable WITHOUT sign, used to store data size.
 * Must be big enough to store size of biggest possible data, for example if size of program memory in the system
 * is 48kB, size of RAM memory is 20kB then uint16_t is enough, if program memory is 120kB, RAM is 32kB then uint32_t is needed.
 * By default size_t is used and it doesn't have to be re-defined if size_t is properly defined for the platform.
 */
typedef size_t Buff_Size_DT;
#endif

#ifndef BUFF_NUM_ELEMS_DT_EXTERNAL
/**
 * @brief Type of variable WITHOUT sign, used to store number of vector or tree elements.
 * Must be big enough to store longest possible vector or tree, for example if there will be used vectors with maximum 5 elements
 * and trees with maximum 21 elements then uint8_t will be enough.
 * By default size_t is used and it doesn't have to be re-defined if size_t is properly defined for the platform.
 */
typedef size_t Buff_Num_Elems_DT;
#endif

#ifndef BUFF_MEMCPY_PARAMS_XT_EXTERNAL
/**
 * @brief Structure which is provided as a parameter of Buff_Vendor_Memcpy_HT handler (vendor-specific memcpy-like function)
 */
typedef struct Buff_Memcpy_Params_eXtended_Tag
{
   /* pointer to destination memory */
   void        *dest;
   /* pointer to source memory */
   const void  *src;
   /* offset in destination memory under which data shall be stored */
   Buff_Size_DT dest_offset;
   /* offset in source memory from which data shall be stored */
   Buff_Size_DT src_offset;
   /* size maximum size of data to be copied by Buff_Vendor_Memcpy_HT;
      function can copy less - in that case no more calls will be executed, even if is_last_part in current call is BUFF_FALSE */
   Buff_Size_DT size;
   /* if BUFF_TRUE then this is last call of Buff_Vendor_Memcpy_HT needed to copy requested data buffer */
   Buff_Bool_DT is_last_part;
}Buff_Memcpy_Params_XT;
#endif

#ifndef BUFF_VENDOR_MEMCPY_HT_EXTERNAL
/**
 * @brief Type of function which is used as memcpy-like vendor-specific function.
 * This function is used by functions which copy data from / to vendor-specific buffers.
 *
 * @result size of data which this function copied; can be less than "size" parameter provided by Buff_Memcpy_Params_XT structure.
 *
 * @param params pointer to structure with parameters of source, destination and size of data to be copied.
 */
typedef Buff_Size_DT (*Buff_Vendor_Memcpy_HT) (const Buff_Memcpy_Params_XT *params);
#endif



#ifndef BUFF_WRITEABLE_VECTOR_XT_EXTERNAL
/**
 * @brief Structure of single cell of writeable vector buffer (vector buffer to which data will be copied)
 */
typedef struct Buff_Writeable_Vector_eXtended_Tag
{
   void        *data;
   Buff_Size_DT size;
}Buff_Writeable_Vector_XT;
#endif

#ifndef BUFF_READABLE_VECTOR_XT_EXTERNAL
/**
 * @brief Structure of single cell of readable vector buffer (vector buffer from which data will be copied)
 */
typedef struct Buff_Readable_Vector_eXtended_Tag
{
   const void  *data;
   Buff_Size_DT size;
}Buff_Readable_Vector_XT;
#endif



typedef struct Buff_Writeable_Tree_eXtended_Tag
{
   union
   {
      struct
      {
         const struct Buff_Writeable_Tree_eXtended_Tag  *branch;
         Buff_Num_Elems_DT                               branch_num_elems;
      }tree;
      struct
      {
         const Buff_Writeable_Vector_XT                 *vector;
         Buff_Num_Elems_DT                               vector_num_elems;
      }vector;
   }elem_type;
   Buff_Bool_DT is_branch;
}Buff_Writeable_Tree_XT;



typedef struct Buff_Readable_Tree_eXtended_Tag
{
   union
   {
      struct
      {
         const struct Buff_Readable_Tree_eXtended_Tag   *branch;
         Buff_Num_Elems_DT                               branch_num_elems;
      }tree;
      struct
      {
         const Buff_Readable_Vector_XT                  *vector;
         Buff_Num_Elems_DT                               vector_num_elems;
      }vector;
   }elem_type;
   Buff_Bool_DT is_branch;
}Buff_Readable_Tree_XT;



#ifndef BUFF_RING_XT_EXTERNAL
/**
 * @brief Structure which describes ring buffer
 */
typedef struct Buff_Ring_eXtended_Tag
{
#if(BUFF_RING_USE_EXTENSIONS)
   struct Buff_Ring_Extensions_eXtended_Tag *extension;
#endif
#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
   struct Buff_Ring_Extensions_eXtended_Tag *protected_extension;
#endif
   void *memory;
#if(BUFF_RING_USE_PROTECTION)
   BUFF_PROTECTION_DECLARE()
#endif
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
   Buff_Size_DT original_size;
#endif
   Buff_Size_DT size;
   Buff_Size_DT busy_size;
#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
   Buff_Size_DT max_busy_size;
#endif
   Buff_Size_DT first_free;
   Buff_Size_DT first_busy;
}Buff_Ring_XT;
#endif

#ifndef BUFF_RING_EXTENSION_ON_READ_EXTERNAL
/**
 * @brief Type of function called as ring buffer extension after data reading.
 */
typedef void (*Buff_Ring_Extension_On_Read)        (Buff_Ring_XT *buf, struct Buff_Ring_Extensions_eXtended_Tag *extension, Buff_Size_DT size, Buff_Bool_DT rewind_occured);
#endif
#ifndef BUFF_RING_EXTENSION_ON_WRITE_EXTERNAL
/**
 * @brief Type of function called as ring buffer extension after data writing.
 */
typedef void (*Buff_Ring_Extension_On_Write)       (Buff_Ring_XT *buf, struct Buff_Ring_Extensions_eXtended_Tag *extension, Buff_Size_DT size, Buff_Bool_DT rewind_occured);
#endif
#ifndef BUFF_RING_EXTENSION_ON_OVERWRITE_EXTERNAL
/**
 * @brief Type of function called as ring buffer extension after data overwriting (modifying existing data in the buffer).
 */
typedef void (*Buff_Ring_Extension_On_Overwrite)   (Buff_Ring_XT *buf, struct Buff_Ring_Extensions_eXtended_Tag *extension, Buff_Size_DT pos, Buff_Size_DT size, Buff_Bool_DT rewind_occured);
#endif
#ifndef BUFF_RING_EXTENSION_ON_REMOVE_EXTERNAL
/**
 * @brief Type of function called as ring buffer extension after data removing.
 */
typedef void (*Buff_Ring_Extension_On_Remove)      (Buff_Ring_XT *buf, struct Buff_Ring_Extensions_eXtended_Tag *extension, Buff_Bool_DT rewind_occured);
#endif

#ifndef BUFF_RING_EXTENSIONS_XT_EXTERNAL
/**
 * @brief Structure which contains ring buffer extensions
 */
typedef struct Buff_Ring_Extensions_eXtended_Tag
{
   void *on_read_params;
   void *on_write_params;
   void *on_overwrite_params;
   void *on_remove_params;
   Buff_Ring_Extension_On_Read      on_read;
   Buff_Ring_Extension_On_Write     on_write;
   Buff_Ring_Extension_On_Overwrite on_overwrite;
   Buff_Ring_Extension_On_Remove    on_remove;
}Buff_Ring_Extensions_XT;
#endif

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY TYPES --------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY INTERFACES -------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

#if(BUFF_USE_VECTOR_BUFFERS)

#ifndef BUFF_GET_READABLE_VECTOR_DATA_SIZE_ENABLED
#define BUFF_GET_READABLE_VECTOR_DATA_SIZE_ENABLED       BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_GET_READABLE_VECTOR_DATA_SIZE_ENABLED)
/**
 * @brief Function which calculates size of readable vector of linear buffers
 *
 * @result size of readable vector
 *
 * @param vector pointer to readable vector for which size shall be calculated
 * @param vector_num_elems number of vector elements (independent linear buffers which create vector data)
 */
Buff_Size_DT Buff_Get_Readable_Vector_Data_Size(const Buff_Readable_Vector_XT *vector, Buff_Num_Elems_DT vector_num_elems);
#endif

#ifndef BUFF_GET_WRITEABLE_VECTOR_DATA_SIZE_ENABLED
#define BUFF_GET_WRITEABLE_VECTOR_DATA_SIZE_ENABLED      BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_GET_WRITEABLE_VECTOR_DATA_SIZE_ENABLED)
/**
 * @brief Function which calculates size of writeable vector of linear buffers
 *
 * @result size of writeable vector
 *
 * @param vector pointer to writeable vector for which size shall be calculated
 * @param vector_num_elems number of vector elements (independent linear buffers which create vector data)
 */
Buff_Size_DT Buff_Get_Writeable_Vector_Data_Size(const Buff_Writeable_Vector_XT *vector, Buff_Num_Elems_DT vector_num_elems);
#endif

#ifndef BUFF_COPY_FROM_VECTOR_ENABLED
#define BUFF_COPY_FROM_VECTOR_ENABLED                    BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_COPY_FROM_VECTOR_ENABLED)
/**
 * @brief Function which copies data from vector of linear buffers to one linear buffer.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - linear buffer into which data will be copied
 * @param src source - vector of linear buffers from which data will be copied
 * @param src_num_elems number of source vector elements (independent linear buffers which create vector data)
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 */
Buff_Size_DT Buff_Copy_From_Vector(
   void                            *dest,
   const Buff_Readable_Vector_XT   *src,
   Buff_Num_Elems_DT                src_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src);
#endif

#ifndef BUFF_COPY_FROM_VECTOR_VENDOR_ENABLED
#define BUFF_COPY_FROM_VECTOR_VENDOR_ENABLED             BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_COPY_FROM_VECTOR_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
/**
 * @brief Function which copies data from vector of linear buffers to vendor-specific buffer.
 * Difference between this function and Buff_Copy_From_Vector is that this function copies data using vendor-specific function
 * instead of "memcpy". For that reason this function can be also used to read data to vendor-specific buffer
 * instead to linear buffer.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - vendor-specific buffer into which data will be copied
 * @param src source - vector of linear buffers from which data will be copied
 * @param src_num_elems number of source vector elements (independent linear buffers which create vector data)
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 * @param vendor_cpy pointer to vendor-specific function which will be used as "memcpy" function by Buff_Copy_From_Vector_Vendor
 */
Buff_Size_DT Buff_Copy_From_Vector_Vendor(
   void                            *dest,
   const Buff_Readable_Vector_XT   *src,
   Buff_Num_Elems_DT                src_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src,
   Buff_Vendor_Memcpy_HT            vendor_cpy);
#endif

#ifndef BUFF_COPY_TO_VECTOR_ENABLED
#define BUFF_COPY_TO_VECTOR_ENABLED                      BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_COPY_TO_VECTOR_ENABLED)
/**
 * @brief Function which copies data from one linear buffer to vector of linear buffers.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - vector of linear buffers into which data will be copied
 * @param dest_num_elems number of destination vector elements (independent linear buffers which create vector data)
 * @param src source - linear buffer from which data will be copied
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 */
Buff_Size_DT Buff_Copy_To_Vector(
   const Buff_Writeable_Vector_XT  *dest,
   Buff_Num_Elems_DT                dest_num_elems,
   const void                      *src,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src);
#endif

#ifndef BUFF_COPY_TO_VECTOR_VENDOR_ENABLED
#define BUFF_COPY_TO_VECTOR_VENDOR_ENABLED               BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_COPY_TO_VECTOR_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
/**
 * @brief Function which copies data from vendor-specific buffer to vector of linear buffers.
 * Difference between this function and Buff_Copy_To_Vector is that this function copies data using vendor-specific function
 * instead of "memcpy". For that reason this function can be also used to read data to vendor-specific buffer
 * instead to linear buffer.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - vector of linear buffers into which data will be copied
 * @param dest_num_elems number of destination vector elements (independent linear buffers which create vector data)
 * @param src source - linear buffer from which data will be copied
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 * @param vendor_cpy pointer to vendor-specific function which will be used as "memcpy" function by Buff_Copy_To_Vector_Vendor
 */
Buff_Size_DT Buff_Copy_To_Vector_Vendor(
   const Buff_Writeable_Vector_XT  *dest,
   Buff_Num_Elems_DT                dest_num_elems,
   const void                      *src,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src,
   Buff_Vendor_Memcpy_HT            vendor_cpy);
#endif

#ifndef BUFF_COPY_VECTOR_TO_VECTOR_ENABLED
#define BUFF_COPY_VECTOR_TO_VECTOR_ENABLED               BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_COPY_VECTOR_TO_VECTOR_ENABLED)
/**
 * @brief Function which copies data from vector of linear buffers to another vector of linear buffers.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - vector of linear buffers into which data will be copied
 * @param dest_num_elems number of destination vector elements (independent linear buffers which create vector data)
 * @param src source - vector of linear buffers from which data will be copied
 * @param src_num_elems number of source vector elements (independent linear buffers which create vector data)
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 */
Buff_Size_DT Buff_Copy_Vector_To_Vector(
   const Buff_Writeable_Vector_XT  *dest,
   Buff_Num_Elems_DT                dest_num_elems,
   const Buff_Readable_Vector_XT   *src,
   Buff_Num_Elems_DT                src_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src);
#endif

#endif /* #if(BUFF_USE_VECTOR_BUFFERS) */



#if(BUFF_USE_TREE_BUFFERS)

#ifndef BUFF_GET_READABLE_TREE_DATA_SIZE_ENABLED
#define BUFF_GET_READABLE_TREE_DATA_SIZE_ENABLED         BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_GET_READABLE_TREE_DATA_SIZE_ENABLED)
/**
 * @brief Function which calculates size readable tree of vectors of linear buffers
 *
 * @result size of readable tree
 *
 * @param tree pointer to readable tree for which size shall be calculated
 * @param tree_num_elems number of elements in tree root
 */
Buff_Size_DT Buff_Get_Readable_Tree_Data_Size(const Buff_Readable_Tree_XT *tree, Buff_Num_Elems_DT tree_num_elems);
#endif

#ifndef BUFF_GET_WRITEABLE_TREE_DATA_SIZE_ENABLED
#define BUFF_GET_WRITEABLE_TREE_DATA_SIZE_ENABLED        BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_GET_WRITEABLE_TREE_DATA_SIZE_ENABLED)
/**
 * @brief Function which calculates size writeable tree of vectors of linear buffers
 *
 * @result size of writeable tree
 *
 * @param tree pointer to writeable tree for which size shall be calculated
 * @param tree_num_elems number of elements in tree root
 */
Buff_Size_DT Buff_Get_Writeable_Tree_Data_Size(const Buff_Writeable_Tree_XT *tree, Buff_Num_Elems_DT tree_num_elems);
#endif

#ifndef BUFF_COPY_FROM_TREE_ENABLED
#define BUFF_COPY_FROM_TREE_ENABLED                      BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_COPY_FROM_TREE_ENABLED)
/**
 * @brief Function which copies data from tree of vectors of linear buffers to one linear buffer.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - linear buffer into which data will be copied
 * @param src source - tree of vectors of linear buffers from which data will be copied
 * @param src_num_elems number of elements in source tree root
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 * @param str_size pointer to variable into which size of data in source buffer will be returned
 * (distance in bytes between begining of source buffer and last byte in last linear part of source analyzed during copying operation -
 * function ends when reaches offset_src & size or end of source or destination). This parameter is useful in recursive calls
 * of this function and it is optional. When not needed, can be set to BUFF_MAKE_INVALID_PTR(Buff_Size_DT).
 */
Buff_Size_DT Buff_Copy_From_Tree(
   void                         *dest,
   const Buff_Readable_Tree_XT  *src,
   Buff_Num_Elems_DT             src_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size);
#endif

#ifndef BUFF_COPY_FROM_TREE_VENDOR_ENABLED
#define BUFF_COPY_FROM_TREE_VENDOR_ENABLED               BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_COPY_FROM_TREE_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
/**
 * @brief Function which copies data from tree of vectors of linear buffers to vendor-specific buffer.
 * Difference between this function and Buff_Copy_From_Tree is that this function copies data using vendor-specific function
 * instead of "memcpy". For that reason this function can be also used to read data to vendor-specific buffer
 * instead to linear buffer.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - vendor-specific buffer into which data will be copied
 * @param src source - tree of vectors of linear buffers from which data will be copied
 * @param src_num_elems number of elements in source tree root
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 * @param str_size pointer to variable into which size of data in source buffer will be returned
 * (distance in bytes between begining of source buffer and last byte in last linear part of source analyzed during copying operation -
 * function ends when reaches offset_src & size or end of source or destination). This parameter is useful in recursive calls
 * of this function and it is optional. When not needed, can be set to BUFF_MAKE_INVALID_PTR(Buff_Size_DT).
 * @param vendor_cpy pointer to vendor-specific function which will be used as "memcpy" function by Buff_Copy_From_Tree_Vendor
 */
Buff_Size_DT Buff_Copy_From_Tree_Vendor(
   void                         *dest,
   const Buff_Readable_Tree_XT  *src,
   Buff_Num_Elems_DT             src_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size,
   Buff_Vendor_Memcpy_HT         vendor_cpy);
#endif

#ifndef BUFF_COPY_TO_TREE_ENABLED
#define BUFF_COPY_TO_TREE_ENABLED                        BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_COPY_TO_TREE_ENABLED)
/**
 * @brief Function which copies data from one linear buffer to tree of vectors of linear buffers.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - tree of vectors of linear buffers into which data will be copied
 * @param dest_num_elems number of elements in destination tree root
 * @param src source - linear buffer from which data will be copied
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 * @param str_size pointer to variable into which size of data in source buffer will be returned
 * (distance in bytes between begining of source buffer and last byte in last linear part of source analyzed during copying operation -
 * function ends when reaches offset_src & size or end of source or destination). This parameter is useful in recursive calls
 * of this function and it is optional. When not needed, can be set to BUFF_MAKE_INVALID_PTR(Buff_Size_DT).
 */
Buff_Size_DT Buff_Copy_To_Tree(
   const Buff_Writeable_Tree_XT *dest,
   Buff_Num_Elems_DT             dest_num_elems,
   const void                   *src,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size);
#endif

#ifndef BUFF_COPY_TO_TREE_VENDOR_ENABLED
#define BUFF_COPY_TO_TREE_VENDOR_ENABLED                 BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_COPY_TO_TREE_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
/**
 * @brief Function which copies data from vendor-specific buffer to tree of vectors of linear buffers.
 * Difference between this function and Buff_Copy_To_Tree is that this function copies data using vendor-specific function
 * instead of "memcpy". For that reason this function can be also used to read data to vendor-specific buffer
 * instead to linear buffer.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - tree of vectors of linear buffers into which data will be copied
 * @param dest_num_elems number of elements in destination tree root
 * @param src source - vendor-specific buffer from which data will be copied
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 * @param str_size pointer to variable into which size of data in source buffer will be returned
 * (distance in bytes between begining of source buffer and last byte in last linear part of source analyzed during copying operation -
 * function ends when reaches offset_src & size or end of source or destination). This parameter is useful in recursive calls
 * of this function and it is optional. When not needed, can be set to BUFF_MAKE_INVALID_PTR(Buff_Size_DT).
 * @param vendor_cpy pointer to vendor-specific function which will be used as "memcpy" function by Buff_Copy_To_Tree_Vendor
 */
Buff_Size_DT Buff_Copy_To_Tree_Vendor(
   const Buff_Writeable_Tree_XT *dest,
   Buff_Num_Elems_DT             dest_num_elems,
   const void                   *src,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size,
   Buff_Vendor_Memcpy_HT         vendor_cpy);
#endif

#ifndef BUFF_COPY_TREE_TO_TREE_ENABLED
#define BUFF_COPY_TREE_TO_TREE_ENABLED                   BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_COPY_TREE_TO_TREE_ENABLED)
/**
 * @brief Function which copies data from tree of vectors of linear buffers to another tree of vectors of linear buffers.
 *
 * @result number of bytes copied from source to destination
 *
 * @param dest destination - tree of vectors of linear buffers into which data will be copied
 * @param dest_num_elems number of elements in destination tree root
 * @param src source - tree of vectors of linear buffers from which data will be copied
 * @param src_num_elems number of elements in source tree root
 * @param size number of bytes to be copied from source to destination
 * @param offset_dest offset in destination buffer between begining of the buffer and first byte to which data will be copied
 * @param offset_src offset in source buffer between begining of the buffer and first byte from which data will be copied
 * @param str_size pointer to variable into which size of data in source buffer will be returned
 * (distance in bytes between begining of source buffer and last byte in last linear part of source analyzed during copying operation -
 * function ends when reaches offset_src & size or end of source or destination). This parameter is useful in recursive calls
 * of this function and it is optional. When not needed, can be set to BUFF_MAKE_INVALID_PTR(Buff_Size_DT).
 * @param end_of_destination_reached pointer to variable into which BUFF_TRUE will be returned if end of destination buffer
 * has been reached.
 */
Buff_Size_DT Buff_Copy_Tree_To_Tree(
   const Buff_Writeable_Tree_XT *dest,
   Buff_Num_Elems_DT             dest_num_elems,
   const Buff_Readable_Tree_XT  *src,
   Buff_Num_Elems_DT             src_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size,
   Buff_Bool_DT                 *end_of_destination_reached);
#endif

#endif /* #if(BUFF_USE_TREE_BUFFERS) */



#if(BUFF_USE_RING_BUFFERS)

#ifndef BUFF_RING_INIT_ENABLED
#define BUFF_RING_INIT_ENABLED                           BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_INIT_ENABLED)
/**
 * @brief Function which initializes ring buffer - sets all internal pointers and indexes properly to be able to operate on buffer.
 *
 * @result BUFF_TRUE if initialization succeeded, BUFF_FALSE otherwise
 *
 * @param buf pointer to buffer to be initialized
 * @param memory pointer to RAM memory on which buffer will operate (store data and read from)
 * @param size size of memory in bytes
 */
Buff_Bool_DT Buff_Ring_Init(Buff_Ring_XT *buf, void *memory, Buff_Size_DT size);
#endif

#ifndef BUFF_RING_DEINIT_ENABLED
#define BUFF_RING_DEINIT_ENABLED                         BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_DEINIT_ENABLED)
/**
 * @brief Function which deinitializes ring buffer - sets all internal pointers to invalid,
 * so no more operations will be allowed on the buffer.
 *
 * @param buf pointer to ring buffer to be deinitialized
 */
void   Buff_Ring_Deinit(Buff_Ring_XT *buf);
#endif

#ifndef BUFF_RING_INIT_EXTENSION_ENABLED
#define BUFF_RING_INIT_EXTENSION_ENABLED                 BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_INIT_EXTENSION_ENABLED && BUFF_RING_USE_EXTENSIONS )
/**
 * @brief Function which initializes to defaults structure with ring buffer extensions.
 * Extension is a function which is called after buffer context modification.
 * There can be: read, write, overwrite and remove extensions. Extensions installed using this function
 * are called after buffer protection unlock (not protected extensions).
 *
 * @param extension pointer to extensions structure to be initialized
 */
void Buff_Ring_Init_Extension(Buff_Ring_Extensions_XT *extension);
#endif

#ifndef BUFF_RING_ADD_EXTENSION_ENABLED
#define BUFF_RING_ADD_EXTENSION_ENABLED                  BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_ADD_EXTENSION_ENABLED && BUFF_RING_USE_EXTENSIONS )
/**
 * @brief Function which installs structure with extensions in buffer.
 * Extension is a function which is called after buffer context modification.
 * There can be: read, write, overwrite and remove extensions. Extensions installed using this function
 * are called after buffer protection unlock (not protected extensions).
 *
 * @result BUFF_TRUE if installation succeedded, BUFF_FALSE otherwise
 *
 * @param buf pointer to ring buffer into which extensions collection shall be installed
 * @param extension pointer to structure with extensions
 */
Buff_Bool_DT Buff_Ring_Add_Extension(Buff_Ring_XT *buf, Buff_Ring_Extensions_XT *extension);
#endif

#ifndef BUFF_RING_ADD_PROTECTED_EXTENSION_ENABLED
#define BUFF_RING_ADD_PROTECTED_EXTENSION_ENABLED        BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_ADD_PROTECTED_EXTENSION_ENABLED && BUFF_RING_USE_PROTECTED_EXTENSIONS )
/**
 * @brief Function which installs structure with extensions in buffer.
 * Extension is a function which is called after buffer context modification.
 * There can be: read, write, overwrite and remove extensions. Extensions installed using this function
 * are called before buffer protection unlock (protected extensions).
 *
 * @result BUFF_TRUE if installation succeedded, BUFF_FALSE otherwise
 *
 * @param buf pointer to ring buffer into which extensions collection shall be installed
 * @param extension pointer to structure with extensions
 */
Buff_Bool_DT Buff_Ring_Add_Protected_Extension(Buff_Ring_XT *buf, Buff_Ring_Extensions_XT *protected_extension);
#endif

#ifndef BUFF_PROTECTION_LOCK_ENABLED
#define BUFF_PROTECTION_LOCK_ENABLED                     BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_PROTECTION_LOCK_ENABLED && BUFF_RING_USE_PROTECTION )
/**
 * @brief Function which permanently locks access to the buffer using protection mechanism. From this moment, only thread/process
 * which locked buffer can operate on it. This function is useful if multiple operations which cannot be interrupted
 * must be executed on buffer.
 *
 * @param buf pointer to buffer for which access shall be locked
 */
void Buff_Protection_Lock(Buff_Ring_XT *buf);
#endif

#ifndef BUFF_PROTECTION_UNLOCK_ENABLED
#define BUFF_PROTECTION_UNLOCK_ENABLED                   BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_PROTECTION_UNLOCK_ENABLED && BUFF_RING_USE_PROTECTION )
/**
 * @brief Function which permanently unlocks access to the buffer using protection mechanism. From this moment, any thread/process
 * can operate on it. This function is useful if multiple operations which cannot be interrupted
 * must be executed on buffer.
 *
 * @param buf pointer to buffer for which access shall be unlocked
 */
void Buff_Protection_Unlock(Buff_Ring_XT *buf);
#endif

#ifndef BUFF_RING_IS_EMPTY
/**
 * @brief Macro which checks if buffer is empty
 *
 * @result true if buffer is empty, false otherwise
 *
 * @param buf pointer to buffer for which state shall be checked
 */
#define BUFF_RING_IS_EMPTY(buf)        (0 == (buf)->busy_size)
#endif

#ifndef BUFF_RING_IS_EMPTY_ENABLED
#define BUFF_RING_IS_EMPTY_ENABLED                       BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_IS_EMPTY_ENABLED)
/**
 * @brief Function which checks if buffer is empty (same as macro BUFF_RING_IS_EMPTY)
 *
 * @result BUFF_TRUE if buffer is empty, BUFF_FALSE otherwise
 *
 * @param buf pointer to buffer for which state shall be checked
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of checking buffer state, BUFF_FALSE otherwise
 */
Buff_Bool_DT Buff_Ring_Is_Empty(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif

#ifndef BUFF_RING_IS_FULL
/**
 * @brief Macro which checks if buffer is full (no free space)
 *
 * @result true if buffer is full, false otherwise
 *
 * @param buf pointer to buffer for which state shall be checked
 */
#define BUFF_RING_IS_FULL(buf)         ((buf)->size == (buf)->busy_size)
#endif

#ifndef BUFF_RING_IS_FULL_ENABLED
#define BUFF_RING_IS_FULL_ENABLED                        BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_IS_FULL_ENABLED)
/**
 * @brief Function which checks if buffer is full (same as macro BUFF_RING_IS_FULL)
 *
 * @result BUFF_TRUE if buffer is full, BUFF_FALSE otherwise
 *
 * @param buf pointer to buffer for which state shall be checked
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of checking buffer state, BUFF_FALSE otherwise
 */
Buff_Bool_DT Buff_Ring_Is_Full(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif

#ifndef BUFF_RING_GET_SIZE
/**
 * @brief Macro which gets size of buffer - size of data which can be stored in buffer when buffer is empty.
 *
 * @result size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 */
#define BUFF_RING_GET_SIZE(buf)        ((buf)->size)
#endif

#ifndef BUFF_RING_GET_SIZE_ENABLED
#define BUFF_RING_GET_SIZE_ENABLED                       BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_GET_SIZE_ENABLED)
/**
 * @brief Function which gets size of buffer - size of data which can be stored in buffer when buffer is empty
 * (same as macro BUFF_RING_GET_SIZE).
 *
 * @result size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of checking buffer state, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Get_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif

#ifndef BUFF_RING_GET_BUSY_SIZE
/**
 * @brief Macro which gets busy size of buffer - size of data stored in buffer at this moment.
 *
 * @result busy size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 */
#define BUFF_RING_GET_BUSY_SIZE(buf)     ((buf)->busy_size)
#endif

#ifndef BUFF_RING_GET_BUSY_SIZE_ENABLED
#define BUFF_RING_GET_BUSY_SIZE_ENABLED                  BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_GET_BUSY_SIZE_ENABLED)
/**
 * @brief Function which gets busy size of buffer - size of data stored in buffer at this moment
 * (same as macro BUFF_RING_GET_BUSY_SIZE).
 *
 * @result busy size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of checking buffer state, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Get_Busy_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif

#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
#ifndef BUFF_RING_GET_MAX_BUSY_SIZE
/**
 * @brief Macro which gets maximum busy size of buffer - maximum size of data stored in buffer since buffer initialization.
 *
 * @result maximum busy size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 */
#define BUFF_RING_GET_MAX_BUSY_SIZE(buf)  ((buf)->max_busy_size)
#endif

#ifndef BUFF_RING_GET_MAX_BUSY_SIZE_ENABLED
#define BUFF_RING_GET_MAX_BUSY_SIZE_ENABLED              BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_GET_MAX_BUSY_SIZE_ENABLED)
/**
 * @brief Function which gets maximum busy size of buffer - maximum size of data stored in buffer since buffer initialization.
 * (same as macro BUFF_RING_GET_MAX_BUSY_SIZE).
 *
 * @result maximum busy size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of checking buffer state, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Get_Max_Busy_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif
#endif

#ifndef BUFF_RING_GET_FREE_SIZE
/**
 * @brief Macro which gets free size of buffer - size of data which can be stored in buffer at this moment.
 *
 * @result free size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 */
#define BUFF_RING_GET_FREE_SIZE(buf)     ((buf)->size - (buf)->busy_size)
#endif

#ifndef BUFF_RING_GET_FREE_SIZE_ENABLED
#define BUFF_RING_GET_FREE_SIZE_ENABLED                  BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_GET_FREE_SIZE_ENABLED)
/**
 * @brief Function which gets free size of buffer - size of data which can be stored in buffer at this moment
 * (same as macro BUFF_RING_GET_FREE_SIZE).
 *
 * @result free size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of checking buffer state, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Get_Free_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif

#ifndef BUFF_RING_GET_CONTINOUS_FREE_SIZE_ENABLED
#define BUFF_RING_GET_CONTINOUS_FREE_SIZE_ENABLED        BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_GET_CONTINOUS_FREE_SIZE_ENABLED)
/**
 * @brief Function which gets first continous free size of buffer - size of continous part of free space in the buffer just after busy area.
 * Value returned by this function will be different than value returned by function Buff_Ring_Get_Free_Size in case
 * if free space is divided (part of free space is at the ned of buffer, part at begining. In this case this function retirns size of part at the end of buffer.
 *
 * @result first continous free size of buffer in bytes, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which size shall be returned
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of checking buffer state, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Get_Continous_Free_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif

#ifndef BUFF_RING_GET_CURRENT_POS
/**
 * @brief Macro which gets current position in the buffer - position of first free byte.
 *
 * @result current position in the buffer, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which position shall be returned
 */
#define BUFF_RING_GET_CURRENT_POS(buf)     ((buf)->first_free)
#endif

#ifndef BUFF_RING_GET_CURRENT_POS_ENABLED
#define BUFF_RING_GET_CURRENT_POS_ENABLED                BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_GET_CURRENT_POS_ENABLED)
/**
 * @brief Function which gets current position in the buffer - position of first free byte.
 *
 * @result current position in the buffer, type of returned data is Buff_Size_DT
 *
 * @param buf pointer to buffer for which position shall be returned
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of checking buffer state, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Get_Current_Pos(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif

#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
/**
 * @brief Function which gets pointer of first free byte in the buffer - position under which next write will store the data
 * (same as macro BUFF_RING_GET_CURRENT_POS). Protection is not locked by this function - it must be done before by Buff_Protection_Lock.
 *
 * @result valid pointer of first free byte in the buffer if there is free space; BUFF_MAKE_INVALID_PTR(void) otherwise
 *
 * @param buf pointer to buffer for "check out" operation shall be executed
 * @param size_needed size of data which is expected to be stored
 * @param overwrite_if_no_free_space if BUFF_TRUE then old data will be removed if there isn't enough free space, BUFF_FALSE otherwise
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer on the begining, leaving active when exit. BUFF_FALSE otherwise
 */
void *Buff_Ring_Data_Check_Out(
   Buff_Ring_XT  *buf,
   Buff_Size_DT   size_needed,
   Buff_Bool_DT   overwrite_if_no_free_space,
   Buff_Bool_DT   use_protection);
#endif

#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
/**
 * Function used to mark that number of bytes has been stored in ring buffer without using "Write" functions.
 * This function is to be used together with Buff_Ring_Get_Free_Space_Ptr which first returns pointer to free area,
 * next under that pointer there can be stored user data and at the end check-in that data to buffer.
 * Protection is not locked by this function - it must be done before by Buff_Protection_Lock and unlocked after Buff_Protection_Unlock.
 *
 * @result position of first byte of data checked-in if check-in succeeded, size of buffer or bigger value otherwise
 * WARNING!!!
 * This is not offset from begining of busy space but offset from begining of buffer memmory (because begining of busy space is floating).
 * This value can be later used in functions Buff_Ring_OverWrite_If_Exist / Buff_Ring_OverWrite_If_Exist_Vendor
 *
 * @param buf pointer to buffer for which data shall be checked-in.
 * @param size size of data which shall be checked-in.
 * @param use_protection if BUFF_TRUE then function will unlock access to the buffer on the exit. BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Data_Check_In(Buff_Ring_XT *buf, Buff_Size_DT size, Buff_Bool_DT use_protection);
#endif

#ifndef BUFF_RING_WRITE_ENABLED
#define BUFF_RING_WRITE_ENABLED                          BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_WRITE_ENABLED)
/**
 * @brief Function which writes data from linear buffer to ring buffer.
 *
 * @result size of data which has been copied to ring buffer
 *
 * @param buf pointer to buffer in which data shall be stored
 * @param data pointer to linear data to be stored in the buffer
 * @param size size of data to be copied from linear buffer to ring buffer
 * @param overwrite_if_no_free_space if BUFF_TRUE and if there is not enough free space then oldest data will be overwritten
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Write(
   Buff_Ring_XT  *buf,
   const void    *data,
   Buff_Size_DT   size,
   Buff_Bool_DT   overwrite_if_no_free_space,
   Buff_Bool_DT   use_protection);
#endif

#ifndef BUFF_RING_WRITE_VENDOR_ENABLED
#define BUFF_RING_WRITE_VENDOR_ENABLED                   BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_WRITE_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
/**
 * @brief Function which writes data from vendor-specific buffer to ring buffer.
 * Difference between this function and Buff_Ring_Write is that this function copies data using vendor-specific function
 * instead of "memcpy". For that reason this function can be also used to write data from vendor-specific buffer
 * instead from linear buffer.
 *
 * @result size of data which has been copied to ring buffer
 *
 * @param buf pointer to buffer in which data shall be stored
 * @param data pointer to vendor-specific data which will be provided to vendor_cpy function as one of parameters
 * @param size size of data to be copied from vendor-specific buffer to ring buffer
 * @param vendor_cpy pointer to vendor-specific function which will be used as "memcpy" function by Buff_Ring_Write_Vendor
 * @param overwrite_if_no_free_space if BUFF_TRUE and if there is not enough free space then oldest data will be overwritten
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Write_Vendor(
   Buff_Ring_XT           *buf,
   const void             *data,
   Buff_Size_DT            size,
   Buff_Vendor_Memcpy_HT   vendor_cpy,
   Buff_Bool_DT            overwrite_if_no_free_space,
   Buff_Bool_DT            use_protection);
#endif

#ifndef BUFF_RING_WRITE_FROM_VECTOR_ENABLED
#define BUFF_RING_WRITE_FROM_VECTOR_ENABLED              BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_WRITE_FROM_VECTOR_ENABLED && BUFF_USE_VECTOR_BUFFERS )
/**
 * @brief Function which writes data from vector of linear buffers (Buff_Readable_Vector_XT) to ring buffer.
 *
 * @result size of data which has been copied to ring buffer
 *
 * @param buf pointer to buffer in which data shall be stored
 * @param data pointer to vector of linear buffers (Buff_Readable_Vector_XT) of data to be stored in the buffer
 * @param data_num_elems number of data vector elements (independent linear buffers which create vector data)
 * @param size size of data to be copied from vector of linear buffers to ring buffer
 * @param offset_data offset in vector of linear buffers from which data will be copied to ring buffer
 * @param overwrite_if_no_free_space if BUFF_TRUE and if there is not enough free space then oldest data will be overwritten
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Write_From_Vector(
   Buff_Ring_XT                    *buf,
   const Buff_Readable_Vector_XT   *data,
   Buff_Num_Elems_DT                data_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_data,
   Buff_Bool_DT                     overwrite_if_no_free_space,
   Buff_Bool_DT                     use_protection);
#endif

#ifndef BUFF_RING_WRITE_FROM_TREE_ENABLED
#define BUFF_RING_WRITE_FROM_TREE_ENABLED                BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_WRITE_FROM_TREE_ENABLED && BUFF_USE_TREE_BUFFERS )
/**
 * @brief Function which writes data from tree of linear buffers (Buff_Readable_Tree_XT) to ring buffer.
 *
 * @result size of data which has been copied to ring buffer
 *
 * @param buf pointer to buffer in which data shall be stored
 * @param data pointer to tree of linear buffers (Buff_Readable_Tree_XT) of data to be stored in the buffer
 * @param data_num_elems number of elements in data tree root
 * @param size size of data to be copied from tree of linear buffers to ring buffer
 * @param offset_data offset in tree of linear buffers from which data will be copied to ring buffer
 * @param overwrite_if_no_free_space if BUFF_TRUE and if there is not enough free space then oldest data will be overwritten
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Write_From_Tree(
   Buff_Ring_XT                 *buf,
   const Buff_Readable_Tree_XT  *data,
   Buff_Num_Elems_DT             data_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_data,
   Buff_Bool_DT                  overwrite_if_no_free_space,
   Buff_Bool_DT                  use_protection);
#endif

#ifndef BUFF_RING_OVERWRITE_IF_EXIST_ENABLED
#define BUFF_RING_OVERWRITE_IF_EXIST_ENABLED             BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_OVERWRITE_IF_EXIST_ENABLED)
/**
 * @brief Function which overwrites data in the buffer starting from pointed position.
 * Function succeeds if starting from pointed position there is at least "size" bytes ocupied in the buffer.
 *
 * @result size of data which has been successfully overwritten
 *
 * @param buf pointer to buffer in which data shall be overwritten
 * @param data pointer to linear data to be stored in the buffer
 * @param size size of data to be copied from linear buffer to ring buffer
 * @param pos begining of block of data in ring buffer which shall be overwritten.
 * WARNING!!!
 * This is not offset from begining of busy space but offset from begining of buffer memmory (because begining of busy space is floating).
 * Function Buff_Ring_Get_Current_Pos can be used to obtain position in buffer where data will be stored (before calling Buff_Ring_Write).
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_OverWrite_If_Exist(
   Buff_Ring_XT  *buf,
   const void    *data,
   Buff_Size_DT   size,
   Buff_Size_DT   pos,
   Buff_Bool_DT   use_protection);
#endif

#ifndef BUFF_RING_OVERWRITE_IF_EXIST_VENDOR_ENABLED
#define BUFF_RING_OVERWRITE_IF_EXIST_VENDOR_ENABLED      BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_OVERWRITE_IF_EXIST_VENDOR_ENABLED)
/**
 * @brief Function which overwrites data in the buffer starting from pointed position.
 * Function succeeds if starting from pointed position there is at least "size" bytes ocupied in the buffer.
 * Difference between this function and Buff_Ring_OverWrite_If_Exist is that this function copies data using vendor-specific function
 * instead of "memcpy". For that reason this function can be also used to overwrite data from vendor-specific buffer
 * instead from linear buffer.
 *
 * @result size of data which has been successfully overwritten
 *
 * @param buf pointer to buffer in which data shall be overwritten
 * @param data pointer to linear data to be stored in the buffer
 * @param size size of data to be copied from linear buffer to ring buffer
 * @param pos begining of block of data in ring buffer which shall be overwritten
 * WARNING!!!
 * This is not offset from begining of busy space but offset from begining of buffer memmory (because begining of busy space is floating).
 * Function Buff_Ring_Get_Current_Pos can be used to obtain position in buffer where data will be stored (before calling Buff_Ring_Write).
 * @param vendor_cpy pointer to vendor-specific function which will be used as "memcpy" function by Buff_Ring_Write_Vendor
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_OverWrite_If_Exist_Vendor(
   Buff_Ring_XT           *buf,
   const void             *data,
   Buff_Size_DT            size,
   Buff_Size_DT            pos,
   Buff_Vendor_Memcpy_HT   vendor_cpy,
   Buff_Bool_DT            use_protection);
#endif

#ifndef BUFF_RING_PEAK_ENABLED
#define BUFF_RING_PEAK_ENABLED                           BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_PEAK_ENABLED)
/**
 * @brief Function which reads data from ring buffer to linear buffer.
 * This function doesn't remove data from ring buffer after reading.
 *
 * @result size of data which has been copied from ring buffer
 *
 * @param buf pointer to buffer from which data shall be read
 * @param data pointer to linear data to which data shall be copied from ring buffer
 * @param size size of data to be copied from ring buffer to linear buffer
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of accessing buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Peak(
   Buff_Ring_XT  *buf,
   void          *data,
   Buff_Size_DT   size,
   Buff_Size_DT   buf_offset,
   Buff_Bool_DT   use_protection);
#endif

#ifndef BUFF_RING_PEAK_VENDOR_ENABLED
#define BUFF_RING_PEAK_VENDOR_ENABLED                    BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_PEAK_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
/**
 * @brief Function which reads data from ring buffer to vendor-specific buffer.
 * This function doesn't remove data from ring buffer after reading.
 * Difference between this function and Buff_Ring_Peak is that this function copies data using vendor-specific function
 * instead of "memcpy". For that reason this function can be also used to read data to vendor-specific buffer
 * instead to linear buffer.
 *
 * @result size of data which has been copied from ring buffer
 *
 * @param buf pointer to buffer from which data shall be read
 * @param data pointer to vendor-specific buffer which will be provided to vendor_cpy function as one of parameters
 * @param size size of data to be copied from ring buffer to vendor-specific buffer
 * @param vendor_cpy pointer to vendor-specific function which will be used as "memcpy" function by Buff_Ring_Peak_Vendor
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of accessing buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Peak_Vendor(
   Buff_Ring_XT           *buf,
   void                   *data,
   Buff_Size_DT            size,
   Buff_Size_DT            buf_offset,
   Buff_Vendor_Memcpy_HT   vendor_cpy,
   Buff_Bool_DT            use_protection);
#endif

#ifndef BUFF_RING_PEAK_TO_VECTOR_ENABLED
#define BUFF_RING_PEAK_TO_VECTOR_ENABLED                 BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_PEAK_TO_VECTOR_ENABLED && BUFF_USE_VECTOR_BUFFERS )
/**
 * @brief Function which reads data from ring buffer to vector of linear buffers (Buff_Writeable_Vector_XT).
 * This function doesn't remove data from ring buffer after reading.
 *
 * @result size of data which has been copied from ring buffer
 *
 * @param buf pointer to buffer from which data shall be read
 * @param data pointer to vector of linear buffers (Buff_Writeable_Vector_XT) to which data will be read from ring buffer
 * @param data_num_elems number of data vector elements (independent linear buffers which create vector data)
 * @param size size of data to be copied from ring buffer to vector of linear buffers
 * @param offset_data offset in vector of linear buffers to which data will be copied from ring buffer
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of accessing buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Peak_To_Vector(
   Buff_Ring_XT                    *buf,
   const Buff_Writeable_Vector_XT  *data,
   Buff_Num_Elems_DT                data_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     buf_offset,
   Buff_Size_DT                     offset_data,
   Buff_Bool_DT                     use_protection);
#endif

#ifndef BUFF_RING_PEAK_TO_TREE_ENABLED
#define BUFF_RING_PEAK_TO_TREE_ENABLED                   BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_PEAK_TO_TREE_ENABLED && BUFF_USE_TREE_BUFFERS )
/**
 * @brief Function which reads data from ring buffer to tree of linear buffers (Buff_Writeable_Tree_XT).
 * This function doesn't remove data from ring buffer after reading.
 *
 * @result size of data which has been copied from ring buffer
 *
 * @param buf pointer to buffer from which data shall be read
 * @param data pointer to tree of linear buffers (Buff_Writeable_Tree_XT) to which data will be read from ring buffer
 * @param data_num_elems number of elements in data tree root
 * @param size size of data to be copied from ring buffer to tree of linear buffers
 * @param offset_data offset in vector of linear buffers to which data will be copied from ring buffer
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of accessing buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Peak_To_Tree(
   Buff_Ring_XT                 *buf,
   const Buff_Writeable_Tree_XT *data,
   Buff_Num_Elems_DT             data_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  buf_offset,
   Buff_Size_DT                  offset_data,
   Buff_Bool_DT                  use_protection);
#endif

#ifndef BUFF_RING_READ_ENABLED
#define BUFF_RING_READ_ENABLED                           BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_READ_ENABLED)
/**
 * @brief Function which reads data from ring buffer to linear buffer.
 * This function removes data from ring buffer after reading.
 *
 * @result size of data which has been copied from ring buffer
 *
 * @param buf pointer to buffer from which data shall be read
 * @param data pointer to linear data to which data shall be copied from ring buffer
 * @param size size of data to be copied from ring buffer to linear buffer
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Read(
   Buff_Ring_XT  *buf,
   void          *data,
   Buff_Size_DT   size,
   Buff_Bool_DT   use_protection);
#endif

#ifndef BUFF_RING_READ_VENDOR_ENABLED
#define BUFF_RING_READ_VENDOR_ENABLED                    BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_READ_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
/**
 * @brief Function which reads data from ring buffer to vendor-specific buffer.
 * This function removes data from ring buffer after reading.
 * Difference between this function and Buff_Ring_Peak is that this function copies data using vendor-specific function
 * instead of "memcpy". For that reason this function can be also used to read data to vendor-specific buffer
 * instead to linear buffer.
 *
 * @result size of data which has been copied from ring buffer
 *
 * @param buf pointer to buffer from which data shall be read
 * @param data pointer to vendor-specific buffer which will be provided to vendor_cpy function as one of parameters
 * @param size size of data to be copied from ring buffer to vendor-specific buffer
 * @param vendor_cpy pointer to vendor-specific function which will be used as "memcpy" function by Buff_Ring_Peak_Vendor
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Read_Vendor(
   Buff_Ring_XT           *buf,
   void                   *data,
   Buff_Size_DT            size,
   Buff_Vendor_Memcpy_HT   vendor_cpy,
   Buff_Bool_DT            use_protection);
#endif

#ifndef BUFF_RING_READ_TO_VECTOR_ENABLED
#define BUFF_RING_READ_TO_VECTOR_ENABLED                 BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_READ_TO_VECTOR_ENABLED && BUFF_USE_VECTOR_BUFFERS )
/**
 * @brief Function which reads data from ring buffer to vector of linear buffers (Buff_Writeable_Vector_XT).
 * This function removes data from ring buffer after reading.
 *
 * @result size of data which has been copied from ring buffer
 *
 * @param buf pointer to buffer from which data shall be read
 * @param data pointer to vector of linear buffers (Buff_Writeable_Vector_XT) to which data will be read from ring buffer
 * @param data_num_elems number of data vector elements (independent linear buffers which create vector data)
 * @param size size of data to be copied from ring buffer to vector of linear buffers
 * @param offset_data offset in vector of linear buffers to which data will be copied from ring buffer
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Read_To_Vector(
   Buff_Ring_XT                    *buf,
   const Buff_Writeable_Vector_XT  *data,
   Buff_Num_Elems_DT                data_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_data,
   Buff_Bool_DT                     use_protection);
#endif

#ifndef BUFF_RING_READ_TO_TREE_ENABLED
#define BUFF_RING_READ_TO_TREE_ENABLED                   BUFF_DEFAULT_FEATURES_STATE
#endif
#if( BUFF_RING_READ_TO_TREE_ENABLED && BUFF_USE_TREE_BUFFERS )
/**
 * @brief Function which reads data from ring buffer to tree of linear buffers (Buff_Writeable_Tree_XT).
 * This function removes data from ring buffer after reading.
 *
 * @result size of data which has been copied from ring buffer
 *
 * @param buf pointer to buffer from which data shall be read
 * @param data pointer to tree of linear buffers (Buff_Writeable_Tree_XT) to which data will be read from ring buffer
 * @param data_num_elems number of elements in data tree root
 * @param size size of data to be copied from ring buffer to tree of linear buffers
 * @param offset_data offset in vector of linear buffers to which data will be copied from ring buffer
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Read_To_Tree(
   Buff_Ring_XT                 *buf,
   const Buff_Writeable_Tree_XT *data,
   Buff_Num_Elems_DT             data_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_data,
   Buff_Bool_DT                  use_protection);
#endif

#ifndef BUFF_RING_TO_RING_COPY_ENABLED
#define BUFF_RING_TO_RING_COPY_ENABLED                   BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_TO_RING_COPY_ENABLED)
/**
 * @brief Function which copies data from one ring buffer to another ring buffer/
 * This function doesn't remove data from source buffer after copying.
 *
 * @result size of data which has been copied
 *
 * @param dest pointer to ring buffer to which data shall be stored
 * @param src pointer to ring buffer from which data shall be copied
 * @param size size of data to be copied from src ring buffer to dest ring buffer
 * @param src_offset offset from begining of busy space in source buffer from which data shall be copied.
 * If src_offset is biffer than buffer size then data will be copied from the begining of bust space (oldest data)
 * @param overwrite_if_no_free_space if BUFF_TRUE and if there is not enough free space in dest buffer then oldest data will be overwritten
 * @param use_protection if BUFF_TRUE then function will lock access to both buffers for moment of modifying them, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_To_Ring_Copy(
   Buff_Ring_XT                 *dest,
   Buff_Ring_XT                 *src,
   Buff_Size_DT                  size,
   Buff_Size_DT                  src_offset,
   Buff_Bool_DT                  overwrite_if_no_free_space,
   Buff_Bool_DT                  use_protection);
#endif

#ifndef BUFF_RING_REMOVE_ENABLED
#define BUFF_RING_REMOVE_ENABLED                         BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_REMOVE_ENABLED)
/**
 * @brief Function which remove specific number of bytes from ring buffer.
 *
 * @result number of bytes which has been removed from ring buffer.
 *
 * @param buf pointer to buffer from which data shall be removed
 * @param size number of bytes to be removed from ring buffer. If this value is bigger than busy size of the buffer then
 * ring buffer will be cleared (there will be remove all the data stored in it).
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
Buff_Size_DT Buff_Ring_Remove(Buff_Ring_XT *buf, Buff_Size_DT size, Buff_Bool_DT use_protection);
#endif

#ifndef BUFF_RING_CLEAR_ENABLED
#define BUFF_RING_CLEAR_ENABLED                          BUFF_DEFAULT_FEATURES_STATE
#endif
#if(BUFF_RING_CLEAR_ENABLED)
/**
 * @brief Function which removed all the data stored in ring buffer.
 *
 * @param buf pointer to buffer which shall be cleared
 * @param use_protection if BUFF_TRUE then function will lock access to the buffer for moment of modifying buffer, BUFF_FALSE otherwise
 */
void Buff_Ring_Clear(Buff_Ring_XT *buf, Buff_Bool_DT use_protection);
#endif
#endif /* #if(BUFF_USE_RING_BUFFERS) */

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY INTERFACES ---------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

#endif

