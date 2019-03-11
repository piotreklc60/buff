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

#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "buff.h"
#include "vector_test.h"
#include "test_utils.h"

static size_t vendor_cpy_cntr;
static size_t vendor_cpy_cntr_limit;

static Buff_Size_DT main_vendor_memcpy(const Buff_Memcpy_Params_XT *params)
{
   uint8_t       *dest = &((uint8_t*)(params->dest))[params->dest_offset];
   const uint8_t *src  = &((const uint8_t*)(params->src))[params->src_offset];
   Buff_Size_DT   cntr;

   for(cntr = 0; (cntr < params->size) && (vendor_cpy_cntr < vendor_cpy_cntr_limit); cntr++)
   {
      dest[cntr] = src[cntr];
      vendor_cpy_cntr++;
   }

   return cntr;
} /* main_vendor_memcpy */

void vector_test(void)
{
   uint8_t *ptr;
   int pos;
   uint8_t src[200];
   uint8_t dest[200];
   int i;
   time_t tt;
   Buff_Readable_Vector_XT  s_v[5];
   Buff_Writeable_Vector_XT d_v[5];
   Buff_Writeable_Vector_XT d_v_short[5];
   Buff_Size_DT dest_offset;
   Buff_Size_DT src_offset;
   Buff_Size_DT size;
   Buff_Size_DT result;

   srand(time(&tt));

   for(i = 0; i < Num_Elems(src); i++)
   {
      src[i] = rand();
   }



   pos = 0;
   ptr = src;

   s_v[0].data = &ptr[pos];
   s_v[0].size = 53;
   pos += 53;

   s_v[1].data = &ptr[pos];
   s_v[1].size = 15;
   pos += 15;

   s_v[2].data = NULL;
   s_v[2].size = 71;
   pos += 0;

   s_v[3].data = &ptr[pos];
   s_v[3].size = 0;
   pos += 0;

   s_v[4].data = &ptr[pos];
   s_v[4].size = sizeof(src) - pos;
   pos = sizeof(src);



   pos = 0;
   ptr = dest;

   d_v[0].data = &ptr[pos];
   d_v[0].size = 43;
   pos += 43;

   d_v[1].data = &ptr[pos];
   d_v[1].size = 30;
   pos += 30;

   d_v[2].data = NULL;
   d_v[2].size = 71;
   pos += 0;

   d_v[3].data = &ptr[pos];
   d_v[3].size = 0;
   pos += 0;

   d_v[4].data = &ptr[pos];
   d_v[4].size = sizeof(src) - pos;
   pos = sizeof(src);



   pos = 0;
   ptr = dest;

   d_v_short[0].data = &ptr[pos];
   d_v_short[0].size = 43;
   pos += 43;

   d_v_short[1].data = &ptr[pos];
   d_v_short[1].size = 30;
   pos += 30;

   d_v_short[2].data = NULL;
   d_v_short[2].size = 71;
   pos += 0;

   d_v_short[3].data = &ptr[pos];
   d_v_short[3].size = 0;
   pos += 0;

   d_v_short[4].data = &ptr[pos];
   d_v_short[4].size = SHORTER_DESTINATION - pos;
   pos = SHORTER_DESTINATION;



   printf("\n\rvector size read test:\n\r");
   if(sizeof(src) != Buff_Get_Readable_Vector_Data_Size(s_v, Num_Elems(s_v)))
   {
      printf("line: %5d; %-36s failed\n\r", __LINE__, "Buff_Get_Readable_Vector_Data_Size");
   }
   else
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Get_Readable_Vector_Data_Size");
   }
   if(sizeof(src) != Buff_Get_Writeable_Vector_Data_Size(d_v, Num_Elems(d_v)))
   {
      printf("line: %5d; %-36s failed\n\r", __LINE__, "Buff_Get_Writeable_Vector_Data_Size");
   }
   else
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Get_Writeable_Vector_Data_Size");
   }



   printf("\n\rvector copy tests:\n\r");



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
            memset(dest, 0, sizeof(dest));

            result = Buff_Copy_From_Vector(dest, s_v, Num_Elems(s_v), size, dest_offset, src_offset);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_From_Vector");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_From_Vector");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_From_Vector", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
            memset(dest, 0, sizeof(dest));

            result = Buff_Copy_To_Vector(d_v, Num_Elems(d_v), src, size, dest_offset, src_offset);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_To_Vector");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_To_Vector");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_To_Vector", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
    //        printf("size: %d, dest_offset: %d, src_offset: %d\n\r", size, dest_offset, src_offset);
            memset(dest, 0, sizeof(dest));

            result = Buff_Copy_To_Vector(d_v_short, Num_Elems(d_v_short), src, size, dest_offset, src_offset);

            if((dest_offset + size) < SHORTER_DESTINATION)
            {
               if(size != result)
               {
                  printf("wrong size returned! %d, expected: %d\n\r", result, size);
                  printf("press any key to continue...\n\r");
                  getc(stdin);
               }
               is_zero(dest, dest_offset);
               is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
               compare_and_print_status(
                  &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_To_Vector-d_v_short");
            }
            else if((size < SHORTER_DESTINATION) && (dest_offset < SHORTER_DESTINATION))
            {
               if((SHORTER_DESTINATION - dest_offset) != result)
               {
                  printf("wrong size returned! %d, expected: %d\n\r", result, size);
                  printf("press any key to continue...\n\r");
                  getc(stdin);
               }
               is_zero(dest, dest_offset);
               is_zero(&dest[SHORTER_DESTINATION], sizeof(dest) - SHORTER_DESTINATION);
               compare_and_print_status(
                  &dest[dest_offset], &src[src_offset], (SHORTER_DESTINATION - dest_offset), sizeof(src) - (SHORTER_DESTINATION - dest_offset), "Buff_Copy_To_Vector-d_v_short");
            }
            else if(dest_offset < SHORTER_DESTINATION)
            {
               if((SHORTER_DESTINATION - dest_offset) != result)
               {
                  printf("wrong size returned! %d, expected: %d\n\r", result, size);
                  printf("press any key to continue...\n\r");
                  getc(stdin);
               }
               is_zero(dest, dest_offset);
               is_zero(&dest[SHORTER_DESTINATION], sizeof(dest) - SHORTER_DESTINATION);
               compare_and_print_status(
                  &dest[dest_offset], &src[src_offset], (SHORTER_DESTINATION - dest_offset), sizeof(src) - (SHORTER_DESTINATION - dest_offset), "Buff_Copy_To_Vector-d_v_short");
            }
            else
            {
               if(0 != result)
               {
                  printf("wrong size returned! %d, expected: %d\n\r", result, 0);
                  printf("press any key to continue...\n\r");
                  getc(stdin);
               }
               is_zero(dest, sizeof(dest));
            }
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_To_Vector-d_v_short");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_To_Vector-d_v_short", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
            memset(dest, 0, sizeof(dest));

            result = Buff_Copy_Vector_To_Vector(d_v, Num_Elems(d_v), s_v, Num_Elems(s_v), size, dest_offset, src_offset);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_Vector_To_Vector");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_Vector_To_Vector");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_Vector_To_Vector", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
    //        printf("size: %d, dest_offset: %d, src_offset: %d\n\r", size, dest_offset, src_offset);
            memset(dest, 0, sizeof(dest));

            result = Buff_Copy_Vector_To_Vector(d_v_short, Num_Elems(d_v_short), s_v, Num_Elems(s_v), size, dest_offset, src_offset);

            if((dest_offset + size) < SHORTER_DESTINATION)
            {
               if(size != result)
               {
                  printf("wrong size returned! %d, expected: %d\n\r", result, size);
                  printf("press any key to continue...\n\r");
                  getc(stdin);
               }
               is_zero(dest, dest_offset);
               is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
               compare_and_print_status(
                  &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_Vector_To_Vector-d_v_short");
            }
            else if((size < SHORTER_DESTINATION) && (dest_offset < SHORTER_DESTINATION))
            {
               if((SHORTER_DESTINATION - dest_offset) != result)
               {
                  printf("wrong size returned! %d, expected: %d\n\r", result, size);
                  printf("press any key to continue...\n\r");
                  getc(stdin);
               }
               is_zero(dest, dest_offset);
               is_zero(&dest[SHORTER_DESTINATION], sizeof(dest) - SHORTER_DESTINATION);
               compare_and_print_status(
                  &dest[dest_offset], &src[src_offset], (SHORTER_DESTINATION - dest_offset), sizeof(src) - (SHORTER_DESTINATION - dest_offset), "Buff_Copy_To_Vector-d_v_short");
            }
            else if(dest_offset < SHORTER_DESTINATION)
            {
               if((SHORTER_DESTINATION - dest_offset) != result)
               {
                  printf("wrong size returned! %d, expected: %d\n\r", result, size);
                  printf("press any key to continue...\n\r");
                  getc(stdin);
               }
               is_zero(dest, dest_offset);
               is_zero(&dest[SHORTER_DESTINATION], sizeof(dest) - SHORTER_DESTINATION);
               compare_and_print_status(
                  &dest[dest_offset], &src[src_offset], (SHORTER_DESTINATION - dest_offset), sizeof(src) - (SHORTER_DESTINATION - dest_offset), "Buff_Copy_To_Vector-d_v_short");
            }
            else
            {
               if(0 != result)
               {
                  printf("wrong size returned! %d, expected: %d\n\r", result, 0);
                  printf("press any key to continue...\n\r");
                  getc(stdin);
               }
               is_zero(dest, sizeof(dest));
            }
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_Vector_To_Vector-d_v_short");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_Vector_To_Vector-d_v_short", failure_cntr);
   }



   printf("\n\rvector vendor copy tests:\n\r");



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
            memset(dest, 0, sizeof(dest));
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = (-1);

            result = Buff_Copy_From_Vector_Vendor(dest, s_v, Num_Elems(s_v), size, dest_offset, src_offset, main_vendor_memcpy);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_From_Vector_Vendor");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_From_Vector_Vendor");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_From_Vector_Vendor", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 3; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
            memset(dest, 0, sizeof(dest));
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = size - 2;

            result = Buff_Copy_From_Vector_Vendor(dest, s_v, Num_Elems(s_v), size, dest_offset, src_offset, main_vendor_memcpy);

            if(vendor_cpy_cntr_limit != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, vendor_cpy_cntr_limit);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + vendor_cpy_cntr_limit], sizeof(dest) - (dest_offset + vendor_cpy_cntr_limit));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], vendor_cpy_cntr_limit, sizeof(src) - vendor_cpy_cntr_limit, "Buff_Copy_From_Vector_Vendor-limit");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_From_Vector_Vendor-limit");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_From_Vector_Vendor-limit", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
            memset(dest, 0, sizeof(dest));
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = (-1);

            result = Buff_Copy_To_Vector_Vendor(d_v, Num_Elems(d_v), src, size, dest_offset, src_offset, main_vendor_memcpy);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_To_Vector_Vendor");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_To_Vector_Vendor");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_To_Vector_Vendor", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 3; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
            memset(dest, 0, sizeof(dest));
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = size - 2;

            result = Buff_Copy_To_Vector_Vendor(d_v, Num_Elems(d_v), src, size, dest_offset, src_offset, main_vendor_memcpy);

            if(vendor_cpy_cntr_limit != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, vendor_cpy_cntr_limit);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + vendor_cpy_cntr_limit], sizeof(dest) - (dest_offset + vendor_cpy_cntr_limit));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], vendor_cpy_cntr_limit, sizeof(src) - vendor_cpy_cntr_limit, "Buff_Copy_To_Vector_Vendor-limit");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-36s succeeded\n\r", __LINE__, "Buff_Copy_To_Vector_Vendor-limit");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_To_Vector_Vendor-limit", failure_cntr);
   }
} /* ring_test */

