#include "vector.h"

#include "../data/memory.h"

#include <Library/UefiBootServicesTableLib.h>

erap_Vector *
erap_Vector_Create( UINTN elem_size, erap_VectorFreeFunc free_func )
{
    erap_Vector *vec = erap_Memory_Malloc(EfiLoaderData, sizeof(erap_Vector));
    vec->length = 0;
    vec->capacity = 0;
    vec->elem_size = elem_size;
    vec->free_func = free_func;
    vec->ptr = NULL;
    return vec;
}

VOID *
erap_Vector_First( const erap_Vector *vec )
{
    return vec->ptr;
}

VOID *
erap_Vector_Last( const erap_Vector *vec )
{
    if (!vec->length)
        return NULL;

    return (VOID *)((unsigned char *)(vec->ptr) + vec->elem_size * (vec->length - 1));
}

VOID *
erap_Vector_PtrAt( const erap_Vector *vec, UINTN index )
{
    return (VOID *)((unsigned char *)(vec->ptr) + vec->elem_size * index);
}

VOID *
erap_Vector_Find( const erap_Vector *vec, const VOID *elem, erap_VectorCmpFunc cmp )
{
    if (!vec->length)
        return NULL;

    UINT8 *ptr = vec->ptr;
    for (UINTN i = 0; i < vec->length; i++, ptr += vec->elem_size)
        if (cmp((VOID *)ptr, elem))
            return ptr;

    return NULL;
}

VOID
erap_Vector_Expand( erap_Vector *vec )
{
    if (!vec->capacity)
    {
        vec->capacity = 1;
        vec->ptr = erap_Memory_Malloc(EfiLoaderData, vec->elem_size);
    }
    else
    {
        vec->capacity *= 2;
        vec->ptr = erap_Memory_ReAlloc(EfiLoaderData, vec->elem_size * vec->length, vec->ptr, vec->elem_size * vec->capacity);
    }
}

VOID
erap_Vector_ExpandUntil( erap_Vector *vec, UINTN size )
{
    while (vec->capacity < size)
        erap_Vector_Expand(vec);
}

VOID
erap_Vector_ShrinkToFit( erap_Vector *vec )
{
    if (vec->capacity > vec->length && vec->length)
    {
        vec->ptr = erap_Memory_ReAlloc(EfiLoaderData, vec->elem_size * vec->length,  vec->ptr, vec->length * vec->elem_size);
        vec->capacity = vec->length;
    }
    else if (vec->capacity && !vec->length)
    {
        erap_Memory_Free(vec->ptr);
        vec->capacity = 0;
    }
}

VOID
erap_Vector_Walk( erap_Vector *vec, erap_VectorWalkFunc func )
{
    if (!vec->length)
        return;

    unsigned char *ptr = vec->ptr;
    for (UINTN i = 0; i < vec->length; i++, ptr += vec->elem_size)
        func((VOID *)ptr);
}

VOID
erap_Vector_Clear( erap_Vector *vec )
{
    if (vec->free_func)
        erap_Vector_Walk(vec, vec->free_func);
    vec->length = 0;
}

VOID
erap_Vector_Insert( erap_Vector *vec, UINTN index, const VOID *elem )
{
    if (index >= vec->length)
        erap_Vector_Push(vec, elem);
    else
    {
        erap_Vector_ExpandUntil(vec, vec->length + 1);
        VOID *ptr_inserting = erap_Vector_PtrAt(vec, index);
        VOID *ptr_shifting = erap_Vector_PtrAt(vec, index + 1);
        UINTN bytes_to_shift = vec->elem_size * (vec->length - index);
        gBS->CopyMem(ptr_shifting, ptr_inserting, bytes_to_shift);
        gBS->CopyMem(ptr_inserting, (VOID *) elem, vec->elem_size);
        vec->length++;
    }
}

VOID
erap_Vector_Replace( erap_Vector *vec, UINTN index, const VOID *elem )
{
    if (index >= vec->length)
        erap_Vector_Push(vec, elem);
    else
    {
        VOID *ptr = erap_Vector_PtrAt(vec, index);
        if (vec->free_func)
            vec->free_func(ptr);
        gBS->CopyMem(ptr, (VOID *) elem, vec->elem_size);
    }
}

VOID
erap_Vector_Delete( erap_Vector *vec, UINTN index )
{
    if (index >= vec->length)
        return;

    VOID *ptr_shifting = erap_Vector_PtrAt(vec, index);
    if (vec->free_func)
        vec->free_func(ptr_shifting);
    VOID *ptr_data = erap_Vector_PtrAt(vec, index + 1);
    UINTN bytes_to_shift = vec->elem_size * (vec->length - index - 1);
    gBS->CopyMem(ptr_shifting, ptr_data, bytes_to_shift);
    vec->length--;
}

VOID
erap_Vector_Take( erap_Vector *vec, UINTN index, VOID *ptr_retrieve )
{
    if (!vec->length)
        return;
    if (index >= vec->length)
        erap_Vector_Pop(vec, ptr_retrieve);
    else
    {
        VOID *ptr_shifting = erap_Vector_PtrAt(vec, index);
        VOID *ptr_data = erap_Vector_PtrAt(vec, index + 1);
        UINTN bytes_to_shift = vec->elem_size * (vec->length - index - 1);
        if (ptr_retrieve)
            gBS->CopyMem(ptr_retrieve, ptr_shifting, vec->elem_size);
        else if (vec->free_func)
            vec->free_func(ptr_shifting);
        gBS->CopyMem(ptr_shifting, ptr_data, bytes_to_shift);
        vec->length--;
    }
}

VOID
erap_Vector_Push( erap_Vector *vec, const VOID *elem )
{
    erap_Vector_ExpandUntil(vec, vec->length + 1);
    gBS->CopyMem(erap_Vector_PtrAt(vec, vec->length), (VOID *) elem, vec->elem_size);
    vec->length++;
}

VOID
erap_Vector_Pop( erap_Vector *vec, VOID *ptr_retrieve )
{
    if (!vec->length)
        return;
    if (ptr_retrieve)
        gBS->CopyMem(ptr_retrieve, erap_Vector_PtrAt(vec, vec->length), vec->elem_size);
    else if (vec->free_func)
        vec->free_func(erap_Vector_PtrAt(vec, vec->length));
    vec->length--;
}

VOID
erap_Vector_Push_Front( erap_Vector *vec, const VOID *elem )
{
    erap_Vector_ExpandUntil(vec, vec->length + 1);
    gBS->CopyMem(erap_Vector_PtrAt(vec, 1), vec->ptr, vec->elem_size * vec->length);
    gBS->CopyMem(vec->ptr, (VOID *) elem, vec->elem_size);
    vec->length++;
}

VOID
erap_Vector_Pop_Front( erap_Vector *vec, VOID *ptr_retrieve )
{
    if (!vec->length)
        return;
    if (ptr_retrieve)
        gBS->CopyMem(ptr_retrieve, vec->ptr, vec->elem_size);
    else if (vec->free_func)
        vec->free_func(vec->ptr);
    vec->length--;
    gBS->CopyMem(vec->ptr, erap_Vector_PtrAt(vec, 1), vec->elem_size * vec->length);
}

VOID
erap_Vector_Destroy( erap_Vector *vec )
{
    erap_Vector_Clear(vec);
    if (vec->capacity)
        erap_Memory_Free(vec->ptr);
    erap_Memory_Free(vec);
}
