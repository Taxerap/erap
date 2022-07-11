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

#include <debug/memory_type.h>
#include <lib/data/memory.h>
#include <lib/data/ucs2.h>
#include <lib/container/ucs2_string.h>
#include <lib/container/vector.h>

#include <Guid/FileInfo.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

VOID
printvec( VOID *data )
{
    INT32 *ptr = data;
    Print(u"%d ", *ptr);
}

EFI_STATUS
EFIAPI
erap_Test_Entry( IN EFI_HANDLE image_handle, IN EFI_SYSTEM_TABLE *system_table )
{
    erap_UCS2String *str = erap_UCS2String_From(u"Hello, world!\n");
    Print(u"Allocated string at %p, str->ptr: %p.\n", str, str->ptr);
    Print(str->ptr);
    Print(u"length: %u, capacity: %u, counted characters: %u, counted characters (raw): %u\n", str->length, str->capacity, erap_UCS2_StrLen(str->ptr), erap_UCS2_StrLen(u"Hello, world!\n"));
    erap_UCS2String_PushC(str, u's');
    Print(str->ptr);
    Print(u"length: %u, capacity: %u, counted characters: %u\n", str->length, str->capacity, erap_UCS2_StrLen(str->ptr));
    erap_UCS2String_PushS(str, u"bsbsb\n");
    Print(str->ptr);
    Print(u"length: %u, capacity: %u, counted characters: %u\n", str->length, str->capacity, erap_UCS2_StrLen(str->ptr));
    erap_UCS2String_Insert(str, 0, u"2");
    Print(str->ptr);
    Print(u"length: %u, capacity: %u, counted characters: %u\n", str->length, str->capacity, erap_UCS2_StrLen(str->ptr));
    erap_UCS2String_Insert(str, 1, u"3");
    Print(str->ptr);
    Print(u"length: %u, capacity: %u, counted characters: %u\n", str->length, str->capacity, erap_UCS2_StrLen(str->ptr));

    erap_UCS2String_Clear(str);
    erap_UCS2String_PushS(str, u"LOLOLOL");
    CHAR16 *chr = erap_UCS2_StrChr(str->ptr, u'O');
    CHAR16 *chr2 = erap_UCS2_StrChr(str->ptr, u'o');
    CHAR16 *rchr = erap_UCS2_StrRChr(str->ptr, u'O');
    CHAR16 *rchr2 = erap_UCS2_StrRChr(str->ptr, u'o');
    Print(u"Chr index: %u, rchr index: %u, chr2 found?: %s rchr2 found?: %s\n", chr - str->ptr, rchr - str->ptr, chr2 ? u"Yes" : u"No", rchr2 ? u"Yes" : u"No");

    erap_UCS2String_Clear(str);
    erap_UCS2String_PushS(str, u"233333");
    UINTN resu = 0;
    StrDecimalToUintnS(str->ptr, NULL, &resu);
    Print(u"Convert result: [%u]\n", resu);

    Print(u"String section done.\n");
    erap_UCS2String_Destroy(str);

    erap_Vector *vec = erap_Vector_Create(sizeof(INT32), NULL);
    erap_Vector_Expand(vec);
    Print(u"Allocated vector at %p, vec->ptr: %p.\n", vec, vec->ptr);

    erap_Vector_Push(vec, &(INT32){ -1 });
    erap_Vector_Push(vec, &(INT32){ -998 });
    erap_Vector_Push(vec, &(INT32){ 233 });
    erap_Vector_Push(vec, &(INT32){ 666 });

    Print(u"Content: [");
    erap_Vector_Walk(vec, printvec);
    Print(u"]\n");

    erap_Vector_Pop(vec, NULL);

    erap_Vector_Insert(vec, 2, &(INT32){ 14 });

    Print(u"Content: [");
    erap_Vector_Walk(vec, printvec);
    Print(u"]\n");

    Print(u"Vector section done.\n");
    erap_Vector_Destroy(vec);

    EFI_STATUS status = EFI_SUCCESS;

    UINTN memory_map_sz = sizeof(EFI_MEMORY_DESCRIPTOR);
    UINTN memory_map_key = 0;
    UINTN memory_descriptor_sz = 0;
    UINT32 memory_descriptor_ver = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = erap_Memory_Malloc(EfiLoaderData, memory_map_sz);

    while (TRUE)
    {
        status = gBS->GetMemoryMap(&memory_map_sz, memory_map, &memory_map_key, &memory_descriptor_sz, &memory_descriptor_ver);
        if (status == EFI_BUFFER_TOO_SMALL)
            memory_map = erap_Memory_ReAlloc(EfiLoaderData, 0, memory_map, memory_map_sz * 2);
        else
            break;
    }
    if (EFI_ERROR(status))
    {
        Print(L"Failed to get memory map: %u.\n", status);
        return status;
    }
    Print(L"Got %u maps of memory.\n", memory_map_sz / memory_descriptor_sz);
    Print(L"Descriptor Version: [%u].\n", memory_descriptor_ver);

    UINT8 *current_descriptor_addr = (UINT8 *) memory_map;
    UINT64 total_memory_sz = 0;
    Print(L"| Index | Type | Physical start | Virtual start | Number of Pages |\n");
    for (UINTN i = 0; i < memory_map_sz / memory_descriptor_sz; i++)
    {
        EFI_MEMORY_DESCRIPTOR *current_descriptor = (EFI_MEMORY_DESCRIPTOR *) current_descriptor_addr;
        Print(L"| %-3u | %-30s | %016x | %016x | %-8u |\n", i,
                                                            erap_GetMemoryTypeString(current_descriptor->Type),
                                                            current_descriptor->PhysicalStart,
                                                            current_descriptor->VirtualStart,
                                                            current_descriptor->NumberOfPages);
        total_memory_sz += current_descriptor->NumberOfPages * 4096;
        current_descriptor_addr += memory_descriptor_sz;
    }
    Print(L"Total memory: %u MiB.\n", total_memory_sz / (1024 * 1024));

    UINTN handles_count = 0;
    EFI_HANDLE *handles_buffer = NULL;
    status = gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &handles_count, &handles_buffer);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to LocateHandleBuffer for <SimpleFileSystemProtocol>.");
        return status;
    }
    Print(L"Grabbed %u handle(s) that support SimpleFileSystemProtocol.\n", handles_count);

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfs;
    status = gBS->OpenProtocol(handles_buffer[0], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&sfs, image_handle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to open SimpleFileSystemProtocol.\n");
        return status;
    }
    Print(L"Opened SimpleFileSystemProtocol handle.\n");

    EFI_FILE_PROTOCOL *root_dir = NULL;
    status = sfs->OpenVolume(sfs, &root_dir);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to open volume.\n");
        return status;
    }
    Print(L"Opened a volume.\n");

    EFI_FILE_PROTOCOL *file_handle = NULL;
    status = root_dir->Open(root_dir, &file_handle, L"\\", EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to open directory.\n");
        return status;
    }
    Print(L"Opened root directory.\n");

    UINTN entry_sz = sizeof(EFI_FILE_PROTOCOL);
    EFI_FILE_PROTOCOL *entry = erap_Memory_Malloc(EfiLoaderData, entry_sz);

    UINTN total_entries = 0;
    while (1)
    {
        status = root_dir->Read(root_dir, &entry_sz, entry);
        if (!entry_sz)
            break;
        if (status == EFI_BUFFER_TOO_SMALL)
        {
            Print(L"Buffer is too small. Reallocating...\n");
            entry = erap_Memory_ReAlloc(EfiLoaderData, 0, entry, entry_sz);
            continue;
        }
        else if (EFI_ERROR(status))
        {
            Print(L"Failed to open entry!\n");
            return status;
        }

        EFI_FILE_INFO *file_info = (EFI_FILE_INFO *) entry;
        Print(L"Found a directory entry: [%s] which IS %sa directory.\n", file_info->FileName, (file_info->Attribute & EFI_FILE_DIRECTORY) ? L"" : L"NOT ");
        total_entries++;
    }
    Print(L"There are total %u entries.\n", total_entries);

    erap_Memory_Free(entry);

    Print(L"Done.\n");
    return EFI_SUCCESS;
}
