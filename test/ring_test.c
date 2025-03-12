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
#include "ring_test.h"
#include "test_utils.h"

typedef struct
{
   size_t on_read_cntr;
   size_t on_write_cntr;
   size_t on_overwrite_cntr;
   size_t on_remove_cntr;
}on_extension_t;

static size_t vendor_cpy_cntr;
static size_t vendor_cpy_cntr_limit;
static on_extension_t on_extensions;
static on_extension_t on_protected_extensions;

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

static void main_ring_on_read(Buff_Ring_XT *buf, Buff_Ring_Extensions_XT *extension, Buff_Size_DT size, Buff_Bool_DT rewind_occured)
{
   on_extension_t *on_extension = (on_extension_t*)(extension->on_read_params);

   on_extension->on_read_cntr++;
}

static void main_ring_on_write(Buff_Ring_XT *buf, Buff_Ring_Extensions_XT *extension, Buff_Size_DT size, Buff_Bool_DT rewind_occured)
{
   on_extension_t *on_extension = (on_extension_t*)(extension->on_write_params);

   on_extension->on_write_cntr++;
}

static void main_ring_on_overwrite(Buff_Ring_XT *buf, Buff_Ring_Extensions_XT *extension, Buff_Size_DT pos, Buff_Size_DT size, Buff_Bool_DT rewind_occured)
{
   on_extension_t *on_extension = (on_extension_t*)(extension->on_overwrite_params);

   on_extension->on_overwrite_cntr++;
}

static void main_ring_on_remove(Buff_Ring_XT *buf, Buff_Ring_Extensions_XT *extension, Buff_Bool_DT rewind_occured)
{
   on_extension_t *on_extension = (on_extension_t*)(extension->on_remove_params);

   on_extension->on_remove_cntr++;
}

static void init_tests(
   Buff_Ring_XT *buf,
   uint8_t *memory,
   size_t memory_size,
   const uint8_t *rand_data,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   memcpy(buf, rand_data, sizeof(Buff_Ring_XT));

   if(BUFF_BOOL_IS_FALSE(Buff_Ring_Init(buf, memory, 0))
      && BUFF_COMPARE_PTRS(uint8_t, NULL, uint8_t, buf->memory)
      && (NULL             == buf->extension)
      && (NULL             == buf->protected_extension)
      && (0                == buf->size)
      && (0                == buf->original_size)
      && (0                == buf->busy_size)
      && (0                == buf->max_busy_size)
      && (0                == buf->first_free)
      && (0                == buf->first_busy)
      && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
      && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
      && (0                == Buff_Ring_Get_Size(buf, true))
      && (0                == Buff_Ring_Get_Busy_Size(buf, true))
      && (0                == Buff_Ring_Get_Max_Busy_Size(buf, true))
      && (0                == Buff_Ring_Get_Free_Size(buf, true))
      && (0                == Buff_Ring_Get_Continous_Free_Size(buf, true))
      && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Init-0");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Init-0");
   }

   memcpy(buf, rand_data, sizeof(Buff_Ring_XT));

   if(BUFF_BOOL_IS_FALSE(Buff_Ring_Init(buf, NULL, memory_size))
      && BUFF_COMPARE_PTRS(uint8_t, NULL, uint8_t, buf->memory)
      && (NULL             == buf->extension)
      && (NULL             == buf->protected_extension)
      && (0                == buf->size)
      && (0                == buf->original_size)
      && (0                == buf->busy_size)
      && (0                == buf->max_busy_size)
      && (0                == buf->first_free)
      && (0                == buf->first_busy)
      && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
      && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
      && (0                == Buff_Ring_Get_Size(buf, true))
      && (0                == Buff_Ring_Get_Busy_Size(buf, true))
      && (0                == Buff_Ring_Get_Max_Busy_Size(buf, true))
      && (0                == Buff_Ring_Get_Free_Size(buf, true))
      && (0                == Buff_Ring_Get_Continous_Free_Size(buf, true))
      && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Init-0");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Init-0");
   }

   memcpy(buf, rand_data, sizeof(Buff_Ring_XT));

   if(BUFF_BOOL_IS_TRUE(Buff_Ring_Init(buf, memory, memory_size))
      && BUFF_COMPARE_PTRS(uint8_t, memory, uint8_t, buf->memory)
      && (NULL             == buf->extension)
      && (NULL             == buf->protected_extension)
      && (memory_size      == buf->size)
      && (memory_size      == buf->original_size)
      && (0                == buf->busy_size)
      && (0                == buf->max_busy_size)
      && (0                == buf->first_free)
      && (0                == buf->first_busy)
      && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
      && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
      && (memory_size      == Buff_Ring_Get_Size(buf, true))
      && (0                == Buff_Ring_Get_Busy_Size(buf, true))
      && (0                == Buff_Ring_Get_Max_Busy_Size(buf, true))
      && (memory_size      == Buff_Ring_Get_Free_Size(buf, true))
      && (memory_size      == Buff_Ring_Get_Continous_Free_Size(buf, true))
      && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Init");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Init");
   }

   memcpy(buf, rand_data, sizeof(Buff_Ring_XT));

   Buff_Ring_Deinit(buf);
   if((NULL    == buf->memory)
      && (NULL == buf->extension)
      && (NULL == buf->protected_extension)
      && (0    == buf->size)
      && (0    == buf->original_size)
      && (0    == buf->busy_size)
      && (0    == buf->max_busy_size)
      && (0    == buf->first_free)
      && (0    == buf->first_busy)
      && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
      && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
      && (0    == Buff_Ring_Get_Size(buf, true))
      && (0    == Buff_Ring_Get_Busy_Size(buf, true))
      && (0    == Buff_Ring_Get_Max_Busy_Size(buf, true))
      && (0    == Buff_Ring_Get_Free_Size(buf, true))
      && (0    == Buff_Ring_Get_Continous_Free_Size(buf, true))
      && (0    == Buff_Ring_Get_Current_Pos(buf, true)))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Deinit");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Deinit");
   }

   memcpy(extension, rand_data, sizeof(Buff_Ring_Extensions_XT));

   Buff_Ring_Init_Extension(extension);
   if((NULL    == extension->on_read_params)
      && (NULL == extension->on_write_params)
      && (NULL == extension->on_overwrite_params)
      && (NULL == extension->on_remove_params)
      && (NULL == extension->on_read)
      && (NULL == extension->on_write)
      && (NULL == extension->on_overwrite)
      && (NULL == extension->on_remove))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Init_Extension");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Init_Extension");
   }

   (void)Buff_Ring_Init(buf, memory, memory_size);

   if(BUFF_BOOL_IS_TRUE(Buff_Ring_Add_Extension(buf, extension))
      && BUFF_COMPARE_PTRS(Buff_Ring_Extensions_XT, extension, Buff_Ring_Extensions_XT, buf->extension))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Add_Extension");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Add_Extension");
   }

   Buff_Ring_Init_Extension(protected_extension);

   if(BUFF_BOOL_IS_TRUE(Buff_Ring_Add_Protected_Extension(buf, protected_extension))
      && BUFF_COMPARE_PTRS(Buff_Ring_Extensions_XT, protected_extension, Buff_Ring_Extensions_XT, buf->protected_extension))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Add_Protected_Extension");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Add_Protected_Extension");
   }
} /* init_tests */

static void write_clear_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int i;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (i                      == Buff_Ring_Get_Current_Pos(buf, true))
            && (0                      == on_extensions.on_read_cntr)
            && (1                      == on_extensions.on_write_cntr)
            && (0                      == on_extensions.on_overwrite_cntr)
            && (0                      == on_extensions.on_remove_cntr)
            && (0                      == on_protected_extensions.on_read_cntr)
            && (1                      == on_protected_extensions.on_write_cntr)
            && (0                      == on_protected_extensions.on_overwrite_cntr)
            && (0                      == on_protected_extensions.on_remove_cntr))
         {
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
            failure_cntr++;
         }
      }
      else if(buf_memory_size == i)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Current_Pos(buf, true))
            && (0                      == on_extensions.on_read_cntr)
            && (1                      == on_extensions.on_write_cntr)
            && (0                      == on_extensions.on_overwrite_cntr)
            && (0                      == on_extensions.on_remove_cntr)
            && (0                      == on_protected_extensions.on_read_cntr)
            && (1                      == on_protected_extensions.on_write_cntr)
            && (0                      == on_protected_extensions.on_overwrite_cntr)
            && (0                      == on_protected_extensions.on_remove_cntr))
         {
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Current_Pos(buf, true))
            && (0                      == on_extensions.on_read_cntr)
            && (1                      == on_extensions.on_write_cntr)
            && (0                      == on_extensions.on_overwrite_cntr)
            && (1                      == on_extensions.on_remove_cntr)
            && (0                      == on_protected_extensions.on_read_cntr)
            && (1                      == on_protected_extensions.on_write_cntr)
            && (0                      == on_protected_extensions.on_overwrite_cntr)
            && (1                      == on_protected_extensions.on_remove_cntr))
         {
         }
         else
         {
            printf("line: %5d; %-36s failed at %d\n\r", __LINE__, "Buff_Ring_Write", i);
            failure_cntr++;
         }
      }

      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;

      Buff_Ring_Clear(buf, true);

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && (0                == buf->first_free)
         && (0                == buf->first_busy)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Continous_Free_Size(buf, true))
         && (0                == Buff_Ring_Get_Current_Pos(buf, true))
         && (0                      == on_extensions.on_read_cntr)
         && (0                      == on_extensions.on_write_cntr)
         && (0                      == on_extensions.on_overwrite_cntr)
         && (1                      == on_extensions.on_remove_cntr)
         && (0                      == on_protected_extensions.on_read_cntr)
         && (0                      == on_protected_extensions.on_write_cntr)
         && (0                      == on_protected_extensions.on_overwrite_cntr)
         && (1                      == on_protected_extensions.on_remove_cntr))
      {
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Clear");
         failure_cntr++;
      }
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write & Clear");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write & Clear");
   }
} /* write_clear_tests */

static void write_peak_read_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   uint8_t *read2,
   size_t read2_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int i;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (i                      == Buff_Ring_Get_Current_Pos(buf, true)))
         {
            if(i == Buff_Ring_Peak(buf, read1, i, 0, true))
            {
               if(i == Buff_Ring_Read(buf, read2, i, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Current_Pos(buf, true)))
         {
            if(buf_memory_size == Buff_Ring_Peak(buf, read1, i, 0, true))
            {
               if(buf_memory_size == Buff_Ring_Read(buf, read2, i, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && (0                == buf->first_free)
         && (0                == buf->first_busy)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Continous_Free_Size(buf, true))
         && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }
   }


   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;


   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true)))
         {
            if(i == Buff_Ring_Peak(buf, read1, i, 0, true))
            {
               if(i == Buff_Ring_Read(buf, read2, i, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true)))
         {
            if(buf_memory_size == Buff_Ring_Peak(buf, read1, i, 0, true))
            {
               if(buf_memory_size == Buff_Ring_Read(buf, read2, i, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }


      buf->first_busy = (i < buf->size) ? i : 0;
      buf->first_free = (i < buf->size) ? i : 0;
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write/Peak/Read");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write/Peak/Read");
   }
} /* write_peak_read_tests */

static void write_peak_vendor_read_vendor_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   uint8_t *read2,
   size_t read2_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int i;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (i                      == Buff_Ring_Get_Current_Pos(buf, true)))
         {
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = i;
            if(i == Buff_Ring_Peak_Vendor(buf, read1, i, 0, main_vendor_memcpy, true))
            {
               vendor_cpy_cntr = 0;
               vendor_cpy_cntr_limit = i;
               if(i == Buff_Ring_Read_Vendor(buf, read2, i, main_vendor_memcpy, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Current_Pos(buf, true)))
         {
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = i;
            if(buf_memory_size == Buff_Ring_Peak_Vendor(buf, read1, i, 0, main_vendor_memcpy, true))
            {
               vendor_cpy_cntr = 0;
               vendor_cpy_cntr_limit = i;
               if(buf_memory_size == Buff_Ring_Read_Vendor(buf, read2, i, main_vendor_memcpy, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && (0                == buf->first_free)
         && (0                == buf->first_busy)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Continous_Free_Size(buf, true))
         && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }
   }


   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;


   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true)))
         {
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = i;
            if(i == Buff_Ring_Peak_Vendor(buf, read1, i, 0, main_vendor_memcpy, true))
            {
               vendor_cpy_cntr = 0;
               vendor_cpy_cntr_limit = i;
               if(i == Buff_Ring_Read_Vendor(buf, read2, i, main_vendor_memcpy, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true)))
         {
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = i;
            if(buf_memory_size == Buff_Ring_Peak_Vendor(buf, read1, i, 0, main_vendor_memcpy, true))
            {
               vendor_cpy_cntr = 0;
               vendor_cpy_cntr_limit = i;
               if(buf_memory_size == Buff_Ring_Read_Vendor(buf, read2, i, main_vendor_memcpy, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }


      buf->first_busy = (i < buf->size) ? i : 0;
      buf->first_free = (i < buf->size) ? i : 0;
   }


   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;


   for(i = 8; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true)))
         {
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = i - 3;
            if((i - 3) == Buff_Ring_Peak_Vendor(buf, read1, i, 0, main_vendor_memcpy, true))
            {
               vendor_cpy_cntr = 0;
               vendor_cpy_cntr_limit = i - 3;
               if((i - 3) == Buff_Ring_Read_Vendor(buf, read2, i, main_vendor_memcpy, true))
               {
                  if(0 != memcmp(src, read1, i - 3))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i - 3))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }

                  Buff_Ring_Remove(buf, 3, true);
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true)))
         {
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = i;
            if(buf_memory_size == Buff_Ring_Peak_Vendor(buf, read1, i, 0, main_vendor_memcpy, true))
            {
               vendor_cpy_cntr = 0;
               vendor_cpy_cntr_limit = i;
               if(buf_memory_size == Buff_Ring_Read_Vendor(buf, read2, i, main_vendor_memcpy, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }


      buf->first_busy = (i < buf->size) ? i : 0;
      buf->first_free = (i < buf->size) ? i : 0;
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write/Peak_Vendor/Read_Vendor");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write/Peak_Vendor/Read_Vendor");
   }
} /* write_peak_vendor_read_vendor_tests */

static void write_peak_vector_read_vector_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   uint8_t *read2,
   size_t read2_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int i;
   Buff_Writeable_Vector_XT peak_v[1];
   Buff_Writeable_Vector_XT read_v[1];

   peak_v[0].data = read1;
   peak_v[0].size = read1_size;
   read_v[0].data = read2;
   read_v[0].size = read2_size;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (i                      == Buff_Ring_Get_Current_Pos(buf, true)))
         {
            if(i == Buff_Ring_Peak_To_Vector(buf, peak_v, Num_Elems(peak_v), i, 0, 0, true))
            {
               if(i == Buff_Ring_Read_To_Vector(buf, read_v, Num_Elems(read_v), i, 0, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Current_Pos(buf, true)))
         {
            if(buf_memory_size == Buff_Ring_Peak_To_Vector(buf, peak_v, Num_Elems(peak_v), i, 0, 0, true))
            {
               if(buf_memory_size == Buff_Ring_Read_To_Vector(buf, read_v, Num_Elems(read_v), i, 0, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && (0                == buf->first_free)
         && (0                == buf->first_busy)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Continous_Free_Size(buf, true))
         && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }
   }


   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;


   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true)))
         {
            if(i == Buff_Ring_Peak_To_Vector(buf, peak_v, Num_Elems(peak_v), i, 0, 0, true))
            {
               if(i == Buff_Ring_Read_To_Vector(buf, read_v, Num_Elems(read_v), i, 0, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true)))
         {
            if(buf_memory_size == Buff_Ring_Peak_To_Vector(buf, peak_v, Num_Elems(peak_v), i, 0, 0, true))
            {
               if(buf_memory_size == Buff_Ring_Read_To_Vector(buf, read_v, Num_Elems(read_v), i, 0, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }


      buf->first_busy = (i < buf->size) ? i : 0;
      buf->first_free = (i < buf->size) ? i : 0;
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write/Peak_To_Vector/Read_To_Vector");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write/Peak_To_Vector/Read_To_Vector");
   }
} /* write_peak_vector_read_vector_tests */

static void write_peak_tree_read_tree_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   uint8_t *read2,
   size_t read2_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int i;
   Buff_Writeable_Vector_XT peak_v[1];
   Buff_Writeable_Vector_XT read_v[1];
   Buff_Writeable_Tree_XT peak_t[1];
   Buff_Writeable_Tree_XT read_t[1];

   peak_v[0].data = read1;
   peak_v[0].size = read1_size;
   read_v[0].data = read2;
   read_v[0].size = read2_size;

   peak_t[0].is_branch = false;
   peak_t[0].elem_type.vector.vector = peak_v;
   peak_t[0].elem_type.vector.vector_num_elems = Num_Elems(peak_v);
   read_t[0].is_branch = false;
   read_t[0].elem_type.vector.vector = read_v;
   read_t[0].elem_type.vector.vector_num_elems = Num_Elems(read_v);

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (i                      == Buff_Ring_Get_Current_Pos(buf, true)))
         {
            if(i == Buff_Ring_Peak_To_Tree(buf, peak_t, Num_Elems(peak_t), i, 0, 0, true))
            {
               if(i == Buff_Ring_Read_To_Tree(buf, read_t, Num_Elems(read_t), i, 0, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Current_Pos(buf, true)))
         {
            if(buf_memory_size == Buff_Ring_Peak_To_Tree(buf, peak_t, Num_Elems(peak_t), i, 0, 0, true))
            {
               if(buf_memory_size == Buff_Ring_Read_To_Tree(buf, read_t, Num_Elems(read_t), i, 0, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && (0                == buf->first_free)
         && (0                == buf->first_busy)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Continous_Free_Size(buf, true))
         && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }
   }


   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;


   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      memset(read1, 0, read1_size);
      memset(read2, 0, read2_size);
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true)))
         {
            if(i == Buff_Ring_Peak_To_Tree(buf, peak_t, Num_Elems(peak_t), i, 0, 0, true))
            {
               if(i == Buff_Ring_Read_To_Tree(buf, read_t, Num_Elems(read_t), i, 0, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, i))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read");
                     }
                     else if((1                      == on_extensions.on_read_cntr)
                        && (1                      == on_extensions.on_write_cntr)
                        && (0                      == on_extensions.on_overwrite_cntr)
                        && (0                      == on_extensions.on_remove_cntr)
                        && (1                      == on_protected_extensions.on_read_cntr)
                        && (1                      == on_protected_extensions.on_write_cntr)
                        && (0                      == on_protected_extensions.on_overwrite_cntr)
                        && (0                      == on_protected_extensions.on_remove_cntr))
                     {
                     }
                     else
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                        failure_cntr++;
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write(buf, src, i, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true)))
         {
            if(buf_memory_size == Buff_Ring_Peak_To_Tree(buf, peak_t, Num_Elems(peak_t), i, 0, 0, true))
            {
               if(buf_memory_size == Buff_Ring_Read_To_Tree(buf, read_t, Num_Elems(read_t), i, 0, true))
               {
                  if(0 != memcmp(src, read1, buf_memory_size))
                  {
                     printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Peak2");
                  }
                  else
                  {
                     if(0 != memcmp(src, read2, buf_memory_size))
                     {
                        printf("line: %5d; %-44s failed\n\r", __LINE__, "memcmp Buff_Ring_Read2");
                     }
                     else
                     {
                     }
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read2");
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Peak2");
               failure_cntr++;
            }
         }
         else
         {
            failure_cntr++;
         }
      }

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true)))
      {
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read-remove");
         failure_cntr++;
      }


      buf->first_busy = (i < buf->size) ? i : 0;
      buf->first_free = (i < buf->size) ? i : 0;
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write/Peak_To_Tree/Read_To_Tree");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write/Peak_To_Tree/Read_To_Tree");
   }
} /* write_peak_tree_read_tree_tests */

static void getters_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   (void)Buff_Ring_Write(buf, src, 32, true, true);

   (void)Buff_Ring_Read(buf, read1, 16, true);

   if(((buf_memory_size - 32) == Buff_Ring_Get_Continous_Free_Size(buf, true))
      && ((buf_memory_size - 16) == Buff_Ring_Get_Free_Size(buf, true)))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Get_Continous_Free_Size");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Get_Continous_Free_Size");
   }

   Buff_Ring_Remove(buf, 8, true);

   if(((buf_memory_size - 32) == Buff_Ring_Get_Continous_Free_Size(buf, true))
      && ((buf_memory_size - 8) == Buff_Ring_Get_Free_Size(buf, true)))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Remove1");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Remove1");
   }

   Buff_Ring_Remove(buf, 8, true);

   if((buf_memory_size   == buf->size)
      && (buf_memory_size  == buf->original_size)
      && (0                == buf->busy_size)
      && (0                == buf->first_free)
      && (0                == buf->first_busy)
      && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
      && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
      && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
      && (0                == Buff_Ring_Get_Busy_Size(buf, true))
      && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true))
      && (buf_memory_size  == Buff_Ring_Get_Continous_Free_Size(buf, true))
      && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Remove2");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Remove2");
   }

} /* getters_tests */

static void write_vendor_clear_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int i;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(i = 1; i < src_size; i++)
   {
      vendor_cpy_cntr = 0;
      vendor_cpy_cntr_limit = buf_memory_size + 1;
      if(i < buf_memory_size)
      {
         if((i == Buff_Ring_Write_Vendor(buf, src, i, main_vendor_memcpy, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (i                      == Buff_Ring_Get_Busy_Size(buf, true))
            && (i                      == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Free_Size(buf, true))
            && ((buf_memory_size - i)  == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (i                      == Buff_Ring_Get_Current_Pos(buf, true))
            && (i == vendor_cpy_cntr))
         {
         }
         else
         {
            failure_cntr++;
         }
      }
      else
      {
         if((buf_memory_size == Buff_Ring_Write_Vendor(buf, src, i, main_vendor_memcpy, true, true))
            && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Empty(buf, true))
            && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Full(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Busy_Size(buf, true))
            && (buf_memory_size        == Buff_Ring_Get_Max_Busy_Size(buf, true))
            && (0                      == Buff_Ring_Get_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Continous_Free_Size(buf, true))
            && (0                      == Buff_Ring_Get_Current_Pos(buf, true))
            && (buf_memory_size == vendor_cpy_cntr))
         {
         }
         else
         {
            failure_cntr++;
         }
      }

      Buff_Ring_Clear(buf, true);

      if((buf_memory_size   == buf->size)
         && (buf_memory_size  == buf->original_size)
         && (0                == buf->busy_size)
         && (0                == buf->first_free)
         && (0                == buf->first_busy)
         && BUFF_BOOL_IS_TRUE(Buff_Ring_Is_Empty(buf, true))
         && BUFF_BOOL_IS_FALSE(Buff_Ring_Is_Full(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Size(buf, true))
         && (0                == Buff_Ring_Get_Busy_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Free_Size(buf, true))
         && (buf_memory_size  == Buff_Ring_Get_Continous_Free_Size(buf, true))
         && (0                == Buff_Ring_Get_Current_Pos(buf, true)))
      {
      }
      else
      {
         failure_cntr++;
      }
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write_Vendor & Clear");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write_Vendor & Clear");
   }
} /* write_vendor_clear_tests */

static void write_vendor_specific_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (buf_memory_size / 4) * 3;

   if(vendor_cpy_cntr_limit == Buff_Ring_Write_Vendor(buf, src, buf_memory_size - 1, main_vendor_memcpy, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   Buff_Ring_Remove(buf, buf_memory_size / 4, true);

   if(Buff_Ring_Get_Busy_Size(buf, true) != buf_memory_size / 2)
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = buf_memory_size / 4 - 1;

   if((vendor_cpy_cntr_limit == Buff_Ring_Write_Vendor(buf, src, buf_memory_size / 2, main_vendor_memcpy, true, true))
      && (vendor_cpy_cntr_limit == vendor_cpy_cntr))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write_Vendor specific");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write_Vendor specific");
   }
} /* write_vendor_specific_tests */

static void write_vector_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   Buff_Readable_Vector_XT vector[3];
   Buff_Size_DT size;
   Buff_Size_DT size1;
   Buff_Size_DT size2;
   Buff_Size_DT size3;
   Buff_Size_DT offset;
   Buff_Size_DT result;
   Buff_Size_DT used_size;

   if(buf_memory_size > 32)
   {
      buf_memory_size = 32;
   }

   if(src_size > 32)
   {
      src_size = 32;
   }

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(size = 0; size < src_size; size++)
   {
      for(size1 = 0; size1 < src_size; size1++)
      {
         for(size2 = 0; size2 < src_size; size2++)
         {
            for(size3 = 0; size3 < src_size; size3++)
            {
               for(offset = 0; offset < src_size; offset++)
               {
                  vector[0].data = &src[0];
                  vector[0].size = size1;
                  vector[1].data = &src[size1];
                  vector[1].size = size2;
                  vector[2].data = &src[size1 + size2];
                  vector[2].size = size3;

                  result = Buff_Ring_Write_From_Vector(buf, vector, Num_Elems(vector), size, offset, false, true);

                  /* vector smaller than data requested to be copied */
                  if((size1 + size2 + size3) < (size + offset))
                  {
                     if((size1 + size2 + size3) < offset)
                     {
                        used_size = 0;
                     }
                     else
                     {
                        used_size = size1 + size2 + size3 - offset;
                     }

                     /* size of data requested to be copied small enough to be put into buffer */
                     if(used_size < buf_memory_size)
                     {
                        if(used_size == result)
                        {
                           if(result == Buff_Ring_Peak(buf, read1, result, 0, true))
                           {
                              if(0 == memcmp(&src[offset], read1, result))
                              {
                              }
                              else
                              {
                                 failure_cntr++;
                              }
                           }
                           else
                           {
                              failure_cntr++;
                           }
                        }
                        else
                        {
                           failure_cntr++;
                        }
                     }
                     /* size requested to be stored in ring buffer is bigger than buffer size; in this case result should be equal to buffer size */
                     else
                     {
                        if(buf_memory_size == result)
                        {
                           if(result == Buff_Ring_Peak(buf, read1, result, 0, true))
                           {
                              if(0 == memcmp(&src[offset], read1, result))
                              {
                              }
                              else
                              {
                                 failure_cntr++;
                              }
                           }
                           else
                           {
                              failure_cntr++;
                           }
                        }
                        else
                        {
                           failure_cntr++;
                        }
                     }
                  }
                  else
                  {
                     used_size = size;

                     /* size of data requested to be copied small enough to be put into buffer */
                     if(used_size < buf_memory_size)
                     {
                        if(used_size == result)
                        {
                           if(result == Buff_Ring_Peak(buf, read1, result, 0, true))
                           {
                              if(0 == memcmp(&src[offset], read1, result))
                              {
                              }
                              else
                              {
                                 failure_cntr++;
                              }
                           }
                           else
                           {
                              failure_cntr++;
                           }
                        }
                        else
                        {
                           failure_cntr++;
                        }
                     }
                     /* size requested to be stored in ring buffer is bigger than buffer size; in this case result should be equal to buffer size */
                     else
                     {
                        if(buf_memory_size == result)
                        {
                           if(result == Buff_Ring_Peak(buf, read1, result, 0, true))
                           {
                              if(0 == memcmp(&src[offset], read1, result))
                              {
                              }
                              else
                              {
                                 failure_cntr++;
                              }
                           }
                           else
                           {
                              failure_cntr++;
                           }
                        }
                        else
                        {
                           failure_cntr++;
                        }
                     }
                  }

                  Buff_Ring_Clear(buf, true);
               }
            }
         }
      }
   }

   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write_Vector");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write_Vector");
   }
} /* write_vector_tests */

static void write_vector_specific_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   Buff_Readable_Vector_XT vector[3];

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   vector[0].data = &src[0];
   vector[0].size = src_size / 5;
   vector[1].data = &src[src_size / 5];
   vector[1].size = src_size / 2;
   vector[2].data = &src[(src_size / 5) + (src_size / 2)];
   vector[2].size = src_size - (vector[0].size + vector[1].size);

   failure_cntr = 0;

   if((buf_memory_size - 1) == Buff_Ring_Write_From_Vector(buf, vector, Num_Elems(vector), buf_memory_size - 1, 0, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   Buff_Ring_Remove(buf, buf_memory_size / 4, true);

   if(Buff_Ring_Get_Busy_Size(buf, true) != (buf_memory_size - 1 - (buf_memory_size / 4)))
   {
      failure_cntr++;
   }

   if(2 == Buff_Ring_Write_From_Vector(buf, vector, Num_Elems(vector), 2, 0, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((buf_memory_size / 2) == Buff_Ring_Write_From_Vector(buf, vector, Num_Elems(vector), buf_memory_size / 2, 0, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(buf_memory_size == Buff_Ring_Write_From_Vector(buf, vector, Num_Elems(vector), buf_memory_size, 0, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write_From_Vector specific");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write_From_Vector specific");
   }
} /* write_vector_specific_tests */

static void write_tree_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   Buff_Readable_Tree_XT tree[1];
   Buff_Readable_Vector_XT vector[3];
   Buff_Size_DT size;
   Buff_Size_DT size1;
   Buff_Size_DT size2;
   Buff_Size_DT size3;
   Buff_Size_DT offset;
   Buff_Size_DT result;
   Buff_Size_DT used_size;

   if(buf_memory_size > 32)
   {
      buf_memory_size = 32;
   }

   if(src_size > 32)
   {
      src_size = 32;
   }

   tree[0].elem_type.vector.vector = vector;
   tree[0].elem_type.vector.vector_num_elems = Num_Elems(vector);
   tree[0].is_branch = BUFF_FALSE;


   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(size = 0; size < src_size; size++)
   {
      for(size1 = 0; size1 < src_size; size1++)
      {
         for(size2 = 0; size2 < src_size; size2++)
         {
            for(size3 = 0; size3 < src_size; size3++)
            {
               for(offset = 0; offset < src_size; offset++)
               {
                  vector[0].data = &src[0];
                  vector[0].size = size1;
                  vector[1].data = &src[size1];
                  vector[1].size = size2;
                  vector[2].data = &src[size1 + size2];
                  vector[2].size = size3;

                  result = Buff_Ring_Write_From_Tree(buf, tree, Num_Elems(tree), size, offset, false, true);

                  /* vector smaller than data requested to be copied */
                  if((size1 + size2 + size3) < (size + offset))
                  {
                     if((size1 + size2 + size3) < offset)
                     {
                        used_size = 0;
                     }
                     else
                     {
                        used_size = size1 + size2 + size3 - offset;
                     }

                     /* size of data requested to be copied small enough to be put into buffer */
                     if(used_size < buf_memory_size)
                     {
                        if(used_size == result)
                        {
                           if(result == Buff_Ring_Peak(buf, read1, result, 0, true))
                           {
                              if(0 == memcmp(&src[offset], read1, result))
                              {
                              }
                              else
                              {
                                 failure_cntr++;
                              }
                           }
                           else
                           {
                              failure_cntr++;
                           }
                        }
                        else
                        {
                           failure_cntr++;
                        }
                     }
                     /* size requested to be stored in ring buffer is bigger than buffer size; in this case result should be equal to buffer size */
                     else
                     {
                        if(buf_memory_size == result)
                        {
                           if(result == Buff_Ring_Peak(buf, read1, result, 0, true))
                           {
                              if(0 == memcmp(&src[offset], read1, result))
                              {
                              }
                              else
                              {
                                 failure_cntr++;
                              }
                           }
                           else
                           {
                              failure_cntr++;
                           }
                        }
                        else
                        {
                           failure_cntr++;
                        }
                     }
                  }
                  else
                  {
                     used_size = size;

                     /* size of data requested to be copied small enough to be put into buffer */
                     if(used_size < buf_memory_size)
                     {
                        if(used_size == result)
                        {
                           if(result == Buff_Ring_Peak(buf, read1, result, 0, true))
                           {
                              if(0 == memcmp(&src[offset], read1, result))
                              {
                              }
                              else
                              {
                                 failure_cntr++;
                              }
                           }
                           else
                           {
                              failure_cntr++;
                           }
                        }
                        else
                        {
                           failure_cntr++;
                        }
                     }
                     /* size requested to be stored in ring buffer is bigger than buffer size; in this case result should be equal to buffer size */
                     else
                     {
                        if(buf_memory_size == result)
                        {
                           if(result == Buff_Ring_Peak(buf, read1, result, 0, true))
                           {
                              if(0 == memcmp(&src[offset], read1, result))
                              {
                              }
                              else
                              {
                                 failure_cntr++;
                              }
                           }
                           else
                           {
                              failure_cntr++;
                           }
                        }
                        else
                        {
                           failure_cntr++;
                        }
                     }
                  }

                  Buff_Ring_Clear(buf, true);
               }
            }
         }
      }
   }

   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write_From_Tree");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write_From_Tree");
   }
} /* write_tree_tests */

static void write_tree_specific_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   Buff_Readable_Tree_XT tree[1];
   Buff_Readable_Vector_XT vector[3];

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   tree[0].elem_type.vector.vector = vector;
   tree[0].elem_type.vector.vector_num_elems = Num_Elems(vector);
   tree[0].is_branch = BUFF_FALSE;

   vector[0].data = &src[0];
   vector[0].size = src_size / 5;
   vector[1].data = &src[src_size / 5];
   vector[1].size = src_size / 2;
   vector[2].data = &src[(src_size / 5) + (src_size / 2)];
   vector[2].size = src_size - (vector[0].size + vector[1].size);

   failure_cntr = 0;

   if((buf_memory_size - 1) == Buff_Ring_Write_From_Tree(buf, tree, Num_Elems(tree), buf_memory_size - 1, 0, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   Buff_Ring_Remove(buf, buf_memory_size / 4, true);

   if(Buff_Ring_Get_Busy_Size(buf, true) != (buf_memory_size - 1 - (buf_memory_size / 4)))
   {
      failure_cntr++;
   }

   if(2 == Buff_Ring_Write_From_Tree(buf, tree, Num_Elems(tree), 2, 0, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((buf_memory_size / 2) == Buff_Ring_Write_From_Tree(buf, tree, Num_Elems(tree), buf_memory_size / 2, 0, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(buf_memory_size == Buff_Ring_Write_From_Tree(buf, tree, Num_Elems(tree), buf_memory_size, 0, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write_From_Tree specific");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write_From_Tree specific");
   }
} /* write_tree_specific_tests */

static void overwrite_if_exist_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   if(src_size > buf_memory_size)
   {
      src_size = buf_memory_size;
   }
   else
   {
      buf_memory_size = src_size;
   }

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   /**
    * create following ocupacy of ring buffer:
    *       busy             free
    * |****************----------------|
    */
   if((src_size / 2) == Buff_Ring_Write(buf, src, src_size / 2, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((src_size / 4) == Buff_Ring_OverWrite_If_Exist(buf, src, src_size / 4, src_size / 8, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(0 == Buff_Ring_OverWrite_If_Exist(buf, src, src_size / 2, src_size / 8, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((src_size / 4) == Buff_Ring_Peak(buf, read1, src_size / 4, src_size / 8, true))
   {
      if(0 == memcmp(src, read1, src_size / 4))
      {
         /* OK */
      }
      else
      {
         failure_cntr++;
      }
   }
   else
   {
      failure_cntr++;
   }

   /**
    * create following ocupacy of ring buffer:
    *    busy        free        busy
    * |********----------------********|
    */

   if((src_size / 4) == Buff_Ring_Write(buf, src, src_size / 4, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   Buff_Ring_Remove(buf, src_size / 2, true);

   if((src_size / 2) == Buff_Ring_Write(buf, src, src_size / 2, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   Buff_Ring_Remove(buf, src_size / 4, true);

   if((src_size / 8) == Buff_Ring_OverWrite_If_Exist(buf, src, src_size / 8, ((src_size / 4) * 3) + 1, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(src_size / 4 == Buff_Ring_OverWrite_If_Exist(buf, src, src_size / 4, ((src_size / 4) * 3) + 1, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(0 == Buff_Ring_OverWrite_If_Exist(buf, src, src_size / 2, ((src_size / 4) * 3) + 1, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((src_size / 8) == Buff_Ring_Peak(buf, read1, src_size / 8, 1, true))
   {
      if(0 == memcmp(src, read1, src_size / 8))
      {
         /* OK */
      }
      else
      {
         failure_cntr++;
      }
   }
   else
   {
      failure_cntr++;
   }


   if((src_size / 8) == Buff_Ring_OverWrite_If_Exist(buf, src, src_size / 8, 1, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if(0 == Buff_Ring_OverWrite_If_Exist(buf, src, src_size / 4, 1, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((src_size / 8) == Buff_Ring_Peak(buf, read1, src_size / 8, 1, true))
   {
      if(0 == memcmp(src, read1, src_size / 8))
      {
         /* OK */
      }
      else
      {
         failure_cntr++;
      }
   }
   else
   {
      failure_cntr++;
   }


   if(0 == Buff_Ring_OverWrite_If_Exist(buf, src, src_size / 4, (src_size / 4), true))
   {
   }
   else
   {
      failure_cntr++;
   }


   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_OverWrite_If_Exist");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_OverWrite_If_Exist");
   }

} /* overwrite_if_exist_tests */

static void overwrite_if_exist_vendor_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   if(src_size > buf_memory_size)
   {
      src_size = buf_memory_size;
   }
   else
   {
      buf_memory_size = src_size;
   }

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   /**
    * create following ocupacy of ring buffer:
    *       busy             free
    * |****************----------------|
    */
   if((src_size / 2) == Buff_Ring_Write(buf, src, src_size / 2, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = src_size;

   if((src_size / 4) == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, src_size / 8, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = src_size;

   if(0 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 2, src_size / 8, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((src_size / 4) == Buff_Ring_Peak(buf, read1, src_size / 4, src_size / 8, true))
   {
      if(0 == memcmp(src, read1, src_size / 4))
      {
         /* OK */
      }
      else
      {
         failure_cntr++;
      }
   }
   else
   {
      failure_cntr++;
   }

   /**
    * create following ocupacy of ring buffer:
    *    busy        free        busy
    * |********----------------********|
    */

   if((src_size / 4) == Buff_Ring_Write(buf, src, src_size / 4, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   Buff_Ring_Remove(buf, src_size / 2, true);

   if((src_size / 2) == Buff_Ring_Write(buf, src, src_size / 2, true, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   Buff_Ring_Remove(buf, src_size / 4, true);

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (src_size / 8) - 2;

   if(((src_size / 8) - 2) == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 8, ((src_size / 4) * 3) + 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = src_size;

   if((src_size / 8) == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 8, ((src_size / 4) * 3) + 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (src_size / 4) - 2;

   if(((src_size / 4) - 2) == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, ((src_size / 4) * 3) + 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (src_size / 4) - 1;

   if(((src_size / 4) - 1) == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, ((src_size / 4) * 3) + 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = src_size;

   if(src_size / 4 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, ((src_size / 4) * 3) + 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (src_size / 4) - 1;

   if(0 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 2, ((src_size / 4) * 3) + 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = src_size;

   if(0 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 2, ((src_size / 4) * 3) + 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((src_size / 4) == Buff_Ring_Peak(buf, read1, src_size / 4, 1, true))
   {
      if(0 == memcmp(src, read1, src_size / 4))
      {
         /* OK */
      }
      else
      {
         failure_cntr++;
      }
   }
   else
   {
      failure_cntr++;
   }


   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (src_size / 8) - 3;

   if(((src_size / 8) - 3) == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 8, 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = src_size;

   if((src_size / 8) == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 8, 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (src_size / 8) - 4;

   if(0 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (src_size / 4) - 1;

   if(0 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = src_size;

   if(0 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, 1, main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   if((src_size / 8) == Buff_Ring_Peak(buf, read1, src_size / 8, 1, true))
   {
      if(0 == memcmp(src, read1, src_size / 8))
      {
         /* OK */
      }
      else
      {
         failure_cntr++;
      }
   }
   else
   {
      failure_cntr++;
   }


   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = (src_size / 4) - 6;

   if(0 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, (src_size / 4), main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }

   vendor_cpy_cntr = 0;
   vendor_cpy_cntr_limit = src_size;

   if(0 == Buff_Ring_OverWrite_If_Exist_Vendor(buf, src, src_size / 4, (src_size / 4), main_vendor_memcpy, true))
   {
   }
   else
   {
      failure_cntr++;
   }


   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_OverWrite_If_Exist_Vendor");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_OverWrite_If_Exist_Vendor");
   }

} /* overwrite_if_exist_vendor_tests */

static void add_read_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int prev_size;
   int size;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   prev_size = 1;

   if(prev_size == Buff_Ring_Write(buf, &src[prev_size], prev_size, true, true))
   {
      /* ok */
   }
   else
   {
      failure_cntr++;
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write 1B");
   }

   for(size = 2; size < (buf_memory_size / 2); size++)
   {
      memset(read1, 0, read1_size);
      if(size == Buff_Ring_Write(buf, &src[size], size, true, true))
      {
         if(prev_size == Buff_Ring_Read(buf, read1, prev_size, true))
         {
            if(0 != memcmp(&src[prev_size], read1, prev_size))
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read-memcmp");
               failure_cntr++;
            }
            else
            {
               /* ok */
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
            failure_cntr++;
         }
      }
      else
      {
         failure_cntr++;
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write 1B");
      }

      prev_size = size;
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write/Read");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write/Read");
   }
} /* add_read_tests */

static void add_vendor_read_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int prev_size;
   int size;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   prev_size = 1;

   if(prev_size == Buff_Ring_Write(buf, &src[prev_size], prev_size, true, true))
   {
      /* ok */
   }
   else
   {
      failure_cntr++;
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write 1B");
   }

   for(size = 2; size < (buf_memory_size / 2); size++)
   {
      vendor_cpy_cntr = 0;
      vendor_cpy_cntr_limit = buf_memory_size + 1;
      memset(read1, 0, read1_size);
      if(size == Buff_Ring_Write_Vendor(buf, &src[size], size, main_vendor_memcpy, true, true))
      {
         if(prev_size == Buff_Ring_Read_Vendor(buf, read1, prev_size, main_vendor_memcpy, true))
         {
            if(0 != memcmp(&src[prev_size], read1, prev_size))
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read_Vendor-memcmp");
               failure_cntr++;
            }
            else
            {
               /* ok */
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read_Vendor");
            failure_cntr++;
         }
      }
      else
      {
         failure_cntr++;
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write_Vendor");
      }

      prev_size = size;
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Write_Vendor/Read");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write_Vendor/Read");
   }
} /* add_vendor_read_tests */

static void check_out_in_test(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   uint8_t *ptr;
   int prev_size;
   int size;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   prev_size = 1;

   if(prev_size == Buff_Ring_Write(buf, &src[prev_size], prev_size, true, true))
   {
      /* ok */
   }
   else
   {
      failure_cntr++;
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write 1B");
   }

   /* add data in a loop with variable size, read in same step data added in previous step */
   for(size = 2; size < (buf_memory_size / 3); size++)
   {
      memset(read1, 0, read1_size);
      ptr = Buff_Ring_Data_Check_Out(buf, size, true, true);
      if(NULL != ptr)
      {
         memcpy(ptr, &src[size], size);

         Buff_Ring_Data_Check_In(buf, size, true);

         if(prev_size == Buff_Ring_Read(buf, read1, prev_size, true))
         {
            if(0 != memcmp(&src[prev_size], read1, prev_size))
            {
               printf("line: %5d; %-36s failed with size: %d\n\r", __LINE__, "Buff_Ring_Data_Check_In", prev_size);
               failure_cntr++;
            }
            else
            {
               /* ok */
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_In");
            failure_cntr++;
         }
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_In");
      }

      prev_size = size;
   }

   /**
    * check out / check in data twice, first with size = sizeof(buf) / 2, next size = sizeof(buf) with option overwrite,
    * at end check if only data from second check_in is present in the buffer
    */
   memset(read1, 0, read1_size);
   ptr = Buff_Ring_Data_Check_Out(buf, buf_memory_size / 2, true, true);
   if(NULL != ptr)
   {
      memcpy(ptr, &src[buf_memory_size / 2], buf_memory_size / 2);

      Buff_Ring_Data_Check_In(buf, buf_memory_size / 2, true);

      ptr = Buff_Ring_Data_Check_Out(buf, buf_memory_size, true, true);
      if(NULL != ptr)
      {
         memcpy(ptr, src, buf_memory_size);

         Buff_Ring_Data_Check_In(buf, buf_memory_size, true);

         if(buf_memory_size == Buff_Ring_Read(buf, read1, buf_memory_size, true))
         {
            if(0 != memcmp(src, read1, buf_memory_size))
            {
               printf("line: %5d; %-36s failed with size: %d\n\r", __LINE__, "Buff_Ring_Data_Check_In", buf_memory_size);
               failure_cntr++;
            }
            else
            {
               /* ok */
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_In");
            failure_cntr++;
         }
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_In");
      }
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_In");
   }

   /**
    * again the same:
    * check out / check in data twice, first with size = sizeof(buf) / 2, next size = sizeof(buf) with option overwrite,
    * at end check if only data from second check_in is present in the buffer
    */
   memset(read1, 0, read1_size);
   ptr = Buff_Ring_Data_Check_Out(buf, buf_memory_size / 2, true, true);
   if(NULL != ptr)
   {
      memcpy(ptr, src, buf_memory_size / 2);

      Buff_Ring_Data_Check_In(buf, buf_memory_size / 2, true);

      ptr = Buff_Ring_Data_Check_Out(buf, buf_memory_size, true, true);
      if(NULL != ptr)
      {
         memcpy(ptr, src, buf_memory_size);

         Buff_Ring_Data_Check_In(buf, buf_memory_size, true);

         if(buf_memory_size == Buff_Ring_Read(buf, read1, buf_memory_size, true))
         {
            if(0 != memcmp(src, read1, buf_memory_size))
            {
               printf("line: %5d; %-36s failed with size: %d\n\r", __LINE__, "Buff_Ring_Data_Check_In", buf_memory_size);
               failure_cntr++;
            }
            else
            {
               /* ok */
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_In");
            failure_cntr++;
         }
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_Out");
      }
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_Out");
   }

   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_Data_Check_In");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_In");
   }

   Buff_Ring_Clear(buf, true);

   ptr = Buff_Ring_Data_Check_Out(buf, buf_memory_size / 6, true, true);

   if(NULL != ptr)
   {
      Buff_Ring_Data_Check_In(buf, buf_memory_size / 6, true);

      Buff_Ring_Remove(buf, Buff_Ring_Get_Busy_Size(buf, true) - 1, true);

      if(1 != Buff_Ring_Get_Busy_Size(buf, true))
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Remove");
      }
      else
      {
         ptr = Buff_Ring_Data_Check_Out(buf, buf_memory_size / 2, true, true);

         if(NULL != ptr)
         {
            Buff_Ring_Data_Check_In(buf, buf_memory_size / 2, true);

            if(((buf_memory_size / 2) + 1) != Buff_Ring_Get_Busy_Size(buf, true))
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_In");
            }
            else
            {
               ptr = Buff_Ring_Data_Check_Out(buf, (buf_memory_size * 5) / 6, false, true);

               if(NULL != ptr)
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_Out???");
               }
               else
               {
                  /* ok */
               }
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_Out");
         }
      }
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Data_Check_Out");
   }
} /* check_out_in_test */

static void add_write_overwrite_if_no_free_space_tests(
   Buff_Ring_XT *buf,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory,
   size_t buf_memory_size,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension,
   Buff_Ring_Extensions_XT *protected_extension)
{
   int prev_size;
   int size;

   (void)Buff_Ring_Init(buf, memory, buf_memory_size);
   Buff_Ring_Init_Extension(extension);
   (void)Buff_Ring_Add_Extension(buf, extension);
   Buff_Ring_Init_Extension(protected_extension);
   (void)Buff_Ring_Add_Protected_Extension(buf, protected_extension);

   extension->on_read             = main_ring_on_read;
   extension->on_write            = main_ring_on_write;
   extension->on_overwrite        = main_ring_on_overwrite;
   extension->on_remove           = main_ring_on_remove;
   extension->on_read_params      = &on_extensions;
   extension->on_write_params     = &on_extensions;
   extension->on_overwrite_params = &on_extensions;
   extension->on_remove_params    = &on_extensions;

   protected_extension->on_read             = main_ring_on_read;
   protected_extension->on_write            = main_ring_on_write;
   protected_extension->on_overwrite        = main_ring_on_overwrite;
   protected_extension->on_remove           = main_ring_on_remove;
   protected_extension->on_read_params      = &on_protected_extensions;
   protected_extension->on_write_params     = &on_protected_extensions;
   protected_extension->on_overwrite_params = &on_protected_extensions;
   protected_extension->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   prev_size = buf_memory_size - 1;

   if(prev_size == Buff_Ring_Write(buf, &src[0], prev_size, true, true))
   {
      prev_size = 16;
      if(prev_size == Buff_Ring_Write(buf, &src[0], prev_size, true, true))
      {
         /* ok */
         prev_size = buf_memory_size - 16;
         if(prev_size == Buff_Ring_Read(buf, read1, prev_size, true))
         {
            if(0 != memcmp(&src[15], read1, prev_size))
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read-memcmp");
               failure_cntr++;
            }
            else
            {
               /* ok */
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
            failure_cntr++;
         }
      }
      else
      {
         failure_cntr++;
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
      }
   }
   else
   {
      failure_cntr++;
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
   }

   Buff_Ring_Clear(buf, true);

   prev_size = buf_memory_size - 1;

   vendor_cpy_cntr_limit = buf_memory_size;
   vendor_cpy_cntr = 0;

   if(prev_size == Buff_Ring_Write_Vendor(buf, &src[0], prev_size, main_vendor_memcpy, true, true))
   {
      vendor_cpy_cntr_limit = buf_memory_size;
      vendor_cpy_cntr = 0;
      prev_size = 16;
      if(prev_size == Buff_Ring_Write_Vendor(buf, &src[0], prev_size, main_vendor_memcpy, true, true))
      {
         /* ok */
         prev_size = buf_memory_size - 16;
         if(prev_size == Buff_Ring_Read_Vendor(buf, read1, prev_size, main_vendor_memcpy, true))
         {
            if(0 != memcmp(&src[15], read1, prev_size))
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read-memcmp");
               failure_cntr++;
            }
            else
            {
               /* ok */
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Read");
            failure_cntr++;
         }
      }
      else
      {
         failure_cntr++;
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
      }
   }
   else
   {
      failure_cntr++;
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
   }

   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "overwrite_if_no_free_space");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "overwrite_if_no_free_space");
   }
} /* add_read_tests */

static void ring_to_ring_copy_tests(
   Buff_Ring_XT *buf1,
   Buff_Ring_XT *buf2,
   const uint8_t *src,
   size_t src_size,
   uint8_t *memory1,
   size_t buf_memory_size1,
   uint8_t *memory2,
   size_t buf_memory_size2,
   uint8_t *read1,
   size_t read1_size,
   Buff_Ring_Extensions_XT *extension1,
   Buff_Ring_Extensions_XT *protected_extension1,
   Buff_Ring_Extensions_XT *extension2,
   Buff_Ring_Extensions_XT *protected_extension2)
{
   size_t i;
   size_t offset;
   size_t on_remove_cntr;

   (void)Buff_Ring_Init(buf1, memory1, buf_memory_size1);

   (void)Buff_Ring_Init(buf2, memory2, buf_memory_size2);
   Buff_Ring_Init_Extension(extension2);
   (void)Buff_Ring_Add_Extension(buf2, extension2);
   Buff_Ring_Init_Extension(protected_extension2);
   (void)Buff_Ring_Add_Protected_Extension(buf2, protected_extension2);

   extension2->on_read              = main_ring_on_read;
   extension2->on_write             = main_ring_on_write;
   extension2->on_overwrite         = main_ring_on_overwrite;
   extension2->on_remove            = main_ring_on_remove;
   extension2->on_read_params       = &on_extensions;
   extension2->on_write_params      = &on_extensions;
   extension2->on_overwrite_params  = &on_extensions;
   extension2->on_remove_params     = &on_extensions;

   protected_extension2->on_read             = main_ring_on_read;
   protected_extension2->on_write            = main_ring_on_write;
   protected_extension2->on_overwrite        = main_ring_on_overwrite;
   protected_extension2->on_remove           = main_ring_on_remove;
   protected_extension2->on_read_params      = &on_protected_extensions;
   protected_extension2->on_write_params     = &on_protected_extensions;
   protected_extension2->on_overwrite_params = &on_protected_extensions;
   protected_extension2->on_remove_params    = &on_protected_extensions;

   failure_cntr = 0;

   for(offset = 0; offset < buf_memory_size1; offset++)
   {
      Buff_Ring_Clear(buf1, true);
      Buff_Ring_Clear(buf2, true);

      if(1 == Buff_Ring_Write(buf1, src, 1, true, true))
      {
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
         failure_cntr++;
      }

      if(offset == Buff_Ring_Write(buf2, src, offset, true, true))
      {
      }
      else
      {
         printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
         failure_cntr++;
      }

      for(i = 1; i < src_size; i++)
      {
         on_extensions.on_read_cntr       = 0;
         on_extensions.on_write_cntr      = 0;
         on_extensions.on_overwrite_cntr  = 0;
         on_extensions.on_remove_cntr     = 0;
         on_protected_extensions.on_read_cntr      = 0;
         on_protected_extensions.on_write_cntr     = 0;
         on_protected_extensions.on_overwrite_cntr = 0;
         on_protected_extensions.on_remove_cntr    = 0;
         if(i < (buf_memory_size1-offset))
         {
            if(i == Buff_Ring_Write(buf1, src, i, true, true))
            {
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
               failure_cntr++;
            }

            if(i == Buff_Ring_To_Ring_Copy(buf2, buf1, i, 1, true, true))
            {
               if((0                      == on_extensions.on_read_cntr)
                  && (1                      == on_extensions.on_write_cntr)
                  && (0                      == on_extensions.on_overwrite_cntr)
                  && (0                      == on_extensions.on_remove_cntr)
                  && (0                      == on_protected_extensions.on_read_cntr)
                  && (1                      == on_protected_extensions.on_write_cntr)
                  && (0                      == on_protected_extensions.on_overwrite_cntr)
                  && (0                      == on_protected_extensions.on_remove_cntr))
               {
               }
               else
               {
                  printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
                  failure_cntr++;
               }

               if(i == Buff_Ring_Peak(buf2, read1, i, offset, true))
               {
                  if(0 != memcmp(src, read1, i))
                  {
                     printf("line: %5d; %-44s failed; i: %d\n\r", __LINE__, "memcmp Buff_Ring_Peak", i);
                  }
                  else
                  {
                  }
               }
               else
               {
                  printf("line: %5d; %-44s failed; buf_busy_size: %d\n\r", __LINE__, "Buff_Ring_Peak", Buff_Ring_Get_Busy_Size(buf2, true));
                  failure_cntr++;
               }
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_To_Ring_Copy");
               failure_cntr++;
            }

            if(i == Buff_Ring_Remove(buf2, i, true))
            {
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Remove");
               failure_cntr++;
            }

            if(i == Buff_Ring_Remove(buf1, i, true))
            {
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Remove");
               failure_cntr++;
            }
         }
      }
   }




   Buff_Ring_Clear(buf1, true);
   Buff_Ring_Clear(buf2, true);

   for(i = 1; i < src_size; i++)
   {
      on_extensions.on_read_cntr       = 0;
      on_extensions.on_write_cntr      = 0;
      on_extensions.on_overwrite_cntr  = 0;
      on_extensions.on_remove_cntr     = 0;
      on_protected_extensions.on_read_cntr      = 0;
      on_protected_extensions.on_write_cntr     = 0;
      on_protected_extensions.on_overwrite_cntr = 0;
      on_protected_extensions.on_remove_cntr    = 0;
      if(i <= (buf_memory_size1))
      {
         if(i == Buff_Ring_Write(buf1, src, i, true, true))
         {
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Write");
            failure_cntr++;
         }

         offset = Buff_Ring_Get_Busy_Size(buf2, true);
         on_remove_cntr = 0;

         if(Buff_Ring_Get_Free_Size(buf2, true) < i)
         {
            offset -= i - Buff_Ring_Get_Free_Size(buf2, true);
            on_remove_cntr = 1;
         }

         if(i == Buff_Ring_To_Ring_Copy(buf2, buf1, i, 0, true, true))
         {
            if((0                      == on_extensions.on_read_cntr)
               && (1                      == on_extensions.on_write_cntr)
               && (0                      == on_extensions.on_overwrite_cntr)
               && (on_remove_cntr         == on_extensions.on_remove_cntr)
               && (0                      == on_protected_extensions.on_read_cntr)
               && (1                      == on_protected_extensions.on_write_cntr)
               && (0                      == on_protected_extensions.on_overwrite_cntr)
               && (on_remove_cntr         == on_protected_extensions.on_remove_cntr))
            {
            }
            else
            {
               printf("line: %5d; %-44s failed\n\r", __LINE__, "cntrs check");
               failure_cntr++;
            }

            if(i == Buff_Ring_Peak(buf2, read1, i, offset, true))
            {
               if(0 != memcmp(src, read1, i))
               {
                  printf("line: %5d; %-44s failed; i: %d\n\r", __LINE__, "memcmp Buff_Ring_Peak", i);
               }
               else
               {
               }
            }
            else
            {
               printf("line: %5d; %-44s failed; buf_busy_size: %d\n\r", __LINE__, "Buff_Ring_Peak", Buff_Ring_Get_Busy_Size(buf2, true));
               failure_cntr++;
            }
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_To_Ring_Copy");
            failure_cntr++;
         }

         if(i == Buff_Ring_Remove(buf1, i, true))
         {
         }
         else
         {
            printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_Remove");
            failure_cntr++;
         }
      }
   }

   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Ring_To_Ring_Copy");
   }
   else
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Ring_To_Ring_Copy");
   }
} /* ring_to_ring_copy_tests */


void ring_test(void)
{
   Buff_Ring_XT buf;
   Buff_Ring_XT buf2;
   Buff_Ring_Extensions_XT extension;
   Buff_Ring_Extensions_XT protected_extension;
   Buff_Ring_Extensions_XT extension2;
   Buff_Ring_Extensions_XT protected_extension2;
   time_t tt;
   int i;
   uint8_t memory[1000];
   uint8_t memory2[1000];
   uint8_t src[2060];
   uint8_t read1[1000];
   uint8_t read2[1000];

   srand(time(&tt));

   for(i = 0; i < Num_Elems(src); i++)
   {
      src[i] = rand();
   }

   printf("\n\rbuff-ring tests:\n\r");

   init_tests(&buf, memory, sizeof(memory), src, &extension, &protected_extension);

   write_clear_tests(&buf, src, sizeof(src), memory, sizeof(memory), &extension, &protected_extension);

   write_peak_read_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), read2, sizeof(read2), &extension, &protected_extension);

   write_peak_vendor_read_vendor_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), read2, sizeof(read2), &extension, &protected_extension);

   write_peak_vector_read_vector_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), read2, sizeof(read2), &extension, &protected_extension);

   write_peak_tree_read_tree_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), read2, sizeof(read2), &extension, &protected_extension);

   getters_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   write_vendor_clear_tests(&buf, src, sizeof(src), memory, sizeof(memory), &extension, &protected_extension);

   write_vendor_specific_tests(&buf, src, sizeof(src), memory, sizeof(memory), &extension, &protected_extension);

   write_vector_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   write_vector_specific_tests(&buf, src, sizeof(src), memory, sizeof(memory), &extension, &protected_extension);

   write_tree_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   write_tree_specific_tests(&buf, src, sizeof(src), memory, sizeof(memory), &extension, &protected_extension);

   overwrite_if_exist_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   overwrite_if_exist_vendor_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   add_read_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   add_vendor_read_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   check_out_in_test(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   add_write_overwrite_if_no_free_space_tests(&buf, src, sizeof(src), memory, sizeof(memory), read1, sizeof(read1), &extension, &protected_extension);

   ring_to_ring_copy_tests(&buf, &buf2, src, sizeof(src), memory, sizeof(memory), memory2, sizeof(memory2), read1, sizeof(read1), &extension, &protected_extension, &extension2, &protected_extension2);
} /* ring_test */

