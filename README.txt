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

There can be 2 types of vector buffers - readable vector (vector read only) and writeable vector (readable and writeable vector). Single element of each type is defined as below:

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
 * Tree buffer which is composed of: pointer to "before" tree, first vector of 2 elements, second vector of 2 elements and "after" tree:
 * before[0].v[0]   = {0x03};
 * before[0].v[1]   = {0xD3, 0xDD};
 *
 * t[0].t    = &before
 * t[1].v[0] = {0x12}
 * t[1].v[1] = {0x36, 0x21}
 * t[2].v[0] = {0x72, 0x4C, 0x31}
 * t[2].v[1] = {0x22}
 * t[3].t    = &after;
 *
 * after_before[0].v[0]   = {0xB2};
 * after_before[0].v[1]   = {0xC2, 0xC3, 0xC4, 0xC5};
 *
 * after[0].t    = &after_before;
 * after[1].v[0] = {0x17, 0x40};
 * after[2].t    = &after_after;
 *
 * after_after[0].v[0]   = {0x01};
 * after_after[0].v[1]   = {0x02, 0x03};
 * after_after[0].v[2]   = {0x04};
 * after_after[1].t      = &after_after_after;
 *
 * after_after_after[0].v[0]   = {0xF1};

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
