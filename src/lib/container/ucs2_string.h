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


#ifndef ERAP_LIB_CONTAINER_UCS2_STRING_H
#define ERAP_LIB_CONTAINER_UCS2_STRING_H

#include <freestandings.h>

#include <Uefi.h>

typedef struct erap_UCS2String
{
    UINTN length;
    UINTN capacity;
    alignas(8) CHAR16 *ptr;
} erap_UCS2String;

erap_UCS2String *
erap_UCS2String_Create( void );

erap_UCS2String *
erap_UCS2String_From( const CHAR16 *original );

void
erap_UCS2String_Expand( erap_UCS2String *str );

void
erap_UCS2String_ExpandUntilOnce( erap_UCS2String *str, UINTN new_size );

void
erap_UCS2String_ShrinkToFit( erap_UCS2String *str );

void
erap_UCS2String_PushC( erap_UCS2String *str, CHAR16 data );

void
erap_UCS2String_PushS( erap_UCS2String *str, const CHAR16 *data );

void
erap_UCS2String_Insert( erap_UCS2String *str, UINTN index, const CHAR16 *data );

void
erap_UCS2String_Clear( erap_UCS2String *str );

void
erap_UCS2String_Destroy( erap_UCS2String *str );

#endif // !ERAP_LIB_CONTAINER_UCS2_STRING_H
