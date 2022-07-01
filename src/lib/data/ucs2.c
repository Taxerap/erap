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

#include "ucs2.h"

UINTN
erap_UCS2_StrLen( CONST CHAR16 *str )
{
    UINTN result = 0;

    while (*str++)
        result++;

    return result;
}

INTN
erap_UCS2_StrCmp( CONST CHAR16 *lhs, CONST CHAR16 *rhs )
{
    return 0;
}

CHAR16 *
erap_UCS2_StrChr( CONST CHAR16 *str, CHAR16 ch )
{
    CHAR16 target = ch;
    CONST CHAR16 *result = NULL;

    while ((target = *str) != u'\0')
    {
        if (target == ch)
        {
            result = str;
            break;
        }
        str++;
    }

    return (CHAR16 *) result;
}

CHAR16 *
erap_UCS2_StrRChr( CONST CHAR16 *str, CHAR16 ch )
{
    for (CONST CHAR16 *end = str + erap_UCS2_StrLen(str); end >= str; end--)
        if (*end == ch)
            return (CHAR16 *) end;

    return NULL;
}

CHAR16 *
erap_UCS2_StrStr( CONST CHAR16 *str, CONST CHAR16 *substr )
{
    CHAR16 *result = NULL;

    return result;
}

CHAR16 *
erap_UCS2_StrRStr( CONST CHAR16 *str, CONST CHAR16 *substr )
{
    CHAR16 *result = NULL;

    return result;
}
