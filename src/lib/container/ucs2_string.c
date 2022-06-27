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

#include "ucs2_string.h"

#include "../data/memory.h"
#include "../data/ucs2.h"

#include <Library/UefiBootServicesTableLib.h>

erap_UCS2String *
erap_UCS2String_Create( void )
{
    erap_UCS2String *result = NULL;
    gBS->AllocatePool(EfiLoaderData, sizeof(erap_UCS2String), (VOID **) &result);
    result->length = 0;
    result->capacity = 0;
    result->ptr = NULL;
    return result;
}

erap_UCS2String *
erap_UCS2String_From( const CHAR16 *original )
{
    erap_UCS2String *result = NULL;
    gBS->AllocatePool(EfiLoaderData, sizeof(erap_UCS2String), (VOID **) &result);

    UINTN characters = erap_UCS2_StrLen(original);
    gBS->AllocatePool(EfiLoaderData, (characters + 1) * sizeof(CHAR16), (VOID **) &(result->ptr));
    gBS->CopyMem(result->ptr, (VOID *) original, characters * sizeof(CHAR16));
    gBS->CopyMem(result->ptr + characters, &(CHAR16){ u'\0' }, sizeof(CHAR16));

    result->length = characters;
    result->capacity = (characters + 1) * sizeof(CHAR16);

    return result;
}

void
erap_UCS2String_Expand( erap_UCS2String *str )
{
    if (!str->capacity)
    {
        gBS->AllocatePool(EfiLoaderData, sizeof(CHAR16), (VOID **) &(str->ptr));
        str->capacity = sizeof(CHAR16);
    }
    else
    {
        str->ptr = erap_Memory_ReAlloc(EfiLoaderData, str->capacity, str->ptr, 2 * str->capacity);
        str->capacity *= 2;
    }
}

void
erap_USC2String_ExpandUntil( erap_UCS2String *str, UINTN new_size )
{
    while (str->capacity < new_size)
        erap_UCS2String_Expand(str);
}

void
erap_UCS2String_ShrinkToFit( erap_UCS2String *str )
{
    if (str->capacity > str->length * sizeof(CHAR16) && str->length)
    {
        str->ptr = erap_Memory_ReAlloc(EfiLoaderData, (str->length + 1) * sizeof(CHAR16), str->ptr, (str->length + 1) * sizeof(CHAR16));
        str->capacity = (str->length + 1) * sizeof(CHAR16);
    }
    else if (str->capacity && !str->length)
    {
        gBS->FreePool(str->ptr);
        str->capacity = 0;
    }
}

void
erap_UCS2String_PushC( erap_UCS2String *str, CHAR16 data )
{
    erap_USC2String_ExpandUntil(str, sizeof(CHAR16) * (str->length + 2));
    str->ptr[str->length++] = data;
    str->ptr[str->length] = u'\0';
}

void
erap_UCS2String_PushS( erap_UCS2String *str, const CHAR16 *data )
{
    UINTN target_length = erap_UCS2_StrLen(data);
    erap_USC2String_ExpandUntil(str, sizeof(CHAR16) * (str->length + target_length + 1));
    gBS->CopyMem(str->ptr + str->length, (VOID *) data, sizeof(CHAR16) * (target_length + 1));
    str->length += target_length;
}

void
erap_UCS2String_Insert( erap_UCS2String *str, UINTN index, const CHAR16 *data )
{
    if (index >= str->length)
        erap_UCS2String_PushS(str, data);
    else
    {
        UINTN target_length = erap_UCS2_StrLen(data);
        erap_USC2String_ExpandUntil(str, sizeof(CHAR16) * (str->length + target_length + 1));
        gBS->CopyMem(str->ptr + index + target_length, str->ptr + index, sizeof(CHAR16) * (str->length - index + 1));
        gBS->CopyMem(str->ptr + index, (VOID *) data, sizeof(CHAR16) * target_length);
        str->length += target_length;
    }
}

void
erap_UCS2String_Clear( erap_UCS2String *str )
{
    if (str->length)
    {
        str->ptr[0] = u'\0';
        str->length = 0;
    }
}

void
erap_UCS2String_Destroy( erap_UCS2String *str )
{
    if (str->capacity)
        gBS->FreePool(str->ptr);
    gBS->FreePool(str);
}
