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

#ifndef ERAP_DEBUG_MEMORY_TYPE_H
#define ERAP_DEBUG_MEMORY_TYPE_H

#include <Library/UefiLib.h>
#include <Uefi.h>

static
inline
CHAR16 *
erap_GetMemoryTypeString( EFI_MEMORY_TYPE type )
{
    switch (type)
    {
        case EfiReservedMemoryType:
            return u"Reserved";
        case EfiLoaderCode:
            return u"UEFIApplicationCode";
        case EfiLoaderData:
            return u"UEFIApplicationData";
        case EfiBootServicesCode:
            return u"UEFIBootServicesCode";
        case EfiBootServicesData:
            return u"UEFIBootServicesData";
        case EfiRuntimeServicesCode:
            return u"UEFIRuntimeServicesCode";
        case EfiRuntimeServicesData:
            return u"UEFIRuntimeServicesData";
        case EfiConventionalMemory:
            return u"UEFIConventionalMemory (Free)";
        case EfiUnusableMemory:
            return u"UEFIUnusableMemory";
        case EfiACPIReclaimMemory:
            return u"ACPIReclaimMemory";
        case EfiACPIMemoryNVS:
            return u"ACPIMemoryNVS";
        case EfiMemoryMappedIO:
            return u"UEFIMemoryMappedIO";
        case EfiMemoryMappedIOPortSpace:
            return u"UEFIMemoryMappedIOPortSpace";
        case EfiPalCode:
            return u"UEFIPalCode";
        case EfiPersistentMemory:
            return u"UEFIPersistentMemory";
        default:
            return u"Unknown";
    }
}

#endif // !ERAP_DEBUG_MEMORY_TYPE_H
