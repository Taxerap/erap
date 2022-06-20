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

#include <Guid/FileInfo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

EFI_STATUS
EFIAPI
erap_Entry( IN EFI_HANDLE image_handle, IN EFI_SYSTEM_TABLE *system_table )
{
    Print(L"Hello world!\n");
    EFI_STATUS status = EFI_SUCCESS;

    UINTN memory_map_sz = sizeof(EFI_MEMORY_DESCRIPTOR);
    UINTN memory_map_key = 0;
    UINTN memory_descriptor_sz = 0;
    UINT32 memory_descriptor_ver = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    status = gBS->AllocatePool(EfiLoaderData, memory_map_sz, (VOID **) &memory_map);
    if (EFI_ERROR(status))
    {
        Print(u"Failed to allocate memory for memory map data.\n");
        return status;
    }

    status = gBS->GetMemoryMap(&memory_map_sz, memory_map, &memory_map_key, &memory_descriptor_sz, &memory_descriptor_ver);
    if (status == EFI_BUFFER_TOO_SMALL)
    {
        gBS->FreePool(memory_map);
        status = gBS->AllocatePool(EfiLoaderData, memory_map_sz, (VOID **) &memory_map);
        if (EFI_ERROR(status))
        {
            Print(u"Failed to allocate memory for memory map data.\n");
            return status;
        }
        status = gBS->GetMemoryMap(&memory_map_sz, memory_map, &memory_map_key, &memory_descriptor_sz, &memory_descriptor_ver);
    }
    if (EFI_ERROR(status))
    {
        Print(u"Failed to get memory map.\n");
        return status;
    }
    Print(u"Got %u maps of memory.\n", memory_map_sz / memory_descriptor_sz);
    Print(u"Descriptor Version: [%u].\n", memory_descriptor_ver);
    UINT8 *current_descriptor_addr = (UINT8 *) memory_map;
    UINT64 total_memory_sz = 0;

    Print(u"| Index | Type | Physical start | Virtual start | Number of Pages |\n");
    for (UINTN i = 0; i < memory_map_sz / memory_descriptor_sz; i++)
    {
        EFI_MEMORY_DESCRIPTOR *current_descriptor = (EFI_MEMORY_DESCRIPTOR *) current_descriptor_addr;
        Print(u"| %-3u | %-30s | %016x | %016x | %u |\n", i,
                                                               erap_GetMemoryTypeString(current_descriptor->Type),
                                                               current_descriptor->PhysicalStart,
                                                               current_descriptor->VirtualStart,
                                                               current_descriptor->NumberOfPages);
        total_memory_sz += current_descriptor->NumberOfPages * 4096;
        current_descriptor_addr += memory_descriptor_sz;
    }
    Print(u"Total memory: %u MiB.\n", total_memory_sz / (1024 * 1024));

    UINTN handles_count = 0;
    EFI_HANDLE *handles_buffer = NULL;
    status = gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &handles_count, &handles_buffer);
    if (EFI_ERROR(status))
    {
        Print(u"Failed to LocateHandleBuffer for <SimpleFileSystemProtocol>.");
        return status;
    }
    Print(u"Grabbed %u handle(s) that support SimpleFileSystemProtocol.\n", handles_count);

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfs;
    status = gBS->OpenProtocol(handles_buffer[0], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&sfs, image_handle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(status))
    {
        Print(u"Failed to open SimpleFileSystemProtocol.\n");
        return status;
    }
    Print(u"Opened SimpleFileSystemProtocol handle.\n");

    EFI_FILE_PROTOCOL *root_dir = NULL;
    status = sfs->OpenVolume(sfs, &root_dir);
    if (EFI_ERROR(status))
    {
        Print(u"Failed to open volume.\n");
        return status;
    }
    Print(u"Opened a volume.\n");

    EFI_FILE_PROTOCOL *file_handle = NULL;
    status = root_dir->Open(root_dir, &file_handle, L"\\", EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(status))
    {
        Print(u"Failed to open directory.\n");
        return status;
    }
    Print(u"Opened root directory.\n");

    EFI_FILE_PROTOCOL *entry = NULL;
    UINTN entry_sz = sizeof(EFI_FILE_PROTOCOL);

    status = gBS->AllocatePool(EfiLoaderData, entry_sz, (VOID **) &entry);
    if (EFI_ERROR(status))
    {
        Print(u"Failed to allocate pool space for file entries!\n");
        return status;
    }

    UINTN total_entries = 0;
    while (1)
    {
        status = root_dir->Read(root_dir, &entry_sz, entry);
        if (!entry_sz)
            break;
        if (status == EFI_BUFFER_TOO_SMALL)
        {
            Print(u"Buffer is too small. Reallocating...\n");
            gBS->FreePool(entry);
            status = gBS->AllocatePool(EfiLoaderData, entry_sz, (VOID **) &entry);
            if (EFI_ERROR(status))
            {
                Print(u"Failed to allocate pool space for file entries!\n");
                return status;
            }
            continue;
        }
        else if (EFI_ERROR(status))
        {
            Print(u"Failed to open entry!\n");
            return status;
        }

        EFI_FILE_INFO *file_info = (EFI_FILE_INFO *)entry;
        Print(u"Loaded a directory entry: %s which is %sa directory.\n", file_info->FileName, (file_info->Attribute & EFI_FILE_DIRECTORY) ? L"" : L"not ");
        total_entries++;
    }
    Print(u"There are total %u entries.\n", total_entries);

    gBS->FreePool(entry);

    Print(u"Done.\n");
    return EFI_SUCCESS;
}
