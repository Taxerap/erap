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

#include "memory.h"

#include <Library/UefiBootServicesTableLib.h>

VOID *
erap_Memory_Malloc( EFI_MEMORY_TYPE type, UINTN size )
{
    VOID *result = NULL;

    gBS->AllocatePool(type, size, &result);

    return result;
}

VOID *
erap_Memory_ReAlloc( EFI_MEMORY_TYPE type, UINTN preserve, VOID *old, UINTN new_size )
{
    VOID *result = NULL;

    gBS->AllocatePool(type, new_size, &result);
    if (preserve)
        gBS->CopyMem(result, old, preserve);
    gBS->FreePool(old);

    return result;
}

VOID
erap_Memory_Free( VOID *ptr )
{
    gBS->FreePool(ptr);
}

INTN
erap_Memory_MemCmp( CONST VOID *lhs, CONST VOID *rhs, UINTN count )
{
    CONST UINT8 *lhs_p = lhs, *rhs_p = rhs;
    for (UINTN i = 0; i < count; i++)
        if (lhs_p[i] != rhs_p[i])
            return 1;

    return 0;
}

VOID
erap_Memory_MemCpy( VOID *dest, CONST VOID *src, UINTN length )
{
    // Even though UEFI Spec 2.9 said:
    /*
        ...The contents of the Destination buffer on exit from this service must match the contents of the Source
        buffer on entry to this service. Due to potential overlaps, the contents of the Source buffer may be 
        modified by this service...
        (Section 7.5 <Miscellaneous Boot Services>: EFI_BOOT_SERVICES.CopyMem())
    */
    // ...it is safe to cast src to non-CONST since InternalMemCopyMem() from Edk2 has 'src' defined as CONST.
    gBS->CopyMem(dest, (VOID *) src, length);
}
