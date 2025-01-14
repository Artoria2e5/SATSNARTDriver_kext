/*
 *  IOSatDriver.h
 *
 *  Created by Jarkko Sonninen on 3.2.2012.
 *  Copyright 2012 Jarkko Sonninen. All rights reserved.
 *
 */

#ifndef _EXTERNAL_SAT_DRIVER_H_
#define _EXTERNAL_SAT_DRIVER_H_

#if defined(KERNEL) && defined(__cplusplus)

#include <IOKit/scsi/IOSCSIPeripheralDeviceType00.h>

class IOSatCommand;
class IOBufferMemoryDescriptor;

#define kSatSmartCapableKey "SatSmartCapable"
#define kPermissiveKey      "Permissive"
#define kDelayIdentifyKey   "DelayIdentify"
#define kPassThroughMode    "PassThroughMode"
#define kMyPropertyKey      "MyProperty"
#define kProductModelKey    "Model"
#define kEnclosureName      "Enclosure Name"

enum
{
    kPassThroughModeNone    = 0,
    kPassThroughModeAuto    = 1,
    kPassThroughModeSAT16   = 2,
    kPassThroughModeSAT12   = 3,
    kPassThroughModeJMicron = 4,
    kPassThroughModeSunplus = 5,
};

enum
{
    kIOSatTDirectionToDevice   = 0,
    kIOSatTDirectionFromDevice = 1,
};

enum
{
    kIOSatTLengthBytes  = 0,
    kIOSatTLengthBlocks = 1
};

enum
{
    kIOSatOffline0s  = 0,
    kIOSatOffline2s  = 1,
    kIOSatOffline6s  = 2,
    kIOSatOffline14s = 3
};

enum
{
    kIOSatProtocolHardReset                 = 0,
    kIOSatProtocolSRST                      = 1,
    kIOSatProtocolNonData                   = 3,
    kIOSatProtocolPIODataIn                 = 4,
    kIOSatProtocolPIODataOut                = 5,
    kIOSatProtocolDMA                       = 6,
    kIOSatProtocolDMAQueued                 = 7,
    kIOSatProtocolDeviceDiagnostic          = 8,
    kIOSatProtocolDEVICERESET               = 9,
    kIOSatProtocolUDMADataIn                = 10,
    kIOSatProtocolUDMADataOut               = 11,
    kIOSatProtocolFPDMA                     = 12,
    kIOSatProtocolReturnResponseInformation = 15
};

enum
{
    kIOSatTLengthNoData        = 0,
    kIOSatTLengthInFeatures    = 1,
    kIOSatTLengthInSectorCount = 2,
    kIOSatTLengthInSTPSIU      = 3
};

class fi_dungeon_driver_IOSatDriver : public IOSCSIPeripheralDeviceType00 {
    OSDeclareDefaultStructors(fi_dungeon_driver_IOSatDriver);

    char serial[21];
    char revision[9];
    char model[41];

    bool fSatSmartCapable;
    bool fPermissive;
    bool fDelayIdentify;
    bool fIdentified;
    int  fPassThroughMode;
    int  fPort;
    int  fDevice;
    int  fCapabilities;

protected:
#ifdef DEBUG
    bool InitializeDeviceSupport(void);
    void TerminateDeviceSupport(void);
#endif
    bool      Send_ATA_IDENTIFY(void);
    bool      Send_ATA_SMART_READ_DATA(void);
    bool      Send_ATA_IDLE(UInt8 value);
    bool      Send_ATA_IDLE_IMMEDIATE(void);
    bool      Send_ATA_STANDBY(UInt8 value);
    bool      Send_ATA_STANDBY_IMMEDIATE(void);
    bool      Send_ATA_CHECK_POWER_MODE(int*);
    bool      Send_ATA_SEND_SOFT_RESET(void);
    OSObject* getParentProperty(const char* key);
    void      parseProperties();

    bool JMicron_get_registers(UInt16 address, UInt8* ptr, UInt16 length);
    bool
    PASS_THROUGH_JMicron(
        SCSITaskIdentifier  request,
        IOMemoryDescriptor* dataBuffer,
        SCSICmdField1Byte   FEATURES,
        SCSICmdField1Byte   SECTOR_COUNT,
        SCSICmdField1Byte   LBA_LOW,
        SCSICmdField1Byte   LBA_MID,
        SCSICmdField1Byte   LBA_HIGH,
        SCSICmdField1Byte   DEVICE,
        SCSICmdField1Byte   COMMAND,
        SCSICmdField1Byte CONTROL, int direction, int transferCount);
    bool
    PASS_THROUGH_Sunplus(
        SCSITaskIdentifier  request,
        IOMemoryDescriptor* dataBuffer,
        SCSICmdField1Byte   FEATURES,
        SCSICmdField1Byte   SECTOR_COUNT,
        SCSICmdField1Byte   LBA_LOW,
        SCSICmdField1Byte   LBA_MID,
        SCSICmdField1Byte   LBA_HIGH,
        SCSICmdField1Byte   DEVICE,
        SCSICmdField1Byte   COMMAND,
        SCSICmdField1Byte CONTROL, int direction, int transferCount);
    bool PASS_THROUGH_12(
        SCSITaskIdentifier  request,
        IOMemoryDescriptor* dataBuffer,
        SCSICmdField3Bit    MULTIPLE_COUNT,
        SCSICmdField4Bit    PROTOCOL,
        SCSICmdField1Bit    EXTEND,
        SCSICmdField2Bit    OFF_LINE,
        SCSICmdField1Bit    CK_COND,
        SCSICmdField1Bit    T_DIR,
        SCSICmdField1Bit    BYT_BLOK,
        SCSICmdField2Bit    T_LENGTH,
        SCSICmdField1Byte   FEATURES,
        SCSICmdField1Byte   SECTOR_COUNT,
        SCSICmdField1Byte   LBA_LOW,
        SCSICmdField1Byte   LBA_MID,
        SCSICmdField1Byte   LBA_HIGH,
        SCSICmdField1Byte   DEVICE,
        SCSICmdField1Byte   COMMAND,
        SCSICmdField1Byte   CONTROL);
    bool PASS_THROUGH_16(
        SCSITaskIdentifier  request,
        IOMemoryDescriptor* dataBuffer,
        SCSICmdField3Bit    MULTIPLE_COUNT,
        SCSICmdField4Bit    PROTOCOL,
        SCSICmdField1Bit    EXTEND,
        SCSICmdField2Bit    OFF_LINE,
        SCSICmdField1Bit    CK_COND,
        SCSICmdField1Bit    T_DIR,
        SCSICmdField1Bit    BYT_BLOK,
        SCSICmdField2Bit    T_LENGTH,
        SCSICmdField2Byte   FEATURES,
        SCSICmdField2Byte   SECTOR_COUNT,
        SCSICmdField2Byte   LBA_LOW,
        SCSICmdField2Byte   LBA_MID,
        SCSICmdField2Byte   LBA_HIGH,
        SCSICmdField1Byte   DEVICE,
        SCSICmdField1Byte   COMMAND,
        SCSICmdField1Byte   CONTROL);
    bool PASS_THROUGH_12or16(
        SCSITaskIdentifier  request,
        IOMemoryDescriptor* dataBuffer,
        SCSICmdField3Bit    MULTIPLE_COUNT,
        SCSICmdField4Bit    PROTOCOL,
        SCSICmdField1Bit    EXTEND,
        SCSICmdField2Bit    OFF_LINE,
        SCSICmdField1Bit    CK_COND,
        SCSICmdField1Bit    T_DIR,
        SCSICmdField1Bit    BYT_BLOK,
        SCSICmdField2Bit    T_LENGTH,
        SCSICmdField2Byte   FEATURES,
        SCSICmdField2Byte   SECTOR_COUNT,
        SCSICmdField2Byte   LBA_LOW,
        SCSICmdField2Byte   LBA_MID,
        SCSICmdField2Byte   LBA_HIGH,
        SCSICmdField1Byte   DEVICE,
        SCSICmdField1Byte   COMMAND,
        SCSICmdField1Byte   CONTROL);
    void SendBuiltInINQUIRY(void);

    static void   sProcessPoll(void* pdtDriver, void* refCon);
    thread_call_t fPollingThread;

public:
    virtual bool init(OSDictionary* dictionary = NULL);
#ifdef DEBUG
    virtual void free(void);
#endif
    virtual IOService* probe(IOService* provider, SInt32* score);
    virtual bool       start(IOService* provider);
    virtual void       stop(IOService* provider);

    virtual char* GetVendorString(void);
    virtual char* GetProductString(void);
    virtual char* GetRevisionString(void);

    virtual IOReturn setProperties(OSObject* properties);

#ifdef DEBUG
    virtual bool attach(IOService* provider);
    virtual void detach(IOService* provider);
    virtual bool open(IOService* forClient,
        IOOptionBits             options,
        void*                    arg);
#endif

    virtual void     CreateStorageServiceNub(void);
    virtual IOReturn sendSMARTCommand(IOSatCommand* command);
    virtual bool     IdentifyDevice(void);
    virtual void     LogAutoSenseData(SCSITaskIdentifier request);

protected:
    // Reserve space for future expansion.
    struct IOSatDriverExpansionData {
    };
    IOSatDriverExpansionData* fIIOSatDriverReserved;

private:
    // Padding for future binary compatibility.
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 0);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 1);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 2);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 3);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 4);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 5);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 6);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 7);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 8);
    OSMetaClassDeclareReservedUnused(fi_dungeon_driver_IOSatDriver, 9);
};

#endif

#endif
