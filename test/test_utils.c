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
#include "test_utils.h"

size_t failure_cntr = 0;

static void debug_format_u8(char *buf, const uint8_t *data, size_t data_size)
{
   int cntr = 0;
   size_t result = 0;

   for(result = 0; result < 8; result++)
   {
      if(data_size > result)
      {
         buf[cntr++] = "0123456789ABCDEF"[(data[result] >> 4) & 0x0F];
         buf[cntr++] = "0123456789ABCDEF"[(data[result] >> 0) & 0x0F];
         buf[cntr++] = ',';
      }
      else
      {
         break;
      }
   }
   buf[cntr++] = ' ';
   if(cntr < 25)
   {
      memset(&buf[cntr], ' ', 25 - cntr);
   }
   buf[25] = '\0';
} /* debug_format_u8 */

static void debug_format_dif8(
   char *buf, const uint8_t *data1, uint_fast16_t data1_size, const uint8_t *data2, uint_fast16_t data2_size)
{
   int cntr = 0;
   uint_fast16_t result;
   uint_fast16_t data_size = (data1_size > data2_size) ? data2_size : data1_size;

   for(result = 0; result < 8; result++)
   {
      if(data_size > result)
      {
         if(data1[result] != data2[result])
         {
            buf[cntr++] = "0123456789ABCDEF"[(data2[result] >> 4) & 0x0F];
            buf[cntr++] = "0123456789ABCDEF"[(data2[result] >> 0) & 0x0F];
            buf[cntr++] = ',';
         }
         else
         {
            buf[cntr++] = '=';
            buf[cntr++] = '=';
            buf[cntr++] = ',';
         }
      }
      else
      {
         break;
      }
   }
   buf[cntr++] = ' ';
   if(cntr < 25)
   {
      memset(&buf[cntr], ' ', 25 - cntr);
   }
   buf[25] = '\0';
} /* debug_format_dif8 */

static void print_data(char *temp, const uint8_t *data, uint_fast16_t data_size, uint_fast16_t num_bytes_to_print)
{
   uint_fast16_t cntr;
   for(cntr = 0; cntr < num_bytes_to_print; cntr += 8)
   {
      if(data_size > cntr)
      {
         debug_format_u8(temp, &data[cntr], data_size - cntr);
         printf("%s", temp);
      }
      else
      {
         printf("%*s", 25, " ");
      }
   }
} /* print_data */

static const uint8_t *print_ascii(
   char *temp, const uint8_t *data, uint_fast16_t data_size, uint_fast16_t num_bytes_to_print)
{
   char *ptr = temp;

   while(0 != data_size)
   {
      if((*data >= ' ') && (*data <= '~'))
      {
         *ptr = *data;
      }
      else
      {
         *ptr = '.';
      }
      ptr++;
      data++;
      data_size--;
   }

   while(ptr < &temp[num_bytes_to_print])
   {
      *ptr = ' ';
      ptr++;
   }

   *ptr = '\0';
   printf("%s", temp);

   return data;
}/* print_ascii */

void log_data(const uint8_t *data, uint_fast16_t data_size)
{
   uint_fast16_t cntr;
   uint8_t asciicntr;
   uint8_t pos_marker_len;
   char temp[33];

   if(data_size < 100)
   {
      pos_marker_len = 2;
   }
   else if(data_size < 1000)
   {
      pos_marker_len = 3;
   }
   else if(data_size < 10000)
   {
      pos_marker_len = 4;
   }
   else
   {
      pos_marker_len = 5;
   }

   for(cntr = 0; cntr < data_size;)
   {
      if((data_size - cntr) >= 32)
      {
         asciicntr = 32;
      }
      else
      {
         asciicntr = data_size - cntr;
      }

      printf("  %0*d-%0*d/%0*d: ",
         pos_marker_len, cntr, pos_marker_len, cntr + asciicntr - 1, pos_marker_len, data_size);

      print_data(temp, data, data_size - cntr, 32);

      printf("-> ");

      data = print_ascii(temp, data, asciicntr, 32);

      cntr += asciicntr;

      printf("\n\r");
   }
} /* log_data */

void compare_data(const uint8_t *data1, const uint8_t *data2, size_t size1, size_t size2)
{
   char *ptr;
   uint_fast16_t cntr;
   uint_fast16_t cntr2;
   uint_fast16_t data_size = (size1 > size2) ? size1 : size2;
   uint8_t asciicntr;
   uint8_t asciicntr1;
   uint8_t asciicntr2;
   uint8_t pos_marker_len;
   char temp[33];

   if(data_size < 100)
   {
      pos_marker_len = 2;
   }
   else if(data_size < 1000)
   {
      pos_marker_len = 3;
   }
   else if(data_size < 10000)
   {
      pos_marker_len = 4;
   }
   else
   {
      pos_marker_len = 5;
   }

   for(cntr = 0; cntr < data_size;)
   {
      if((data_size - cntr) >= 16)
      {
         asciicntr = 16;
      }
      else
      {
         asciicntr = data_size - cntr;
      }

      if(size1 > cntr)
      {
         if((size1 - cntr) >= 16)
         {
            asciicntr1 = 16;
         }
         else
         {
            asciicntr1 = size1 - cntr;
         }
      }
      else
      {
         asciicntr1 = 0;
      }

      if(size2 > cntr)
      {
         if((size2 - cntr) >= 16)
         {
            asciicntr2 = 16;
         }
         else
         {
            asciicntr2 = size2 - cntr;
         }
      }
      else
      {
         asciicntr2 = 0;
      }

      printf("%0*d-%0*d/%0*d: ",
         pos_marker_len, cntr, pos_marker_len, cntr + asciicntr - 1, pos_marker_len, data_size);

      print_data(temp, data1, asciicntr1, 16);

#if(DATA_COMPARE_PRINT_BOTH_CONTEXTS)
      printf("<-> ");

      print_data(temp, data2, asciicntr2, 16);
#endif

      printf("dif in 2: ");

      for(cntr2 = 0; cntr2 < 16; cntr2 += 8)
      {
         if((asciicntr1 > cntr2) && (asciicntr2 > cntr2))
         {
            debug_format_dif8(temp, &data1[cntr2], size1 - (cntr2 + cntr), &data2[cntr2], size2 - (cntr2 + cntr));
            printf("%s", temp);
         }
         else
         {
            printf("%*s", 25, " ");
         }
      }

      printf("-> ");
      cntr += asciicntr;

      asciicntr = (asciicntr1 > asciicntr2) ? asciicntr2 : asciicntr1;

      (void)print_ascii(temp, data1, asciicntr1, 16);

#if(DATA_COMPARE_PRINT_BOTH_CONTEXTS)
      printf(" <-> ");

      (void)print_ascii(temp, data2, asciicntr2, 16);
#endif

      printf(" dif in 2: ");

      ptr = temp;

      temp[16] = '\0';
      cntr2 = 0;
      asciicntr1 = asciicntr;
      while(0 != asciicntr)
      {
         if(data1[cntr2] != data2[cntr2])
         {
            if((data2[cntr2] >= ' ') && (data2[cntr2] <= '~'))
            {
               if('=' != data2[cntr2])
               {
                  *ptr = data2[cntr2];
               }
               else
               {
                  *ptr = '.';
               }
            }
            else
            {
               *ptr = '.';
            }
         }
         else
         {
            *ptr = '=';
         }
         ptr++;
         cntr2++;
         asciicntr--;
      }
      data1 += 16;
      data2 += 16;
      *ptr = '\0';
      printf("%s\n\r", temp);
   }
}

void is_zero(const uint8_t *buf, size_t size)
{
   size_t cntr;

   for(cntr = 0; cntr < size; cntr++)
   {
      if(0 != buf[cntr])
      {
         printf("invalid data in zero-field!\n\r");
         failure_cntr++;
         printf("press any key to continue...\n\r");
         getc(stdin);
         break;
      }
   }
}

void compare_and_print_status(const uint8_t *dest, const uint8_t *src, size_t desc_size, size_t src_size, const char *desc)
{
   if(0 != memcmp(dest, src, (desc_size > src_size) ? src_size : desc_size))
   {
      printf("%-36s failed!\n\r", desc);
      printf("compare\n\r");
      compare_data(dest, src, desc_size, src_size);
      failure_cntr++;
      printf("press any key to continue...\n\r");
      getc(stdin);
   }
}


