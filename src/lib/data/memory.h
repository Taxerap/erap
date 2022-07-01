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

#ifndef ERAP_LIB_DATA_MEMORY_H
#define ERAP_LIB_DATA_MEMORY_H

#include <Uefi.h>

VOID *
erap_Memory_Malloc( EFI_MEMORY_TYPE type, UINTN size );

VOID *
erap_Memory_ReAlloc( EFI_MEMORY_TYPE type, UINTN preserve, VOID *old, UINTN new_size );

VOID
erap_Memory_Free( VOID *ptr );

INTN
erap_Memory_MemCmp( CONST VOID *lhs, CONST VOID *rhs, UINTN count );

VOID
erap_Memory_MemCpy( VOID *dest, CONST VOID *src, UINTN length );

#endif // !ERAP_LIB_DATA_MEMORY_H
