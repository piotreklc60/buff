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
#include "tree_test.h"
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

void tree_test(void)
{
   uint8_t *ptr;
   int pos;
   uint8_t src[200];
   uint8_t dest[200];
   int i;
   time_t tt;

   Buff_Readable_Tree_XT   s_t[6];
   Buff_Readable_Tree_XT   s_t1[2];
   Buff_Readable_Tree_XT   s_t2[3];
   Buff_Readable_Vector_XT s_v1[5];
   Buff_Readable_Vector_XT s_t1v1[3];
   Buff_Readable_Vector_XT s_t1v2[1];
   Buff_Readable_Tree_XT   s_t2t1[1];
   Buff_Readable_Vector_XT s_t2v1[2];
   Buff_Readable_Vector_XT s_t2t1v1[1];

   Buff_Writeable_Tree_XT   d_t[6];
   Buff_Writeable_Tree_XT   d_t1[2];
   Buff_Writeable_Tree_XT   d_t2[3];
   Buff_Writeable_Vector_XT d_v1[5];
   Buff_Writeable_Vector_XT d_t1v1[3];
   Buff_Writeable_Vector_XT d_t1v2[1];
   Buff_Writeable_Tree_XT   d_t2t1[1];
   Buff_Writeable_Vector_XT d_t2v1[2];
   Buff_Writeable_Vector_XT d_t2t1v1[1];

   Buff_Writeable_Tree_XT   d_t_short[6];
   Buff_Writeable_Tree_XT   d_t1_short[2];
   Buff_Writeable_Tree_XT   d_t2_short[3];
   Buff_Writeable_Vector_XT d_v1_short[5];
   Buff_Writeable_Vector_XT d_t1v1_short[3];
   Buff_Writeable_Vector_XT d_t1v2_short[1];
   Buff_Writeable_Tree_XT   d_t2t1_short[1];
   Buff_Writeable_Vector_XT d_t2v1_short[2];
   Buff_Writeable_Vector_XT d_t2t1v1_short[1];
   Buff_Size_DT dest_offset;
   Buff_Size_DT src_offset;
   Buff_Size_DT size;
   Buff_Size_DT result;

   srand(time(&tt));

   for(i = 0; i < Num_Elems(src); i++)
   {
      src[i] = rand();
   }



   s_t[0].elem_type.tree.branch                = NULL;
   s_t[0].elem_type.tree.branch_num_elems      = 0;
   s_t[0].is_branch = BUFF_TRUE;

   s_t[1].elem_type.tree.branch                = s_t1;
   s_t[1].elem_type.tree.branch_num_elems      = Num_Elems(s_t1);
   s_t[1].is_branch = BUFF_TRUE;

   s_t[2].elem_type.tree.branch                = NULL;
   s_t[2].elem_type.tree.branch_num_elems      = 0;
   s_t[2].is_branch = BUFF_TRUE;

   s_t[3].elem_type.vector.vector              = NULL;
   s_t[3].elem_type.vector.vector_num_elems    = 0;
   s_t[3].is_branch = BUFF_FALSE;

   s_t[4].elem_type.vector.vector              = s_v1;
   s_t[4].elem_type.vector.vector_num_elems    = Num_Elems(s_v1);
   s_t[4].is_branch = BUFF_FALSE;

   s_t[5].elem_type.tree.branch                = s_t2;
   s_t[5].elem_type.tree.branch_num_elems      = Num_Elems(s_t2);
   s_t[5].is_branch = BUFF_TRUE;


   s_t1[0].elem_type.vector.vector             = s_t1v1;
   s_t1[0].elem_type.vector.vector_num_elems   = Num_Elems(s_t1v1);
   s_t1[0].is_branch = BUFF_FALSE;

   s_t1[1].elem_type.vector.vector             = s_t1v2;
   s_t1[1].elem_type.vector.vector_num_elems   = Num_Elems(s_t1v2);
   s_t1[1].is_branch = BUFF_FALSE;


   s_t2[0].elem_type.tree.branch               = s_t2t1;
   s_t2[0].elem_type.tree.branch_num_elems     = Num_Elems(s_t2t1);
   s_t2[0].is_branch = BUFF_TRUE;

   s_t2[1].elem_type.tree.branch               = NULL;
   s_t2[1].elem_type.tree.branch_num_elems     = 0;
   s_t2[1].is_branch = BUFF_TRUE;

   s_t2[2].elem_type.vector.vector             = s_t2v1;
   s_t2[2].elem_type.vector.vector_num_elems   = Num_Elems(s_t2v1);
   s_t2[2].is_branch = BUFF_FALSE;


   s_t2t1[0].elem_type.vector.vector           = s_t2t1v1;
   s_t2t1[0].elem_type.vector.vector_num_elems = Num_Elems(s_t2t1v1);
   s_t2t1[0].is_branch = BUFF_FALSE;



   pos = 0;
   ptr = src;

   s_t1v1[0].data = &ptr[pos];
   s_t1v1[0].size = 17;
   pos += 17;

   s_t1v1[1].data = &ptr[pos];
   s_t1v1[1].size = 0;
   pos += 0;

   s_t1v1[2].data = &ptr[pos];
   s_t1v1[2].size = 7;
   pos += 7;


   s_t1v2[0].data = &ptr[pos];
   s_t1v2[0].size = 33;
   pos += 33;


   s_v1[0].data = NULL;
   s_v1[0].size = 17;
   pos += 0;

   s_v1[1].data = &ptr[pos];
   s_v1[1].size = 5;
   pos += 5;

   s_v1[2].data = NULL;
   s_v1[2].size = 1;
   pos += 0;

   s_v1[3].data = &ptr[pos];
   s_v1[3].size = 1;
   pos += 1;

   s_v1[4].data = &ptr[pos];
   s_v1[4].size = 0;
   pos += 0;


   s_t2t1v1[0].data = &ptr[pos];
   s_t2t1v1[0].size = 100;
   pos += 100;


   s_t2v1[0].data = &ptr[pos];;
   s_t2v1[0].size = 15;
   pos += 15;

   s_t2v1[1].data = &ptr[pos];
   s_t2v1[1].size = sizeof(src) - pos;
   pos = sizeof(src);





   d_t[0].elem_type.tree.branch                = NULL;
   d_t[0].elem_type.tree.branch_num_elems      = 0;
   d_t[0].is_branch = BUFF_TRUE;

   d_t[1].elem_type.tree.branch                = d_t1;
   d_t[1].elem_type.tree.branch_num_elems      = Num_Elems(d_t1);
   d_t[1].is_branch = BUFF_TRUE;

   d_t[2].elem_type.tree.branch                = NULL;
   d_t[2].elem_type.tree.branch_num_elems      = 0;
   d_t[2].is_branch = BUFF_TRUE;

   d_t[3].elem_type.vector.vector              = NULL;
   d_t[3].elem_type.vector.vector_num_elems    = 0;
   d_t[3].is_branch = BUFF_FALSE;

   d_t[4].elem_type.vector.vector              = d_v1;
   d_t[4].elem_type.vector.vector_num_elems    = Num_Elems(d_v1);
   d_t[4].is_branch = BUFF_FALSE;

   d_t[5].elem_type.tree.branch                = d_t2;
   d_t[5].elem_type.tree.branch_num_elems      = Num_Elems(d_t2);
   d_t[5].is_branch = BUFF_TRUE;


   d_t1[0].elem_type.vector.vector             = d_t1v1;
   d_t1[0].elem_type.vector.vector_num_elems   = Num_Elems(d_t1v1);
   d_t1[0].is_branch = BUFF_FALSE;

   d_t1[1].elem_type.vector.vector             = d_t1v2;
   d_t1[1].elem_type.vector.vector_num_elems   = Num_Elems(d_t1v2);
   d_t1[1].is_branch = BUFF_FALSE;


   d_t2[0].elem_type.tree.branch               = d_t2t1;
   d_t2[0].elem_type.tree.branch_num_elems     = Num_Elems(d_t2t1);
   d_t2[0].is_branch = BUFF_TRUE;

   d_t2[1].elem_type.tree.branch               = NULL;
   d_t2[1].elem_type.tree.branch_num_elems     = 0;
   d_t2[1].is_branch = BUFF_TRUE;

   d_t2[2].elem_type.vector.vector             = d_t2v1;
   d_t2[2].elem_type.vector.vector_num_elems   = Num_Elems(d_t2v1);
   d_t2[2].is_branch = BUFF_FALSE;


   d_t2t1[0].elem_type.vector.vector           = d_t2t1v1;
   d_t2t1[0].elem_type.vector.vector_num_elems = Num_Elems(d_t2t1v1);
   d_t2t1[0].is_branch = BUFF_FALSE;



   pos = 0;
   ptr = dest;

   d_t1v1[0].data = &ptr[pos];
   d_t1v1[0].size = 20;
   pos += 20;

   d_t1v1[1].data = &ptr[pos];
   d_t1v1[1].size = 0;
   pos += 0;

   d_t1v1[2].data = &ptr[pos];
   d_t1v1[2].size = 7;
   pos += 7;


   d_t1v2[0].data = &ptr[pos];
   d_t1v2[0].size = 30;
   pos += 30;


   d_v1[0].data = NULL;
   d_v1[0].size = 17;
   pos += 0;

   d_v1[1].data = &ptr[pos];
   d_v1[1].size = 3;
   pos += 3;

   d_v1[2].data = NULL;
   d_v1[2].size = 1;
   pos += 0;

   d_v1[3].data = &ptr[pos];
   d_v1[3].size = 11;
   pos += 11;

   d_v1[4].data = &ptr[pos];
   d_v1[4].size = 0;
   pos += 0;


   d_t2t1v1[0].data = &ptr[pos];
   d_t2t1v1[0].size = 100;
   pos += 100;


   d_t2v1[0].data = &ptr[pos];
   d_t2v1[0].size = 15;
   pos += 15;

   d_t2v1[1].data = &ptr[pos];
   d_t2v1[1].size = sizeof(src) - pos;
   pos = sizeof(src);





   d_t_short[0].elem_type.tree.branch                = NULL;
   d_t_short[0].elem_type.tree.branch_num_elems      = 0;
   d_t_short[0].is_branch = BUFF_TRUE;

   d_t_short[1].elem_type.tree.branch                = d_t1_short;
   d_t_short[1].elem_type.tree.branch_num_elems      = Num_Elems(d_t1_short);
   d_t_short[1].is_branch = BUFF_TRUE;

   d_t_short[2].elem_type.tree.branch                = NULL;
   d_t_short[2].elem_type.tree.branch_num_elems      = 0;
   d_t_short[2].is_branch = BUFF_TRUE;

   d_t_short[3].elem_type.vector.vector              = NULL;
   d_t_short[3].elem_type.vector.vector_num_elems    = 0;
   d_t_short[3].is_branch = BUFF_FALSE;

   d_t_short[4].elem_type.vector.vector              = d_v1_short;
   d_t_short[4].elem_type.vector.vector_num_elems    = Num_Elems(d_v1_short);
   d_t_short[4].is_branch = BUFF_FALSE;

   d_t_short[5].elem_type.tree.branch                = d_t2_short;
   d_t_short[5].elem_type.tree.branch_num_elems      = Num_Elems(d_t2_short);
   d_t_short[5].is_branch = BUFF_TRUE;


   d_t1_short[0].elem_type.vector.vector             = d_t1v1_short;
   d_t1_short[0].elem_type.vector.vector_num_elems   = Num_Elems(d_t1v1_short);
   d_t1_short[0].is_branch = BUFF_FALSE;

   d_t1_short[1].elem_type.vector.vector             = d_t1v2_short;
   d_t1_short[1].elem_type.vector.vector_num_elems   = Num_Elems(d_t1v2_short);
   d_t1_short[1].is_branch = BUFF_FALSE;


   d_t2_short[0].elem_type.tree.branch               = d_t2t1_short;
   d_t2_short[0].elem_type.tree.branch_num_elems     = Num_Elems(d_t2t1_short);
   d_t2_short[0].is_branch = BUFF_TRUE;

   d_t2_short[1].elem_type.tree.branch               = NULL;
   d_t2_short[1].elem_type.tree.branch_num_elems     = 0;
   d_t2_short[1].is_branch = BUFF_TRUE;

   d_t2_short[2].elem_type.vector.vector             = d_t2v1_short;
   d_t2_short[2].elem_type.vector.vector_num_elems   = Num_Elems(d_t2v1_short);
   d_t2_short[2].is_branch = BUFF_FALSE;


   d_t2t1_short[0].elem_type.vector.vector           = d_t2t1v1_short;
   d_t2t1_short[0].elem_type.vector.vector_num_elems = Num_Elems(d_t2t1v1_short);
   d_t2t1_short[0].is_branch = BUFF_FALSE;



   pos = 0;
   ptr = dest;

   d_t1v1_short[0].data = &ptr[pos];
   d_t1v1_short[0].size = 20;
   pos += 20;

   d_t1v1_short[1].data = &ptr[pos];
   d_t1v1_short[1].size = 0;
   pos += 0;

   d_t1v1_short[2].data = &ptr[pos];
   d_t1v1_short[2].size = 7;
   pos += 7;


   d_t1v2_short[0].data = &ptr[pos];
   d_t1v2_short[0].size = 30;
   pos += 30;


   d_v1_short[0].data = NULL;
   d_v1_short[0].size = 17;
   pos += 0;

   d_v1_short[1].data = &ptr[pos];
   d_v1_short[1].size = 3;
   pos += 3;

   d_v1_short[2].data = NULL;
   d_v1_short[2].size = 1;
   pos += 0;

   d_v1_short[3].data = &ptr[pos];
   d_v1_short[3].size = 11;
   pos += 11;

   d_v1_short[4].data = &ptr[pos];
   d_v1_short[4].size = 0;
   pos += 0;


   d_t2t1v1_short[0].data = &ptr[pos];
   d_t2t1v1_short[0].size = 80;
   pos += 80;


   d_t2v1_short[0].data = &ptr[pos];
   d_t2v1_short[0].size = 15;
   pos += 15;

   d_t2v1_short[1].data = &ptr[pos];
   d_t2v1_short[1].size = SHORTER_DESTINATION - pos;
   pos = SHORTER_DESTINATION;



   printf("\n\rtree size read test:\n\r");
   if(sizeof(src) != Buff_Get_Readable_Tree_Data_Size(s_t, Num_Elems(s_t)))
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Get_Readable_Tree_Data_Size");
   }
   else
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Get_Readable_Tree_Data_Size");
   }
   if(sizeof(src) != Buff_Get_Writeable_Tree_Data_Size(d_t, Num_Elems(d_t)))
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Get_Writeable_Tree_Data_Size");
   }
   else
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Get_Writeable_Tree_Data_Size");
   }



   printf("\n\rtree contineous element size read test:\n\r");
   if(100 != Buff_Readable_Tree_Get_Contineous_Size(s_t, Num_Elems(s_t), 63))
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Readable_Tree_Get_Contineous_Size");
   }
   else
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Readable_Tree_Get_Contineous_Size");
   }
   if(93 != Buff_Readable_Tree_Get_Contineous_Size(s_t, Num_Elems(s_t), 70))
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Readable_Tree_Get_Contineous_Size");
   }
   else
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Readable_Tree_Get_Contineous_Size");
   }
   if(11 != Buff_Writeable_Tree_Get_Contineous_Size(d_t, Num_Elems(d_t), 60))
   {
      printf("line: %5d; %-44s failed\n\r", __LINE__, "Buff_Writeable_Tree_Get_Contineous_Size");
   }
   else
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Writeable_Tree_Get_Contineous_Size");
   }



   printf("\n\rtree copy tests:\n\r");



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
  //          printf("size: %d, dest_offset: %d, src_offset: %d\n\r", size, dest_offset, src_offset);
            memset(dest, 0, sizeof(dest));

            result = Buff_Copy_From_Tree(dest, s_t, Num_Elems(s_t), size, dest_offset, src_offset, NULL);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_From_Tree");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_From_Tree");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_From_Tree", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
            memset(dest, 0, sizeof(dest));

            result = Buff_Copy_To_Tree(d_t, Num_Elems(d_t), src, size, dest_offset, src_offset, NULL);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_To_Tree");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_To_Tree");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_To_Tree", failure_cntr);
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

            result = Buff_Copy_To_Tree(d_t_short, Num_Elems(d_t_short), src, size, dest_offset, src_offset, NULL);

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
                  &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_To_Tree-d_t_short");
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
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_To_Tree-d_t_short");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_To_Tree-d_t_short", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 1; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
 //           printf("size: %d, dest_offset: %d, src_offset: %d\n\r", size, dest_offset, src_offset);
            memset(dest, 0, sizeof(dest));

            result = Buff_Copy_Tree_To_Tree(d_t, Num_Elems(d_t), s_t, Num_Elems(s_t), size, dest_offset, src_offset, NULL, NULL);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_Tree_To_Tree");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_Tree_To_Tree");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_Tree_To_Tree", failure_cntr);
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

            result = Buff_Copy_Tree_To_Tree(d_t_short, Num_Elems(d_t_short), s_t, Num_Elems(s_t), size, dest_offset, src_offset, NULL, NULL);

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
                  &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_Tree_To_Tree-d_t_short");
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
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_Tree_To_Tree-d_t_short");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_Tree_To_Tree-d_t_short", failure_cntr);
   }



   printf("\n\rtree vendor copy tests:\n\r");


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

            result = Buff_Copy_From_Tree_Vendor(dest, s_t, Num_Elems(s_t), size, dest_offset, src_offset, NULL, main_vendor_memcpy);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_From_Tree_Vendor");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_From_Tree_Vendor");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_From_Tree_Vendor", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 3; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
  //          printf("size: %d, dest_offset: %d, src_offset: %d\n\r", size, dest_offset, src_offset);
            memset(dest, 0, sizeof(dest));
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = size - 2;

            result = Buff_Copy_From_Tree_Vendor(dest, s_t, Num_Elems(s_t), size, dest_offset, src_offset, NULL, main_vendor_memcpy);

            if(vendor_cpy_cntr_limit != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, vendor_cpy_cntr_limit);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + vendor_cpy_cntr_limit], sizeof(dest) - (dest_offset + vendor_cpy_cntr_limit));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], vendor_cpy_cntr_limit, sizeof(src) - vendor_cpy_cntr_limit, "Buff_Copy_From_Tree_Vendor-limit");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_From_Tree_Vendor-limit");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_From_Tree_Vendor-limit", failure_cntr);
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

            result = Buff_Copy_To_Tree_Vendor(d_t, Num_Elems(d_t), src, size, dest_offset, src_offset, NULL, main_vendor_memcpy);

            if(size != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, size);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + size], sizeof(dest) - (dest_offset + size));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], size, sizeof(src) - size, "Buff_Copy_To_Tree_Vendor");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_To_Tree_Vendor");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_To_Tree_Vendor", failure_cntr);
   }



   failure_cntr = 0;
   for(size = 3; size < sizeof(dest); size++)
   {
      for(dest_offset = 0; dest_offset < (sizeof(dest) - size); dest_offset++)
      {
         for(src_offset = 0; src_offset < (sizeof(dest) - size); src_offset++)
         {
 //           printf("size: %d, dest_offset: %d, src_offset: %d\n\r", size, dest_offset, src_offset);
            memset(dest, 0, sizeof(dest));
            vendor_cpy_cntr = 0;
            vendor_cpy_cntr_limit = size - 2;

            result = Buff_Copy_To_Tree_Vendor(d_t, Num_Elems(d_t), src, size, dest_offset, src_offset, NULL, main_vendor_memcpy);

            if(vendor_cpy_cntr_limit != result)
            {
               printf("wrong size returned! %d, expected: %d\n\r", result, vendor_cpy_cntr_limit);
               printf("press any key to continue...\n\r");
               getc(stdin);
            }
            is_zero(dest, dest_offset);
            is_zero(&dest[dest_offset + vendor_cpy_cntr_limit], sizeof(dest) - (dest_offset + vendor_cpy_cntr_limit));
            compare_and_print_status(
               &dest[dest_offset], &src[src_offset], vendor_cpy_cntr_limit, sizeof(src) - vendor_cpy_cntr_limit, "Buff_Copy_To_Tree_Vendor-limit");
         }
      }

      printf("step %d / %d\r", size, sizeof(dest));
   }
   if(0 == failure_cntr)
   {
      printf("line: %5d; %-44s succeeded\n\r", __LINE__, "Buff_Copy_To_Tree_Vendor-limit");
   }
   else
   {
      printf("%-36s failed %d times\n\r", "Buff_Copy_To_Tree_Vendor-limit", failure_cntr);
   }
} /* tree_test */

