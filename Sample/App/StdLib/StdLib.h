
//****************************************************************************/
//		FUNCTION DECLARATIONS
//****************************************************************************/
UINTN Sprintf_va_list( char *buffer, char *format, VA_LIST arg );
UINTN EFIAPI Sprintf(char *Buffer, char *Format, ...);
UINTN EFIAPI Sprintf_s(char *Buffer, UINTN BufferSize, char *Format, ...);
UINTN Sprintf_s_va_list(char *Buffer, UINTN BufferSize, char *Format, VA_LIST Marker);
UINTN Swprintf_va_list(CHAR16 *buffer, CHAR16 *format, VA_LIST arg);
UINTN EFIAPI Swprintf(CHAR16 *Buffer, CHAR16 *Format, ...);
UINTN EFIAPI Swprintf_s(CHAR16 *Buffer, UINTN BufferSize, CHAR16 *Format, ...);
UINTN Swprintf_s_va_list(CHAR16 *Buffer, UINTN BufferSize, CHAR16 *Format, VA_LIST Marker);

long StrtolEx(char *nptr,char **endptr,int base, int increment);
char * Strupr(char *str);
char * ItoaEx(INT64 value, char *string,int radix, BOOLEAN is_int64);
UINTN Strlen(char *);
CHAR16 * Wcsupr(CHAR16 *str);
CHAR16 * ItowEx(INT64 value, CHAR16 *string,int radix, BOOLEAN is_int64);

#define Strtol(nptr,endptr,base) StrtolEx(nptr,endptr,base,1)
#define I64toa(value,string,radix) ItoaEx(value,string,radix,TRUE)
#define Itoa(value,string,radix) ItoaEx((INT32)value,string,radix,FALSE)
#define Wcstol(nptr,endptr,base) StrtolEx((char*)nptr,(char**)endptr,base,2)
#define I64tow(value,string,radix) ItowEx(value,string,radix,TRUE)
#define Itow(value,string,radix) ItowEx((INT32)value,string,radix,FALSE)
