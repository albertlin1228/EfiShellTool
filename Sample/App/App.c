#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/Smbios.h>
#include <PiDxe.h>
#include <Library/PciLib.h>
#include <Library/DxeServicesTableLib.h>
#include <StdLib/StdLib.h>
#include <App.h>
#include <AppLib/PciInfo.h>

UINTN gRow = 0; //The position to set the Row cursor to. 
UINTN gColumn = 0; //The position to set the Column cursor to.
UINTN gMaxRow = 0;
UINTN gMaxColumn = 0;
UINTN gDisplayMode;
CHAR16 *gScreenBuffer = NULL;
CHAR16 *gStringLine;
EFI_INPUT_KEY gInputKey;

VOID SwitchScreen()
{
    gST->ConOut->ClearScreen (gST->ConOut);
    gST->ConOut->EnableCursor(gST->ConOut,TRUE);    
    gST->ConOut->SetAttribute(gST->ConOut,EFI_TEXT_ATTR(EFI_WHITE,EFI_BLUE));
    gST->ConOut->SetCursorPosition( gST->ConOut,0,gRow);
}

VOID DisplayString(
  UINTN Column,
  UINTN Font,
  UINTN BackGroundColor
)
{
    gST->ConOut->SetAttribute(gST->ConOut,EFI_TEXT_ATTR(Font,BackGroundColor));
    gST->ConOut->SetCursorPosition( gST->ConOut,Column,gRow);
    gST->ConOut->OutputString(gST->ConOut,gStringLine);
    gBS->FreePool (gStringLine);
}

VOID
ListMainItem()
{
    gRow=0;
    
    gStringLine = AllocateZeroPool (0x100);
    Swprintf(gStringLine,L"1. List PCI device");
    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gRow=1;
    
    gStringLine = AllocateZeroPool (0x100);    
    Swprintf(gStringLine,L"2. Search IO port");
    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gRow=2;
    
    gStringLine = AllocateZeroPool (0x100);    
    Swprintf(gStringLine,L"3. Search MMIO address");
    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gRow=3;
    
    gStringLine = AllocateZeroPool (0x100);    
    Swprintf(gStringLine,L"4. List SMBIOS table");
    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gRow=0;
    
}

VOID
MovePage(
  UINT8         PageIndex,
  UINT8         MaxPage,
  PCI_DEV_LIST  *PciDevList,
  UINT16        PciDevCount
)
{
    UINT8  Index;
    
    gRow=0;
    
    SwitchScreen();
    
    gStringLine = AllocateZeroPool (0x100);
    
    StrCat(gStringLine,L"  No.  Bus  Dev  Fun    VID    DID");
    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    for(Index=(ONE_PAGE_PCI_LIST * PageIndex);Index < (ONE_PAGE_PCI_LIST * PageIndex + ONE_PAGE_PCI_LIST);Index++)
    {
        if(Index < PciDevCount)
        {
            gRow++;
            gStringLine = AllocateZeroPool (0x100);
        
            Swprintf(gStringLine,L"  %03d   %02X   %02X   %02X   %02X   %02X",\
                                Index,
                                PciDevList[Index].Bus,\
                                PciDevList[Index].Dev,\
                                PciDevList[Index].Fun,\
                                PciDevList[Index].VendorId,\
                                PciDevList[Index].DeviceId
                                );

            DisplayString(0,EFI_WHITE,EFI_BLUE);
        }
    }
    
    gRow++;
    
    gStringLine = AllocateZeroPool (0x100);
    
    Swprintf(gStringLine,L"Page/MaxPage:%d/%d",PageIndex,MaxPage);

    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gST->ConOut->SetCursorPosition( gST->ConOut,4,1);

}

VOID
TransformType(
  IN   UINT32 RegVal,
  IN   UINT8  Type,
  OUT  UINT8  *Reg0,
  OUT  UINT8  *Reg1,
  OUT  UINT8  *Reg2,
  OUT  UINT8  *Reg3
)
{
    if(Type == 8)
    {
        *Reg0=(RegVal & 0xff000000)>>24;
        *Reg1=(RegVal & 0xff0000)>>16;
        *Reg2=(RegVal & 0xff00)>>8;
        *Reg3=(RegVal & 0xff)>>0;
    }
}

BOOLEAN
IsPcieDev(
  IN  UINT8         PciReg[][16]
)
{
    UINT8 x=0,y=0;
    UINT8 Offset=0,CapabilityOffset=0,CapId=0,ReturnVal=FALSE;
    
    CapabilityOffset=PciReg[3][4];
    
    do 
    {
        x=CapabilityOffset/16;
        y=CapabilityOffset%16;
        
        CapId = PciReg[x][y];
        
        if (CapId == 0x10)//Pcie dev 
        {
            ReturnVal=TRUE;
            break;
        }
        
        y=y++;
        
        CapabilityOffset = PciReg[x][y];
    } 
    while (CapabilityOffset != 0);
    
    return ReturnVal;
    
}

/*
VOID
DisplayPcieDevice(
  IN  UINT8         PcieDev,
  IN  UINT8         PageIndex,
  IN  UINT8         MaxPage,
  IN  PCI_DEV_LIST  *PciDevList,
  IN  UINT16        PciDevCount,
  IN  UINTN         NowList,
  OUT UINT8         PciReg[][16]
)
{
    UINTN EventIndex=0;
            

}
*/

VOID
DisplayPciReg(
  IN  UINT8         PageIndex,
  IN  UINT8         MaxPage,
  IN  PCI_DEV_LIST  *PciDevList,
  IN  UINT16        PciDevCount,
  IN  UINTN         NowList,
  OUT UINT8         PciReg[][16],
  OUT BOOLEAN       *IsPcie
)
{
    UINT8 Index=0,x=0,y=0,Reg0=0,Reg1=0,Reg2=0,Reg3=0;
    UINT32 RegVal=0;
            
    gRow=0;
    SwitchScreen();
    gStringLine = AllocateZeroPool (0x100);
    Swprintf(gStringLine,L"    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gRow=1;
    gStringLine = AllocateZeroPool (0x100);
    Swprintf(gStringLine,L"======================================================================");
    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gRow=2;
    for(x=0;x<=0xF;x++)
    {
        gStringLine = AllocateZeroPool (0x100);
        
        Swprintf(gStringLine,L"%X0",x);
        
        DisplayString(0,EFI_WHITE,EFI_BLUE);
        
        for(y=0;y<0x4;y++)
        {    
            gStringLine = AllocateZeroPool (0x100);

            RegVal=MmioRead32((PciDevList[NowList].PFA)+(16*x)+(4*y));
            
            TransformType(RegVal,8,&Reg3,&Reg2,&Reg1,&Reg0);
            
            Swprintf(gStringLine,L"%02X %02X %02X %02X",Reg0,Reg1,Reg2,Reg3);
            
            DisplayString(12*y+4,EFI_WHITE,EFI_BLUE);
            
            PciReg[x][4*y]=Reg0;
            PciReg[x][4*y+1]=Reg1;
            PciReg[x][4*y+2]=Reg2;
            PciReg[x][4*y+3]=Reg3;
#if 0            
            Swprintf(gStringLine,L"%08X",RegVal);            
            
            DisplayString(10*y+4);
#endif            
        }
        
        gRow++;
    }
    gStringLine = AllocateZeroPool (0x100);
    Swprintf(gStringLine,L"Bus:%02X  Dev:%02X  Fun:%02X  Starting Address:0x%8X",\
            PciDevList[NowList].Bus,\
            PciDevList[NowList].Dev,\
            PciDevList[NowList].Fun,\
            PciDevList[NowList].PFA
            );
    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gRow++;
    gStringLine = AllocateZeroPool (0x100);
    *IsPcie=IsPcieDev(PciReg);
    Swprintf(gStringLine,L"CapId:%02X PCIe:%x",PciReg[3][4],*IsPcie);
    DisplayString(0,EFI_WHITE,EFI_BLUE);
}

VOID 
DisplayPci(
  IN  UINT8         PageIndex,
  IN  UINT8         MaxPage,
  IN  PCI_DEV_LIST  *PciDevList,
  IN  UINT16        PciDevCount,
  IN  UINTN         NowList,
  OUT UINT8         PciReg[][16]
)
{
    BOOLEAN IsPcie=FALSE;
    UINT32 RegVal=0;
    UINTN EventIndex=0;
   
    NowList=ONE_PAGE_PCI_LIST * PageIndex + NowList;

    DisplayPciReg(PageIndex,MaxPage,PciDevList,PciDevCount,NowList,PciReg,&IsPcie);
    
    if(IsPcie == 1 )
    {
        gRow++;
        gStringLine = AllocateZeroPool (0x100);
        Swprintf(gStringLine,L"Next  Previous");
        DisplayString(0,EFI_WHITE,EFI_BLUE);
         
        MaxPage=16;
        PageIndex=0;
        
        do
        {   
            if (gInputKey.ScanCode == EFI_SCAN_PGDN)
            {        
                gRow=0;
                
                SwitchScreen();            

                if(PageIndex < MaxPage)
                    PageIndex++;
                else
                    PageIndex = MaxPage;
                
                PciDevList[NowList].PFA=PciDevList[NowList].PFA+0x100;
                
                DisplayPciReg(PageIndex,MaxPage,PciDevList,PciDevCount,NowList,PciReg,&IsPcie);
                
                gRow=0;
            }
            
            if (gInputKey.ScanCode == EFI_SCAN_PGUP)
            {            
                gRow=0;
                
                SwitchScreen();            
                
                if(PageIndex >= 1)
                    PageIndex--;
                else
                    PageIndex=0;
                
                PciDevList[NowList].PFA=PciDevList[NowList].PFA-0x100;
                
                DisplayPciReg(PageIndex,MaxPage,PciDevList,PciDevCount,NowList,PciReg,&IsPcie);
                
                gRow=0;
            }
            
            gBS->WaitForEvent (1, &(gST->ConIn->WaitForKey), &EventIndex);
            gST->ConIn->ReadKeyStroke( gST->ConIn, &gInputKey );
        }
        while(gInputKey.ScanCode != EFI_SCAN_ESC);
    } 
       
}

VOID
ConvertCharToInt(
  UINT8  *Integer
)
{

    switch(gInputKey.UnicodeChar)
    {
        case 48:
            *Integer=0;
            break;
        case 49:
            *Integer=1;
            break;
        case 50:
            *Integer=2;
            break;
        case 51:
            *Integer=3;
            break;
        case 52:
            *Integer=4;
            break;
        case 53:
            *Integer=5;
            break;
        case 54:
            *Integer=6;
            break;
        case 55:
            *Integer=7;
            break;
        case 56:
            *Integer=8;
            break;
        case 57:
            *Integer=9;
            break;
        case 97:
            *Integer=10;
            break;
        case 98:
            *Integer=11;
            break;
        case 99:
            *Integer=12;
            break;
        case 100:
            *Integer=13;
            break;
        case 101:
            *Integer=14;
            break;
        case 102:
            *Integer=15;
            break;
    }
}

VOID
WriteBackVal(
  IN  UINT8         InputVal,
  IN  UINT8         PageIndex,
  IN  UINT8         MaxPage,
  IN  PCI_DEV_LIST  *PciDevList,
  IN  UINT16        PciDevCount,
  IN  UINTN         NowList,
  OUT UINT8         PciReg[][16]
)
{
    UINTN x=0,y=0,TempRow=0;
    UINT32 Offset;
    UINT8 ReadVal=0;
    UINTN TotalList=0;
    
    TempRow=gRow;
    
    TotalList=ONE_PAGE_PCI_LIST * PageIndex + NowList;
    
    x=gRow-2;
    y=(gColumn-5)/3;
    Offset=(UINT32)(16*x+y);
    
    MmioWrite8((PciDevList[TotalList].PFA)+Offset,InputVal);
    
    ReadVal=MmioRead8((PciDevList[TotalList].PFA)+Offset);
    
    SwitchScreen();
    
    DisplayPci(PageIndex,MaxPage,PciDevList,PciDevCount,NowList,PciReg);
    
    gRow=TempRow;
    
    gST->ConOut->SetCursorPosition( gST->ConOut,gColumn,gRow);
}

VOID
PciEnumeration(
)
{
    EFI_STATUS Status=EFI_SUCCESS;
    UINT16 Bus=0,PciDevCount=1;
    UINT8 Dev=0,Fun=0,Index=0,i=0;
    UINT32 RegValue=0;
    UINT8 PageIndex=0,MaxPage=0;
    UINT32 VidDid=0;
    UINT32 Address=0;
    UINT16 Vid=0,Did=0;
//    EFI_GCD_MEMORY_SPACE_DESCRIPTOR  *MemorySpaceMap;
    UINTN NumberOfDescriptors=0;
    UINTN EventIndex=0,EventIndex1=0,NowList=0;
    UINT64 TotalMemorySize=0;
    PCI_DEV_LIST *PciDevList=NULL;
    UINT32 RegCachLineSize=0;
    UINT8  IsMulFun=0;
    UINT8  PciReg[16][16];
    UINT8  Intger1=0,Intger2=0,InputVal=0;
    UINTN  KeyIndex;
    UINT8  Count=0;
        
    //Initializing Input key 
    gInputKey.ScanCode=0;

    gInputKey.UnicodeChar=0;
        
    ZeroMem (PciDevList, sizeof (PCI_DEV_LIST));
    
    gRow=0;
    
    SwitchScreen();
    
    //
    // Get the GCD Memory Descriptor for the PCI Express Bus/Dev/Func specified by Address
    //
#if 0
    Status = gDS->GetMemorySpaceMap (&NumberOfDescriptors, &MemorySpaceMap);
    
    for(Index=0;Index<NumberOfDescriptors;Index++)
    {
        if (    MemorySpaceMap[Index].GcdMemoryType != EfiGcdMemoryTypeSystemMemory
             || (MemorySpaceMap[Index].Capabilities & EFI_MEMORY_NV) != 0
             || MemorySpaceMap[Index].BaseAddress > MAX_ADDRESS
        ) continue;
        TotalMemorySize += MemorySpaceMap[Index].Length;
    }

    
    Swprintf(StringLine,L"==TotalMemorySize:%x==",TotalMemorySize);
    
    gColumn++;
#endif    
    for(Bus=0; Bus<MAX_BUS; Bus++)
    {   
        for(Dev=0; Dev<MAX_DEV; Dev++)
        {            
            for(Fun=0; Fun<MAX_FUN; Fun++)
            {          
                Address = PCI_ACCESS_ENABLE_BIT + ((UINT8)Bus << 20) + (Dev << 15) + (Fun << 12);
                
                VidDid=MmioRead32(Address);
                
                Did = (VidDid & 0xffff0000) >> 16;
                Vid = (VidDid & 0x0000ffff);
                
                if(VidDid == 0xffffffff)
                {
                    if(Fun == 0)
                        break;
                    else
                        continue;
                }
                                                                         
                if(VidDid != 0xffffffff)
                {                                                         
                    PciDevList[Index].PFA=Address;
                    PciDevList[Index].Bus=(UINT8)Bus;
                    PciDevList[Index].Dev=Dev;
                    PciDevList[Index].Fun=Fun;
                    PciDevList[Index].VendorId=Vid;
                    PciDevList[Index].DeviceId=Did;
                    
                    Index++;
                    
                    //IoWrite32(PCI_INDEX_PORT,Address+0x0C);
                    //RegCachLineSize=IoRead32(PCI_DATA_PORT);
                    RegCachLineSize=MmioRead32(Address+0x0C);
                    IsMulFun=(RegCachLineSize & 0x00ff0000)>>16;
                    IsMulFun=IsMulFun>>7;
                    
                    if(Fun == 0 && IsMulFun == 0)
                        break;

                 }//if(VidDid != 0xffffffff)
            }//fun
        }//dev
    }//bus
    
    PciDevCount = Index;//Remove last addition
    
    MaxPage=(PciDevCount/20)+1;
    
    gRow=0;
    
    MovePage(0,MaxPage,PciDevList,PciDevCount);
    
    gStringLine = AllocateZeroPool (0x100);
    
    Swprintf(gStringLine,L"Page/MaxPage:%d/%d",PageIndex,MaxPage);

    DisplayString(0,EFI_WHITE,EFI_BLUE);
    
    gRow=1;
    
    gST->ConOut->SetCursorPosition( gST->ConOut,4,1);

    do
    {        
        if (gInputKey.ScanCode == EFI_SCAN_DN)
        {
            if(gRow >= 20)
                gRow = 19;
            
            gST->ConOut->SetCursorPosition( gST->ConOut,4,gRow+1);
            gRow++; 
        }
        
        if (gInputKey.ScanCode == EFI_SCAN_UP)
        {           
            if(gRow <= 1)
                gRow = 2;
            
            gST->ConOut->SetCursorPosition( gST->ConOut,4,gRow-1);
            gRow--;            
        }
        
        if (gInputKey.ScanCode == EFI_SCAN_PGDN)
        {        
            gRow=0;
            
            SwitchScreen();            

            if(PageIndex < MaxPage)
                PageIndex++;
            else
                PageIndex = MaxPage;
            
            MovePage(PageIndex,MaxPage,PciDevList,PciDevCount);
            
            gRow=0;
        }
        
        if (gInputKey.ScanCode == EFI_SCAN_PGUP)
        {            
            gRow=0;
            
            SwitchScreen();            
            
            if(PageIndex >= 1)
                PageIndex--;
            else
                PageIndex=0;
            
            MovePage(PageIndex,MaxPage,PciDevList,PciDevCount);
            
            gRow=0;
        }
        
        if (gInputKey.UnicodeChar == CHAR_CARRIAGE_RETURN)//enter into pci list
        {
            if(gRow > 0)
                NowList=gRow-1;
            
            SwitchScreen();
            
            DisplayPci(PageIndex,MaxPage,PciDevList,PciDevCount,NowList,PciReg);
            
            gRow=2;
            gColumn=5;
                        
            do
            {
                if(gInputKey.UnicodeChar == 101)//"enter" to edit
                {
                    gInputKey.UnicodeChar=0;
                    gInputKey.ScanCode=0;
                    
                    gStringLine = AllocateZeroPool (0x100);
                
                    Swprintf(gStringLine,L"  ");
                    
                    gColumn=gColumn-1;
                    
                    DisplayString(gColumn,EFI_WHITE,EFI_BLACK);
                    
                    gColumn=gColumn+1;
                    
                    gST->ConOut->SetCursorPosition( gST->ConOut,gColumn,gRow);
                    
                    Count=0;
                    
                    do
                    {
                        gBS->WaitForEvent(1, &(gST->ConIn->WaitForKey), &KeyIndex); 
                        Status = gST->ConIn->ReadKeyStroke(gST->ConIn,&gInputKey);
                        
                        Count++;
                        
                        if(gInputKey.UnicodeChar == CHAR_CARRIAGE_RETURN)
                        {
                            WriteBackVal(InputVal,PageIndex,MaxPage,PciDevList,PciDevCount,NowList,PciReg);
                            break;
                        }
                        
                        if((gInputKey.UnicodeChar < 48 || gInputKey.UnicodeChar > 57) && \
                           (gInputKey.UnicodeChar < 97 || gInputKey.UnicodeChar > 102)
                           )
                        {
                            Count=0;
                        }
               
                        if(Count == 1)
                        {
                            gStringLine = AllocateZeroPool (0x100);
                                                
                            ConvertCharToInt(&Intger1);
                                                
                            Swprintf(gStringLine,L"0%X",Intger1);
                                                
                            gColumn=gColumn-1;

                            DisplayString(gColumn,EFI_WHITE,EFI_BLACK);
                                                
                            gColumn=gColumn+1;
                                                                    
                            gST->ConOut->SetCursorPosition( gST->ConOut,gColumn,gRow);
                            
                            InputVal=Intger1;
                        }
                        else if(Count == 2)
                        {
                            gStringLine = AllocateZeroPool (0x100);
                            
                            ConvertCharToInt(&Intger2);
                            
                            Swprintf(gStringLine,L"%X%X",Intger1,Intger2);
                        
                            gColumn=gColumn-1;

                            DisplayString(gColumn,EFI_WHITE,EFI_BLACK);
                        
                            gColumn=gColumn+1;
                                                                
                            gST->ConOut->SetCursorPosition( gST->ConOut,gColumn,gRow);
                        
                            InputVal=Intger1*16+Intger2;
                            
                            WriteBackVal(InputVal,PageIndex,MaxPage,PciDevList,PciDevCount,NowList,PciReg);
                            
                            break;
                        }
                    }
                    while(gInputKey.UnicodeChar != CHAR_CARRIAGE_RETURN);
                }
                
                if (gInputKey.ScanCode == EFI_SCAN_DN)
                {
                    gST->ConOut->SetCursorPosition( gST->ConOut,0,gRow+1);
                    
                    if(gRow<17)
                        gRow++;
                    else
                        gRow=17;
                }
                
                if (gInputKey.ScanCode == EFI_SCAN_UP)
                {                
                    gST->ConOut->SetCursorPosition( gST->ConOut,0,gRow-1);
                    
                    if(gRow>2)
                        gRow--;
                    else
                        gRow=2;
                }
                
                if (gInputKey.ScanCode == EFI_SCAN_RIGHT)
                {                
                    gST->ConOut->SetCursorPosition( gST->ConOut,gColumn+3,0);
                    
                    if(gColumn<49)
                        gColumn=gColumn+3;
                    else
                        gColumn=50;
                }
                
                if (gInputKey.ScanCode == EFI_SCAN_LEFT)
                {                
                    gST->ConOut->SetCursorPosition( gST->ConOut,gColumn-3,0);
                    
                    if(gColumn>7)
                        gColumn=gColumn-3;
                    else
                        gColumn=5;
                }

                gST->ConOut->SetCursorPosition( gST->ConOut,gColumn,gRow);
                           
                gBS->Stall(100000);     //100ms
                gBS->WaitForEvent (1, &(gST->ConIn->WaitForKey), &EventIndex1);
                gST->ConIn->ReadKeyStroke( gST->ConIn, &gInputKey );

            }
            while(gInputKey.ScanCode != EFI_SCAN_ESC);
               
            MovePage(PageIndex,MaxPage,PciDevList,PciDevCount);
               
            gST->ConOut->SetCursorPosition( gST->ConOut,4,1);
               
            gRow=0;
        }
        
        gBS->Stall(100000);     //100ms
        gBS->WaitForEvent (1, &(gST->ConIn->WaitForKey), &EventIndex);
        gST->ConIn->ReadKeyStroke( gST->ConIn, &gInputKey );
    }
    while(gInputKey.ScanCode != EFI_SCAN_ESC);
    
    gBS->FreePool (PciDevList);
    
    SwitchScreen();
    
    ListMainItem();
       
    gST->ConOut->SetCursorPosition( gST->ConOut,0,0);
       
    gRow=0;
//    gBS->FreePool (MemorySpaceMap);
}

VOID
PciConfiguration()
{   
    PciEnumeration();
}

VOID
IoPortConfiguration()
{
    gStringLine = AllocateZeroPool (0x100);
    
    SwitchScreen();
    
    StrCat(gStringLine,L"IOIO");
    
    DisplayString(0,EFI_WHITE,EFI_BLUE);   
}

VOID
MmioConfiguration()
{
    gStringLine = AllocateZeroPool (0x100);
    
    SwitchScreen();
    
    StrCat(gStringLine,L"MMIO");
    
    DisplayString(0,EFI_WHITE,EFI_BLUE);   
}

VOID
ListSmBiosTable()
{
    gStringLine = AllocateZeroPool (0x100);
    
    SwitchScreen();
    
    StrCat(gStringLine,L"SMBIOS");
    
    DisplayString(0,EFI_WHITE,EFI_BLUE);   
}

#if 0  
EFI_STATUS
EFIAPI
CheckMemMap (
    IN EFI_EVENT    Event,
    IN VOID     *Context )
{
    EFI_STATUS      Status = EFI_SUCCESS;
    UINTN                               NumberOfDescriptors;
    EFI_GCD_MEMORY_SPACE_DESCRIPTOR     *MemorySpaceMap;
    UINTN                               Index;
    
    //
    // Get the memory space map from GCD
    //
    Status = gDS->GetMemorySpaceMap (
                    &NumberOfDescriptors,
                    &MemorySpaceMap
                    );
    
    for (Index = 0; Index < NumberOfDescriptors; Index++)
    {      
        OEM_TRACE("[%d]\n Attributes:%x\n BaseAddress:%x\n Capabilities:%x\n DeviceHandle:%x\n GcdMemoryType:%x\n ImageHandle:%x\n Length:%x\n",
                Index,
                MemorySpaceMap[Index].Attributes,
                MemorySpaceMap[Index].BaseAddress,
                MemorySpaceMap[Index].Capabilities,
                MemorySpaceMap[Index].DeviceHandle,
                MemorySpaceMap[Index].GcdMemoryType,
                MemorySpaceMap[Index].ImageHandle,
                MemorySpaceMap[Index].Length
                );       
    }
    
    pBS->CloseEvent(Event);
    
    return EFI_SUCCESS;
}
#endif 


EFI_STATUS
ApplicationEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
    UINTN                       Index;
    UINTN                       Size=0;
    SIMPLE_TEXT_OUTPUT_MODE     *SimpleTextOutputModePtr ;
    UINT8                       ExitTimer=0;
    EFI_STATUS                  Status=EFI_SUCCESS;
    UINTN                       LastLine;
    ExitTimer=0;
    gRow =0;
    LastLine = gColumn;
    gColumn =0;
    
    //Initializing Input key 
    gInputKey.ScanCode=0;

    gInputKey.UnicodeChar=0;

    if(!EFI_ERROR(Status) && Size == 1) return EFI_NOT_READY;
    gST->ConOut->Reset( gST->ConOut, FALSE );

    gST->ConOut->SetMode( gST->ConOut, EfiConsoleControlScreenMaxValue );
    gST->ConOut->QueryMode(gST->ConOut, EfiConsoleControlScreenMaxValue,&gMaxColumn, &gMaxRow)  ;

    if (gMaxRow != 100)
    {
        SimpleTextOutputModePtr = gST->ConOut->Mode;
        gST->ConOut->SetMode( gST->ConOut, EfiConsoleControlScreenGraphics);
        gST->ConOut->QueryMode(gST->ConOut, SimpleTextOutputModePtr->Mode,&gMaxColumn, &gMaxRow)  ;
    }

    SimpleTextOutputModePtr = gST->ConOut->Mode;
    gDisplayMode = SimpleTextOutputModePtr->Mode;

    gST->ConOut->ClearScreen ( gST->ConOut);
    gST->ConOut->EnableCursor(gST->ConOut,TRUE);
    
    ListMainItem();

    gST->ConOut->SetCursorPosition( gST->ConOut,0,0);
    
    do
    {      
        if (gInputKey.ScanCode == EFI_SCAN_DN)
        {
            gST->ConOut->SetCursorPosition( gST->ConOut,0,gRow+1);
            gRow++;                
        }
            
        if (gInputKey.ScanCode == EFI_SCAN_UP)
        {                
            gST->ConOut->SetCursorPosition( gST->ConOut,0,gRow-1);
            gRow--;            
        }
            
        if (gInputKey.UnicodeChar == CHAR_CARRIAGE_RETURN)
        {
            switch(gRow)
            {
                case 0:
                    PciConfiguration();
                    break;
                case 1:
                    IoPortConfiguration();
                    break;
                case 2:
                    MmioConfiguration();
                    break;
                case 3:
                    ListSmBiosTable();
                    break;
             }
/*                
             do
             {                
                 gBS->Stall(100000);     //100ms
                 gBS->WaitForEvent (1, &(gST->ConIn->WaitForKey), &Index);
                 gST->ConIn->ReadKeyStroke( gST->ConIn, &gInputKey );

             }
             while(gInputKey.ScanCode != EFI_SCAN_ESC);
                
             ListMainItem();
                
             gST->ConOut->SetCursorPosition( gST->ConOut,0,0);
                
             gColumn=0;
*/             
        }
            
        gBS->Stall(100000);     //100ms
        gBS->WaitForEvent (1, &(gST->ConIn->WaitForKey), &Index);
        gST->ConIn->ReadKeyStroke( gST->ConIn, &gInputKey );
    }
    while(gInputKey.ScanCode != EFI_SCAN_ESC);

    gST->ConOut->ClearScreen ( gST->ConOut);
    gBS->FreePool(gScreenBuffer);
    
    return EFI_SUCCESS;
}
