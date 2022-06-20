#include <Library/UefiLib.h>
#include <Uefi.h>

static
CHAR16 *
erap_GetMemoryTypeString( UINT32 type )
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
