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

#ifndef BUFF_RING_C_
#define BUFF_RING_C_
#endif

#include "buff.h"

#ifdef BUFF_INCLUDE_IMP_CFG
#include "buff_cfg.h"
#endif

#ifndef BUFF_RING_IS_FREE_SPACE_CONTINOUS
#define BUFF_RING_IS_FREE_SPACE_CONTINOUS(buf)     ((0 == (buf)->first_busy) || ((buf)->first_free < (buf)->first_busy))
#endif

#ifndef BUFF_RING_GET_ORIGINAL_FREE_SIZE
#define BUFF_RING_GET_ORIGINAL_FREE_SIZE(buf)      ((buf)->original_size - (buf)->busy_size)
#endif

#if(BUFF_SAVE_PROGRAM_MEMORY)
static Buff_Size_DT buff_vendor_memcpy(const Buff_Memcpy_Params_XT *params)
{
   uint8_t       *dest = &((uint8_t*)(params->dest))[params->dest_offset];
   const uint8_t *src  = &((const uint8_t*)(params->src))[params->src_offset];
   Buff_Size_DT   cntr;

   for(cntr = 0; cntr < params->size; cntr++)
   {
      dest[cntr] = src[cntr];
   }

   return params->size;
} /* buff_vendor_memcpy */
#endif

#if(BUFF_USE_VECTOR_BUFFERS || BUFF_USE_TREE_BUFFERS)

#if( (!defined(BUFF_GET_READABLE_VECTOR_DATA_SIZE_EXTERNAL)) && (BUFF_GET_READABLE_VECTOR_DATA_SIZE_ENABLED || BUFF_GET_WRITEABLE_VECTOR_DATA_SIZE_ENABLED || BUFF_GET_READABLE_TREE_DATA_SIZE_ENABLED || BUFF_GET_WRITEABLE_TREE_DATA_SIZE_ENABLED) )
Buff_Size_DT Buff_Get_Readable_Vector_Data_Size(const Buff_Readable_Vector_XT *vector, Buff_Num_Elems_DT vector_num_elems)
{
   const Buff_Readable_Vector_XT*elem;
   Buff_Size_DT                  result = 0;
   Buff_Num_Elems_DT             cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Readable_Vector_XT, vector)))
   {
      for(cntr = 0; cntr < vector_num_elems; cntr++)
      {
         elem = &vector[cntr];

         if(BUFF_LIKELY(BUFF_CHECK_PTR(const void, elem->data)))
         {
            result += elem->size;
         }
      }
   }

   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Get_Readable_Vector_Data_size */
#endif

#endif
#if(BUFF_USE_VECTOR_BUFFERS)

#if( (!defined(BUFF_GET_WRITEABLE_VECTOR_DATA_SIZE_EXTERNAL)) && BUFF_GET_WRITEABLE_VECTOR_DATA_SIZE_ENABLED )
Buff_Size_DT Buff_Get_Writeable_Vector_Data_Size(const Buff_Writeable_Vector_XT *vector, Buff_Num_Elems_DT vector_num_elems)
{
   Buff_Size_DT result;

   BUFF_ENTER_FUNC();

   result = Buff_Get_Readable_Vector_Data_Size((const Buff_Readable_Vector_XT*)vector, vector_num_elems);

   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Get_Writeable_Vector_Data_size */
#endif



#if( (!defined(BUFF_COPY_FROM_VECTOR_EXTERNAL)) && BUFF_COPY_FROM_VECTOR_ENABLED )
Buff_Size_DT Buff_Copy_From_Vector(
   void                            *dest,
   const Buff_Readable_Vector_XT   *src,
   Buff_Num_Elems_DT                src_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Readable_Tree_XT tree;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = src;
   tree.elem_type.vector.vector_num_elems = src_num_elems;

#if(BUFF_USE_VENDOR_BUFFERS)
   return Buff_Copy_From_Tree_Vendor(
      dest, &tree, 1, size, offset_dest, offset_src, BUFF_MAKE_INVALID_PTR(Buff_Size_DT), buff_vendor_memcpy);
#else
   return Buff_Copy_From_Tree(
      dest, &tree, 1, size, offset_dest, offset_src, BUFF_MAKE_INVALID_PTR(Buff_Size_DT));
#endif

#elif(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VENDOR_BUFFERS)

   return Buff_Copy_From_Vector_Vendor(dest, src, src_num_elems, size, offset_dest, offset_src, buff_vendor_memcpy);

#else

   const Buff_Readable_Vector_XT *src_vector;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(void, dest) && BUFF_CHECK_PTR(Buff_Readable_Vector_XT, src)))
   {
      for(cntr = 0; cntr < src_num_elems; cntr++)
      {
         src_vector = &src[cntr];

         if(BUFF_LIKELY(BUFF_CHECK_PTR(const void, src_vector->data)))
         {
            if(offset_src < src_vector->size)
            {
               if((src_vector->size - offset_src) < size)
               {
                  part = src_vector->size - offset_src;
                  memcpy(&((uint8_t*)dest)[offset_dest], &((const uint8_t*)(src_vector->data))[offset_src], part);
                  offset_dest += part;
                  offset_src  += part;
                  result      += part;
                  size        -= part;
               }
               else
               {
                  memcpy(&((uint8_t*)dest)[offset_dest], &((const uint8_t*)(src_vector->data))[offset_src], size);
                  result      += size;
                  break;
               }
            }

            offset_src -= src_vector->size;
         }
      }
   }

   BUFF_EXIT_FUNC();

   return result;

#endif
} /* Buff_Copy_From_Vector */
#endif



#if( (!defined(BUFF_COPY_FROM_VECTOR_VENDOR_EXTERNAL)) && BUFF_COPY_FROM_VECTOR_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
Buff_Size_DT Buff_Copy_From_Vector_Vendor(
   void                            *dest,
   const Buff_Readable_Vector_XT   *src,
   Buff_Num_Elems_DT                src_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src,
   Buff_Vendor_Memcpy_HT            vendor_cpy)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Readable_Tree_XT tree;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = src;
   tree.elem_type.vector.vector_num_elems = src_num_elems;

   return Buff_Copy_From_Tree_Vendor(
      dest, &tree, 1, size, offset_dest, offset_src, BUFF_MAKE_INVALID_PTR(Buff_Size_DT), vendor_cpy);

#else

   const Buff_Readable_Vector_XT *src_vector;
   Buff_Memcpy_Params_XT memcpy_params;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      memcpy_result;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(void, dest) && BUFF_CHECK_PTR(Buff_Readable_Vector_XT, src)
      && BUFF_CHECK_HANDLER(Buff_Vendor_Memcpy_HT, vendor_cpy)))
   {
      memcpy_params.dest = dest;

      for(cntr = 0; cntr < src_num_elems; cntr++)
      {
         src_vector = &src[cntr];

         if(BUFF_LIKELY(BUFF_CHECK_PTR(const void, src_vector->data)))
         {
            if(offset_src < src_vector->size)
            {
               memcpy_params.src          = src_vector->data;
               memcpy_params.dest_offset  = offset_dest;
               memcpy_params.src_offset   = offset_src;

               if((src_vector->size - offset_src) < size)
               {
                  part = src_vector->size - offset_src;

                  memcpy_params.size = part;
                  memcpy_params.is_last_part = BUFF_FALSE;

                  memcpy_result = vendor_cpy(&memcpy_params);

                  if(memcpy_result < part)
                  {
                     result      += memcpy_result;
                     break;
                  }
                  else
                  {
                     offset_dest += part;
                     offset_src  += part;
                     result      += part;
                     size        -= part;
                  }
               }
               else
               {
                  memcpy_params.size = size;
                  memcpy_params.is_last_part = BUFF_TRUE;

                  memcpy_result   = vendor_cpy(&memcpy_params);

                  if(BUFF_UNLIKELY(memcpy_result < size))
                  {
                     size         = memcpy_result;
                  }
                  result         += size;
                  break;
               }
            }

            offset_src -= src_vector->size;
         }
      }
   }

   BUFF_EXIT_FUNC();

   return result;

#endif
} /* Buff_Copy_From_Vector_Vendor */
#endif



#if( (!defined(BUFF_COPY_TO_VECTOR_EXTERNAL)) && BUFF_COPY_TO_VECTOR_ENABLED )
Buff_Size_DT Buff_Copy_To_Vector(
   const Buff_Writeable_Vector_XT  *dest,
   Buff_Num_Elems_DT                dest_num_elems,
   const void                      *src,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Writeable_Tree_XT tree;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = dest;
   tree.elem_type.vector.vector_num_elems = dest_num_elems;

#if(BUFF_USE_VENDOR_BUFFERS)
   return Buff_Copy_To_Tree_Vendor(
      &tree, 1, src, size, offset_dest, offset_src, BUFF_MAKE_INVALID_PTR(Buff_Size_DT), buff_vendor_memcpy);
#else
   return Buff_Copy_To_Tree(
      &tree, 1, src, size, offset_dest, offset_src, BUFF_MAKE_INVALID_PTR(Buff_Size_DT));
#endif

#elif(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VENDOR_BUFFERS)

   return Buff_Copy_To_Vector_Vendor(dest, dest_num_elems, src, size, offset_dest, offset_src, buff_vendor_memcpy);

#else

   const Buff_Writeable_Vector_XT *dest_vector;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(Buff_Writeable_Vector_XT, dest) && BUFF_CHECK_PTR(void, src)))
   {
      for(cntr = 0; cntr < dest_num_elems; cntr++)
      {
         dest_vector = &dest[cntr];

         if(BUFF_LIKELY(BUFF_CHECK_PTR(void, dest_vector->data)))
         {
            if(offset_dest < dest_vector->size)
            {
               if((dest_vector->size - offset_dest) < size)
               {
                  part = dest_vector->size - offset_dest;
                  memcpy( &((uint8_t*)(dest_vector->data))[offset_dest], &((const uint8_t*)src)[offset_src], part);
                  offset_dest += part;
                  offset_src  += part;
                  result      += part;
                  size        -= part;
               }
               else
               {
                  memcpy( &((uint8_t*)(dest_vector->data))[offset_dest], &((const uint8_t*)src)[offset_src], size);
                  result      += size;
                  break;
               }
            }

            offset_dest -= dest_vector->size;
         }
      }
   }

   BUFF_EXIT_FUNC();

   return result;

#endif
} /* Buff_Copy_To_Vector */
#endif



#if( (!defined(BUFF_COPY_TO_VECTOR_VENDOR_EXTERNAL)) && BUFF_COPY_TO_VECTOR_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
Buff_Size_DT Buff_Copy_To_Vector_Vendor(
   const Buff_Writeable_Vector_XT  *dest,
   Buff_Num_Elems_DT                dest_num_elems,
   const void                      *src,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src,
   Buff_Vendor_Memcpy_HT            vendor_cpy)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Writeable_Tree_XT tree;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = dest;
   tree.elem_type.vector.vector_num_elems = dest_num_elems;

   return Buff_Copy_To_Tree_Vendor(
      &tree, 1, src, size, offset_dest, offset_src, BUFF_MAKE_INVALID_PTR(Buff_Size_DT), vendor_cpy);

#else

   const Buff_Writeable_Vector_XT *dest_vector;
   Buff_Memcpy_Params_XT memcpy_params;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      memcpy_result;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(Buff_Writeable_Vector_XT, dest) && BUFF_CHECK_PTR(void, src)
      && BUFF_CHECK_HANDLER(Buff_Vendor_Memcpy_HT, vendor_cpy)))
   {
      memcpy_params.src = src;

      for(cntr = 0; cntr < dest_num_elems; cntr++)
      {
         dest_vector = &dest[cntr];

         if(BUFF_LIKELY(BUFF_CHECK_PTR(void, dest_vector->data)))
         {
            if(offset_dest < dest_vector->size)
            {
               memcpy_params.dest         = dest_vector->data;
               memcpy_params.dest_offset  = offset_dest;
               memcpy_params.src_offset   = offset_src;

               if((dest_vector->size - offset_dest) < size)
               {
                  part = dest_vector->size - offset_dest;

                  memcpy_params.size = part;
                  memcpy_params.is_last_part = BUFF_FALSE;

                  memcpy_result = vendor_cpy(&memcpy_params);

                  if(memcpy_result < part)
                  {
                     result      += memcpy_result;
                     break;
                  }
                  else
                  {
                     offset_dest += part;
                     offset_src  += part;
                     result      += part;
                     size        -= part;
                  }
               }
               else
               {
                  memcpy_params.size = size;
                  memcpy_params.is_last_part = BUFF_TRUE;

                  memcpy_result   = vendor_cpy(&memcpy_params);

                  if(BUFF_UNLIKELY(memcpy_result < size))
                  {
                     size         = memcpy_result;
                  }
                  result         += size;
                  break;
               }
            }

            offset_dest -= dest_vector->size;
         }
      }
   }

   BUFF_EXIT_FUNC();

   return result;

#endif
} /* Buff_Copy_To_Vector_Vendor */
#endif



#if( (!defined(BUFF_COPY_VECTOR_TO_VECTOR_EXTERNAL)) && BUFF_COPY_VECTOR_TO_VECTOR_ENABLED )
Buff_Size_DT Buff_Copy_Vector_To_Vector(
   const Buff_Writeable_Vector_XT  *dest,
   Buff_Num_Elems_DT                dest_num_elems,
   const Buff_Readable_Vector_XT   *src,
   Buff_Num_Elems_DT                src_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_dest,
   Buff_Size_DT                     offset_src)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Writeable_Tree_XT tree_dest;
   Buff_Readable_Tree_XT tree_src;

   tree_dest.is_branch = BUFF_FALSE;
   tree_dest.elem_type.vector.vector           = dest;
   tree_dest.elem_type.vector.vector_num_elems = dest_num_elems;

   tree_src.is_branch = BUFF_FALSE;
   tree_src.elem_type.vector.vector            = src;
   tree_src.elem_type.vector.vector_num_elems  = src_num_elems;

   return Buff_Copy_Tree_To_Tree(
      &tree_dest,
      1,
      &tree_src,
      1,
      size,
      offset_dest,
      offset_src,
      BUFF_MAKE_INVALID_PTR(Buff_Size_DT),
      BUFF_MAKE_INVALID_PTR(Buff_Bool_DT));

#else

   const Buff_Readable_Vector_XT *src_vector;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      ret_part;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(Buff_Writeable_Vector_XT, dest) && BUFF_CHECK_PTR(Buff_Readable_Vector_XT, src)
      && (dest_num_elems > 0)))
   {
      for(cntr = 0; cntr < src_num_elems; cntr++)
      {
         src_vector = &src[cntr];

         if(BUFF_LIKELY(BUFF_CHECK_PTR(void, src_vector->data)))
         {
            if(offset_src < src_vector->size)
            {
               if((src_vector->size - offset_src) < size)
               {
                  part        = src_vector->size - offset_src;
                  ret_part    = Buff_Copy_To_Vector(dest, dest_num_elems, src_vector->data, part, offset_dest, offset_src);
                  offset_dest+= ret_part;
                  offset_src += ret_part;
                  result     += ret_part;
                  size       -= ret_part;

                  if(ret_part < part)
                  {
                     break;
                  }
               }
               else
               {
                  ret_part    = Buff_Copy_To_Vector(dest, dest_num_elems, src_vector->data, size, offset_dest, offset_src);
                  result     += ret_part;
                  break;
               }
            }

            offset_src       -= src_vector->size;
         }
      }
   }

   BUFF_EXIT_FUNC();

   return result;

#endif
} /* Buff_Copy_Vector_To_Vector */
#endif

#endif /* #if(BUFF_USE_VECTOR_BUFFERS) */



#if(BUFF_USE_TREE_BUFFERS)

#if( (!defined(BUFF_GET_READABLE_TREE_DATA_SIZE_EXTERNAL)) && (BUFF_GET_READABLE_TREE_DATA_SIZE_ENABLED || BUFF_GET_WRITEABLE_TREE_DATA_SIZE_ENABLED) )
Buff_Size_DT Buff_Get_Readable_Tree_Data_Size(const Buff_Readable_Tree_XT *tree, Buff_Num_Elems_DT tree_num_elems)
{
   const Buff_Readable_Tree_XT  *elem;
   Buff_Size_DT                  result = 0;
   Buff_Num_Elems_DT             cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Readable_Tree_XT, tree)))
   {
      for(cntr = 0; cntr < tree_num_elems; cntr++)
      {
         elem = &tree[cntr];

         if(BUFF_UNLIKELY(BUFF_BOOL_IS_TRUE(elem->is_branch)))
         {
            result += Buff_Get_Readable_Tree_Data_Size(elem->elem_type.tree.branch, elem->elem_type.tree.branch_num_elems);
         }
         else
         {
            result += Buff_Get_Readable_Vector_Data_Size(elem->elem_type.vector.vector, elem->elem_type.vector.vector_num_elems);
         }
      }
   }

   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Get_Readable_Tree_Data_size */
#endif



#if( (!defined(BUFF_GET_WRITEABLE_TREE_DATA_SIZE_EXTERNAL)) && BUFF_GET_WRITEABLE_TREE_DATA_SIZE_ENABLED )
Buff_Size_DT Buff_Get_Writeable_Tree_Data_Size(const Buff_Writeable_Tree_XT *tree, Buff_Num_Elems_DT tree_num_elems)
{
   Buff_Size_DT result;

   BUFF_ENTER_FUNC();

   result = Buff_Get_Readable_Tree_Data_Size((const Buff_Readable_Tree_XT*)tree, tree_num_elems);

   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Get_Writeable_Tree_Data_size */
#endif



#if( (!defined(BUFF_COPY_FROM_TREE_EXTERNAL)) && BUFF_COPY_FROM_TREE_ENABLED )
Buff_Size_DT Buff_Copy_From_Tree(
   void                         *dest,
   const Buff_Readable_Tree_XT  *src,
   Buff_Num_Elems_DT             src_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VENDOR_BUFFERS)

   return Buff_Copy_From_Tree_Vendor(dest, src, src_num_elems, size, offset_dest, offset_src, str_size, buff_vendor_memcpy);

#else

   const Buff_Readable_Tree_XT   *tree_elem;
   const Buff_Readable_Vector_XT *vector_elem;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      ret_size_elem = 0;
   Buff_Size_DT      ret_size_sub_elem;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT tree_cntr;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(void, dest) && BUFF_CHECK_PTR(Buff_Readable_Tree_XT, src) && (src_num_elems > 0)))
   {
      for(tree_cntr = 0; tree_cntr < src_num_elems; tree_cntr++)
      {
         tree_elem = &src[tree_cntr];

         if(BUFF_UNLIKELY(BUFF_BOOL_IS_TRUE(tree_elem->is_branch)))
         {
            if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Readable_Tree_XT, tree_elem->elem_type.tree.branch)
               && (tree_elem->elem_type.tree.branch_num_elems > 0)))
            {
               ret_size_sub_elem = 0;

               (void)Buff_Copy_From_Tree(
                  dest,
                  tree_elem->elem_type.tree.branch,
                  tree_elem->elem_type.tree.branch_num_elems,
                  size,
                  offset_dest,
                  offset_src,
                  &ret_size_sub_elem);

               ret_size_elem    += ret_size_sub_elem;

               if(offset_src < ret_size_sub_elem)
               {
                  if((ret_size_sub_elem - offset_src) < size)
                  {
                     part        = ret_size_sub_elem - offset_src;
                     offset_dest+= part;
                     offset_src += part;
                     result     += part;
                     size       -= part;
                  }
                  else
                  {
                     result     += size;
                     break;
                  }
               }

               offset_src       -= ret_size_sub_elem;
            }
         }
         else if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Readable_Vector_XT, tree_elem->elem_type.vector.vector)))
         {
            for(cntr = 0; cntr < tree_elem->elem_type.vector.vector_num_elems; cntr++)
            {
               vector_elem = &(tree_elem->elem_type.vector.vector[cntr]);

               if(BUFF_LIKELY(BUFF_CHECK_PTR(const void, vector_elem->data)))
               {
                  if(offset_src < vector_elem->size)
                  {
                     if((vector_elem->size - offset_src) < size)
                     {
                        part = vector_elem->size - offset_src;
                        memcpy(&((uint8_t*)dest)[offset_dest], &((const uint8_t*)(vector_elem->data))[offset_src], part);
                        offset_dest    += part;
                        offset_src     += part;
                        result         += part;
                        size           -= part;
                     }
                     else
                     {
                        memcpy(&((uint8_t*)dest)[offset_dest], &((const uint8_t*)(vector_elem->data))[offset_src], size);
                        result         += size;
                        ret_size_elem  += size + offset_src;
                        size            = 0;
                        break;
                     }
                  }

                  offset_src        -= vector_elem->size;
                  ret_size_elem     += vector_elem->size;
               }
            }

            if(0 == size)
            {
               break;
            }
         }
      }

      if(BUFF_CHECK_PTR(Buff_Size_DT, str_size))
      {
         *str_size = ret_size_elem;
      }
   }

   BUFF_EXIT_FUNC();

   return result;

#endif
} /* Buff_Copy_From_Tree */
#endif



#if( (!defined(BUFF_COPY_FROM_TREE_VENDOR_EXTERNAL)) && BUFF_COPY_FROM_TREE_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
Buff_Size_DT Buff_Copy_From_Tree_Vendor(
   void                         *dest,
   const Buff_Readable_Tree_XT  *src,
   Buff_Num_Elems_DT             src_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size,
   Buff_Vendor_Memcpy_HT         vendor_cpy)
{
   const Buff_Readable_Tree_XT   *tree_elem;
   const Buff_Readable_Vector_XT *vector_elem;
   Buff_Memcpy_Params_XT memcpy_params;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      memcpy_result;
   Buff_Size_DT      ret_size_elem = 0;
   Buff_Size_DT      ret_size_sub_elem;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT tree_cntr;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(void, dest) && BUFF_CHECK_PTR(Buff_Readable_Tree_XT, src) && (src_num_elems > 0)
      && BUFF_CHECK_HANDLER(Buff_Vendor_Memcpy_HT, vendor_cpy)))
   {
      memcpy_params.dest = dest;

      for(tree_cntr = 0; tree_cntr < src_num_elems; tree_cntr++)
      {
         tree_elem = &src[tree_cntr];

         if(BUFF_UNLIKELY(BUFF_BOOL_IS_TRUE(tree_elem->is_branch)))
         {
            if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Readable_Tree_XT, tree_elem->elem_type.tree.branch)
               && (tree_elem->elem_type.tree.branch_num_elems > 0)))
            {
               ret_size_sub_elem = 0;

               memcpy_result = Buff_Copy_From_Tree_Vendor(
                  dest,
                  tree_elem->elem_type.tree.branch,
                  tree_elem->elem_type.tree.branch_num_elems,
                  size,
                  offset_dest,
                  offset_src,
                  &ret_size_sub_elem,
                  vendor_cpy);

               ret_size_elem    += ret_size_sub_elem;

               /* vendor memcpy took less data than provided */
               if(memcpy_result > ret_size_sub_elem)
               {
                  result        += memcpy_result;
                  break;
               }
               else if(offset_src < ret_size_sub_elem)
               {
                  if((ret_size_sub_elem - offset_src) < size)
                  {
                     part        = ret_size_sub_elem - offset_src;
                     offset_dest+= part;
                     offset_src += part;
                     result     += part;
                     size       -= part;
                  }
                  else
                  {
                     result     += size;
                     break;
                  }
               }

               offset_src       -= ret_size_sub_elem;
            }
         }
         else if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Readable_Vector_XT, tree_elem->elem_type.vector.vector)))
         {
            for(cntr = 0; cntr < tree_elem->elem_type.vector.vector_num_elems; cntr++)
            {
               vector_elem = &(tree_elem->elem_type.vector.vector[cntr]);

               if(BUFF_LIKELY(BUFF_CHECK_PTR(const void, vector_elem->data)))
               {
                  if(offset_src < vector_elem->size)
                  {
                     memcpy_params.src          = vector_elem->data;
                     memcpy_params.dest_offset  = offset_dest;
                     memcpy_params.src_offset   = offset_src;

                     if((vector_elem->size - offset_src) < size)
                     {
                        part = vector_elem->size - offset_src;

                        memcpy_params.size = part;
                        memcpy_params.is_last_part = BUFF_FALSE;

                        memcpy_result = vendor_cpy(&memcpy_params);

                        /* vendor memcpy took less data than provided; do not increase ret_size_elem */
                        if(memcpy_result < part)
                        {
                           ret_size_elem= 0;
                           result      += memcpy_result;
                           size         = 0;
                           break;
                        }
                        else
                        {
                           offset_dest += part;
                           offset_src  += part;
                           result      += part;
                           size        -= part;
                        }
                     }
                     else
                     {
                        memcpy_params.size = size;
                        memcpy_params.is_last_part = BUFF_TRUE;

                        memcpy_result  = vendor_cpy(&memcpy_params);
                        if(BUFF_UNLIKELY(memcpy_result < size))
                        {
                           size        = memcpy_result;
                        }
                        result        += size;
                        ret_size_elem += size + offset_src;
                        size           = 0;
                        break;
                     }
                  }

                  offset_src    -= vector_elem->size;
                  ret_size_elem += vector_elem->size;
               }
            }

            if(0 == size)
            {
               break;
            }
         }
      }

      if(BUFF_CHECK_PTR(Buff_Size_DT, str_size))
      {
         *str_size = ret_size_elem;
      }
   }

   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Copy_From_Tree_Vendor */
#endif



#if( (!defined(BUFF_COPY_TO_TREE_EXTERNAL)) && BUFF_COPY_TO_TREE_ENABLED )
Buff_Size_DT Buff_Copy_To_Tree(
   const Buff_Writeable_Tree_XT *dest,
   Buff_Num_Elems_DT             dest_num_elems,
   const void                   *src,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VENDOR_BUFFERS)

   return Buff_Copy_To_Tree_Vendor(dest, dest_num_elems, src, size, offset_dest, offset_src, str_size, buff_vendor_memcpy);

#else

   const Buff_Writeable_Tree_XT   *tree_elem;
   const Buff_Writeable_Vector_XT *vector_elem;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      ret_size_elem = 0;
   Buff_Size_DT      ret_size_sub_elem;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT tree_cntr;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(const void, src) && BUFF_CHECK_PTR(Buff_Writeable_Tree_XT, dest)
      && (dest_num_elems > 0)))
   {
      for(tree_cntr = 0; tree_cntr < dest_num_elems; tree_cntr++)
      {
         tree_elem = &dest[tree_cntr];

         if(BUFF_UNLIKELY(BUFF_BOOL_IS_TRUE(tree_elem->is_branch)))
         {
            if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Writeable_Tree_XT, tree_elem->elem_type.tree.branch)
               && (tree_elem->elem_type.tree.branch_num_elems > 0)))
            {
               ret_size_sub_elem = 0;

               (void)Buff_Copy_To_Tree(
                  tree_elem->elem_type.tree.branch,
                  tree_elem->elem_type.tree.branch_num_elems,
                  src,
                  size,
                  offset_dest,
                  offset_src,
                  &ret_size_sub_elem);

               ret_size_elem    += ret_size_sub_elem;

               if(offset_dest < ret_size_sub_elem)
               {
                  if((ret_size_sub_elem - offset_dest) < size)
                  {
                     part        = ret_size_sub_elem - offset_dest;
                     offset_dest+= part;
                     offset_src += part;
                     result     += part;
                     size       -= part;
                  }
                  else
                  {
                     result     += size;
                     break;
                  }
               }

               offset_dest      -= ret_size_sub_elem;
            }
         }
         else if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Writeable_Vector_XT, tree_elem->elem_type.vector.vector)))
         {
            for(cntr = 0; cntr < tree_elem->elem_type.vector.vector_num_elems; cntr++)
            {
               vector_elem = &(tree_elem->elem_type.vector.vector[cntr]);

               if(BUFF_LIKELY(BUFF_CHECK_PTR(void, vector_elem->data)))
               {
                  if(offset_dest < vector_elem->size)
                  {
                     if((vector_elem->size - offset_dest) < size)
                     {
                        part = vector_elem->size - offset_dest;
                        memcpy( &((uint8_t*)(vector_elem->data))[offset_dest], &((const uint8_t*)src)[offset_src], part);
                        offset_dest    += part;
                        offset_src     += part;
                        result         += part;
                        size           -= part;
                     }
                     else
                     {
                        memcpy( &((uint8_t*)(vector_elem->data))[offset_dest], &((const uint8_t*)src)[offset_src], size);
                        result         += size;
                        ret_size_elem  += size + offset_dest;
                        size            = 0;
                        break;
                     }
                  }

                  offset_dest          -= vector_elem->size;
                  ret_size_elem        += vector_elem->size;
               }
            }

            if(0 == size)
            {
               break;
            }
         }
      }

      if(BUFF_CHECK_PTR(Buff_Size_DT, str_size))
      {
         *str_size = ret_size_elem;
      }
   }

   BUFF_EXIT_FUNC();

   return result;

#endif
} /* Buff_Copy_To_Tree */
#endif



#if( (!defined(BUFF_COPY_TO_TREE_VENDOR_EXTERNAL)) && BUFF_COPY_TO_TREE_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
Buff_Size_DT Buff_Copy_To_Tree_Vendor(
   const Buff_Writeable_Tree_XT *dest,
   Buff_Num_Elems_DT             dest_num_elems,
   const void                   *src,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size,
   Buff_Vendor_Memcpy_HT         vendor_cpy)
{
   const Buff_Writeable_Tree_XT   *tree_elem;
   const Buff_Writeable_Vector_XT *vector_elem;
   Buff_Memcpy_Params_XT memcpy_params;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      memcpy_result;
   Buff_Size_DT      ret_size_elem = 0;
   Buff_Size_DT      ret_size_sub_elem;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT tree_cntr;
   Buff_Num_Elems_DT cntr;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(const void, src) && BUFF_CHECK_PTR(Buff_Writeable_Tree_XT, dest) && (dest_num_elems > 0)
      && BUFF_CHECK_HANDLER(Buff_Vendor_Memcpy_HT, vendor_cpy)))
   {
      memcpy_params.src = src;

      for(tree_cntr = 0; tree_cntr < dest_num_elems; tree_cntr++)
      {
         tree_elem = &dest[tree_cntr];

         if(BUFF_UNLIKELY(BUFF_BOOL_IS_TRUE(tree_elem->is_branch)))
         {
            if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Writeable_Tree_XT, tree_elem->elem_type.tree.branch)
               && (tree_elem->elem_type.tree.branch_num_elems > 0)))
            {
               ret_size_sub_elem = 0;

               memcpy_result = Buff_Copy_To_Tree_Vendor(
                  tree_elem->elem_type.tree.branch,
                  tree_elem->elem_type.tree.branch_num_elems,
                  src,
                  size,
                  offset_dest,
                  offset_src,
                  &ret_size_sub_elem,
                  vendor_cpy);

               ret_size_elem    += ret_size_sub_elem;

               /* vendor memcpy took less data than provided */
               if(memcpy_result > ret_size_sub_elem)
               {
                  result        += memcpy_result;
                  break;
               }
               else if(offset_dest < ret_size_sub_elem)
               {
                  if((ret_size_sub_elem - offset_dest) < size)
                  {
                     part        = ret_size_sub_elem - offset_dest;
                     offset_dest+= part;
                     offset_src += part;
                     result     += part;
                     size       -= part;
                  }
                  else
                  {
                     result     += size;
                     break;
                  }
               }

               offset_dest      -= ret_size_sub_elem;
            }
         }
         else if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Writeable_Vector_XT, tree_elem->elem_type.vector.vector)))
         {
            for(cntr = 0; cntr < tree_elem->elem_type.vector.vector_num_elems; cntr++)
            {
               vector_elem = &(tree_elem->elem_type.vector.vector[cntr]);

               if(BUFF_LIKELY(BUFF_CHECK_PTR(void, vector_elem->data)))
               {
                  if(offset_dest < vector_elem->size)
                  {
                     memcpy_params.dest         = vector_elem->data;
                     memcpy_params.dest_offset  = offset_dest;
                     memcpy_params.src_offset   = offset_src;

                     if((vector_elem->size - offset_dest) < size)
                     {
                        part = vector_elem->size - offset_dest;

                        memcpy_params.size = part;
                        memcpy_params.is_last_part = BUFF_FALSE;

                        memcpy_result = vendor_cpy(&memcpy_params);

                        /* vendor memcpy took less data than provided; do not increase ret_size_elem */
                        if(memcpy_result < part)
                        {
                           ret_size_elem= 0;
                           result      += memcpy_result;
                           size         = 0;
                           break;
                        }
                        else
                        {
                           offset_dest += part;
                           offset_src  += part;
                           result      += part;
                           size        -= part;
                        }
                     }
                     else
                     {
                        memcpy_params.size = size;
                        memcpy_params.is_last_part = BUFF_TRUE;

                        memcpy_result  = vendor_cpy(&memcpy_params);
                        if(BUFF_UNLIKELY(memcpy_result < size))
                        {
                           size        = memcpy_result;
                        }
                        result        += size;
                        ret_size_elem += size + offset_dest;
                        size           = 0;
                        break;
                     }
                  }

                  offset_dest       -= vector_elem->size;
                  ret_size_elem     += vector_elem->size;
               }
            }

            if(0 == size)
            {
               break;
            }
         }
      }

      if(BUFF_CHECK_PTR(Buff_Size_DT, str_size))
      {
         *str_size = ret_size_elem;
      }
   }

   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Copy_To_Tree_Vendor */
#endif



#if( (!defined(BUFF_COPY_TREE_TO_TREE_EXTERNAL)) && BUFF_COPY_TREE_TO_TREE_ENABLED )
Buff_Size_DT Buff_Copy_Tree_To_Tree(
   const Buff_Writeable_Tree_XT *dest,
   Buff_Num_Elems_DT             dest_num_elems,
   const Buff_Readable_Tree_XT  *src,
   Buff_Num_Elems_DT             src_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_dest,
   Buff_Size_DT                  offset_src,
   Buff_Size_DT                 *str_size,
   Buff_Bool_DT                 *end_of_destination_reached)
{
   const Buff_Readable_Tree_XT   *tree_elem;
   const Buff_Readable_Vector_XT *vector_elem;
   Buff_Size_DT      result = 0;
   Buff_Size_DT      ret_size_elem = 0;
   Buff_Size_DT      ret_size_sub_elem;
   Buff_Size_DT      ret_part;
   Buff_Size_DT      part;
   Buff_Num_Elems_DT tree_cntr;
   Buff_Num_Elems_DT cntr;
   Buff_Bool_DT      end_of_destination = BUFF_FALSE;
   Buff_Bool_DT      end_of_destination_sub_elem;

   BUFF_ENTER_FUNC();

   if(BUFF_LIKELY((size > 0)
      && BUFF_CHECK_PTR(Buff_Writeable_Tree_XT, dest) && (dest_num_elems > 0)
      && BUFF_CHECK_PTR(Buff_Readable_Tree_XT, src) && (src_num_elems > 0)))
   {
      for(tree_cntr = 0; tree_cntr < src_num_elems; tree_cntr++)
      {
         tree_elem = &src[tree_cntr];

         if(BUFF_UNLIKELY(BUFF_BOOL_IS_TRUE(tree_elem->is_branch)))
         {
            if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Readable_Tree_XT, tree_elem->elem_type.tree.branch)
               && (tree_elem->elem_type.tree.branch_num_elems > 0)))
            {
               end_of_destination_sub_elem = BUFF_FALSE;

               ret_size_sub_elem = 0;

               ret_part = Buff_Copy_Tree_To_Tree(
                  dest,
                  dest_num_elems,
                  tree_elem->elem_type.tree.branch,
                  tree_elem->elem_type.tree.branch_num_elems,
                  size,
                  offset_dest,
                  offset_src,
                  &ret_size_sub_elem,
                  &end_of_destination_sub_elem);

               ret_size_elem    += ret_size_sub_elem;

               /* less data was copied to destination than requested */
               if(BUFF_BOOL_IS_TRUE(end_of_destination_sub_elem))
               {
                  result        += ret_part;
                  end_of_destination  = BUFF_TRUE;
                  break;
               }
               else if(offset_src < ret_size_sub_elem)
               {
                  result        += ret_part;

                  if(ret_part < size)
                  {
                     offset_dest+= ret_part;
                     offset_src += ret_part;
                     size       -= ret_part;
                  }
                  else
                  {
                     break;
                  }
               }

               offset_src       -= ret_size_sub_elem;
            }
         }
         else if(BUFF_LIKELY(BUFF_CHECK_PTR(const Buff_Readable_Vector_XT, tree_elem->elem_type.vector.vector)))
         {
            for(cntr = 0; cntr < tree_elem->elem_type.vector.vector_num_elems; cntr++)
            {
               vector_elem = &(tree_elem->elem_type.vector.vector[cntr]);

               if(BUFF_LIKELY(BUFF_CHECK_PTR(const void, vector_elem->data)))
               {
                  if(offset_src < vector_elem->size)
                  {
                     if((vector_elem->size - offset_src) < size)
                     {
                        part        = vector_elem->size - offset_src;
                        ret_part    = Buff_Copy_To_Tree(
                           dest,
                           dest_num_elems,
                           vector_elem->data,
                           part,
                           offset_dest,
                           offset_src,
                           BUFF_MAKE_INVALID_PTR(Buff_Size_DT));
                        offset_dest+= ret_part;
                        offset_src += ret_part;
                        result     += ret_part;
                        size       -= ret_part;

                        /* less data was copied to destination than requested */
                        if(ret_part < part)
                        {
                           size                 = 0;
                           end_of_destination   = BUFF_TRUE;
                           break;
                        }
                     }
                     else
                     {
                        ret_part       = Buff_Copy_To_Tree(
                           dest,
                           dest_num_elems,
                           vector_elem->data,
                           size,
                           offset_dest,
                           offset_src,
                           BUFF_MAKE_INVALID_PTR(Buff_Size_DT));

                        ret_size_elem += ret_part + offset_src;

                        result        += ret_part;
                        size           = 0;
                        break;
                     }
                  }

                  offset_src          -= vector_elem->size;
                  ret_size_elem       += vector_elem->size;
               }
            }

            if(0 == size)
            {
               break;
            }
         }
      }

      if(BUFF_CHECK_PTR(Buff_Bool_DT, end_of_destination_reached))
      {
         *end_of_destination_reached = end_of_destination;
      }

      if(BUFF_CHECK_PTR(Buff_Size_DT, str_size))
      {
         *str_size = ret_size_elem;
      }
   }

   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Copy_Tree_To_Tree */
#endif
#endif /* #if(BUFF_USE_TREE_BUFFERS) */



#if(BUFF_USE_RING_BUFFERS)

#if( (!defined(RING_BUF_INIT_EXTERNAL)) && BUFF_RING_INIT_ENABLED )
Buff_Bool_DT Buff_Ring_Init(Buff_Ring_XT *buf, void *memory, Buff_Size_DT size)
{
   Buff_Bool_DT result = BUFF_FALSE;

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
      if((size > 0) && BUFF_CHECK_PTR(void, memory))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         buf->extension  = BUFF_MAKE_INVALID_PTR(Buff_Ring_Extensions_XT);
#endif
#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         buf->protected_extension  = BUFF_MAKE_INVALID_PTR(Buff_Ring_Extensions_XT);
#endif
         buf->memory          = memory;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
         buf->original_size   = size;
#endif
         buf->size            = size;
         buf->busy_size       = 0;
#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
         buf->max_busy_size   = 0;
#endif
         buf->first_free      = 0;
         buf->first_busy      = 0;

#if(BUFF_RING_USE_PROTECTION)
         if(BUFF_PROTECTION_CREATE(buf))
         {
            if(BUFF_PROTECTION_INIT(buf))
            {
               result = BUFF_TRUE;
            }
         }
#else
         result = BUFF_TRUE;
#endif
      }

      if(!result)
      {
#if(BUFF_RING_USE_EXTENSIONS)
         buf->extension  = BUFF_MAKE_INVALID_PTR(Buff_Ring_Extensions_XT);
#endif
#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         buf->protected_extension  = BUFF_MAKE_INVALID_PTR(Buff_Ring_Extensions_XT);
#endif
         buf->memory          = BUFF_MAKE_INVALID_PTR(void);
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
         buf->original_size   = 0;
#endif
         buf->size            = 0;
         buf->busy_size       = 0;
#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
         buf->max_busy_size   = 0;
#endif
         buf->first_free      = 0;
         buf->first_busy      = 0;
      }
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Init */
#endif



#if( (!defined(BUFF_RING_DEINIT_EXTERNAL)) && BUFF_RING_DEINIT_ENABLED )
void   Buff_Ring_Deinit(Buff_Ring_XT *buf)
{
   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      BUFF_PROTECTION_LOCK(buf);
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      buf->extension  = BUFF_MAKE_INVALID_PTR(Buff_Ring_Extensions_XT);
#endif
#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
      buf->protected_extension  = BUFF_MAKE_INVALID_PTR(Buff_Ring_Extensions_XT);
#endif
      buf->memory          = BUFF_MAKE_INVALID_PTR(void);
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
      buf->original_size   = 0;
#endif
      buf->size            = 0;
      buf->busy_size       = 0;
#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
      buf->max_busy_size   = 0;
#endif
      buf->first_free      = 0;
      buf->first_busy      = 0;

#if(BUFF_RING_USE_PROTECTION)
      BUFF_PROTECTION_UNLOCK(buf);

      BUFF_PROTECTION_DEINIT(buf);
      BUFF_PROTECTION_DESTROY(buf);
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();
} /* Buff_Ring_Deinit */
#endif



#if( (!defined(BUFF_RING_INIT_EXTENSION_EXTERNAL)) && BUFF_RING_INIT_EXTENSION_ENABLED && BUFF_RING_USE_EXTENSIONS )
void Buff_Ring_Init_Extension(Buff_Ring_Extensions_XT *extension)
{
   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension)))
   {
      extension->on_read_params        = BUFF_MAKE_INVALID_PTR(void);
      extension->on_write_params       = BUFF_MAKE_INVALID_PTR(void);
      extension->on_overwrite_params   = BUFF_MAKE_INVALID_PTR(void);
      extension->on_remove_params      = BUFF_MAKE_INVALID_PTR(void);
      extension->on_read               = BUFF_MAKE_INVALID_HANDLER(Buff_Ring_Extension_On_Read);
      extension->on_write              = BUFF_MAKE_INVALID_HANDLER(Buff_Ring_Extension_On_Write);
      extension->on_overwrite          = BUFF_MAKE_INVALID_HANDLER(Buff_Ring_Extension_On_Overwrite);
      extension->on_remove             = BUFF_MAKE_INVALID_HANDLER(Buff_Ring_Extension_On_Remove);
   }
} /* Buff_Ring_Init_Extension */
#endif



#if( (!defined(BUFF_RING_ADD_EXTENSION_EXTERNAL)) && BUFF_RING_ADD_EXTENSION_ENABLED && BUFF_RING_USE_EXTENSIONS )
Buff_Bool_DT Buff_Ring_Add_Extension(Buff_Ring_XT *buf, Buff_Ring_Extensions_XT *extension)
{
   Buff_Bool_DT result = BUFF_FALSE;

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
      buf->extension = extension;

      result = BUFF_TRUE;
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Add_Extension */
#endif



#if( (!defined(BUFF_RING_ADD_PROTECTED_EXTENSION_EXTERNAL)) && BUFF_RING_ADD_PROTECTED_EXTENSION_ENABLED && BUFF_RING_USE_PROTECTED_EXTENSIONS )
Buff_Bool_DT Buff_Ring_Add_Protected_Extension(Buff_Ring_XT *buf, Buff_Ring_Extensions_XT *protected_extension)
{
   Buff_Bool_DT result = BUFF_FALSE;

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
      buf->protected_extension = protected_extension;

      result = BUFF_TRUE;
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Add_Protected_Extension */
#endif



#if( (!defined(BUFF_PROTECTION_LOCK_EXTERNAL)) && BUFF_PROTECTION_LOCK_ENABLED && BUFF_RING_USE_PROTECTION )
void Buff_Protection_Lock(Buff_Ring_XT *buf)
{
   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
      BUFF_PROTECTION_LOCK(buf);
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();
} /* Buff_Lock_Protection */
#endif



#if( (!defined(BUFF_PROTECTION_UNLOCK_EXTERNAL)) && BUFF_PROTECTION_UNLOCK_ENABLED && BUFF_RING_USE_PROTECTION )
void Buff_Protection_Unlock(Buff_Ring_XT *buf)
{
   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
      BUFF_PROTECTION_UNLOCK(buf);
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();
} /* Buff_Unlock_Protection */
#endif



#if( (!defined(RING_BUF_IS_EMPTY_EXTERNAL)) && BUFF_RING_IS_EMPTY_ENABLED )
Buff_Bool_DT Buff_Ring_Is_Empty(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
   Buff_Bool_DT result = BUFF_TRUE;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      result = BUFF_RING_IS_EMPTY(buf);

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Is_Empty */
#endif



#if( (!defined(RING_BUF_IS_FULL_EXTERNAL)) && BUFF_RING_IS_FULL_ENABLED )
Buff_Bool_DT Buff_Ring_Is_Full(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
   Buff_Bool_DT result = BUFF_TRUE;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      result = BUFF_RING_IS_FULL(buf);

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Is_Full */
#endif



#if( (!defined(RING_BUF_GET_SIZE_EXTERNAL)) && BUFF_RING_GET_SIZE_ENABLED )
Buff_Size_DT Buff_Ring_Get_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
   Buff_Size_DT result = 0;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      result = BUFF_RING_GET_SIZE(buf);

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Get_Size */
#endif



#if( (!defined(RING_BUF_GET_BUSY_SIZE_EXTERNAL)) && BUFF_RING_GET_BUSY_SIZE_ENABLED )
Buff_Size_DT Buff_Ring_Get_Busy_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
   Buff_Size_DT result = 0;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      result = BUFF_RING_GET_BUSY_SIZE(buf);

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Get_Busy_Size */
#endif



#if( (!defined(RING_BUF_GET_BUSY_SIZE_EXTERNAL)) && BUFF_RING_GET_BUSY_SIZE_ENABLED && BUFF_RING_USE_BUSY_SIZE_MONITORING )
Buff_Size_DT Buff_Ring_Get_Max_Busy_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
   Buff_Size_DT result = 0;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      result = BUFF_RING_GET_MAX_BUSY_SIZE(buf);

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Get_Max_Busy_Size */
#endif



#if( (!defined(RING_BUF_GET_FREE_SIZE_EXTERNAL)) && BUFF_RING_GET_FREE_SIZE_ENABLED )
Buff_Size_DT Buff_Ring_Get_Free_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
   Buff_Size_DT result = 0;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      result = BUFF_RING_GET_FREE_SIZE(buf);

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Get_Free_Size */
#endif



#if( (!defined(RING_BUF_GET_CONTINOUS_FREE_SIZE_EXTERNAL)) && BUFF_RING_GET_CONTINOUS_FREE_SIZE_ENABLED )
Buff_Size_DT Buff_Ring_Get_Continous_Free_Size(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
   Buff_Size_DT result = 0;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      if(BUFF_RING_IS_FREE_SPACE_CONTINOUS(buf))
      {
         result = BUFF_RING_GET_FREE_SIZE(buf);
      }
      else
      {
         result = buf->size - buf->first_free;
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Get_Continous_Free_Size */
#endif



#if( (!defined(BUFF_RING_GET_CURRENT_POS_EXTERNAL)) && BUFF_RING_GET_CURRENT_POS_ENABLED )
Buff_Size_DT Buff_Ring_Get_Current_Pos(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
   Buff_Size_DT result = 0;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      result = BUFF_RING_GET_CURRENT_POS(buf);

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
}/* Buff_Ring_Get_Current_Pos */
#endif



#if( (!defined(BUFF_RING_DATA_CHECK_OUT_EXTERNAL)) && BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED )
void *Buff_Ring_Data_Check_Out(
   Buff_Ring_XT  *buf,
   Buff_Size_DT   size_needed,
   Buff_Bool_DT   overwrite_if_no_free_space,
   Buff_Bool_DT   use_protection)
{
   void          *result = BUFF_MAKE_INVALID_PTR(void);
   Buff_Bool_DT   resize_occured = BUFF_FALSE;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf) && (size_needed <= buf->original_size)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      /* free space is not continous - part of free space is at the end of the buffer, part on the begining */
      if(!BUFF_RING_IS_FREE_SPACE_CONTINOUS(buf))
      {
         /* part of free space at the end of the buffer is too small to contain requested size */
         if(size_needed > (buf->size - buf->first_free))
         {
            buf->size         = buf->first_free;
            buf->first_free   = 0;
            resize_occured    = BUFF_TRUE;
         }
      }

#if(BUFF_RING_ALLOW_OLD_DATA_OVERWRITE)
      if((size_needed > BUFF_RING_GET_FREE_SIZE(buf)) && BUFF_BOOL_IS_TRUE(overwrite_if_no_free_space))
      {
         Buff_Ring_Remove(buf, size_needed - BUFF_RING_GET_FREE_SIZE(buf), BUFF_FALSE);
      }
#endif

      /* return pointer if there is enough free space */
      if(size_needed <= BUFF_RING_GET_FREE_SIZE(buf))
      {
         result = (void*)(&((uint8_t*)(buf->memory))[BUFF_RING_GET_CURRENT_POS(buf)]);
      }
      /* there is not enough space so invalid pointer is returned. anyway, if resize occured restore original size. */
      else if(BUFF_BOOL_IS_TRUE(resize_occured))
      {
         buf->first_free   = buf->size;
         buf->size         = buf->original_size;
      }
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Data_Check_Out */
#endif



#if( (!defined(BUFF_RING_DATA_CHECK_IN_EXTERNAL)) && BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED )
Buff_Size_DT Buff_Ring_Data_Check_In(Buff_Ring_XT *buf, Buff_Size_DT size, Buff_Bool_DT use_protection)
{
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Write on_write;
#endif
   Buff_Size_DT result = (Buff_Size_DT)(-1);
   Buff_Size_DT part_at_the_end_size = 0;
   Buff_Size_DT cntr;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
      if(BUFF_LIKELY((size <= BUFF_RING_GET_FREE_SIZE(buf)) && (size > 0)))
      {
         buf->busy_size += size;

#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
         if(buf->busy_size >= buf->max_busy_size)
         {
            buf->max_busy_size = buf->busy_size;
         }
#endif

         /* do buffer de-fragmentation */
         if(BUFF_UNLIKELY((0 == buf->first_free) && (buf->size != buf->original_size)))
         {
            part_at_the_end_size = (size <= (buf->original_size - buf->size)) ? size : buf->original_size - buf->size;

            memcpy(&(((uint8_t*)(buf->memory))[buf->size]), buf->memory, part_at_the_end_size);

            size -= part_at_the_end_size;

            for(cntr = 0; cntr < size; cntr++)
            {
               ((uint8_t*)(buf->memory))[cntr] = ((uint8_t*)(buf->memory))[cntr + part_at_the_end_size];
            }

            result          = buf->size;
            buf->first_free = buf->size + part_at_the_end_size;
            buf->size       = buf->original_size;

#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            if(size > 0)
            {
               rewind_occured = BUFF_TRUE;
            }
#endif
         }
         else
         {
            result = buf->first_free;
         }
         buf->first_free += size;

         if(BUFF_UNLIKELY(buf->first_free >= buf->size))
         {
            buf->first_free -= buf->size;
         }

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_write = extension->on_write;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
            {
               on_write(buf, extension, part_at_the_end_size + size, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && ((Buff_Size_DT)(-1) != result))
      {
         on_write = extension->on_write;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
         {
            on_write(buf, extension, part_at_the_end_size + size, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Data_Check_In */
#endif



#if( (!defined(RING_BUF_WRITE_EXTERNAL)) && BUFF_RING_WRITE_ENABLED )
Buff_Size_DT Buff_Ring_Write(
   Buff_Ring_XT  *buf,
   const void    *data,
   Buff_Size_DT   size,
   Buff_Bool_DT   overwrite_if_no_free_space,
   Buff_Bool_DT   use_protection)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VENDOR_BUFFERS)

   return Buff_Ring_Write_Vendor(buf, data, size, buff_vendor_memcpy, overwrite_if_no_free_space, use_protection);

#elif(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Readable_Tree_XT tree;
   Buff_Readable_Vector_XT vector;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = &vector;
   tree.elem_type.vector.vector_num_elems = 1;

   vector.data = data;
   vector.size = size;

   return Buff_Ring_Write_From_Tree(
      buf, &tree, 1, size, 0, overwrite_if_no_free_space, use_protection);

#elif(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VECTOR_BUFFERS)

   Buff_Readable_Vector_XT vector;

   vector.data = data;
   vector.size = size;

   return Buff_Ring_Write_From_Vector(
      buf, &vector, 1, size, 0, overwrite_if_no_free_space, use_protection);

#else

#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Write on_write;
#endif
   Buff_Size_DT part_size;
   Buff_Size_DT count = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(void, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = BUFF_RING_GET_FREE_SIZE(buf);

      if(BUFF_LIKELY(size <= count))
      {
         count = size;
      }
#if(BUFF_RING_ALLOW_OLD_DATA_OVERWRITE)
      else if(BUFF_BOOL_IS_TRUE(overwrite_if_no_free_space))
      {
         if(BUFF_LIKELY(size < buf->size))
         {
            Buff_Ring_Remove(buf, size - count, BUFF_FALSE);

            count = size;
         }
         else
         {
            Buff_Ring_Clear(buf, BUFF_FALSE);

            count = buf->size;
         }
      }
#endif

      if(BUFF_LIKELY(count > 0))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         data_changed = BUFF_TRUE;
#endif
         if(BUFF_LIKELY(buf->first_free <= (buf->size - count)))
         {
            memcpy( &((uint8_t*)(buf->memory))[buf->first_free], data, count);

            buf->busy_size      += count;
            buf->first_free     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_FALSE;
#endif

            if(BUFF_UNLIKELY(buf->first_free >= buf->size))
            {
               buf->first_free   = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured    = BUFF_TRUE;
#endif
            }
         }
         else
         {
            part_size            = buf->size - buf->first_free;
            memcpy( &((uint8_t*)(buf->memory))[buf->first_free], data, part_size);
            data                 = &((uint8_t*)data)[part_size];
            part_size            = count - part_size;
            memcpy(buf->memory, data, part_size);

            buf->busy_size      += count;
            buf->first_free      = part_size;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_TRUE;
#endif
         }

#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
         if(buf->busy_size >= buf->max_busy_size)
         {
            buf->max_busy_size = buf->busy_size;
         }
#endif

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_write = extension->on_write;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
            {
               on_write(buf, extension, count, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_write = extension->on_write;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
         {
            on_write(buf, extension, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;

#endif
} /* Buff_Ring_Write */
#endif



#if( (!defined(BUFF_RING_WRITE_VENDOR_EXTERNAL)) && BUFF_RING_WRITE_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
Buff_Size_DT Buff_Ring_Write_Vendor(
   Buff_Ring_XT           *buf,
   const void             *data,
   Buff_Size_DT            size,
   Buff_Vendor_Memcpy_HT   vendor_cpy,
   Buff_Bool_DT            overwrite_if_no_free_space,
   Buff_Bool_DT            use_protection)
{
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Write on_write;
#endif
   Buff_Memcpy_Params_XT params;
   Buff_Size_DT part_size;
   Buff_Size_DT count = 0;
   Buff_Size_DT ret_count;
   Buff_Size_DT ret_count2;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(void, data)
      && BUFF_CHECK_HANDLER(Buff_Vendor_Memcpy_HT, vendor_cpy)
      && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = BUFF_RING_GET_FREE_SIZE(buf);

      if(BUFF_LIKELY(size <= count))
      {
         count = size;
      }
#if(BUFF_RING_ALLOW_OLD_DATA_OVERWRITE)
      else if(BUFF_BOOL_IS_TRUE(overwrite_if_no_free_space))
      {
         if(BUFF_LIKELY(size < buf->size))
         {
            Buff_Ring_Remove(buf, size - count, BUFF_FALSE);

            count = size;
         }
         else
         {
            Buff_Ring_Clear(buf, BUFF_FALSE);

            count = buf->size;
         }
      }
#endif

      if(BUFF_LIKELY(count > 0))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         data_changed = BUFF_TRUE;
#endif
         params.dest             = buf->memory;
         params.src              = data;
         params.dest_offset      = buf->first_free;
         params.src_offset       = 0;

         if(BUFF_LIKELY(buf->first_free <= (buf->size - count)))
         {
            params.size          = count;
            params.is_last_part  = BUFF_TRUE;

            ret_count = vendor_cpy(&params);

            /* vendor_cpy might copy less data than requested - depends on vendor_cpy - in this case reduce count */
            if(BUFF_LIKELY(ret_count <= count))
            {
               count = ret_count;
            }

            buf->busy_size      += count;
            buf->first_free     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_FALSE;
#endif

            if(BUFF_UNLIKELY(buf->first_free >= buf->size))
            {
               buf->first_free   = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured    = BUFF_TRUE;
#endif
            }
         }
         else
         {
            part_size            = buf->size - buf->first_free;

            params.size          = part_size;
            params.is_last_part  = BUFF_FALSE;

            ret_count = vendor_cpy(&params);

            /* if vendor_cpy copied less data than requested then don't process second buff part */
            if(ret_count < part_size)
            {
               count                = ret_count;
               buf->busy_size      += ret_count;
               buf->first_free     += ret_count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured       = BUFF_FALSE;
#endif
            }
            else
            {
               ret_count            = part_size;
               params.dest          = buf->memory;
               params.src           = data;
               params.dest_offset   = 0;
               params.src_offset    = part_size;
               part_size            = count - part_size;
               params.size          = part_size;
               params.is_last_part  = BUFF_TRUE;

               buf->busy_size      += ret_count;
               buf->first_free      = 0;

               ret_count2 = vendor_cpy(&params);

               /* vendor_cpy might copy in second step less data than requested - in this case reduce count and part_size */
               if(BUFF_LIKELY(ret_count2 <= part_size))
               {
                  count     = ret_count + ret_count2;
                  part_size = ret_count2;
               }

               buf->busy_size      += part_size;
               buf->first_free      = part_size;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured       = BUFF_TRUE;
#endif
            }
         }

#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
         if(buf->busy_size >= buf->max_busy_size)
         {
            buf->max_busy_size = buf->busy_size;
         }
#endif

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_write = extension->on_write;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
            {
               on_write(buf, extension, count, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_write = extension->on_write;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
         {
            on_write(buf, extension, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;
} /* Buff_Ring_Write_Vendor */
#endif



#if( (!defined(BUFF_RING_WRITE_FROM_VECTOR_EXTERNAL)) && BUFF_RING_WRITE_FROM_VECTOR_ENABLED && BUFF_USE_VECTOR_BUFFERS )
Buff_Size_DT Buff_Ring_Write_From_Vector(
   Buff_Ring_XT                    *buf,
   const Buff_Readable_Vector_XT   *data,
   Buff_Num_Elems_DT                data_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_data,
   Buff_Bool_DT                     overwrite_if_no_free_space,
   Buff_Bool_DT                     use_protection)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Readable_Tree_XT tree;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = data;
   tree.elem_type.vector.vector_num_elems = data_num_elems;

   return Buff_Ring_Write_From_Tree(buf, &tree, 1, size, offset_data, overwrite_if_no_free_space, use_protection);

#else

#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Write on_write;
#endif
   Buff_Writeable_Vector_XT vector[2];
   Buff_Size_DT      count = 0;
   Buff_Num_Elems_DT vector_num_elems;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT      rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT      data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Readable_Vector_XT, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = BUFF_RING_GET_FREE_SIZE(buf);

      if(BUFF_LIKELY(size <= count))
      {
         count = size;
      }
#if(BUFF_RING_ALLOW_OLD_DATA_OVERWRITE)
      else if(BUFF_BOOL_IS_TRUE(overwrite_if_no_free_space))
      {
         if(BUFF_LIKELY(size < buf->size))
         {
            Buff_Ring_Remove(buf, size - count, BUFF_FALSE);

            count = size;
         }
         else
         {
            Buff_Ring_Clear(buf, BUFF_FALSE);

            count = buf->size;
         }
      }
#endif

      if(BUFF_LIKELY(count > 0))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         data_changed = BUFF_TRUE;
#endif
         if(buf->first_free >= buf->first_busy)
         {
            if(BUFF_LIKELY(buf->first_busy > 0))
            {
               vector_num_elems  = 2;
               vector[1].data    = buf->memory;
               vector[1].size    = buf->first_busy;
            }
            else
            {
               vector_num_elems  = 1;
            }
            vector[0].size       = buf->size - buf->first_free;
         }
         else
         {
            vector_num_elems     = 1;
            vector[0].size       = buf->first_busy - buf->first_free;
         }
         vector[0].data          = &((uint8_t*)(buf->memory))[buf->first_free];

         count = Buff_Copy_Vector_To_Vector(vector, vector_num_elems, data, data_num_elems, count, 0, offset_data);

         if(BUFF_LIKELY(buf->first_free <= (buf->size - count)))
         {
            buf->busy_size      += count;
            buf->first_free     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_FALSE;
#endif

            if(BUFF_UNLIKELY(buf->first_free >= buf->size))
            {
               buf->first_free   = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured    = BUFF_TRUE;
#endif
            }
         }
         else
         {
            buf->busy_size      += count;
            buf->first_free      = buf->first_free + count - buf->size;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_TRUE;
#endif
         }

#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
         if(buf->busy_size >= buf->max_busy_size)
         {
            buf->max_busy_size = buf->busy_size;
         }
#endif

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_write = extension->on_write;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
            {
               on_write(buf, extension, count, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_write = extension->on_write;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
         {
            on_write(buf, extension, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;

#endif
} /* Buff_Ring_Write_From_Vector */
#endif



#if( (!defined(BUFF_RING_WRITE_FROM_TREE_EXTERNAL)) && BUFF_RING_WRITE_FROM_TREE_ENABLED && BUFF_USE_TREE_BUFFERS )
Buff_Size_DT Buff_Ring_Write_From_Tree(
   Buff_Ring_XT                 *buf,
   const Buff_Readable_Tree_XT  *data,
   Buff_Num_Elems_DT             data_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_data,
   Buff_Bool_DT                  overwrite_if_no_free_space,
   Buff_Bool_DT                  use_protection)
{
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Write on_write;
#endif
   Buff_Writeable_Vector_XT vector[2];
   Buff_Writeable_Tree_XT tree;
   Buff_Size_DT count = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Readable_Tree_XT, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = BUFF_RING_GET_FREE_SIZE(buf);

      if(BUFF_LIKELY(size <= count))
      {
         count = size;
      }
#if(BUFF_RING_ALLOW_OLD_DATA_OVERWRITE)
      else if(BUFF_BOOL_IS_TRUE(overwrite_if_no_free_space))
      {
         if(BUFF_LIKELY(size < buf->size))
         {
            Buff_Ring_Remove(buf, size - count, BUFF_FALSE);

            count = size;
         }
         else
         {
            Buff_Ring_Clear(buf, BUFF_FALSE);

            count = buf->size;
         }
      }
#endif

      if(BUFF_LIKELY(count > 0))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         data_changed = BUFF_TRUE;
#endif
         tree.elem_type.vector.vector  = vector;
         tree.is_branch                = BUFF_FALSE;

         if(buf->first_free >= buf->first_busy)
         {
            if(BUFF_LIKELY(buf->first_busy > 0))
            {
               vector[1].data                         = buf->memory;
               vector[1].size                         = buf->first_busy;
               tree.elem_type.vector.vector_num_elems = 2;
            }
            else
            {
               tree.elem_type.vector.vector_num_elems = 1;
            }
            vector[0].size                            = buf->size - buf->first_free;
         }
         else
         {
            tree.elem_type.vector.vector_num_elems    = 1;
            vector[0].size                            = buf->first_busy - buf->first_free;
         }
         vector[0].data                               = &((uint8_t*)(buf->memory))[buf->first_free];

         count = Buff_Copy_Tree_To_Tree(
            &tree,
            1,
            data,
            data_num_elems,
            count,
            0,
            offset_data,
            BUFF_MAKE_INVALID_PTR(Buff_Size_DT),
            BUFF_MAKE_INVALID_PTR(Buff_Bool_DT));

         if(BUFF_LIKELY(buf->first_free <= (buf->size - count)))
         {
            buf->busy_size      += count;
            buf->first_free     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_FALSE;
#endif

            if(BUFF_UNLIKELY(buf->first_free >= buf->size))
            {
               buf->first_free   = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured    = BUFF_TRUE;
#endif
            }
         }
         else
         {
            buf->busy_size      += count;
            buf->first_free      = buf->first_free + count - buf->size;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_TRUE;
#endif
         }

#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
         if(buf->busy_size >= buf->max_busy_size)
         {
            buf->max_busy_size = buf->busy_size;
         }
#endif

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_write = extension->on_write;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
            {
               on_write(buf, extension, count, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_write = extension->on_write;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
         {
            on_write(buf, extension, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;
} /* Buff_Ring_Write_From_Tree */
#endif



#if( (!defined(RING_BUF_OVERWRITE_IF_EXIST_EXTERNAL)) && BUFF_RING_OVERWRITE_IF_EXIST_ENABLED )
Buff_Size_DT Buff_Ring_OverWrite_If_Exist(
   Buff_Ring_XT  *buf,
   const void    *data,
   Buff_Size_DT   size,
   Buff_Size_DT   pos,
   Buff_Bool_DT   use_protection)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VENDOR_BUFFERS)

   return Buff_Ring_OverWrite_If_Exist_Vendor(buf, data, size, pos, buff_vendor_memcpy, use_protection);

#else

#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Overwrite on_overwrite;
#endif
   Buff_Size_DT part_size;
   Buff_Size_DT count = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(void, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = buf->busy_size;

      if(BUFF_LIKELY((size <= count) && (pos < buf->size)))
      {
         count = size;

         if(BUFF_LIKELY(count > 0))
         {
#if(BUFF_RING_USE_EXTENSIONS)
            data_changed = BUFF_TRUE;
#endif
            /* check if busy area is continous (doesn't start at end of memory and ends on begining) */
            if(buf->first_free > buf->first_busy)
            {
               /* check if whole memory requested to be modified is on busy area */
               if(BUFF_LIKELY((pos >= buf->first_busy) && (pos <= (buf->first_free - size))))
               {
                  memcpy( &((uint8_t*)(buf->memory))[pos], data, count);
               }
               /* memory requested to be modified is not fully inside busy area */
               else
               {
                  count = 0;
               }
            }
            else
            {
               /* check if begining of memory requested to be modified is on the begining of busy area (end of memory) */
               if(pos >= buf->first_busy)
               {
                  /* check if whole memory requested to be modified is on the begining of busy area (end of memory) */
                  if(pos <= (buf->size - size))
                  {
                     memcpy( &((uint8_t*)(buf->memory))[pos], data, count);
                  }
                  /*
                  * check if memory requested to be modified is partially on the begining of busy area (end of memory)
                  * and partially on the end of busy area (begining of memory)
                  */
                  else if((pos + size - buf->size) <= buf->first_free)
                  {
                     part_size      = buf->size - pos;
                     memcpy( &((uint8_t*)(buf->memory))[pos], data, part_size);
                     data           = &((uint8_t*)data)[part_size];
                     part_size      = count - part_size;
                     memcpy(buf->memory, data, part_size);
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
                     rewind_occured = BUFF_TRUE;
#endif
                  }
                  /* memory requested to be modified is not fully inside busy area */
                  else
                  {
                     count = 0;
                  }
               }
               /* check if whole memory requested to be modified is on the end of busy area (begining of memory) */
               else if(pos < buf->first_free)
               {
                  if(size <= (buf->first_free - pos))
                  {
                     memcpy( &((uint8_t*)(buf->memory))[pos], data, count);
                  }
                  /* memory requested to be modified is not fully inside busy area */
                  else
                  {
                     count = 0;
                  }
               }
               /* memory requested to be modified starts outside busy area */
               else
               {
                  count = 0;
               }
            }

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
            extension = buf->protected_extension;

            if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
            {
               on_overwrite = extension->on_overwrite;

               if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Overwrite, on_overwrite))
               {
                  on_overwrite(buf, extension, pos, count, rewind_occured);
               }
            }
#endif
         }
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_overwrite = extension->on_overwrite;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Overwrite, on_overwrite))
         {
            on_overwrite(buf, extension, pos, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;

#endif
} /* Buff_Ring_OverWrite_If_Exist */
#endif



#if( (!defined(RING_BUF_OVERWRITE_IF_EXIST_VENDOR_EXTERNAL)) && BUFF_RING_OVERWRITE_IF_EXIST_VENDOR_ENABLED )
Buff_Size_DT Buff_Ring_OverWrite_If_Exist_Vendor(
   Buff_Ring_XT           *buf,
   const void             *data,
   Buff_Size_DT            size,
   Buff_Size_DT            pos,
   Buff_Vendor_Memcpy_HT   vendor_cpy,
   Buff_Bool_DT            use_protection)
{
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Overwrite on_overwrite;
#endif
   Buff_Memcpy_Params_XT params;
   Buff_Size_DT part_size;
   Buff_Size_DT count = 0;
   Buff_Size_DT ret_count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(void, data)
      && BUFF_CHECK_HANDLER(Buff_Vendor_Memcpy_HT, vendor_cpy)
      && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = buf->busy_size;

      if(BUFF_LIKELY((size <= count) && (pos < buf->size)))
      {
         count = size;

         if(BUFF_LIKELY(count > 0))
         {
#if(BUFF_RING_USE_EXTENSIONS)
            data_changed = BUFF_TRUE;
#endif
            params.dest          = buf->memory;
            params.src           = data;
            params.dest_offset   = pos;
            params.src_offset    = 0;

            /* check if busy area is continous (doesn't start at end of memory and ends on begining) */
            if(buf->first_free > buf->first_busy)
            {
               /* check if whole memory requested to be modified is on busy area */
               if(BUFF_LIKELY((pos >= buf->first_busy) && (pos <= (buf->first_free - size))))
               {
                  params.size          = count;
                  params.is_last_part  = BUFF_TRUE;

                  ret_count = vendor_cpy(&params);

                  if(BUFF_LIKELY(ret_count <= count))
                  {
                     count = ret_count;
                  }
               }
               /* memory requested to be modified is not fully inside busy area */
               else
               {
                  count = 0;
               }
            }
            else
            {
               /* check if begining of memory requested to be modified is on the begining of busy area (end of memory) */
               if(pos >= buf->first_busy)
               {
                  /* check if whole memory requested to be modified is on the begining of busy area (end of memory) */
                  if(pos <= (buf->size - size))
                  {
                     params.size          = count;
                     params.is_last_part  = BUFF_TRUE;

                     ret_count = vendor_cpy(&params);

                     if(BUFF_LIKELY(ret_count <= count))
                     {
                        count = ret_count;
                     }
                  }
                  /*
                  * check if memory requested to be modified is partially on the begining of busy area (end of memory)
                  * and partially on the end of busy area (begining of memory)
                  */
                  else if((pos + size - buf->size) <= buf->first_free)
                  {
                     part_size            = buf->size - pos;

                     params.size          = part_size;
                     params.is_last_part  = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
                     rewind_occured       = BUFF_TRUE;
#endif

                     ret_count = vendor_cpy(&params);

                     /* copy second part only if size returned in first copy is same as requested */
                     if(BUFF_LIKELY(part_size <= ret_count))
                     {
                        ret_count            = part_size;
                        params.dest          = buf->memory;
                        params.src           = data;
                        params.dest_offset   = 0;
                        params.src_offset    = part_size;
                        part_size            = count - part_size;
                        params.size          = part_size;
                        params.is_last_part  = BUFF_TRUE;

                        ret_count += vendor_cpy(&params);

                        if(BUFF_LIKELY(ret_count <= count))
                        {
                           count = ret_count;
                        }
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
                        rewind_occured       = BUFF_TRUE;
#endif
                     }
                     else
                     {
                        count = ret_count;
                     }
                  }
                  /* memory requested to be modified is not fully inside busy area */
                  else
                  {
                     count = 0;
                  }
               }
               /* check if whole memory requested to be modified is on the end of busy area (begining of memory) */
               else if(pos < buf->first_free)
               {
                  if(size <= (buf->first_free - pos))
                  {
                     params.size          = count;
                     params.is_last_part  = BUFF_TRUE;

                     ret_count = vendor_cpy(&params);

                     if(BUFF_LIKELY(ret_count <= count))
                     {
                        count = ret_count;
                     }
                  }
                  /* memory requested to be modified is not fully inside busy area */
                  else
                  {
                     count = 0;
                  }
               }
               /* memory requested to be modified is outside busy area */
               else
               {
                  count = 0;
               }
            }

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
            extension = buf->protected_extension;

            if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
            {
               on_overwrite = extension->on_overwrite;

               if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Overwrite, on_overwrite))
               {
                  on_overwrite(buf, extension, pos, count, rewind_occured);
               }
            }
#endif
         }
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_overwrite = extension->on_overwrite;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Overwrite, on_overwrite))
         {
            on_overwrite(buf, extension, pos, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;
} /* Buff_Ring_OverWrite_If_Exist_Vendor */
#endif



#if( (!defined(RING_BUF_PEAK_EXTERNAL)) && BUFF_RING_PEAK_ENABLED )
Buff_Size_DT Buff_Ring_Peak(
   Buff_Ring_XT  *buf,
   void          *data,
   Buff_Size_DT   size,
   Buff_Size_DT   buf_offset,
   Buff_Bool_DT   use_protection)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VENDOR_BUFFERS)

   return Buff_Ring_Peak_Vendor(buf, data, size, buf_offset, buff_vendor_memcpy, use_protection);

#elif(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Writeable_Tree_XT tree;
   Buff_Writeable_Vector_XT vector;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = &vector;
   tree.elem_type.vector.vector_num_elems = 1;

   vector.data = data;
   vector.size = size;

   return Buff_Ring_Peak_To_Tree(
      buf, &tree, 1, size, buf_offset, 0, use_protection);

#elif(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VECTOR_BUFFERS)

   Buff_Writeable_Vector_XT vector;

   vector.data = data;
   vector.size = size;

   return Buff_Ring_Peak_To_Vector(
      buf, &vector, 1, size, buf_offset, 0, use_protection);

#else

   Buff_Size_DT read_start;
   Buff_Size_DT part_size;
   Buff_Size_DT count = 0;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(void, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      if(BUFF_LIKELY(buf_offset < buf->busy_size))
      {
         count = buf->busy_size - buf_offset;
      }

      if(BUFF_LIKELY(size <= count))
      {
         count = size;
      }

      if(BUFF_LIKELY(count > 0))
      {
         read_start = buf->first_busy + buf_offset - buf->size;

         if(BUFF_LIKELY(buf->first_busy < (buf->size - buf_offset)))
         {
            read_start += buf->size;
         }

         if(BUFF_LIKELY(read_start <= (buf->size - count)))
         {
            memcpy(data, &((const uint8_t*)(buf->memory))[read_start], count);
         }
         else
         {
            part_size   = buf->size - read_start;
            memcpy(data, &((const uint8_t*)(buf->memory))[read_start], part_size);

            data        = &((uint8_t*)data)[part_size];
            part_size   = count - part_size;
            memcpy(data, buf->memory, part_size);
         }
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;

#endif
} /* Buff_Ring_Peak */
#endif



#if( (!defined(BUFF_RING_PEAK_VENDOR_EXTERNAL)) && BUFF_RING_PEAK_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
Buff_Size_DT Buff_Ring_Peak_Vendor(
   Buff_Ring_XT           *buf,
   void                   *data,
   Buff_Size_DT            size,
   Buff_Size_DT            buf_offset,
   Buff_Vendor_Memcpy_HT   vendor_cpy,
   Buff_Bool_DT            use_protection)
{
   Buff_Memcpy_Params_XT params;
   Buff_Size_DT read_start;
   Buff_Size_DT part_size;
   Buff_Size_DT count = 0;
   Buff_Size_DT ret_count;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(void, data)
      && BUFF_CHECK_HANDLER(Buff_Vendor_Memcpy_HT, vendor_cpy)
      && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      if(BUFF_LIKELY(buf_offset < buf->busy_size))
      {
         count = buf->busy_size - buf_offset;
      }

      if(BUFF_LIKELY(size <= count))
      {
         count = size;
      }

      if(BUFF_LIKELY(count > 0))
      {
         read_start = buf->first_busy + buf_offset - buf->size;

         if(BUFF_LIKELY(buf->first_busy < (buf->size - buf_offset)))
         {
            read_start += buf->size;
         }

         params.dest             = data;
         params.src              = buf->memory;
         params.dest_offset      = 0;
         params.src_offset       = read_start;

         if(BUFF_LIKELY(read_start <= (buf->size - count)))
         {
            params.size          = count;
            params.is_last_part  = BUFF_TRUE;

            ret_count = vendor_cpy(&params);

            if(BUFF_LIKELY(ret_count <= count))
            {
               count = ret_count;
            }
         }
         else
         {
            part_size = buf->size - read_start;

            params.size          = part_size;
            params.is_last_part  = BUFF_FALSE;

            ret_count = vendor_cpy(&params);

            if(BUFF_LIKELY(part_size <= ret_count))
            {
               ret_count            = part_size;
               params.dest          = data;
               params.src           = buf->memory;
               params.dest_offset   = part_size;
               params.src_offset    = 0;
               part_size            = count - part_size;
               params.size          = part_size;
               params.is_last_part  = BUFF_TRUE;

               ret_count += vendor_cpy(&params);

               if(BUFF_LIKELY(ret_count <= count))
               {
                  count = ret_count;
               }
            }
            else
            {
               count = ret_count;
            }
         }
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;
} /* Buff_Ring_Peak_Vendor */
#endif



#if( (!defined(BUFF_RING_PEAK_TO_VECTOR_EXTERNAL)) && BUFF_RING_PEAK_TO_VECTOR_ENABLED && BUFF_USE_VECTOR_BUFFERS )
Buff_Size_DT Buff_Ring_Peak_To_Vector(
   Buff_Ring_XT                    *buf,
   const Buff_Writeable_Vector_XT  *data,
   Buff_Num_Elems_DT                data_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     buf_offset,
   Buff_Size_DT                     offset_data,
   Buff_Bool_DT                     use_protection)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Writeable_Tree_XT tree;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = data;
   tree.elem_type.vector.vector_num_elems = data_num_elems;

   return Buff_Ring_Peak_To_Tree(buf, &tree, 1, size, buf_offset, offset_data, use_protection);

#else

   Buff_Readable_Vector_XT vector[2];
   Buff_Size_DT      read_start;
   Buff_Size_DT      count = 0;
   Buff_Num_Elems_DT vector_num_elems;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Writeable_Vector_XT, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      if(BUFF_LIKELY(buf_offset < buf->busy_size))
      {
         count = buf->busy_size - buf_offset;
      }

      if(BUFF_LIKELY(size <= count))
      {
         count = size;
      }

      if(BUFF_LIKELY(count > 0))
      {
         read_start = buf->first_busy + buf_offset - buf->size;

         if(BUFF_LIKELY(buf->first_busy < (buf->size - buf_offset)))
         {
            read_start += buf->size;
         }

         if(read_start >= buf->first_free)
         {
            if(BUFF_LIKELY(buf->first_free > 0))
            {
               vector[1].data    = buf->memory;
               vector[1].size    = buf->first_free;
               vector_num_elems  = 2;
            }
            else
            {
               vector_num_elems  = 1;
            }
            vector[0].size       = buf->size - read_start;
         }
         else
         {
            vector_num_elems     = 1;
            vector[0].size       = buf->first_free - read_start;
         }
         vector[0].data          = &((const uint8_t*)(buf->memory))[read_start];

         count = Buff_Copy_Vector_To_Vector(data, data_num_elems, vector, vector_num_elems, count, offset_data, 0);
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;

#endif
} /* Buff_Ring_Peak_To_Vector */
#endif



#if( (!defined(BUFF_RING_PEAK_TO_TREE_EXTERNAL)) && BUFF_RING_PEAK_TO_TREE_ENABLED && BUFF_USE_TREE_BUFFERS )
Buff_Size_DT Buff_Ring_Peak_To_Tree(
   Buff_Ring_XT                 *buf,
   const Buff_Writeable_Tree_XT *data,
   Buff_Num_Elems_DT             data_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  buf_offset,
   Buff_Size_DT                  offset_data,
   Buff_Bool_DT                  use_protection)
{
   Buff_Readable_Vector_XT vector[2];
   Buff_Readable_Tree_XT tree;
   Buff_Size_DT read_start;
   Buff_Size_DT count = 0;

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Writeable_Tree_XT, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      if(BUFF_LIKELY(buf_offset < buf->busy_size))
      {
         count = buf->busy_size - buf_offset;
      }

      if(BUFF_LIKELY(size <= count))
      {
         count = size;
      }

      if(BUFF_LIKELY(count > 0))
      {
         read_start = buf->first_busy + buf_offset - buf->size;

         if(BUFF_LIKELY(buf->first_busy < (buf->size - buf_offset)))
         {
            read_start += buf->size;
         }

         tree.elem_type.vector.vector  = vector;
         tree.is_branch = BUFF_FALSE;

         if(read_start >= buf->first_free)
         {
            if(BUFF_LIKELY(buf->first_free > 0))
            {
               vector[1].data                         = buf->memory;
               vector[1].size                         = buf->first_free;
               tree.elem_type.vector.vector_num_elems = 2;
            }
            else
            {
               tree.elem_type.vector.vector_num_elems = 1;
            }
            vector[0].size                            = buf->size - read_start;
         }
         else
         {
            tree.elem_type.vector.vector_num_elems    = 1;
            vector[0].size                            = buf->first_free - read_start;
         }
         vector[0].data                               = &((const uint8_t*)(buf->memory))[read_start];

         count = Buff_Copy_Tree_To_Tree(
            data,
            data_num_elems,
            &tree,
            1,
            count,
            offset_data,
            0,
            BUFF_MAKE_INVALID_PTR(Buff_Size_DT),
            BUFF_MAKE_INVALID_PTR(Buff_Bool_DT));
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;
} /* Buff_Ring_Peak_To_Tree */
#endif



#if( (!defined(RING_BUF_READ_EXTERNAL)) && BUFF_RING_READ_ENABLED )
Buff_Size_DT Buff_Ring_Read(
   Buff_Ring_XT  *buf,
   void          *data,
   Buff_Size_DT   size,
   Buff_Bool_DT   use_protection)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VENDOR_BUFFERS)

   return Buff_Ring_Read_Vendor(buf, data, size, buff_vendor_memcpy, use_protection);

#elif(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Writeable_Tree_XT tree;
   Buff_Writeable_Vector_XT vector;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = &vector;
   tree.elem_type.vector.vector_num_elems = 1;

   vector.data = data;
   vector.size = size;

   return Buff_Ring_Read_To_Tree(
      buf, &tree, 1, size, 0, use_protection);

#elif(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_VECTOR_BUFFERS)

   Buff_Writeable_Vector_XT vector;

   vector.data = data;
   vector.size = size;

   return Buff_Ring_Read_To_Vector(
      buf, &vector, 1, size, 0, use_protection);

#else

#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Read on_read;
#endif
   Buff_Size_DT part_size;
   Buff_Size_DT count = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(void, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = buf->busy_size;

      if(BUFF_LIKELY(size <= buf->busy_size))
      {
         count = size;
      }

      if(BUFF_LIKELY(count > 0))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         data_changed = BUFF_TRUE;
#endif
         if(BUFF_LIKELY(buf->first_busy <= (buf->size - count)))
         {
            memcpy(data, &((const uint8_t*)(buf->memory))[buf->first_busy], count);

            buf->busy_size      -= count;
            buf->first_busy     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_FALSE;
#endif

            if(BUFF_UNLIKELY(buf->first_busy >= buf->size))
            {
               buf->first_busy   = 0;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
               buf->size         = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured    = BUFF_TRUE;
#endif
            }
         }
         else
         {
            part_size            = buf->size - buf->first_busy;
            memcpy(data, &((const uint8_t*)(buf->memory))[buf->first_busy], part_size);
            data                 = &((uint8_t*)data)[part_size];
            part_size            = count - part_size;
            memcpy(data, buf->memory, part_size);

            buf->busy_size      -= count;
            buf->first_busy      = part_size;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
            buf->size            = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_TRUE;
#endif
         }

         if(BUFF_UNLIKELY(0 == buf->busy_size))
         {
            buf->first_busy = 0;
            buf->first_free = 0;
         }

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_read = extension->on_read;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Read, on_read))
            {
               on_read(buf, extension, count, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_read = extension->on_read;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Read, on_read))
         {
            on_read(buf, extension, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;

#endif
} /* Buff_Ring_Read */
#endif



#if( (!defined(BUFF_RING_READ_VENDOR_EXTERNAL)) && BUFF_RING_READ_VENDOR_ENABLED && BUFF_USE_VENDOR_BUFFERS )
Buff_Size_DT Buff_Ring_Read_Vendor(
   Buff_Ring_XT           *buf,
   void                   *data,
   Buff_Size_DT            size,
   Buff_Vendor_Memcpy_HT   vendor_cpy,
   Buff_Bool_DT            use_protection)
{
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Read on_read;
#endif
   Buff_Memcpy_Params_XT params;
   Buff_Size_DT part_size;
   Buff_Size_DT count = 0;
   Buff_Size_DT ret_count;
   Buff_Size_DT ret_count2;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(void, data)
      && BUFF_CHECK_HANDLER(Buff_Vendor_Memcpy_HT, vendor_cpy)
      && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = buf->busy_size;

      if(BUFF_LIKELY(size <= buf->busy_size))
      {
         count = size;
      }

      if(BUFF_LIKELY(count > 0))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         data_changed = BUFF_TRUE;
#endif
         params.dest             = data;
         params.src              = buf->memory;
         params.dest_offset      = 0;
         params.src_offset       = buf->first_busy;

         if(BUFF_LIKELY(buf->first_busy <= (buf->size - count)))
         {
            params.size          = count;
            params.is_last_part  = BUFF_TRUE;

            ret_count = vendor_cpy(&params);

            /* vendor_cpy might copy less data than requested - depends on vendor_cpy - in this case reduce count */
            if(BUFF_LIKELY(ret_count <= count))
            {
               count = ret_count;
            }

            buf->busy_size      -= count;
            buf->first_busy     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_FALSE;
#endif

            if(BUFF_UNLIKELY(buf->first_busy >= buf->size))
            {
               buf->first_busy   = 0;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
               buf->size         = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured    = BUFF_TRUE;
#endif
            }
         }
         else
         {
            part_size            = buf->size - buf->first_busy;

            params.size          = part_size;
            params.is_last_part  = BUFF_FALSE;

            ret_count = vendor_cpy(&params);

            /* if vendor_cpy copied less data than requested then don't process second buff part */
            if(ret_count < part_size)
            {
               count                = ret_count;
               buf->busy_size      -= count;
               buf->first_busy     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured       = BUFF_FALSE;
#endif
            }
            else
            {
               ret_count            = part_size;
               params.dest          = data;
               params.src           = buf->memory;
               params.dest_offset   = part_size;
               params.src_offset    = 0;
               part_size            = count - part_size;
               params.size          = part_size;
               params.is_last_part  = BUFF_TRUE;

               buf->busy_size      -= ret_count;
               buf->first_busy      = 0;

               ret_count2 = vendor_cpy(&params);

               /* vendor_cpy might copy in second step less data than requested - in this case reduce count and part_size */
               if(BUFF_LIKELY(ret_count2 <= part_size))
               {
                  count     = ret_count + ret_count2;
                  part_size = ret_count2;
               }

               buf->busy_size      -= part_size;
               buf->first_busy      = part_size;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
               buf->size            = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured       = BUFF_TRUE;
#endif
            }
         }

         if(BUFF_UNLIKELY(0 == buf->busy_size))
         {
            buf->first_busy = 0;
            buf->first_free = 0;
         }

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_read = extension->on_read;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Read, on_read))
            {
               on_read(buf, extension, count, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_read = extension->on_read;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Read, on_read))
         {
            on_read(buf, extension, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;
} /* Buff_Ring_Read_Vendor */
#endif



#if( (!defined(BUFF_RING_READ_TO_VECTOR_EXTERNAL)) && BUFF_RING_READ_TO_VECTOR_ENABLED && BUFF_USE_VECTOR_BUFFERS )
Buff_Size_DT Buff_Ring_Read_To_Vector(
   Buff_Ring_XT                    *buf,
   const Buff_Writeable_Vector_XT  *data,
   Buff_Num_Elems_DT                data_num_elems,
   Buff_Size_DT                     size,
   Buff_Size_DT                     offset_data,
   Buff_Bool_DT                     use_protection)
{
#if(BUFF_SAVE_PROGRAM_MEMORY && BUFF_USE_TREE_BUFFERS)

   Buff_Writeable_Tree_XT tree;

   tree.is_branch = BUFF_FALSE;
   tree.elem_type.vector.vector           = data;
   tree.elem_type.vector.vector_num_elems = data_num_elems;

   return Buff_Ring_Read_To_Tree(buf, &tree, 1, size, offset_data, use_protection);

#else

#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Read on_read;
#endif
   Buff_Readable_Vector_XT vector[2];
   Buff_Size_DT      count = 0;
   Buff_Num_Elems_DT vector_num_elems;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT      rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT      data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Writeable_Vector_XT, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = buf->busy_size;

      if(BUFF_LIKELY(size <= buf->busy_size))
      {
         count = size;
      }

      if(BUFF_LIKELY(count > 0))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         data_changed = BUFF_TRUE;
#endif
         if(buf->first_busy >= buf->first_free)
         {
            if(BUFF_LIKELY(buf->first_free > 0))
            {
               vector[1].data    = buf->memory;
               vector[1].size    = buf->first_free;
               vector_num_elems  = 2;
            }
            else
            {
               vector_num_elems  = 1;
            }
            vector[0].size       = buf->size - buf->first_busy;
         }
         else
         {
            vector_num_elems     = 1;
            vector[0].size       = buf->first_free - buf->first_busy;
         }
         vector[0].data          = &((const uint8_t*)(buf->memory))[buf->first_busy];

         count = Buff_Copy_Vector_To_Vector(data, data_num_elems, vector, vector_num_elems, count, offset_data, 0);

         if(BUFF_LIKELY(buf->first_busy <= (buf->size - count)))
         {
            buf->busy_size      -= count;
            buf->first_busy     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_FALSE;
#endif

            if(BUFF_UNLIKELY(buf->first_busy >= buf->size))
            {
               buf->first_busy   = 0;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
               buf->size         = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured    = BUFF_TRUE;
#endif
            }
         }
         else
         {
            buf->busy_size      -= count;
            buf->first_busy      = buf->first_busy + count - buf->size;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
            buf->size            = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_TRUE;
#endif
         }

         if(BUFF_UNLIKELY(0 == buf->busy_size))
         {
            buf->first_busy = 0;
            buf->first_free = 0;
         }

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_read = extension->on_read;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Read, on_read))
            {
               on_read(buf, extension, count, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_read = extension->on_read;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Read, on_read))
         {
            on_read(buf, extension, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;

#endif
} /* Buff_Ring_Read_To_Vector */
#endif



#if( (!defined(BUFF_RING_READ_TO_TREE_EXTERNAL)) && BUFF_RING_READ_TO_TREE_ENABLED && BUFF_USE_TREE_BUFFERS )
Buff_Size_DT Buff_Ring_Read_To_Tree(
   Buff_Ring_XT                 *buf,
   const Buff_Writeable_Tree_XT *data,
   Buff_Num_Elems_DT             data_num_elems,
   Buff_Size_DT                  size,
   Buff_Size_DT                  offset_data,
   Buff_Bool_DT                  use_protection)
{
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Read on_read;
#endif
   Buff_Readable_Vector_XT vector[2];
   Buff_Readable_Tree_XT tree;
   Buff_Size_DT count = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Writeable_Tree_XT, data) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      count = buf->busy_size;

      if(BUFF_LIKELY(size <= buf->busy_size))
      {
         count = size;
      }

      if(BUFF_LIKELY(count > 0))
      {
#if(BUFF_RING_USE_EXTENSIONS)
         data_changed = BUFF_TRUE;
#endif
         tree.elem_type.vector.vector  = vector;
         tree.is_branch = BUFF_FALSE;

         if(buf->first_busy >= buf->first_free)
         {
            if(BUFF_LIKELY(buf->first_free > 0))
            {
               vector[1].data                         = buf->memory;
               vector[1].size                         = buf->first_free;
               tree.elem_type.vector.vector_num_elems = 2;
            }
            else
            {
               tree.elem_type.vector.vector_num_elems = 1;
            }
            vector[0].size                            = buf->size - buf->first_busy;
         }
         else
         {
            tree.elem_type.vector.vector_num_elems    = 1;
            vector[0].size                            = buf->first_free - buf->first_busy;
         }
         vector[0].data                               = &((const uint8_t*)(buf->memory))[buf->first_busy];

         count = Buff_Copy_Tree_To_Tree(
            data,
            data_num_elems,
            &tree,
            1,
            count,
            offset_data,
            0,
            BUFF_MAKE_INVALID_PTR(Buff_Size_DT),
            BUFF_MAKE_INVALID_PTR(Buff_Bool_DT));

         if(BUFF_LIKELY(buf->first_busy <= (buf->size - count)))
         {
            buf->busy_size      -= count;
            buf->first_busy     += count;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_FALSE;
#endif

            if(BUFF_UNLIKELY(buf->first_busy >= buf->size))
            {
               buf->first_busy   = 0;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
               buf->size         = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured    = BUFF_TRUE;
#endif
            }
         }
         else
         {
            buf->busy_size      -= count;
            buf->first_busy      = buf->first_busy + count - buf->size;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
            buf->size            = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured       = BUFF_TRUE;
#endif
         }

         if(BUFF_UNLIKELY(0 == buf->busy_size))
         {
            buf->first_busy = 0;
            buf->first_free = 0;
         }

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
         extension = buf->protected_extension;

         if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
         {
            on_read = extension->on_read;

            if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Read, on_read))
            {
               on_read(buf, extension, count, rewind_occured);
            }
         }
#endif
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_read = extension->on_read;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Read, on_read))
         {
            on_read(buf, extension, count, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return count;
} /* Buff_Ring_Read_To_Tree */
#endif



#if( (!defined(BUFF_RING_TO_RING_COPY_EXTERNAL)) && BUFF_RING_TO_RING_COPY_ENABLED )
Buff_Size_DT Buff_Ring_To_Ring_Copy(
   Buff_Ring_XT                 *dest,
   Buff_Ring_XT                 *src,
   Buff_Size_DT                  size,
   Buff_Size_DT                  src_offset,
   Buff_Bool_DT                  overwrite_if_no_free_space,
   Buff_Bool_DT                  use_protection)
{
   uint8_t *data_to;
   const uint8_t *data_from;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Write on_write;
#endif
   Buff_Size_DT read_start;
   Buff_Size_DT part_size_src;
   Buff_Size_DT part_size_dest;
   Buff_Size_DT size_to_copy = 0;
   Buff_Size_DT size_to_read = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#if(BUFF_RING_USE_EXTENSIONS)
   Buff_Bool_DT data_changed = BUFF_FALSE;
#endif
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(dest);
   BUFF_DEBUG_RING(src);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, dest) && BUFF_CHECK_PTR(Buff_Ring_XT, src)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(dest);
         BUFF_PROTECTION_LOCK(src);
      }
#endif

      /* check if src_offset points to busy area of source buffer - otherwise copying will not be executed */
      if(BUFF_LIKELY(src_offset < src->busy_size))
      {
         size_to_read = src->busy_size - src_offset;

         if(BUFF_LIKELY(size <= size_to_read))
         {
            size_to_read = size;
         }

         size_to_copy = BUFF_RING_GET_FREE_SIZE(dest);

         if(BUFF_LIKELY(size_to_read <= size_to_copy))
         {
            size_to_copy = size_to_read;
         }
#if(BUFF_RING_ALLOW_OLD_DATA_OVERWRITE)
         else if(BUFF_BOOL_IS_TRUE(overwrite_if_no_free_space))
         {
            if(BUFF_LIKELY(size_to_read < dest->size))
            {
               Buff_Ring_Remove(dest, size_to_read - size_to_copy, BUFF_FALSE);

               size_to_copy = size_to_read;
            }
            else
            {
               Buff_Ring_Clear(dest, BUFF_FALSE);

               size_to_copy = dest->size;
            }
         }
#endif
         if(BUFF_LIKELY(size_to_copy > 0))
         {
#if(BUFF_RING_USE_EXTENSIONS)
            data_changed = BUFF_TRUE;
#endif
            read_start = src->first_busy + src_offset - src->size;

            if(BUFF_LIKELY(src->first_busy < (src->size - src_offset)))
            {
               read_start += src->size;
            }

            /* destination memory is continous */
            if(BUFF_LIKELY(dest->first_free <= (dest->size - size_to_copy)))
            {
               data_to = &((uint8_t*)(dest->memory))[dest->first_free];

               /* source memory is continous */
               if(BUFF_LIKELY(read_start <= (src->size - size_to_copy)))
               {
                  memcpy(data_to, &((const uint8_t*)(src->memory))[read_start], size_to_copy);
               }
               /* source memory is split */
               else
               {
                  part_size_src  = src->size - read_start;
                  memcpy(data_to, &((const uint8_t*)(src->memory))[read_start], part_size_src);

                  data_to        = &((uint8_t*)data_to)[part_size_src];
                  part_size_src  = size_to_copy - part_size_src;
                  memcpy(data_to, src->memory, part_size_src);
               }

               dest->busy_size     += size_to_copy;
               dest->first_free    += size_to_copy;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured       = BUFF_FALSE;
#endif

               if(BUFF_UNLIKELY(dest->first_free >= dest->size))
               {
                  dest->first_free  = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
                  rewind_occured    = BUFF_TRUE;
#endif
               }
            }
            /* destination memory is split */
            else
            {
               data_from      = &((const uint8_t*)(src->memory))[read_start];
               part_size_dest = dest->size - dest->first_free;

               /* source memory is continous */
               if(BUFF_LIKELY(read_start <= (src->size - size_to_copy)))
               {
                  memcpy( &((uint8_t*)(dest->memory))[dest->first_free], data_from, part_size_dest);
                  data_from            = &((uint8_t*)data_from)[part_size_dest];
                  part_size_dest       = size_to_copy - part_size_dest;
                  memcpy(dest->memory, data_from, part_size_dest);

                  dest->first_free     = part_size_dest;
               }
               /* source memory is split */
               else
               {
                  part_size_src  = src->size - read_start;

                  if(part_size_src > part_size_dest)
                  {
                     memcpy( &((uint8_t*)(dest->memory))[dest->first_free], data_from, part_size_dest);
                     data_from            = &((uint8_t*)data_from)[part_size_dest];
                     memcpy(dest->memory, data_from, part_size_src - part_size_dest);
                     dest->first_free  = part_size_src - part_size_dest;
                     part_size_src     = size_to_copy - part_size_src;
                     memcpy( &((uint8_t*)(dest->memory))[dest->first_free], src->memory, part_size_src);
                     dest->first_free += part_size_src;
                  }
                  else
                  {
                     memcpy( &((uint8_t*)(dest->memory))[dest->first_free], data_from, part_size_src);
                     dest->first_free += part_size_src;
                     memcpy( &((uint8_t*)(dest->memory))[dest->first_free], src->memory, part_size_dest - part_size_src);
                     src_offset        = part_size_dest - part_size_src;
                     part_size_dest    = size_to_copy - part_size_dest;
                     memcpy(dest->memory, &((uint8_t*)(src->memory))[src_offset], part_size_dest);
                     dest->first_free  = part_size_dest;
                  }
               }

               dest->busy_size     += size_to_copy;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
               rewind_occured       = BUFF_TRUE;
#endif
            }

#if(BUFF_RING_USE_BUSY_SIZE_MONITORING)
            if(dest->busy_size >= dest->max_busy_size)
            {
               dest->max_busy_size = dest->busy_size;
            }
#endif

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
            extension = dest->protected_extension;

            if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
            {
               on_write = extension->on_write;

               if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
               {
                  on_write(dest, extension, size_to_copy, rewind_occured);
               }
            }
#endif
         }
      }

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(dest);
         BUFF_PROTECTION_UNLOCK(src);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = dest->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension) && BUFF_BOOL_IS_TRUE(data_changed))
      {
         on_write = extension->on_write;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Write, on_write))
         {
            on_write(dest, extension, size_to_copy, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(dest);
   BUFF_DEBUG_RING(src);

   BUFF_EXIT_FUNC();

   return size_to_copy;
} /* Buff_Ring_To_Ring_Copy */
#endif



#if( (!defined(RING_BUF_REMOVE_EXTERNAL)) && BUFF_RING_REMOVE_ENABLED )
Buff_Size_DT Buff_Ring_Remove(Buff_Ring_XT *buf, Buff_Size_DT size, Buff_Bool_DT use_protection)
{
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Remove on_remove;
#endif
   Buff_Size_DT result = 0;
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Bool_DT rewind_occured = BUFF_FALSE;
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY((size > 0) && BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      if(BUFF_LIKELY(size < buf->busy_size))
      {
         buf->busy_size      -= size;
         buf->first_busy     += size;
         if(BUFF_UNLIKELY(buf->first_busy >= buf->size))
         {
            buf->first_busy  -= buf->size;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
            buf->size         = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
            rewind_occured    = BUFF_TRUE;
#endif
         }
         result               = size;
      }
      else
      {
         result               = buf->busy_size;
         buf->first_free      = 0;
         buf->first_busy      = 0;
         buf->busy_size       = 0;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
         buf->size            = buf->original_size;
#endif
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
         rewind_occured       = BUFF_TRUE;
#endif
      }

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
      extension = buf->protected_extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
      {
         on_remove = extension->on_remove;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Remove, on_remove))
         {
            on_remove(buf, extension, rewind_occured);
         }
      }
#endif

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
      {
         on_remove = extension->on_remove;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Remove, on_remove))
         {
            on_remove(buf, extension, rewind_occured);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();

   return result;
} /* Buff_Ring_Remove */
#endif



#if( (!defined(RING_BUF_CLEAR_EXTERNAL)) && BUFF_RING_CLEAR_ENABLED )
void Buff_Ring_Clear(Buff_Ring_XT *buf, Buff_Bool_DT use_protection)
{
#if(BUFF_RING_USE_EXTENSIONS || BUFF_RING_USE_PROTECTED_EXTENSIONS)
   Buff_Ring_Extensions_XT *extension;
   Buff_Ring_Extension_On_Remove on_remove;
#endif

#if(!BUFF_RING_USE_PROTECTION)
   BUFF_UNUSED_PARAM(use_protection);
#endif

   BUFF_ENTER_FUNC();
   BUFF_DEBUG_RING(buf);

   if(BUFF_LIKELY(BUFF_CHECK_PTR(Buff_Ring_XT, buf)))
   {
#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_LOCK(buf);
      }
#endif

      buf->first_free   = 0;
      buf->first_busy   = 0;
      buf->busy_size    = 0;
#if(BUFF_RING_DATA_CHECK_IN_CHECK_OUT_ENABLED)
      buf->size         = buf->original_size;
#endif

#if(BUFF_RING_USE_PROTECTED_EXTENSIONS)
      extension = buf->protected_extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
      {
         on_remove = extension->on_remove;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Remove, on_remove))
         {
            on_remove(buf, extension, BUFF_TRUE);
         }
      }
#endif

#if(BUFF_RING_USE_PROTECTION)
      if(BUFF_BOOL_IS_TRUE(use_protection))
      {
         BUFF_PROTECTION_UNLOCK(buf);
      }
#endif

#if(BUFF_RING_USE_EXTENSIONS)
      extension = buf->extension;

      if(BUFF_CHECK_PTR(Buff_Ring_Extensions_XT, extension))
      {
         on_remove = extension->on_remove;

         if(BUFF_CHECK_HANDLER(Buff_Ring_Extension_On_Remove, on_remove))
         {
            on_remove(buf, extension, BUFF_TRUE);
         }
      }
#endif
   }

   BUFF_DEBUG_RING(buf);
   BUFF_EXIT_FUNC();
} /* Buff_Ring_Clear */
#endif
#endif /* #if(BUFF_USE_RING_BUFFERS) */



#ifdef BUFF_INCLUDE_POST_IMP
#include "buff_post_imp.h"
#endif

#ifdef BUFF_RING_C_
#undef BUFF_RING_C_
#endif

