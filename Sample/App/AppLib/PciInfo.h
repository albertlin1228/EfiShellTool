#define MAX_BUS 256
#define MAX_DEV 32
#define MAX_FUN 8
#define PCI_ACCESS_ENABLE_BIT 0x80000000
#define PCI_INDEX_PORT 0xCF8
#define PCI_DATA_PORT 0xCFC
#define CACHE_LINE_SIZE 0xC

//forward declaration
typedef struct _PCI_DEV_LIST PCI_DEV_LIST;

typedef struct _PCI_DEV_LIST {
    UINT32   PFA;
    UINT16   Bus;
    UINT8    Dev;
    UINT8    Fun;
    UINT16   VendorId;
    UINT16   DeviceId;
    UINT8    IsMulFun;
}PCI_DEV_LIST;
