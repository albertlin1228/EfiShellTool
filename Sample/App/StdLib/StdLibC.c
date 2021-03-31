
#include <StdLib.h>
#include <Base.h>
#include <Library/BaseLib.h>

#define true  TRUE
#define false FALSE


//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Strlen
//
// Description:
//  UINTN Strlen(IN CHAR8 *string) takes a null-terminated CHAR8 string and
// returns its UINTN length (not including the null-terminator).
//
// Input:
//  IN CHAR8 *string
// Pointer to a null-terminated CHAR8 string.
//
// Output:
//  UINTN length of the string (not including the null-terminator).
//
// Modified:
//
// Referrals:
// 
// Notes:   
// 
//<AMI_PHDR_END>
//*************************************************************************
UINTN Strlen(char *string) {
    UINTN length=0;
    while(*string++) length++;
    return length;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Wcslen
//
// Description:
//  UINTN Wcslen(IN CHAR16 *string) takes a null-terminated CHAR16 string and
// returns its UINTN length (not including the null-terminator).
//
// Input:
//  IN CHAR16 *string
// Pointer to a null-terminated CHAR16 string.
//
// Output:
//  UINTN length of the string (not including the null-terminator).
//
// Modified:
//
// Referrals:
// 
// Notes:   
// 
//<AMI_PHDR_END>
//*************************************************************************
UINTN Wcslen(CHAR16 *string) {
    UINTN length=0;
    while(*string++) length++;
    return length;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Wcsupr
//
// Description:
//  CHAR16* Wcsupr(IN CHAR16 *str) converts a CHAR16 string to all uppercase
// characters.
//
// Input:
//  IN CHAR16 *str
// CHAR16 string to be converted to all uppercase.
//
// Output:
//  CHAR16* string pointer to the start of the modified string.
//
// Modified:
//
// Referrals:
// 
// Notes:   
//  Original string will be modified!
// 
//<AMI_PHDR_END>
//*************************************************************************
CHAR16 * Wcsupr(CHAR16 *str) {
    CHAR16 *strptr=str;
    while (*strptr) {   //End of string?
        if (*strptr >=0x61 && *strptr<=0x7a) *strptr-=0x20; //If char is lower case, convert to upper.
        strptr++;       //Next char
    }
    return str;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Strupr
//
// Description:
//  CHAR8* Strupr(IN CHAR8 *str) converts a CHAR8 string to all uppercase
// characters.
//
// Input:
//  IN CHAR8 *str
// CHAR8 string to be converted to all uppercase.
//
// Output:
//  CHAR8* string pointer to the start of the modified string.
//
// Modified:
//
// Referrals:
// 
// Notes:   
//  Original string will be modified!
// 
//<AMI_PHDR_END>
//*************************************************************************
char * Strupr(char *str) {
    char *strptr=str;
    while (*strptr) {   //End of string?
        if (*strptr >=0x61 && *strptr<=0x7a) *strptr-=0x20; //If char is lower case, convert to upper.
        strptr++;       //Next char
    }
    return str;
}
//*************************************************************************
//
// Name: Sprintf_va_list
//
// Description:
//  UINTN Sprintf_va_list(OUT CHAR8 *buffer, IN CHAR8 *format,
// IN va_list arg) produces a null-terminated ASCII string in the output
// buffer.  The ASCII string is produced by parsing the format string
// specified by format.  Arguments are pulled from the variable argument
// list, specified by arg, based on the contents of the format string.  The
// number of ASCII characters in the produced output buffer is returned, not
// including the null-terminator.
//
// Input:
//  buffer - Pointer to a null-terminated output ASCII string buffer.  
//  User is responsible for allocating the necessary memory resources!
//
//  BufferSize - Size of the buffer in bytes.
//
//  format - Pointer to a null-terminated format ASCII string.
//
//  arg - Marker for the variable argument list.
//
// Output:
//  UINTN - number of ASCII characters in the produced output buffer, not
//  including the null-terminator.
//
// Referrals:
//  va_arg
//  Strtol
//  Sprintf
//  Strupr
//  I64toa
//  Itoa
//  Strlen
// 
// Notes: Refer to Sprintf function for format string syntax.
//
//*************************************************************************
UINTN Sprintf_va_list( char *buffer, char *format, VA_LIST arg )
{
    return Sprintf_s_va_list( buffer, 0, format, arg );
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Sprintf
//
// Description:
//  UINTN Sprintf(OUT CHAR8 *Buffer, IN CHAR8 *Format, IN ...) produces a
// null-terminated ASCII string in the output Buffer.  The ASCII string is
// produced by parsing the format string specified by Format.  Arguments are 
// pulled from the variable argument list based on the contents of the format
// string.  The number of ASCII characters in the produced output buffer is
// returned, not including the null-terminator. See notes for format string 
// information.
//
// Input:
//  OUT CHAR8 *Buffer
// Pointer to a null-terminated output ASCII string buffer.  User is
// responsible for allocating the necessary memory resources!
//
//  IN CHAR8 *Format
// Pointer to a null-terminated format ASCII string.
//
//  IN ...
// Variable argument list which provides the data/variables used within the
// format string.
//
// Output:
//  UINTN number of ASCII characters in the produced output buffer, not
// including the null-terminator.
//
// Modified:
//
// Referrals:
//  va_start
//  Sprintf_va_list
//  va_end
// 
// Notes:
//  Objects inside the format string have the following syntax.
//   %[flags][width]type
//
//  *** [flags] ***
//  
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
// . Flag  .  Description
// .       . 
// . -     .  The field is left justified.  If flag is not specified, then 
// .       . the field is right justified.
// .       . 
// . space .  Prefix a space character to a number.  Only valid for types X, 
// .       . x, and d.
// .       .  
// . +     .  Prefix a plus character to a number.  Only valid for types X,
// .       . x, and d.  If both space and `+' are specified, then space is
// .       . ignored.
// .       .
// . 0     .  Pad with `0' characters to the left of a number.  Only valid
// .       . for types X, x, and d.
// .       .
// . L, l  .  The number being printed is a UINT64.  Only valid for types X,
// .       . x, and d.  If this flag is not specified, then the number being
// .       . printed is an int.
// .       .
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
//  
//  NOTE
//   All invalid [flags] are ignored.
//
// *** [width] ***
//
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
// . Width  .  Description
// .        .
// . *      .  The width of the field is specified by a UINTN argument in the
// .        . argument list.
// .        .
// . Number .  The number specified as a decimal value represents the width of
// .        . the field.
// .        .
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
//
// NOTE
//  If [width] is not specified, then a field width of 0 is assumed.
//
// *** type ***
// 
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
// . Type  .  Description
// .       .
// . %     .  Print a `%'.
// .       .
// . c     .  The argument is an ASCII character. 
// .       .
// . x     .  The argument is a hexadecimal number.  The characters used are 
// .       . 0..9 and a..f.  If the flag `l' is not specified, then the
// .       . argument is assumed to be an int.
// .       .
// . X     .  The argument is a hexadecimal number.  The characters used are 
// .       . 0..9 and A..F.  If the flag `l' is not specified, then the
// .       . argument is assumed to be an int.
// .       .
// . d     .  The argument is a decimal number.  If the flag `l' is not
// .       . specified, then the argument is assumed to be an int.
// .       .
// . i     .  The same as `d'.
// .       .
// . s     .  The argument is a pointer to null-terminated ASCII string.
// .       .
// . a     .  The same as `s'.
// .       .
// . S     .  The argument is a pointer to a null-terminated Unicode string.
// .       .
// . g     .  The argument is a pointer to a GUID structure.  The GUID is
// .       . printed in the format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx.
// .       .
// . G     .  The argument is a pointer to a GUID structure.  The GUID is
// .       . printed in the format XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX.
// .       .
// . r     .  The argument is an EFI_STATUS value.  This value is converted
// .       . to a string.
// .       .
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
//
// NOTE
//  All invalid type characters are copied into the result string.
//
//<AMI_PHDR_END>
//*************************************************************************
UINTN EFIAPI Sprintf( char *Buffer, char *Format, ...)
{
    VA_LIST	ArgList;
    UINTN Ret;
    VA_START(ArgList,Format);
    Ret = Sprintf_va_list( Buffer, Format, ArgList );
    VA_END(ArgList);
    return Ret;
}

//*************************************************************************
//
// Name: Sprintf_s_va_list
//
// Description:
//  UINTN Sprintf_s_va_list(OUT CHAR8 *Buffer, IN UINTN BufferSize,
// IN CHAR8 *Format, IN va_list Marker) produces a null-terminated ASCII
// string in the output Buffer of size BufferSize.  The ASCII string is
// produced by parsing the format string specified by Format.  Arguments are
// pulled from the variable argument list, Marker, based on the contents of
// the format string.  The number of ASCII characters in the produced output
// buffer is returned, not including the null-terminator.
//
// Input:
//  OUT CHAR8 *Buffer
// Pointer to a null-terminated output ASCII string buffer.  User is
// responsible for allocating the necessary memory resources!
//
//  IN UINTN BufferSize
// The size, in bytes, of the output Buffer.
//
//  IN CHAR8 *Format
// Pointer to a null-terminated format ASCII string.
//
//  IN va_list Marker
// Marker for the variable argument list.
//
// Output:
//  UINTN number of ASCII characters in the produced output buffer, not
// including the null-terminator.
//
// Modified:
//
// Referrals:
//  Sprintf_va_list
//  MemCpy
// 
// Notes:	
//  This is a helper function for Sprintf_s.  Refer to Sprintf function for
// format string syntax.
// 
//*************************************************************************
UINTN Sprintf_s_va_list(char *Buffer, UINTN BufferSize, char *Format, VA_LIST Marker)
{
	char filler;
	int width;
	char numbuffer[32];
	UINTN strlength;
	char *strp = Format;
	char *buffp = Buffer;
	char *_strp, *str2p;
	int	radix;
    BOOLEAN its_int64;
    UINTN n;

    if (Buffer==NULL || Format==NULL) return -1;
    //If BuuferSize is 0, no size check required
    while(BufferSize!=1 && *strp) {
        if (*strp != '%'){
            *buffp++ = *strp++;
            BufferSize--;
            continue;
        }
        strp++; //skip %
        if (*strp=='%') {
            strp++;
            *buffp++ = '%';
            BufferSize--;
            continue;
        }
        
        filler = ' ';
    
        //Get flags
        if (*strp == '0') { 
            filler = '0'; 
            strp++;
        }

        //Get Width
        if (*strp == '*') { //width is supplied by argument.
            strp++;
            width = VA_ARG(Marker,int);
        } else {
            width = Strtol(strp,&_strp,10);	//If no width, then 0 returned.
            strp=_strp;
        }
    
        //Get type.
        if (*strp == 's' || *strp == 'a'/*to be compatible with Intel Print library*/) { //string
            char *str = VA_ARG(Marker,char *);
            while (*str) {
                if ( --BufferSize == 0 ) {
                    *buffp = 0; // null terminator
                    return buffp - Buffer;
                }
                *buffp++ = *str++;
            }
            ++strp;
            continue;
        }
        
        if (*strp == 'S') { // unicode string
            CHAR16 *str = VA_ARG(Marker,CHAR16 *);
            while (*str) {
                if ( --BufferSize == 0 ) {
                    *buffp = 0; // null terminator
                    return buffp - Buffer;
                }
                *buffp++ = (char)*str++;
            }
            ++strp;
            continue;
        }
        
        if (*strp == 'c') { //character
            *buffp++ = VA_ARG(Marker,char);
            ++strp;
            continue;
        }
        
        if ((*strp & 0xdf) == 'G') {     //'G' or 'g'
            EFI_GUID *guid = VA_ARG(Marker,EFI_GUID*);
            CHAR8 *origp = buffp;
            UINT32 Data32Guid;
            UINT16 Data16Guid1;
            UINT16 Data16Guid2;

            Data32Guid = ReadUnaligned32(&(guid->Data1));
            Data16Guid1 = ReadUnaligned16(&(guid->Data2));
            Data16Guid2 = ReadUnaligned16(&(guid->Data3));
            n = Sprintf_s( 
                buffp,
                BufferSize,
                "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                Data32Guid, Data16Guid1, Data16Guid2, guid->Data4[0],
                guid->Data4[1], guid->Data4[2], guid->Data4[3], guid->Data4[4],
                guid->Data4[5], guid->Data4[6], guid->Data4[7]
            );
            buffp += n;
            if (*strp == 'G') 
                Strupr(origp);
            BufferSize -= n;
            ++strp;
            continue;
        }
#if 0        
        if (*strp == 'r') {
            EFI_STATUS Status = VA_ARG(Marker,EFI_STATUS);
            char *StatusCodeString = GetStatusCodeString(Status);

            if (StatusCodeString==NULL)
                n = Sprintf_s( buffp, BufferSize, "%s(%X)", EfiUnknown, Status );
            else
                n = Sprintf_s( buffp, BufferSize, "%s", StatusCodeString );                

            buffp += n;
            BufferSize -= n;
            ++strp;
            continue;
        }
#endif        
        if (*strp == 'l') { 
            strp++; 
            its_int64 = TRUE;
        } else 
            its_int64 = FALSE;
		//Aptio V: we are adding %p support to be compatible with EDKII code
        if (*strp == 'p' && sizeof(VOID*) == 8) {
       		its_int64 = 1;
        }
        if (*strp == 'd' || *strp == 'i') 
            radix = 10;
        else if ( (*strp & 0xdf) == 'X' || *strp == 'p' )
            radix = 16; //'X' or 'x' or 'p'
        else 
            continue;	//invalid *strp
        
        if (its_int64) 
            I64toa( VA_ARG(Marker,INT64), numbuffer, radix );
        else 
            Itoa( VA_ARG(Marker,int), numbuffer, radix );
        if (*strp == 'X' || *strp == 'p') 
            Strupr(numbuffer);
        
        strlength = Strlen(numbuffer);
        while ( strlength++ < (unsigned)width ) {
            if ( --BufferSize == 0 ) {
                *buffp = 0; // null terminator
                return buffp - Buffer;
            }
            *buffp++ = filler;
        }
        
        str2p = numbuffer;
        while (*str2p) {
            if ( --BufferSize == 0 ) {
                *buffp = 0; // null terminator
                return buffp - Buffer;
            }
            *buffp++ = *str2p++;
        }
        
        strp++;
    }
    
    *buffp = 0;
    return buffp - Buffer;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Sprintf_s
//
// Description:
//  UINTN Sprintf_s(OUT CHAR8 *Buffer, IN UINTN BufferSize, IN CHAR8 *Format,
// IN ...) produces a null-terminated ASCII string in the output Buffer of
// size BufferSize.  The ASCII string is produced by parsing the format
// string specified by Format.  Arguments are pulled from the variable
// argument list based on the contents of the format string.  The number of
// ASCII characters in the produced output buffer is returned, not including
// the null-terminator.
//
// Input:
//  OUT CHAR8 *Buffer
// Pointer to a null-terminated output ASCII string buffer.  User is
// responsible for allocating the necessary memory resources!
//
//  IN UINTN BufferSize
// The size, in bytes, of the output Buffer.
//
//  IN CHAR8 *Format
// Pointer to a null-terminated format ASCII string.
//
//  IN ...
// Variable argument list of data/variables used within the format string.
//
// Output:
//  UINTN number of ASCII characters in the produced output buffer, not
// including the null-terminator.
//
// Modified:
//
// Referrals:
//  va_start
//  Sprintf_s_va_list
//  va_end
// 
// Notes:	
//  Refer to Sprintf function for format string syntax.
// 
//<AMI_PHDR_END>
//*************************************************************************
UINTN EFIAPI Sprintf_s( char *Buffer, UINTN BufferSize, char *Format, ... )
{
    VA_LIST ArgList;
    UINTN Ret;
    VA_START(ArgList, Format);
    Ret = Sprintf_s_va_list( Buffer, BufferSize, Format, ArgList );
	VA_END(ArgList);
	return Ret;
}

//*************************************************************************
//
// Name: Swprintf_va_list
//
// Description:
//  UINTN Swprintf_va_list(OUT CHAR16 *buffer, IN CHAR16 *format,
// IN va_list arg) produces a null-terminated Unicode string in the output
// buffer.  The Unicode string is produced by parsing the format string
// specified by format.  Arguments are pulled from the variable argument list
// specified by arg based on the contents of the format string.  The number 
// of Unicode characters in the produced output buffer is returned, not
// including the null-terminator.
//
// Input:
//  OUT CHAR16 *buffer
// Pointer to a null-terminated output Unicode string buffer.  User is
// responsible for allocating the necessary memory resources!
//
//  IN CHAR16 *format
// Pointer to a null-terminated format Unicode string.
//
//  IN va_list arg
// Marker for the variable argument list.
//
// Output:
//  UINTN number of Unicode characters in the produced output buffer, not
// including the null-terminator.
//
// Modified:
//
// Referrals:
//  VA_ARG
//  Wcstol
//  Swprintf
//  Wcsupr
//  I64tow
//  Itow
//  Wcslen
// 
// Notes:	
//  Refer to Sprintf function for format string syntax.
// 
//*************************************************************************
UINTN Swprintf_va_list(CHAR16 *buffer, CHAR16 *format, VA_LIST arg)
{
    return Swprintf_s_va_list( buffer, 0, format, arg );
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Swprintf
//
// Description:
//  UINTN Swprintf(OUT CHAR16 *Buffer, IN CHAR16 *Format, IN ...) produces a
// null-terminated Unicode string in the output Buffer. The Unicode string is
// produced by parsing the format string specified by Format.  Arguments are
// pulled from the variable argument list based on the contents of the format
// string.  The number of Unicode characters in the produced output buffer is
// returned, not including the null-terminator.  
//
// Input:
//  OUT CHAR16 *Buffer
// Pointer to a null-terminated output Unicode string buffer.  User is
// responsible for allocating the necssary memory resources!
//
//  IN CHAR16 *Format
// Pointer to a null-terminated format Unicode string.
//
//  IN ...
// Variable arguement list of data/variables used within the format string.
//
// Output:
//  UINTN number of Unicode characters in the produced output buffer, not
// including the null-terminator.
//
// Modified:
//
// Referrals:
//  va_start
//  Swprintf_va_list
//  va_end
// 
// Notes:	
//  See Sprintf function for format string details.
//
//<AMI_PHDR_END>
//*************************************************************************
UINTN EFIAPI Swprintf(CHAR16 *Buffer, CHAR16 *Format, ...)
{
    VA_LIST ArgList;
    UINTN Ret;
    VA_START(ArgList,Format);
    Ret = Swprintf_va_list(Buffer,Format,ArgList);
	VA_END(ArgList);
	return Ret;
}

//*************************************************************************
//
// Name: Swprintf_s_va_list
//
// Description:
//  UINTN Swprintf_s_va_list(OUT CHAR16 *Buffer, IN UINTN BufferSize,
// IN CHAR16 *Format, IN va_list Marker) produces a null-terminated Unicode
// string in the output Buffer of size BufferSize.  The Unicode string is
// produced by parsing the format string specified by Format.  Arguments are
// pulled from the variable argument list based on the contents of the format
// string.  The number of Unicode characters in the produced output buffer is 
// returned, not including the null-terminator.
//
// Input:
//  OUT CHAR16 *Buffer
// Pointer to a null-terminated output Unicode string buffer.  User is
// responsible for allocating the necessary memory resources!
//
//  IN UINTN BufferSize
// The size, in bytes, of the output Buffer.
//
//  IN CHAR16 *Format
// Pointer to a null-terminated format Unicode string.
//
//  IN va_list Marker
// Marker for the variable argument list.
//
// Output:
//  UINTN number of ASCII characters in the produced output buffer, not
// including the null-terminator.
//
// Modified:
//
// Referrals:
//  Swprintf_va_list
//  MemCpy
// 
// Notes:	
//  This is a helper function for Swprintf_s.  Refer to Sprintf function for
// format string syntax.
// 
//*************************************************************************
UINTN Swprintf_s_va_list(CHAR16 *Buffer, UINTN BufferSize, CHAR16 *Format, VA_LIST Marker)
{
    CHAR16 filler;
    int width;
    CHAR16 numbuffer[32];
    UINTN strlength;
    CHAR16 *strp = Format;
    CHAR16 *buffp = Buffer;
    CHAR16 *_strp, *str2p;
    int radix;
    BOOLEAN its_int64;
    UINTN n;

    if (Buffer==NULL || Format==NULL) return -1;
    //If BuuferSize is 0, no size check required
    while(BufferSize!=1 && *strp) {
        if (*strp != '%'){
            *buffp++ = *strp++;
            BufferSize--;
            continue;
        }
        strp++; //skip %
        if (*strp=='%') {
            strp++;
            *buffp++ = '%';
            BufferSize--;
            continue;
        }
        
        filler = ' ';
    
        //Get flags
        if (*strp == '0') { 
            filler = '0'; 
            strp++;
        }

        //Get Width
        if (*strp == '*') { //width is supplied by argument.
            strp++;
            width = VA_ARG(Marker, int);
        } else {
            width = Wcstol( strp, &_strp, 10 );    //If no width, then 0 returned.
            strp=_strp;
        }
    
        //Get type.
        if (*strp == 's') { // unicode string
            CHAR16 *str = VA_ARG(Marker, CHAR16*);
            while (*str) {
                if ( --BufferSize == 0 ) {
                    *buffp = 0; // null terminator
                    return buffp - Buffer;
                }
                *buffp++ = *str++;
            }
            ++strp;
            continue;
        }

        if ( *strp == 'S' || *strp == 'a' /*to be compatible with Intel Print library*/ ) { //string
            char *str = VA_ARG(Marker, char *);
            while (*str) {
                if ( --BufferSize == 0 ) {
                    *buffp = 0; // null terminator
                    return buffp - Buffer;
                }
                *buffp++ = *str++;
            }
            ++strp;
            continue;
        }

        if (*strp == 'c') { //character
            *buffp++ = VA_ARG(Marker, CHAR16);
            ++strp;
            continue;
        }

        if ((*strp & 0xdf) == 'G') {     //'G' or 'g'
            EFI_GUID *guid = VA_ARG(Marker, EFI_GUID*);
            CHAR16 *origp = buffp;
            n = Swprintf_s(
                buffp, 
                BufferSize,
                L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                guid->Data1, guid->Data2, guid->Data3, guid->Data4[0],
                guid->Data4[1], guid->Data4[2], guid->Data4[3], guid->Data4[4],
                guid->Data4[5], guid->Data4[6], guid->Data4[7]
            );
            buffp += n;
            if (*strp == 'G') 
                Wcsupr(origp);
            BufferSize -= n;
            ++strp;
            continue;
        }
#if 0
        if (*strp == 'r') {
            EFI_STATUS Status = VA_ARG(Marker, EFI_STATUS);
            char *StatusCodeString = GetStatusCodeString(Status);

            if (StatusCodeString==NULL)
                n = Swprintf_s( buffp, BufferSize, L"%S(%X)", EfiUnknown, Status );
            else
                n = Swprintf_s( buffp, BufferSize, L"%S", StatusCodeString );                

            buffp += n;
            BufferSize -= n;
            ++strp;
            continue;
        }
#endif
        if (*strp == 'l') { 
            strp++; 
            its_int64 = TRUE;
        } else 
            its_int64 = FALSE;
		//Aptio V: we are adding %p support to be compatible with EDKII code
        if (*strp == 'p' && sizeof(VOID*) == 8) {
       		its_int64 = 1;
        }    
        if (*strp == 'd' || *strp == 'i') 
            radix = 10;
        else if ((*strp & 0xdf) == 'X') 
            radix = 16;  //'X' or 'x'
        else 
            continue;    //invalid *strp

        if (its_int64) 
            I64tow( VA_ARG(Marker,INT64), numbuffer, radix );
        else 
            Itow( VA_ARG(Marker,int), numbuffer, radix );
        if (*strp == 'X' || *strp == 'p') 
            Wcsupr(numbuffer);

        strlength = Wcslen(numbuffer);
        while ( strlength++ < (unsigned)width ) {
            if ( --BufferSize == 0 ) {
                *buffp = 0; // null terminator
                return buffp - Buffer;
            }
            *buffp++ = filler;
        }

        str2p = numbuffer;
        while (*str2p) {
            if ( --BufferSize == 0 ) {
                *buffp = 0; // null terminator
                return buffp - Buffer;
            }
            *buffp++ = *str2p++;
        }
            
        strp++;
    }
    
    *buffp = 0;
    return buffp - Buffer;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Swprintf_s
//
// Description:
//  UINTN Swprintf_s(OUT CHAR16 *Buffer, IN UINTN BufferSize,
// IN CHAR16 *Format, IN ...) produces a null-terminated Unicode string in
// the output Buffer of size BufferSize.  The Unicode string is produced by
// parsing the format string specified by Format.  Arguments are pulled from
// the variable argument list based on the contents of the format string.
// The number of Unicode characters in the produced output buffer is
// returned, not including the null-terminator.
//
// Input:
//  OUT CHAR16 *Buffer
// Pointer to a null-terminated output Unicode string buffer.  User is
// responsible for allocating the necessary memory resources!
//
//  IN UINTN BufferSize
// The size, in bytes, of the output Buffer.
//
//  IN CHAR16 *Format
// Pointer to a null-terminated format Unicode string.
//
//  IN ...
// Variable argument list of data/variables used within the format string.
//
// Output:
//  UINTN number of Unicode characters in the produced output buffer, not
// including the null-terminator.
//
// Modified:
//
// Referrals:
//  va_start
//  Swprintf_s_va_list
//  va_end
// 
// Notes:	
//  Refer to Sprintf function for format string syntax.
// 
//<AMI_PHDR_END>
//*************************************************************************
UINTN EFIAPI Swprintf_s(CHAR16 *Buffer, UINTN BufferSize, CHAR16 *Format, ...)
{
    VA_LIST ArgList;
    UINTN Ret;
    VA_START(ArgList,Format);
    Ret = Swprintf_s_va_list(Buffer,BufferSize,Format,ArgList);
	VA_END(ArgList);
	return Ret;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: StrtolEx
//
// Description:
//  INT64 StrtolEx(IN CHAR8 *nptr, OUT CHAR8 **endptr, IN INT32 base,
// IN INT32 increment) converts a CHAR8 string representation of a number
// into an INT64 representation.
//
// Input:
//  IN CHAR8 *nptr
// CHAR8 string to be converted into an INT64.
//
//  OUT CHAR8 **endptr
// CHAR8 string pointer to the final character read from nptr.
//
//  IN INT32 base
// The base of the string being passed in.  If 0, string will be parsed by
// defaults; preceding "0x" treated as hexadecimal, preceding "0" treated as
// octal, everything else treated as decimal.
//
//  IN INT32 increment
// The number of characters in between each number digit.  For example,
// "14131" would be 1; "1 4 1 3 1" would be 2.
//
// Output:
//  INT64 representation of the number in the ASCII nptr string using the
// provided base and increment amount.
//
// Modified:
//
// Referrals:
// 
// Notes:   
// 
//<AMI_PHDR_END>
//*************************************************************************
long StrtolEx(char *nptr,char **endptr,int base, int increment) {
    unsigned char overflow=false;
    char sign=1;
    char digit;
    long value=0;

    while(*nptr == ' ' || *nptr == '\t') nptr+=increment;
    if (*nptr == '\0') {*endptr=(char*)nptr;return 0;}

    if (*nptr == '-') {sign = -1;nptr+=increment;}
    if (*nptr == '+') nptr+=increment;

    if (!base) {
        base=10;
        if (*nptr == '0') {
            base=8;
            nptr+=increment;
            if ((*nptr&0xdf)=='X') {    //Check for 'x' or 'X'
                base=16;
                nptr+=increment;
            }
        }
    }
    while(true) {
        if (*nptr >= '0' && *nptr <='9') digit=*nptr-'0';
        else if ((*nptr&0xdf)>='A'&& (*nptr&0xdf)<='Z') digit = (*nptr&0xdf) - 'A' + 0xa;
        else break;
        if (digit >= base) break;
        value = value * base + digit;
        if (sign==1) {
            if ((unsigned) value >= (unsigned) 0x80000000) overflow=true;
        } else if ((unsigned) value > (unsigned) 0x80000000) overflow=true;
        nptr+=increment;
    }
    *endptr=(char*)nptr;
    if (overflow) {
        value=0x7fffffff;
        if (sign==-1) value++;
    }
    return value*sign;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: Div64
//
// Description:
//  UINT64 Div64(IN UINT64 Dividend, IN UINTN Divisor, 
// OUT UINTN *Remainder OPTIONAL) divides a 64-bit number, Dividend, by the
// Divisor, which can be up to 31-bits.
//
// Input:
//  IN UINT64 Dividend
// The 64-bit number to be divided.
//
//  IN UINT Divisor
// The number to divide Dividend by; may not exceed 31-bits in size.
//
//  OUT UINTN *Remainder OPTIONAL
// The remainder of the division.  Provide NULL if undesired; otherwise user
// is responsible for handling the necessary memory resources.
//
// Output:
//  UINT64 result of dividing Dividend by Divisor.
//
// Modified:
//
// Referrals:
// 
// Notes:   
// 
//<AMI_PHDR_END>
//*************************************************************************
UINT64 Div64 (
    IN UINT64   Dividend,
    IN UINTN    Divisor,    //Can only be 31 bits.
    OUT UINTN   *Remainder OPTIONAL
    )
{
    UINT64  Result = Dividend/Divisor;
    if (Remainder) *Remainder=Dividend%Divisor;
    return Result;
}

//*************************************************************************
//
// Name: ItoaHelper
//
// Description:
//  CHAR8* ItoaHelper(IN INT64 value, IN OUT CHAR8 *string, IN INT32 radix,
// IN BOOLEAN is_int64) is a helper function for ItoaEx and ItowEx which 
// converts an INT64 value into a CHAR8 ASCII representation in reverse
// order.
//
// Input:
//  IN INT64 value
// Value to be converted.
//
//  IN OUT CHAR8 *string
// Pointer to a CHAR8 string buffer that will hold the result of the reverse
// conversion.  User is responsible for allocating the necessary memory
// resources.
//
//  IN INT32 radix
// Radix of the conversion.
//
//  IN BOOLEAN is_int64
// TRUE if value is a 64-bit integer; FALSE if it's 32-bit.
//
// Output:
//  CHAR8* string pointer to the last character added to the input string, 
// which now contains an ASCII string that is the reverse of what value 
// represents.
//
// Modified:
//
// Referrals:
//  Div64
// 
// Notes:   
// 
//*************************************************************************
char * ItoaHelper(INT64 value, char *string,int radix, BOOLEAN is_int64) {
    UINTN digit;
    UINT64 v = (value>=0) 
               ? (UINT64) value
               : (radix==10) 
                 ? (UINT64)-value 
                 : (is_int64) 
                   ? (UINT64)value 
                   : (UINT32)value;
    if (v)
        while (v) {
            #ifdef MDE_CPU_ARM
                v = DivU64x32Remainder(v,radix,&digit);
            #else
                v = Div64(v,radix,&digit);
            #endif // MDE_CPU_ARM
            if (digit<0xa) *string=(char)(digit+'0');
            else *string=(char)(digit-0xa+'a');
            string++;
        }
    else *string++='0';
    if (radix==10 && value<0) *string++='-';
    *string--=0;
    return string;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: ItowEx
//
// Description:
//  CHAR16* ItowEx(IN INT64 value, IN OUT CHAR16 *string, IN INT32 radix, 
// IN BOOLEAN is_int64) converts an INT64 value into a 16-bit Unicode 
// representation and returns a CHAR16 pointer to the string.
//
// Input:
//  IN INT64 value
// Value that needs to be converted into an ASCII representation.
//
//  IN OUT CHAR16 *string
// Pointer to a CHAR8 string buffer which will contain the result of the
// conversion.
//
//  IN INT32 radix
// Radix of the conversion.  For example, 10 for decimal, 16 for hexadecimal,
// etc.
//
//  IN BOOLEAN is_int64
// TRUE if value is a 64-bit integer; FALSE if it's 32-bit.
//
// Output:
//  CHAR16* string pointer, string, that contains the 16-bit Unicode
// representation of value in the user requested radix.
//
// Modified:
//
// Referrals:
//  ItoaHelper
// 
// Notes:   
// 
//<AMI_PHDR_END>
//*************************************************************************
CHAR16 * ItowEx(INT64 value, CHAR16 *string,int radix, BOOLEAN is_int64) {
    char s[0x100];
    CHAR16 *str = string;
    char *send=ItoaHelper(value,(char*)s,radix,is_int64);
    //convert to unicode
    while(send>=s) {
        *str++ = *send--;
    }
    *str=0;
    return string;
}

//*************************************************************************
//<AMI_PHDR_START>
//
// Name: ItoaEx
//
// Description:
//  CHAR8* ItoaEx(IN INT64 value, IN OUT CHAR8 *string, IN INT32 radix, 
// IN BOOLEAN is_int64) converts an INT64 value into a CHAR8 ASCII 
// representation and returns a pointer to the ASCII string.
//
// Input:
//  IN INT64 value
// Value that needs to be converted into an ASCII representation.
//
//  IN OUT CHAR8 *string
// Pointer to a CHAR8 string buffer which will contain the result of the
// conversion.  User is responsible for allocating the necessary memory
// resources.
//
//  IN INT32 radix
// Radix of the conversion.  For example, 10 for decimal, 16 for hexadecimal,
// etc.
//
//  IN BOOLEAN is_int64
// TRUE if value is a 64-bit integer; FALSE if it's 32-bit.
//
// Output:
//  CHAR8* string pointer, string, that contains the ASCII representation of
// value in the user requested radix.
//
// Modified:
//
// Referrals:
//  ItoaHelper
// 
// Notes:   
// 
//<AMI_PHDR_END>
//*************************************************************************
char * ItoaEx(INT64 value, char *string,int radix, BOOLEAN is_int64) {
    char *strp=string;
    char *str2p=ItoaHelper(value,strp,radix,is_int64);
    //reverse string
    while(strp<str2p) {
        char temp=*strp;
        *strp=*str2p;
        *str2p=temp;
        strp++;str2p--;
    }
    return string;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2009, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
