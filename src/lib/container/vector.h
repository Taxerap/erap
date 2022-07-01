/*************************************************************************************************
*                                                                                                *
*                                            [ erap ]                                            *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                            (ɔ) 2022 Taxerap, some rights reserved.                             *
*                                                                                                *
*              Erap is a free software. You can freely do whatever you want with it              *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*               Erap is created, intended to be useful, but without any warranty.                *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*                     <https://github.com/Taxerap/Erap/blob/master/LICENSE>.                     *
*                                                                                                *
*         For more information about the project, please visit the Github repository at          *
*                               <https://github.com/Taxerap/erap>.                               *
*                                                                                                *
**************************************************************************************************/

#ifndef ERAP_LIB_CONTAINER_VECTOR_H
#define ERAP_LIB_CONTAINER_VECTOR_H

#include <freestandings.h>

#include <Uefi.h>

typedef VOID (*erap_VectorFreeFunc)( VOID * );
typedef VOID (*erap_VectorWalkFunc)( VOID * );
typedef BOOLEAN (*erap_VectorCmpFunc)( CONST VOID *, CONST VOID * );

typedef struct erap_Vector
{
    UINTN elem_size;
    UINTN capacity;
    UINTN length;
    erap_VectorFreeFunc free_func;
    alignas(8) VOID *ptr;
} erap_Vector;

///
/// \brief Create a vector
///
/// Create a vector.
///
/// \param elem_size Size of vector's elements
/// \param free_func Function to call when the vector frees an element
///
/// \return Created vector
///
erap_Vector *
erap_Vector_Create( UINTN elem_size, erap_VectorFreeFunc free_func );

///
/// \brief Get the first element
///
/// Get the first element of the vector.
///
/// \param vec Vector
///
/// \return Pointer to the first element
///
VOID *
erap_Vector_First( CONST erap_Vector *vec );

///
/// \brief Get the last element
///
/// Get the last element of the vector.
///
/// \param vec Vector
///
/// \return Pointer to the last element
///
VOID *
erap_Vector_Last( CONST erap_Vector *vec );

///
/// \brief Get the indexed element
///
/// Get the pointer to some place in the vector.<BR />
/// If the index is out of bound, it returns the last element of the vector, if any.
///
/// \param vec Vector
/// \param index Index to access
///
/// \return Pointer to the location after index
///
VOID *
erap_Vector_PtrAt( CONST erap_Vector *vec, UINTN index );

///
/// \brief Find an element in the vector
///
/// Search for an element in the given vector.
///
/// \param vec Vector
/// \param data Data to search
/// \param cmp Comparision function
///
/// \return Pointer to the found element, NULL otherwise.
///
VOID *
erap_Vector_Find( CONST erap_Vector *vec, CONST VOID *data, erap_VectorCmpFunc cmp );

///
/// \brief Expand the vector
///
/// Expand the vector, usually doubling its capacity unless the length is 0.
///
/// \param vec Vector to expand
///
VOID
erap_Vector_Expand( erap_Vector *vec );

///
/// \brief Expand the vector
///
/// Expand the vector until its capacity reaches the set limit.
///
/// \param vec Vector to expand
/// \param size Target size for expanding
///
VOID
erap_Vector_ExpandUntil( erap_Vector *vec, UINTN size );

///
/// \brief Walkthrough the vector
///
/// Walk the vector by a function.
///
/// \param vec Vector to walk
/// \param func Function for walking
///
VOID
erap_Vector_Walk( erap_Vector *vec, erap_VectorWalkFunc func );

///
/// \brief Clear out a vector
///
/// Clear the content of a vector. Its capacity is unchanged.
///
/// \param vec Vector to clear
///
VOID
erap_Vector_Clear( erap_Vector *vec );

///
/// \brief Shrink the vector
///
/// Shrink the vector to just enough to fit its contents.
///
/// \param vec Vector to shrink
///
VOID
erap_Vector_ShrinkToFit( erap_Vector *vec );

///
/// \brief Insert element to vector
///
/// Insert an element to vector.<BR />
/// If index is out of bound, it is understood to push the element to the end of vector.
///
/// \param vec Vector to insert
/// \param index Index to insert
/// \param elem Element to insert
///
VOID
erap_Vector_Insert( erap_Vector *vec, UINTN index, CONST VOID *elem );

///
/// \brief Replace an element
///
/// Replace an element with a new one, the old one will be overriden.<BR />
/// If index is out of bound, it is understood to push the element to last of vector.
///
/// \param vec Vector to operate
/// \param index Index to replace
/// \param elem Element to replace
///
VOID
erap_Vector_Replace( erap_Vector *vec, UINTN index, CONST VOID *elem );

///
/// \brief Delete an element
///
/// Delete an element in index.<BR />
/// If index is out of bound, it does nothing.
///
/// \param vec Vector to operate
/// \param index Index to delete
///
VOID
erap_Vector_Delete( erap_Vector *vec, UINTN index );

///
/// \brief Take an element
///
/// Take an element out of the vector.<BR />
/// If index is out of bound, it is understood to pop the last element of the vector.
/// If ptr_retrieve is NULL the free function will be called.
///
/// \param vec Vector to operate
/// \param index Index to take out
/// \param ptr_retrieve Pointer to retrieve the result
///
VOID
erap_Vector_Take( erap_Vector *vec, UINTN index, VOID *ptr_retrieve );

///
/// \brief Push an element
///
/// Push an element into the end of vector.
///
/// \param vec Vector to push into
/// \param elem Element to be pushed
///
VOID
erap_Vector_Push( erap_Vector *vec, CONST VOID *elem );

///
/// \brief Pop the last element of vector
///
/// Pop the last element of vector into the pointer.<BR />
/// If ptr_retrieve is NULL the free function will be called.
///
/// \param vec Vector to pop out
/// \param ptr_retrieve Pointer to retrieve the result
///
VOID
erap_Vector_Pop( erap_Vector *vec, VOID *ptr_retrieve );

///
/// \brief Push an element to front
///
/// Push an element into the front of vector.
///
/// \param vec Vector to push into
/// \param elem Element to be pushed
///
VOID
erap_Vector_Push_Front( erap_Vector *vec, CONST VOID *elem );

///
/// \brief Pop the first element of vector
///
/// Pop the first element of vector into the pointer.<BR />
/// Whether ptr_retrieve is NULL or not, the first element will be overriden.
///
/// \param vec Vector to pop out
/// \param ptr_retrieve Pointer to retrieve the result
///
VOID
erap_Vector_Pop_Front( erap_Vector *vec, VOID *ptr_retrieve );

///
/// \brief Destroy a vector
///
/// Destroy a vector, freeing all its elements.
///
/// \param vec Vector to destroy
///
VOID
erap_Vector_Destroy( erap_Vector *vec );

#endif // !ERAP_LIB_CONTAINER_VECTOR_H
