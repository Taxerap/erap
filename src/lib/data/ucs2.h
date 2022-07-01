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

#ifndef ERAP_LIB_DATA_UCS2_H
#define ERAP_LIB_DATA_UCS2_H

#include <Uefi.h>

UINTN
erap_UCS2_StrLen( CONST CHAR16 *str );

INTN
erap_UCS2_StrCmp( CONST CHAR16 *lhs, CONST CHAR16 *rhs );

CHAR16 *
erap_UCS2_StrChr( CONST CHAR16 *str, CHAR16 ch );

CHAR16 *
erap_UCS2_StrRChr( CONST CHAR16 *str, CHAR16 ch );

CHAR16 *
erap_UCS2_StrStr( CONST CHAR16 *str, CONST CHAR16 *substr );

CHAR16 *
erap_UCS2_StrRStr( CONST CHAR16 *str, CONST CHAR16 *substr );

#endif // !ERAP_LIB_DATA_UCS2_H
