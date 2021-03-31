// Protocol/SimpleTextOut.h
typedef EFI_SIMPLE_TEXT_OUTPUT_MODE  SIMPLE_TEXT_OUTPUT_MODE;

typedef enum {
    EfiConsoleControlScreenText,
    EfiConsoleControlScreenGraphics,
    EfiConsoleControlScreenMaxValue
} EFI_CONSOLE_CONTROL_SCREEN_MODE;

#define MAX_DCSCREEN_PAGE          3
#define INFO_ITEM_STR_SIZE_100   100
#define INFO_ITEM_STR_SIZE_80     80
#define INFO_ITEM_STR_SIZE_20     20

#define ONE_PAGE_PCI_LIST         20

//
// EFI Scan codes 
//
#define EFI_SCAN_NULL           0x00
#define EFI_SCAN_UP             0x01
#define EFI_SCAN_DN             0x02
#define EFI_SCAN_RIGHT          0x03
#define EFI_SCAN_LEFT           0x04
#define EFI_SCAN_HOME           0x05
#define EFI_SCAN_END            0x06
#define EFI_SCAN_INS            0x07
#define EFI_SCAN_DEL            0x08
#define EFI_SCAN_PGUP           0x09
#define EFI_SCAN_PGDN           0x0A
#define EFI_SCAN_F1             0x0B
#define EFI_SCAN_F2             0x0C
#define EFI_SCAN_F3             0x0D
#define EFI_SCAN_F4             0x0E
#define EFI_SCAN_F5             0x0F
#define EFI_SCAN_F6             0x10
#define EFI_SCAN_F7             0x11
#define EFI_SCAN_F8             0x12
#define EFI_SCAN_F9             0x13
#define EFI_SCAN_F10            0x14
#define EFI_SCAN_F11            0x15
#define EFI_SCAN_F12            0x16
#define EFI_SCAN_ESC            0x17

#define CHAR_CARRIAGE_RETURN  0x000D

//
// Physical memory persistence attribute. 
// The memory region supports byte-addressable non-volatility.
//
#define EFI_MEMORY_NV               0x0000000000008000ULL
