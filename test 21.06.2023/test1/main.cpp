#include <stdio.h>
#include "ut_common.h"
#include "IMRMMirrorControlCallbacks.hpp"
#include "IMirroringCtrlStubCbAdapter.hpp"
#include "MirroringCtrlServer.hpp"
#include "mockMirroringCtrlServer.h"
#include "mockdevmgr_interface.h"
#include "MRMDevCtrlUtils.hpp"
#include "DevMgrAppCtrlEAP.hpp"
#include "MRM_Utils.hpp"
#include "osal_mock.h"
#include "IRegisterServiceMonitor.hpp"
#include "system_func.h"

#define MAX_INST_NAME_SIZE        (50)
#define MAX_CONNECTION_ID_SIZE    (40)

extern ALES_VOID MRMUtilsCallStateUpdate(HANDLE handle, ALES_VOID * userArg,
    sDevMgr_CallStateUpdate *psUpdates);
extern ALES_VOID MRMUtilsCommunicationsUpdate(HANDLE handle, ALES_VOID * userArg,
    sDevMgr_CommunicationUpdate *psCommUpdate);

unsigned char gboRaceLogs = false;
using ::testing::AtLeast;
using ::testing::NiceMock;
using ::testing::Mock;
using namespace std;

/* To test CP Stack Mock */
#define CP_MOCK
#ifdef CP_MOCK
#include "mockCPInterface.h"
#include "MirrCP.hpp"
#endif

/* To test AAP Stack Mock */
//#define AAP_MOCK
#ifdef AAP_MOCK
#include "mockAAPInterface.h"
#include "MirrAAP.hpp"
#endif

#ifdef GCOV_FLUSH
extern "C" AAP_VOID __gcov_flush();
#endif /* ifdef GCOV_FLUSH */

class CCTest : public ::testing::Test
{
    public:
    void init_osal_mocks(OsalMock *oOsalMock);
    protected:
        CCTest() {}
        virtual ~CCTest() {}
};

void CCTest::init_osal_mocks(OsalMock *oOsalMock)
{
    std::cout <<"init_osal_mocks\n";
    /* Osal mock */
    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_malloc(_) )
        .WillRepeatedly(Invoke(mem_malloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_signal(_) )
        .WillRepeatedly(Invoke(thread_condition_signal));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_wait(_,_) )
        .WillRepeatedly(Invoke(thread_condition_wait));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_wait(_) )
        .WillRepeatedly(Invoke(semaphore_wait));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_post(_) )
        .WillRepeatedly(Invoke(semaphore_post));
}

#ifdef CP_MOCK
TEST_F(CCTest, destroyMirroringSession1)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    CPInterfaceMock *poCpMock = CPInterfaceMock::getInstance();

    /* Osal mock */
    init_osal_mocks(oOsalMock);
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_signal(_) )
        .WillRepeatedly(Invoke(thread_condition_signal));
    /* CP Mock */
    EXPECT_CALL(*poCpMock, CarPlayDestroy(_) )
        .WillRepeatedly(Return(CP_FAILURE));

    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_CREATED;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.destroyMirroringSession(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP])));

    free(mirCtrlCbs);
    delete oOsalMock;
    delete poCpMock;
}

TEST_F(CCTest, destroyMirroringSession2)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    CPInterfaceMock *poCpMock = CPInterfaceMock::getInstance();

    /* Osal mock */
    init_osal_mocks(oOsalMock);
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_signal(_) )
        .WillRepeatedly(Invoke(thread_condition_signal));
    /* CP Mock */
    EXPECT_CALL(*poCpMock, CarPlayDestroy(_) )
        .WillRepeatedly(Return(CP_FAILURE));

    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_CREATED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].pvCPHandle = (void *)1;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.destroyMirroringSession(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP])));

    free(mirCtrlCbs);
    delete oOsalMock;
    delete poCpMock;
}

/* Test case :
 * 1. Wifi ctrl event without any device on the list
 * 2. Wifi ctrl event with a USB and a BT device on the list */
TEST_F(CCTest, CarPlayClientNotificationCb1)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    MRM_DevInfoInternal sConnectedDevice = {};
    ALES_UINT64 macid = 0;
    sCarPlayClientInfoRef inClientInfo = reinterpret_cast<sCarPlayClientInfoRef>(calloc(1, sizeof(sCarPlayClientInfo)));

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    /* Prepare client info */
    inClientInfo->infoMask = (BIT(CARPLAY_CLIENTINFO_TYPE) | BIT(CARPLAY_CLIENTINFO_BTMAC));
    inClientInfo->type = CARPLAY_CLIENTTYPE_USB_WIFI;
    inClientInfo->btMAC[0] = 0x12;
    inClientInfo->btMAC[1] = 0x12;
    inClientInfo->btMAC[2] = 0x12;
    inClientInfo->btMAC[3] = 0x12;
    inClientInfo->btMAC[4] = 0x12;
    inClientInfo->btMAC[5] = 0x12;
    macid = (static_cast<ALES_UINT64>(inClientInfo->btMAC[0]) << 40) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[1]) << 32) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[2]) << 24) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[3]) << 16) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[4]) << 8) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[5]));

    /* Giving Wifi ctrl client updated event without any device on the list */
    CarPlayClientNotificationCb(&mirrctrl, 12345, CARPLAY_CLIENT_UPDATED, inClientInfo);

    /* Add a BT unactivated device to the device list first */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_CP;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_BLUETOOTH;
    sConnectedDevice.sDevInfo.acSerialiNumber = static_cast<ostringstream*>(&(ostringstream()
                << macid))->str();
    sConnectedDevice.bIsWiFiDeviceActivated = false;
    sConnectedDevice.clientHandle = 0;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    /* Add a USB activated device to the device list first */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_CP;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB;
    sConnectedDevice.sDevInfo.acSerialiNumber = static_cast<ostringstream*>(&(ostringstream()
                << macid))->str();
    sConnectedDevice.bIsUSBDeviceActivated = true;
    sConnectedDevice.clientHandle = 12345;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    /* Giving Wifi ctrl client updated event with a USB and a BT device on the list */
    CarPlayClientNotificationCb(&mirrctrl, 12345, CARPLAY_CLIENT_UPDATED, inClientInfo);

    free(inClientInfo);
    free(mirCtrlCbs);
    delete oOsalMock;
}

/* Test case :
 * 1. USB ctrl event with a Wifi device on the list (without BT transport)
 *    No active session with Wifi device
 */
TEST_F(CCTest, CarPlayClientNotificationCb3)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    MRM_DevInfoInternal sConnectedDevice = {};
    ALES_UINT64 macid = 0;
    sCarPlayClientInfoRef inClientInfo = reinterpret_cast<sCarPlayClientInfoRef>(calloc(1, sizeof(sCarPlayClientInfo)));

    init_osal_mocks(oOsalMock);
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    /* Prepare client info */
    inClientInfo->infoMask = (BIT(CARPLAY_CLIENTINFO_TYPE) | BIT(CARPLAY_CLIENTINFO_BTMAC));
    inClientInfo->type = CARPLAY_CLIENTTYPE_USB_WIFI;
    inClientInfo->btMAC[0] = 0x12;
    inClientInfo->btMAC[1] = 0x12;
    inClientInfo->btMAC[2] = 0x12;
    inClientInfo->btMAC[3] = 0x12;
    inClientInfo->btMAC[4] = 0x12;
    inClientInfo->btMAC[5] = 0x12;
    macid = (static_cast<ALES_UINT64>(inClientInfo->btMAC[0]) << 40) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[1]) << 32) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[2]) << 24) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[3]) << 16) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[4]) << 8) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[5]));

    /* Add a Wifi activated device to the device list */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_CP;
    sConnectedDevice.sDevInfo.ulDevHandle = 56789;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_WIFI;
    sConnectedDevice.sDevInfo.acSerialiNumber = static_cast<ostringstream*>(&(ostringstream()
                << macid))->str();
    sConnectedDevice.bIsWiFiDeviceActivated = true;
    sConnectedDevice.clientHandle = 12345;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    /* Add a USB unactivated device to the device list */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_CP;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB;
    sConnectedDevice.sDevInfo.acSerialiNumber = static_cast<ostringstream*>(&(ostringstream()
                << macid))->str();
    sConnectedDevice.bIsUSBDeviceActivated = false;
    sConnectedDevice.clientHandle = 0;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    /* No session active with Wifi device */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_CREATED;

    /* Giving USB ctrl client updated event */
    CarPlayClientNotificationCb(&mirrctrl, 12345, CARPLAY_CLIENT_UPDATED, inClientInfo);

    free(inClientInfo);
    free(mirCtrlCbs);
    delete oOsalMock;
}

/* Test case :
 * 1. Wifi Ctrl client disconnection in client_updated callback
 */
TEST_F(CCTest, CarPlayClientNotificationCb2)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    MRM_DevInfoInternal sConnectedDevice = {};
    ALES_UINT64 macid = 0;
    sCarPlayClientInfoRef inClientInfo = reinterpret_cast<sCarPlayClientInfoRef>(calloc(1, sizeof(sCarPlayClientInfo)));

    init_osal_mocks(oOsalMock);
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    /* Prepare client info */
    inClientInfo->infoMask = (BIT(CARPLAY_CLIENTINFO_TYPE) | BIT(CARPLAY_CLIENTINFO_BTMAC));
    inClientInfo->type = CARPLAY_CLIENTTYPE_USB;
    inClientInfo->btMAC[0] = 0x12;
    inClientInfo->btMAC[1] = 0x12;
    inClientInfo->btMAC[2] = 0x12;
    inClientInfo->btMAC[3] = 0x12;
    inClientInfo->btMAC[4] = 0x12;
    inClientInfo->btMAC[5] = 0x12;
    macid = (static_cast<ALES_UINT64>(inClientInfo->btMAC[0]) << 40) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[1]) << 32) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[2]) << 24) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[3]) << 16) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[4]) << 8) |
            (static_cast<ALES_UINT64>(inClientInfo->btMAC[5]));

    /* Add a Wifi/USB activated device to the device list */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_CP;
    sConnectedDevice.sDevInfo.ulDevHandle = 56789;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = (mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_WIFI |
        mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB);
    sConnectedDevice.sDevInfo.acSerialiNumber = static_cast<ostringstream*>(&(ostringstream()
                << macid))->str();
    sConnectedDevice.bIsWiFiDeviceActivated = true;
    sConnectedDevice.bIsUSBDeviceActivated = true;
    sConnectedDevice.clientHandle = 12345;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    /* No session active with Wifi device */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_CREATED;

    /* Giving USB ctrl client updated event */
    CarPlayClientNotificationCb(&mirrctrl, 12345, CARPLAY_CLIENT_UPDATED, inClientInfo);

    free(inClientInfo);
    free(mirCtrlCbs);
    delete oOsalMock;
}

/* Test case :
 * 1. CarPlay stack returning error on startCarPlay
 */
class MRMCbs : public IMRMMirrorControlCallbacks
{
    public:
    MRMCbs():IMRMMirrorControlCallbacks(nullptr){};
    ~MRMCbs(){};
    virtual void onSessionStateChanged(MRM_MIRRORING_HANDLE ulMirroringHandle,
                                       MRM_SessionStatus eSessionStatus,
                                       MRM_ConnectFailReason eReason);
    virtual void onStartMic(MRM_MIRRORING_HANDLE ulMirroringHandle);
    virtual void onStopMic(MRM_MIRRORING_HANDLE ulMirroringHandle);
};

void MRMCbs::onSessionStateChanged(MRM_MIRRORING_HANDLE ulMirroringHandle,
                                       MRM_SessionStatus eSessionStatus,
                                       MRM_ConnectFailReason eReason)
{
    UNUSED_PARAM(ulMirroringHandle);
    UNUSED_PARAM(eSessionStatus);
    UNUSED_PARAM(eReason);
    std::cout <<"MRMCbs::onSessionStateChanged\n";
}
void MRMCbs::onStartMic(MRM_MIRRORING_HANDLE ulMirroringHandle)
{
    UNUSED_PARAM(ulMirroringHandle);
    std::cout <<"MRMCbs::onStartMic\n";
}
void MRMCbs::onStopMic(MRM_MIRRORING_HANDLE ulMirroringHandle)
{
    UNUSED_PARAM(ulMirroringHandle);
    std::cout <<"MRMCbs::onStopMic\n";
}

TEST_F(CCTest, CarPlayStartCompletedCb)
{
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();

    init_osal_mocks(oOsalMock);
    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    CarPlayStartCompletedCb(&mirrctrl, CP_UNSUPPORTED);
    CarPlayStartCompletedCb(&mirrctrl, CP_DEVICE_NOT_CONNECTED);
    delete oOsalMock;
}

/* Test case :
 * 1. Different combinations of app state change callbacks from carplay stack
 */
TEST_F(CCTest, CarPlayAppStateChangedCb)
{
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();

    init_osal_mocks(oOsalMock);
    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].bNavStarted = false;
    CarPlayAppStateChangedCb(&mirrctrl, CARPLAY_APPSTATE_TURNBYTURN, CARPLAY_ENTITY_CONTROLLER, CARPLAY_SPEECH_SPEAKING);
    CarPlayAppStateChangedCb(&mirrctrl, CARPLAY_APPSTATE_TURNBYTURN, CARPLAY_ENTITY_NONE, CARPLAY_SPEECH_NONE);

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].bMicStarted = false;
    CarPlayAppStateChangedCb(&mirrctrl, CARPLAY_APPSTATE_PHONECALL, CARPLAY_ENTITY_CONTROLLER, CARPLAY_SPEECH_SPEAKING);
    CarPlayAppStateChangedCb(&mirrctrl, CARPLAY_APPSTATE_PHONECALL, CARPLAY_ENTITY_NONE, CARPLAY_SPEECH_NONE);

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].bMicStarted = false;
    CarPlayAppStateChangedCb(&mirrctrl, CARPLAY_APPSTATE_SPEECH, CARPLAY_ENTITY_CONTROLLER, CARPLAY_SPEECH_RECOGNIZING);
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].sAudioState.eRequestedStreamType =
        mirroring::MRM_AudioStreamType::E_MRM_AUDIO_STREAM_VR;
    CarPlayAppStateChangedCb(&mirrctrl, CARPLAY_APPSTATE_SPEECH, CARPLAY_ENTITY_CONTROLLER, CARPLAY_SPEECH_SPEAKING);
    CarPlayAppStateChangedCb(&mirrctrl, CARPLAY_APPSTATE_SPEECH, CARPLAY_ENTITY_CONTROLLER, CARPLAY_SPEECH_RECOGNIZING);
    CarPlayAppStateChangedCb(&mirrctrl, CARPLAY_APPSTATE_SPEECH, CARPLAY_ENTITY_NONE, CARPLAY_SPEECH_NONE);

    delete oOsalMock;
}

/* Test Case :
 * 1. CarPlayStop error cases
 */
TEST_F(CCTest, stopMirroringSession)
{
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();
    CPInterfaceMock *poCpMock = CPInterfaceMock::getInstance();
    MRM_DevInfoInternal sConnectedDevice = {};
    ALES_UINT64 macid = 0;

    init_osal_mocks(oOsalMock);
    /* CP Mock */
    EXPECT_CALL(*poCpMock, CarPlayStop(_,_) )
        .WillOnce(Return(CP_INVALID_HANDLE) )
        .WillOnce(Return(CP_NOT_STOPPED) )
        .WillOnce(Return(CP_INVALID_ARGS) )
        .WillOnce(Return(CP_DEVICE_NOT_CONNECTED) );

    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    /* Add a USB unactivated device to the device list */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_CP;
    sConnectedDevice.sDevInfo.ulDevHandle = 56789;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB;
    macid = (static_cast<ALES_UINT64>(12) << 40) |
            (static_cast<ALES_UINT64>(12) << 32) |
            (static_cast<ALES_UINT64>(12) << 24) |
            (static_cast<ALES_UINT64>(12) << 16) |
            (static_cast<ALES_UINT64>(12) << 8) |
            (static_cast<ALES_UINT64>(12));
    sConnectedDevice.sDevInfo.acSerialiNumber = static_cast<ostringstream*>(&(ostringstream()
                << macid))->str();
    sConnectedDevice.bIsUSBDeviceActivated = true;
    sConnectedDevice.clientHandle = 12345;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    std::cout <<"[GMOCK] CarPlayStop returning CP_INVALID_HANDLE\n";
    /* Session already started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].ulDevHandle = 56789;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.stopMirroringSession(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP])));

    sleep(1);
    std::cout <<"[GMOCK] CarPlayStop returning CP_NOT_STOPPED\n";
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].ulDevHandle = 56789;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_STOPPED, mirrctrl.stopMirroringSession(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP])));

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].ulDevHandle = 56789;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.stopMirroringSession(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP])));

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].ulDevHandle = 56789;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.stopMirroringSession(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP])));

    delete oOsalMock;
    delete poCpMock;
}

/* Test Case :
 * 1. CarPlayReleaseResource/CarPlayAvailableResource error cases for setAudioFocus
 */
TEST_F(CCTest, CarPlayReleaseResource)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    CPInterfaceMock *poCpMock = CPInterfaceMock::getInstance();
    mirroring::MRM_AudioResourceChange sAudioResourceChange = {};

    init_osal_mocks(oOsalMock);
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    /* CP Mock */
    EXPECT_CALL(*poCpMock, CarPlayAvailableResource(_,_,_) )
        .WillOnce(Return(CP_INVALID_HANDLE) )
        .WillOnce(Return(CP_INVALID_ARGS) )
        .WillOnce(Return(CP_FAILURE) );
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_GAIN;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_MAIN;

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_STARTED;
    /* Does not have audio focus */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].sAudioState.ucHasAudioFocus = 0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].pvCPHandle = (ALES_VOID *)1;

    /* To avoid unc=solicited gain check, set bIsRequested flag */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].sAudioState.ucAudioRequested = 0x1;
       // mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_MAIN;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP]),
         sAudioResourceChange));

    /* To avoid unc=solicited gain check, set bIsRequested flag */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].sAudioState.ucAudioRequested = 0X1;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP]),
         sAudioResourceChange));

    /* To avoid unc=solicited gain check, set bIsRequested flag */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].sAudioState.ucAudioRequested = 1;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP]),
         sAudioResourceChange));

    delete oOsalMock;
    delete poCpMock;
    free(mirCtrlCbs);
}

/* Test Case :
 * 1. CarPlayAppState error cases
 */
TEST_F(CCTest, setNativeAudioSource)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    CPInterfaceMock *poCpMock = CPInterfaceMock::getInstance();

    init_osal_mocks(oOsalMock);
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    /* CP Mock */
    EXPECT_CALL(*poCpMock, CarPlayAppState(_,_,_,_) )
        .WillOnce(Return(CP_INVALID_HANDLE) )
        .WillOnce(Return(CP_INVALID_ARGS) )
        .WillOnce(Return(CP_FAILURE) );

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].pvCPHandle = (ALES_VOID *)1;

    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setNativeAudioSource(0,
         mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_PHONE));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.setNativeAudioSource(0,
         mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_PHONE));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setNativeAudioSource(0,
         mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_PHONE));

    delete oOsalMock;
    delete poCpMock;
    free(mirCtrlCbs);
}

/* Test Case :
 * 1. startMirroringSession error cases for unactivated devices
 */
TEST_F(CCTest, startMirroringSessionFailure)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    CPInterfaceMock *poCpMock = CPInterfaceMock::getInstance();
    MRM_DevInfoInternal sConnectedDevice = {};
    ALES_UINT64 macid = 0;

    init_osal_mocks(oOsalMock);
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_timedwait(_,_) )
        .WillRepeatedly(Return(1));

    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    /* Add a USB unactivated device to the device list */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_CP;
    sConnectedDevice.sDevInfo.ulDevHandle = 56789;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB;
    macid = (static_cast<ALES_UINT64>(12) << 40) |
            (static_cast<ALES_UINT64>(12) << 32) |
            (static_cast<ALES_UINT64>(12) << 24) |
            (static_cast<ALES_UINT64>(12) << 16) |
            (static_cast<ALES_UINT64>(12) << 8) |
            (static_cast<ALES_UINT64>(12));
    sConnectedDevice.sDevInfo.acSerialiNumber = static_cast<ostringstream*>(&(ostringstream()
                << macid))->str();
    sConnectedDevice.bIsUSBDeviceActivated = false;
    sConnectedDevice.clientHandle = 0;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    /* Session created */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_CREATED;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.startMirroringSession(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP]),
         56789, E_MRM_VIDEO_PROJECTION_AUTO));

     /* Add a USB unactivated device to the device list */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_CP;
    sConnectedDevice.sDevInfo.ulDevHandle = 4567;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_WIFI;
    macid = (static_cast<ALES_UINT64>(12) << 40) |
            (static_cast<ALES_UINT64>(12) << 32) |
            (static_cast<ALES_UINT64>(12) << 24) |
            (static_cast<ALES_UINT64>(12) << 16) |
            (static_cast<ALES_UINT64>(12) << 8) |
            (static_cast<ALES_UINT64>(12));
    sConnectedDevice.sDevInfo.acSerialiNumber = static_cast<ostringstream*>(&(ostringstream()
                << macid))->str();
    sConnectedDevice.bIsWiFiDeviceActivated = false;
    sConnectedDevice.clientHandle = 0;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    /* Session created */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_CREATED;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.startMirroringSession(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP]),
         4567, E_MRM_VIDEO_PROJECTION_AUTO));

    free (mirCtrlCbs);
    delete oOsalMock;
    delete poCpMock;
}

/* Test Case :
 * 1. CarPlayReleaseResource/CarPlayAvailableResource error cases for setVideoFocus
 */
TEST_F(CCTest, CarPlayReleaseResourceVideo)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    CPInterfaceMock *poCpMock = CPInterfaceMock::getInstance();
    mirroring::MRM_VideoResourceChange sVideoResourceChange = {};

    init_osal_mocks(oOsalMock);
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    /* CP Mock */
    EXPECT_CALL(*poCpMock, CarPlayAvailableResource(_,_,_) )
        .WillOnce(Return(CP_INVALID_HANDLE) )
        .WillOnce(Return(CP_INVALID_ARGS) );

    sVideoResourceChange.eVideoStreamState = E_MRM_VIDEO_STATE_GAIN;
    sVideoResourceChange.bUnsolicited = false;
    sVideoResourceChange.eVideoFocusType = E_MRM_INTERRUPT_ALLOWED;

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_STARTED;
    /* Does not have video focus */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].sVideoState.ucHasVideoFocus = 0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].pvCPHandle = (ALES_VOID *)1;

    /* To avoid unc=solicited gain check, set bIsRequested flag */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].sVideoState.bVideoRequested = true;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setVideoFocus(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP]),
         sVideoResourceChange));

    /* To avoid unc=solicited gain check, set bIsRequested flag */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].sVideoState.bVideoRequested = true;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setVideoFocus(0,
         reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP]),
         sVideoResourceChange));

    delete oOsalMock;
    delete poCpMock;
    free(mirCtrlCbs);
}
#endif

#ifdef AAP_MOCK
/* Test case :
 * 1. CarPlay stack returning error on startCarPlay
 */

class MRMCbs : public IMRMMirrorControlCallbacks
{
    public:
    MRMCbs():IMRMMirrorControlCallbacks(nullptr){};
    ~MRMCbs(){};
    virtual void onSessionStateChanged(MRM_MIRRORING_HANDLE ulMirroringHandle,
                                       MRM_SessionStatus eSessionStatus,
                                       MRM_ConnectFailReason eReason);
    virtual void onStartMic(MRM_MIRRORING_HANDLE ulMirroringHandle);
    virtual void onStopMic(MRM_MIRRORING_HANDLE ulMirroringHandle);
    virtual void onAudioFocusChange(MRM_MIRRORING_HANDLE ulMirroringHandle,
        MRM_FocusType eFocusType,
        MRM_AudioChannel eAudioChannel,
        MRM_AudioStreamType eAudioStreamType);
};

void MRMCbs::onSessionStateChanged(MRM_MIRRORING_HANDLE ulMirroringHandle,
                                       MRM_SessionStatus eSessionStatus,
                                       MRM_ConnectFailReason eReason)
{
    UNUSED_PARAM(ulMirroringHandle);
    UNUSED_PARAM(eSessionStatus);
    UNUSED_PARAM(eReason);
    std::cout <<"MRMCbs::onSessionStateChanged\n";
}
void MRMCbs::onStartMic(MRM_MIRRORING_HANDLE ulMirroringHandle)
{
    UNUSED_PARAM(ulMirroringHandle);
    std::cout <<"MRMCbs::onStartMic\n";
}
void MRMCbs::onStopMic(MRM_MIRRORING_HANDLE ulMirroringHandle)
{
    UNUSED_PARAM(ulMirroringHandle);
    std::cout <<"MRMCbs::onStopMic\n";
}
void MRMCbs::onAudioFocusChange(MRM_MIRRORING_HANDLE ulMirroringHandle,
        MRM_FocusType eFocusType,
        MRM_AudioChannel eAudioChannel,
        MRM_AudioStreamType eAudioStreamType)
{
    UNUSED_PARAM(ulMirroringHandle);
    UNUSED_PARAM(eFocusType);
    UNUSED_PARAM(eAudioChannel);
    UNUSED_PARAM(eAudioStreamType);
    std::cout <<"MRMCbs::onAudioFocusChange\n";
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, aap_nav_status_cb)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAP_NavigationStatus *psNavStatus = (AAP_NavigationStatus *)
        calloc (1, sizeof(AAP_NavigationStatus));

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    psNavStatus->eMesgTye = AAP_NAV_MSG_TYPE_NEXT_TURN_INFO;
    psNavStatus->sNextTurnInfo.uiRoadLength = 10;
    strcpy((ALES_CHAR *)psNavStatus->sNextTurnInfo.aucRoadName, "Abcd Road");
    psNavStatus->sNextTurnInfo.eTurnSide = AAP_NAV_TURNSIDE_LEFT;

    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_NAME_CHANGE;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_SLIGHT_TURN;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_TURN;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_SHARP_TURN;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_U_TURN;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_ON_RAMP;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_OFF_RAMP;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_FORK;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_MERGE;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_ROUNDABOUT_ENTER;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_ROUNDABOUT_EXIT;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_ROUNDABOUT_ENTER_AND_EXIT;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_STRAIGHT;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_FERRY_BOAT;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_FERRY_TRAIN;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_DESTINATION;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnInfo.eNextTurnEnum = AAP_NAV_NEXT_TURN_ENUM_UNKNOWN;
    aap_nav_status_cb(&mirrctrl, psNavStatus);

    psNavStatus->eMesgTye = AAP_NAV_MSG_TYPE_NEXT_TURN_DIST_INFO;

    psNavStatus->sNextTurnDistance.eDistanceUnit = AAP_NAV_NEXT_TURN_DIST_UNIT_METERS;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnDistance.eDistanceUnit = AAP_NAV_NEXT_TURN_DIST_UNIT_KILOMETERS;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnDistance.eDistanceUnit = AAP_NAV_NEXT_TURN_DIST_UNIT_KILOMETERS_P1;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnDistance.eDistanceUnit = AAP_NAV_NEXT_TURN_DIST_UNIT_MILES;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnDistance.eDistanceUnit = AAP_NAV_NEXT_TURN_DIST_UNIT_MILES_P1;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnDistance.eDistanceUnit = AAP_NAV_NEXT_TURN_DIST_UNIT_FEET;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnDistance.eDistanceUnit = AAP_NAV_NEXT_TURN_DIST_UNIT_YARDS;
    aap_nav_status_cb(&mirrctrl, psNavStatus);
    psNavStatus->sNextTurnDistance.eDistanceUnit = AAP_NAV_NEXT_TURN_DIST_UNIT_UNKNOWN;
    aap_nav_status_cb(&mirrctrl, psNavStatus);

    delete oOsalMock;
    free(mirCtrlCbs);
    free(psNavStatus);
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, aap_phone_status_cb)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAP_PhoneStatus *psPhoneStatus = (AAP_PhoneStatus *)
        calloc (1, sizeof(AAP_PhoneStatus));

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    psPhoneStatus->uiSignStrength = 0;
    aap_phone_status_cb(&mirrctrl, psPhoneStatus);
    psPhoneStatus->uiSignStrength = 20*1;
    aap_phone_status_cb(&mirrctrl, psPhoneStatus);
    psPhoneStatus->uiSignStrength = 20*2;
    aap_phone_status_cb(&mirrctrl, psPhoneStatus);
    psPhoneStatus->uiSignStrength = 20*3;
    aap_phone_status_cb(&mirrctrl, psPhoneStatus);
    psPhoneStatus->uiSignStrength = 20*4;
    aap_phone_status_cb(&mirrctrl, psPhoneStatus);
    psPhoneStatus->uiSignStrength = 20*5;
    aap_phone_status_cb(&mirrctrl, psPhoneStatus);

    delete oOsalMock;
    free(mirCtrlCbs);
    free(psPhoneStatus);
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, create_session_retry)
{
    mirroring::MRM_Response responseObj;
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();

    init_osal_mocks(oOsalMock);

 /* AAP Mock */
#ifdef AAP_GAL_1_4
    EXPECT_CALL(*poAapMock, aap_create_session(_,_,_,_) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_SUCCESS) );
#else
    EXPECT_CALL(*poAapMock, aap_create_session(_,_,_) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_SUCCESS) );
#endif /* AAP_GAL_1_4 */
    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    /* Session registered */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_REGISTERED;
    responseObj = mirrctrl.createMirroringSession(12345,mirroring::MRM_SessionType::E_MRM_AAP, "/etc/aap_system_attributes.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, responseObj.eRetVal);

    EXPECT_CALL(*oOsalMock, ales_osal_file_check_existence(_))
        .WillOnce(Return(1));

    responseObj = mirrctrl.createMirroringSession(0,mirroring::MRM_SessionType::E_MRM_AAP, "/etc/aap_system_attributes.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, responseObj.eRetVal);

    delete oOsalMock;
    delete poAapMock;
    free(mirCtrlCbs);
}

/* Test case :
 * 1.
 */

TEST_F(CCTest, destroy_session_retry)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();
    init_osal_mocks(oOsalMock);
    /* AAP Mock */
    EXPECT_CALL(*poAapMock, aap_destroy_session(_) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_SUCCESS) );
    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    /* Session created */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_CREATED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.destroyMirroringSession(1234,
            reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP])));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.destroyMirroringSession(0,
            reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP])));
    delete oOsalMock;
    delete poAapMock;
    free(mirCtrlCbs);
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, start_session_retry)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();
    MRM_DevInfoInternal sConnectedDevice = {};

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    /* AAP Mock */
    EXPECT_CALL(*poAapMock, aap_start_session(_,_) )
        .WillOnce(Return(AAP_ERR_INVALID_HANDLE) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_SUCCESS) );
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_REGISTERED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;

    /* Add a USB unactivated device to the device list */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_AAP;
    sConnectedDevice.sDevInfo.ulDevHandle = 4567;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SESSION_NOT_CREATED, mirrctrl.startMirroringSession(0,
            reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
            4567, E_MRM_VIDEO_PROJECTION_AUTO));

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_CREATED;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.startMirroringSession(0,
            reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
            4567, E_MRM_VIDEO_PROJECTION_AUTO));

    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.startMirroringSession(0,
            reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
            4567, E_MRM_VIDEO_PROJECTION_AUTO));

    delete oOsalMock;
    delete poAapMock;
    free(mirCtrlCbs);
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, stop_session_retry)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();
    MRM_DevInfoInternal sConnectedDevice = {};

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    /* AAP Mock */
    EXPECT_CALL(*poAapMock, aap_stop_session(_) )
        .WillOnce(Return(AAP_SUCCESS) );

    /* Session created */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;

    /* Add a USB unactivated device to the device list */
    sConnectedDevice.sDevInfo.eCurrSessionType = mirroring::MRM_SessionType::E_MRM_AAP;
    sConnectedDevice.sDevInfo.ulDevHandle = 4567;
    sConnectedDevice.sDevInfo.uiVendorID = 0x05ac;
    sConnectedDevice.sDevInfo.uiProductID = 0x12a8;
    sConnectedDevice.sDevInfo.acDeviceName = "abcd";
    sConnectedDevice.sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB;
    mirrctrl.connectedDevInfoList.push_back(sConnectedDevice);

    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.stopMirroringSession(1234,
            reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP])));

    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.stopMirroringSession(0,
            reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP])));

    delete oOsalMock;
    delete poAapMock;
    free(mirCtrlCbs);
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, send_touch_input)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();
    mirroring::MRM_TouchEvent sTouchEvent = {};
    mirroring::MRM_TouchPoint sTPoint = {};

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    /* AAP Mock */
    EXPECT_CALL(*poAapMock, aap_send_touch_input(_,_) )
        .WillRepeatedly(Return(AAP_SUCCESS) );

    /* Session created */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_PROJECTION_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;

    //sTouchEvent.iActionIndex = 1;
    sTPoint.iX = 0;
    sTPoint.iY = 0;
    sTPoint.iTouchId = 0;
    sTPoint.eTouchAction = mirroring::MRM_TouchAction::E_MRM_TOUCH_ACTION_POINTER_DOWN;
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.sendTouchInput(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sTouchEvent));

    vector<mirroring::MRM_TouchPoint>::reverse_iterator tPointItr;
    tPointItr = sTouchEvent.asTouchPoint.rbegin();
    tPointItr->eTouchAction = mirroring::MRM_TouchAction::E_MRM_TOUCH_ACTION_POINTER_UP;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.sendTouchInput(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sTouchEvent));

    tPointItr->eTouchAction = (mirroring::MRM_TouchAction)5; /* Invalid */
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.sendTouchInput(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sTouchEvent));


    //sTouchEvent.iActionIndex = 11;
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    sTouchEvent.asTouchPoint.push_back(sTPoint);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.sendTouchInput(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sTouchEvent));

    delete oOsalMock;
    delete poAapMock;
    free(mirCtrlCbs);
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, aap_setAudioStreamFocusState1)
{
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();
    mirroring::MRM_AudioResourceChange sAudioResourceChange = {};

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    /* AAP Mock */
    EXPECT_CALL(*poAapMock, aap_send_stream_focus_state(_,_,_,_) )
        .WillRepeatedly(Return(AAP_SUCCESS) );
    EXPECT_CALL(*poAapMock, aap_audio_setup(_,_,_) )
        .WillRepeatedly(Return(AAP_SUCCESS) );

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x1;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_MAIN;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_AUDIO_STATE_GAIN_TRANSIENT;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_NAV;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_GAIN_TRANSIENT;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_MAIN;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x1;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_MAIN;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_AUDIO_STATE_GAIN_TRANSIENT;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_GAIN_TRANSIENT;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_MAIN;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_ALT;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x1;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_MAIN;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_AUDIO_STATE_GAIN_TRANSIENT;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_GAIN_TRANSIENT;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_MAIN;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    delete oOsalMock;
    delete poAapMock;
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, aap_setAudioStreamFocusState2)
{
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();
    mirroring::MRM_AudioResourceChange sAudioResourceChange = {};

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    /* AAP Mock */
    EXPECT_CALL(*poAapMock, aap_send_stream_focus_state(_,_,_,_) )
        .WillRepeatedly(Return(AAP_SUCCESS) );
    EXPECT_CALL(*poAapMock, aap_audio_setup(_,_,_) )
        .WillRepeatedly(Return(AAP_SUCCESS) );

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_ALT;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x1;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_MAIN;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_AUDIO_STATE_GAIN;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_GAIN;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_MAIN;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_MAIN;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_ALT;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_ALT;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_AUDIO_STATE_GAIN_TRANSIENT;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_GAIN_TRANSIENT;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALT;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    delete oOsalMock;
    delete poAapMock;
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, aap_setAudioStreamFocusState3)
{
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();
    mirroring::MRM_AudioResourceChange sAudioResourceChange = {};

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    /* AAP Mock */
    EXPECT_CALL(*poAapMock, aap_send_stream_focus_state(_,_,_,_) )
        .WillRepeatedly(Return(AAP_FAILURE) );
    EXPECT_CALL(*poAapMock, aap_audio_setup(_,_,_) )
        .WillRepeatedly(Return(AAP_FAILURE) );

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_NONE;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_AUDIO_STATE_GAIN;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_READY;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALL;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_NONE;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_AUDIO_STATE_GAIN;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_READY;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALL;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_NONE;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_AUDIO_STATE_GAIN;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_BUSY;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALL;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));


    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_NONE;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_STREAM_STATE_NONE;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_READY;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALL;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_ALLOWED, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_MAIN;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_STREAM_STATE_NONE;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAltChannelFocusState =
        mirroring::MRM_FocusType::E_MRM_FOCUS_GAIN;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_BUSY;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALL;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_ALT;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_STREAM_STATE_NONE;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAltChannelFocusState =
        mirroring::MRM_FocusType::E_MRM_FOCUS_GAIN;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_BUSY;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALL;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucHasAudioFocus = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucAudioRequested = 0x0;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.ucRequestedChannel |=
        mirroring::MRM_AudioChannel::E_MRM_AUDIO_CHANNEL_ALT;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAAPMDAudioReqState = AAP_STREAM_STATE_NONE;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].sAudioState.eAltChannelFocusState =
        mirroring::MRM_FocusType::E_MRM_FOCUS_LOSS;

    mirrctrl.uwCurrentAudioSource = mirroring::MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA;
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_BUSY;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALL;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, mirrctrl.setAudioFocus(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        sAudioResourceChange));

    delete oOsalMock;
    delete poAapMock;
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, getStackVersion)
{
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    EXPECT_CALL(*poAapMock, aap_get_sdk_version(_) )
        .WillRepeatedly(Return(AAP_FAILURE) );
    mirrctrl.getStackVersion(0, mirroring::MRM_StackType::E_MRM_AAP_STACK_VER);
    delete oOsalMock;
    delete poAapMock;
}

/* Test case :
 * 1.
 */
TEST_F(CCTest, sendBtAuthData_failure)
{
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    /*EXPECT_CALL(*poAapMock, aap_bt_ready_to_pair(_,_) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_SUCCESS) );*/

      EXPECT_CALL(*poAapMock, aap_send_bt_auth_data(_,_,_) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) );
        //.WillOnce(Return(AAP_SUCCESS) );

    /* Session started */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_STARTED;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapHandle = 1234;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_RESOURCE_BUSY, mirrctrl.sendBtAuthData(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        std::string("testdata")));

    EXPECT_CALL(*poAapMock, aap_send_bt_auth_data(_,_,_) )
        .WillOnce(Return(AAP_SUCCESS) );
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.sendBtAuthData(0,
        reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP]),
        std::string("testdata")));

    delete oOsalMock;
    delete poAapMock;
}

TEST_F(CCTest, aap_setConfigParams_retry)
{
    mirroring::MRM_Response responseObj;
    MRMCbs mirCtrlCbs;
    OsalMock *oOsalMock = OsalMock::getInstance();
    AAPInterfaceMock *poAapMock = AAPInterfaceMock::getInstance();

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(reinterpret_cast<IMRMMirrorControlCallbacks*>(&mirCtrlCbs));

    /* AAP Mock */
#ifdef AAP_GAL_1_4
    EXPECT_CALL(*poAapMock, aap_create_session(_,_,_,_) )
        .WillRepeatedly(Return(AAP_SUCCESS) );
#else
    EXPECT_CALL(*poAapMock, aap_create_session(_,_,_) )
        .WillRepeatedly(Return(AAP_SUCCESS) );
#endif /*AAP_GAL_1_4 */
    EXPECT_CALL(*poAapMock, aap_set_attributes(_,_) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_ERR_IFACE_BUSY) )
        .WillOnce(Return(AAP_SUCCESS) );

    /* Session registered */
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_REGISTERED;

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapAttributes.auiBTPairMethod = MRM_BT_PAIRING_NUMERIC |
        MRM_BT_PAIRING_PIN | MRM_BT_PAIRING_PASSKEY;
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].aapAttributes.uiNumPairTypes = 3;
    EXPECT_CALL(*oOsalMock, ales_osal_file_check_existence(_))
        .WillRepeatedly(Return(1));
    responseObj = mirrctrl.createMirroringSession(0,mirroring::MRM_SessionType::E_MRM_AAP, "/etc/aap_system_attributes.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, responseObj.eRetVal);

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_REGISTERED;
    responseObj = mirrctrl.createMirroringSession(0,mirroring::MRM_SessionType::E_MRM_AAP, "/etc/aap_system_attributes.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, responseObj.eRetVal);

    delete oOsalMock;
    delete poAapMock;
}

#endif
#if 0
TEST_F(CCTest, startMirroringSession)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    std::cout<<"mirrctrl.serviceHandle: "<<mirrctrl.getServiceHandle()<<"\n";
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.startMirroringSession(1,2,3,mirroring::MRM_VideoProjectionType::E_MRM_VIDEO_PROJECTION_USER));
    // EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.startMirroringSession(0,2,3,mirroring::MRM_VideoProjectionType::E_MRM_VIDEO_PROJECTION_USER));
    free(mirCtrlCbs);
    delete oOsalMock;
}

TEST_F(CCTest, createMirroringSession)
{
    mirroring::MRM_Response responseObj;
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    std::cout<<"mirrctrl.serviceHandle: "<<mirrctrl.getServiceHandle()<<"\n";
    responseObj = mirrctrl.createMirroringSession(1,mirroring::MRM_SessionType::E_MRM_AAP,"/etc/test.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, responseObj.eRetVal);
    responseObj = mirrctrl.createMirroringSession(0,mirroring::MRM_SessionType::E_MRM_NONE,"/etc/test.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, responseObj.eRetVal);

    EXPECT_CALL(*oOsalMock, ales_osal_file_check_existence(_))
        .WillRepeatedly(Return(1));

    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_AAP].lCurrentState = E_MRM_STATE_REGISTERED;
    responseObj = mirrctrl.createMirroringSession(0,mirroring::MRM_SessionType::E_MRM_AAP,"/etc/aap_system_attributes.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, responseObj.eRetVal);
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CP].lCurrentState = E_MRM_STATE_REGISTERED;
    responseObj = mirrctrl.createMirroringSession(0,mirroring::MRM_SessionType::E_MRM_CP,"/etc/carplay_attributes.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, responseObj.eRetVal);
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CL].lCurrentState = E_MRM_STATE_REGISTERED;
    responseObj = mirrctrl.createMirroringSession(0,mirroring::MRM_SessionType::E_MRM_CL,"/etc/carlife_system_attributes.xml");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, responseObj.eRetVal);
    free(mirCtrlCbs);
    delete oOsalMock;
}

TEST_F(CCTest, stopMirroringSession)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.stopMirroringSession(1,2));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.stopMirroringSession(0,5));
    free(mirCtrlCbs);
    delete oOsalMock;
}


TEST_F(CCTest, destroyMirroringSession)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    init_osal_mocks(oOsalMock);

    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.destroyMirroringSession(1,2));
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CL].lCurrentState = E_MRM_STATE_CREATED;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.destroyMirroringSession(0,2));
                //reinterpret_cast<mirroring::MRM_MIRRORING_HANDLE>(&mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CL])));
    free(mirCtrlCbs);
    delete oOsalMock;
}


TEST_F(CCTest, sendTouchInput)
{
    MRM_TouchEvent sTouchEvent;
    MRM_TouchPoint sPoint;
    sTouchEvent.iActionIndex = 1;
    sPoint.iX = 8;
    sPoint.iY = 2;
    sPoint.iTouchId = 1;
    sPoint.eTouchAction = E_MRM_TOUCH_ACTION_DOWN;
    sTouchEvent.asTouchPoint.push_back(sPoint);
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.sendTouchInput(1,2, sTouchEvent));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.sendTouchInput(0,5, sTouchEvent));
    free(mirCtrlCbs);
    delete oOsalMock;
}

TEST_F(CCTest, sendKeyInput)
{
    MRM_KeyEvent sKey;
    sKey.eKeyPressType = MRM_KeyPressType::E_MRM_KEY_PRESS_NORMAL;
    sKey.eKeyType = MRM_KeyType::E_MRM_MEDIA_KEY;
    sKey.eKeyCode = MRM_KeyCodes::E_MRM_KEYCODE_MEDIA_PLAY_PAUSE;
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.sendKeyInput(1, 2, sKey));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.sendKeyInput(0, 2, sKey));
    free(mirCtrlCbs);
    delete oOsalMock;
}

TEST_F(CCTest, sendLocationData)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.sendLocationData(1, 2, "$GPGSA,A,3,04,04,13,03,04,17,,,,,,,0.6,0.4,0.5*36"));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.sendLocationData(0, 2, "$GPGSA,A,3,04,04,13,03,04,17,,,,,,,0.6,0.4,0.5*36"));
    free(mirCtrlCbs);
    delete oOsalMock;
}

TEST_F(CCTest, setNativeAudioSource)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.setNativeAudioSource(1, MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_MEDIA));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, mirrctrl.setNativeAudioSource(0, MRM_NativeAudioSource::E_NATIVE_AUDIO_SRC_PHONE));
    free(mirCtrlCbs);
    delete oOsalMock;
}

TEST_F(CCTest, setAudioFocus)
{
    MRM_AudioResourceChange sAudioResourceChange;
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    sAudioResourceChange.eAudioStreamState = E_MRM_AUDIO_STATE_GAIN;
    sAudioResourceChange.eAudioChannel = E_MRM_AUDIO_CHANNEL_ALL;
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, mirrctrl.setAudioFocus(1, 5, sAudioResourceChange));
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, mirrctrl.setAudioFocus(0, 5, sAudioResourceChange));
    free(mirCtrlCbs);
    delete oOsalMock;
}

TEST_F(CCTest, registerMirrCtrlMediacallback_Invalid)
{
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    MRMUtils::registerMirrCtrlMediacallback(1, NULL);
    free(mirCtrlCbs);
    delete oOsalMock;
}

TEST_F(CCTest, devMgrCtrlRegister)
{
    cout<<"MRMUtils::devCtrlHandle: "<<MRMUtils::devCtrlHandle<<"\n";
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_malloc(_) )
        .WillRepeatedly(Invoke(mem_malloc));
    ON_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillByDefault(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_)).Times(2);
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlRegister(NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);
    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlRegister1)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_UNKNOWN_FAILURE));

    eRetType = devMgrCtrlRegister(NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlRegister2)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillOnce(Return(DEV_MGR_UNKNOWN_FAILURE) );
    eRetType = devMgrCtrlRegister(NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlRegister3)
{
    mirroring::MRM_RetType eRetType;
    MRM::MirroringTypes::RetType iRetval;

    cout<< "iRetval: "<<iRetval<<"\n";
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) )
        .WillRepeatedly(Return(DEV_MGR_UNKNOWN_FAILURE));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) );
    eRetType = devMgrCtrlRegister(NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlRegister4)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_) )
        .WillOnce(Return(DEV_MGR_UNKNOWN_FAILURE) );
    eRetType = devMgrCtrlRegister(NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlRegister5)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_malloc(_) )
        .WillRepeatedly(Invoke(mem_malloc));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillOnce(Return(DEV_MGR_UNKNOWN_FAILURE) );
    eRetType = devMgrCtrlRegister(NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlRegister6)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS))
        .WillOnce(Return(DEV_MGR_PROCESS_ALREADY_REGISTERED))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlRegister(NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SESSION_ALREADY_STARTED, eRetType);
    delete odevMgrInterfaceMock;
    delete oOsalMock;
}

TEST_F(CCTest, devMgrCtrlSendLocationData)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgriPodSendLocationInformation(_,_,_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlSendLocationData("test", 1, NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlSendLocationData1)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgriPodSendLocationInformation(_,_,_,_) )
        .WillOnce(Return(DEV_MGR_IAP_IPOD_IN_STOP_STATE) );

    eRetType = devMgrCtrlSendLocationData("test", 1, NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlSendVehicleData)
{
    mirroring::MRM_RetType eRetType;
    mirroring::MRM_RangeData srangeData;
    srangeData.range = 67;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgriPodSendVehicleUpdates(_,_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) );
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));

    eRetType = devMgrCtrlSendVehicleData(1,
        mirroring::MRM_VehicleDataType::E_MRM_RANGE_DATA, (ALES_VOID *)&srangeData);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlSendVehicleData1)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    eRetType = devMgrCtrlSendVehicleData(1,
        mirroring::MRM_VehicleDataType::E_MRM_RPM, NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_IMPLEMENTED, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlSendVehicleData2)
{
    mirroring::MRM_RetType eRetType;
    mirroring::MRM_OutsideTempData sOutsideTempData;
    sOutsideTempData.outsideTemp = 90;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgriPodSendVehicleUpdates(_,_,_) )
        .WillOnce(Return(DEV_MGR_COMMAND_NOT_SUPPORTED) );
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));

    eRetType = devMgrCtrlSendVehicleData(1,
        mirroring::MRM_VehicleDataType::E_MRM_OUTSIDE_TEMP_DATA, (ALES_VOID *)&sOutsideTempData);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlDeRegService)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrDeRegisterProcess(_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrDeRegisterEAPCallback(_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlDeRegService();
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
}

TEST_F(CCTest, devMgrCtrlDeRegService1)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrDeRegisterProcess(_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) )
        .WillOnce(Return(DEV_MGR_UNKNOWN_FAILURE) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrDeRegisterEAPCallback(_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlDeRegService();
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlDeRegService2)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrDeRegisterProcess(_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrDeRegisterEAPCallback(_) )
        .WillOnce(Return(DEV_MGR_ERROR_DEVICE_OPENED) );

    eRetType = devMgrCtrlDeRegService();
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrCtrlDeRegService3)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    MRMUtils::devCtrlHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrDeRegisterProcess(_,_) )
        .WillRepeatedly(Return(DEV_MGR_UNKNOWN_FAILURE));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrDeRegisterEAPCallback(_) )
        .WillOnce(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlDeRegService();
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, devMgrAppCtrlOpenDevice)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();

    eRetType = devMgrAppCtrlOpenDevice(NULL, 345);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, eRetType);

    delete odevMgrInterfaceMock;
}

TEST_F(CCTest, devMgrAppCtrlOpenDevice1)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();
    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.ulDevMgrHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    cout << "MRMUtils::devCtrlHandle: "<<MRMUtils::devCtrlHandle<<"\n";

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrOpenDevice(_,_) )
        .WillOnce(Return(DEV_MGR_SUCCESS));
    eRetType = devMgrAppCtrlOpenDevice(&mirrctrl, 123);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);
    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
    free(mirrctrl.ulDevMgrHandle);
    mirrctrl.ulDevMgrHandle = nullptr;
}

TEST_F(CCTest, devMgrAppCtrlOpenDevice2)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));

    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.ulDevMgrHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrOpenDevice(_,_) )
        .WillOnce(Return(DEV_MGR_UNKNOWN_FAILURE));

    eRetType = devMgrAppCtrlOpenDevice(&mirrctrl, 123);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
    free(mirrctrl.ulDevMgrHandle);
    mirrctrl.ulDevMgrHandle = nullptr;
}

TEST_F(CCTest, devMgrAppCtrlIPodAppLaunch)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();
    char const *p = "com.baidu.lbs.CarLife";

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    eRetType = devMgrAppCtrlIPodAppLaunch(NULL, 345,
                "com.baidu.CarLifeVehicleProtocol",
                "com.baidu.lbs.CarLife");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
}

TEST_F(CCTest, devMgrAppCtrlIPodAppLaunch1)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    eRetType = devMgrAppCtrlIPodAppLaunch(&mirrctrl, 345,
                NULL, "com.baidu.lbs.CarLife");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
}

TEST_F(CCTest, devMgrAppCtrlIPodAppLaunch2)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);

    eRetType = devMgrAppCtrlIPodAppLaunch(&mirrctrl, 345,
                "com.baidu.CarLifeVehicleProtocol",NULL);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
}

TEST_F(CCTest, devMgrAppCtrlIPodAppLaunch3)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.ulDevMgrHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgriPodLaunchApp(_,_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));

    eRetType = devMgrAppCtrlIPodAppLaunch(&mirrctrl, 345,
                "com.baidu.CarLifeVehicleProtocol",
                "com.baidu.lbs.CarLife");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
    free(mirrctrl.ulDevMgrHandle);
    mirrctrl.ulDevMgrHandle = nullptr;
}

TEST_F(CCTest, devMgrAppCtrlIPodAppLaunch4)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.ulDevMgrHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgriPodLaunchApp(_,_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_IAP_IPOD_IN_STOP_STATE));

    eRetType = devMgrAppCtrlIPodAppLaunch(&mirrctrl, 345,
                "com.baidu.CarLifeVehicleProtocol",
                "com.baidu.lbs.CarLife");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
    free(mirrctrl.ulDevMgrHandle);
    mirrctrl.ulDevMgrHandle = nullptr;
}

TEST_F(CCTest, devMgrAppCtrlCloseDevice)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();

    eRetType = devMgrAppCtrlCloseDevice(NULL, 345);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, eRetType);

    delete odevMgrInterfaceMock;
}

TEST_F(CCTest, devMgrAppCtrlCloseDevice1)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    eRetType = devMgrAppCtrlCloseDevice(&mirrctrl, 345);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
}

TEST_F(CCTest, devMgrAppCtrlCloseDevice2)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.ulDevMgrHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrCloseDevice(_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    eRetType = devMgrAppCtrlCloseDevice(&mirrctrl, 345);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
    free(mirrctrl.ulDevMgrHandle);
    mirrctrl.ulDevMgrHandle = nullptr;
}

TEST_F(CCTest, devMgrAppCtrlCloseDevice3)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.ulDevMgrHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrCloseDevice(_,_) )
        .WillRepeatedly(Return(DEV_MGR_DEVICE_BUSY));
    eRetType = devMgrAppCtrlCloseDevice(&mirrctrl, 345);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
    free(mirrctrl.ulDevMgrHandle);
    mirrctrl.ulDevMgrHandle = nullptr;
}

TEST_F(CCTest, devMgrResetConnectedDev)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.ulDevMgrHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrResetConnectedDevice(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    eRetType = devMgrResetConnectedDev(mirrctrl.ulDevMgrHandle, 123453);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
    free(mirrctrl.ulDevMgrHandle);
    mirrctrl.ulDevMgrHandle = nullptr;
}

TEST_F(CCTest, devMgrResetConnectedDev1)
{
    mirroring::MRM_RetType eRetType;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    mirrctrl.ulDevMgrHandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrResetConnectedDevice(_,_,_,_) )
        .WillOnce(Return(DEV_MGR_UNKNOWN_FAILURE));
    eRetType = devMgrResetConnectedDev(mirrctrl.ulDevMgrHandle, 123453);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(mirCtrlCbs);
    free(mirrctrl.ulDevMgrHandle);
    mirrctrl.ulDevMgrHandle = nullptr;
}

/*updateDeviceList is void function. test by checking only for prints*/
TEST_F(CCTest, updateDeviceList)
{
    updateDeviceList(NULL);
}

/*updateDeviceList is void function. test by checking only for prints*/
TEST_F(CCTest, updateDeviceList1)
{
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    IMRMMirrorControlCallbacks* mirCtrlCbs = (IMRMMirrorControlCallbacks*)
        malloc(sizeof(IMRMMirrorControlCallbacks));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    sDevMgrDeviceList sDevMgrDeviceList = {};
    {
        InSequence s;
        EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrGetNumDevices(_,_) )
            .WillOnce(Return(DEV_MGR_UNKNOWN_FAILURE))
            .RetiresOnSaturation();
        EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrGetNumDevices(_,_) )
            .WillOnce(DoAll(SetArgPointee<1>(0), Return(DEV_MGR_SUCCESS)))
            .RetiresOnSaturation();
        EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrGetNumDevices(_,_) )
            .WillOnce(DoAll(SetArgPointee<1>(1), Return(DEV_MGR_SUCCESS)))
            .RetiresOnSaturation();
        EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrGetDeviceList(_,_,_,_) )
            .WillOnce(Return(DEV_MGR_UNKNOWN_FAILURE))
            .RetiresOnSaturation();
    }
    {
        InSequence s;
        EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrGetNumDevices(_,_) )
            .WillOnce(DoAll(SetArgPointee<1>(1), Return(DEV_MGR_SUCCESS)));
        EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrGetDeviceList(_,_,_,_) )
            .WillOnce(DoAll(SetArgPointee<1>(sDevMgrDeviceList), Return(DEV_MGR_SUCCESS)))
            .RetiresOnSaturation();
    }
    updateDeviceList(&mirrctrl);
    updateDeviceList(&mirrctrl);
    updateDeviceList(&mirrctrl);
    updateDeviceList(&mirrctrl);
    free(mirCtrlCbs);
    free(sDevMgrDeviceList.devHandle);
    delete odevMgrInterfaceMock;
    delete oOsalMock;
}

TEST_F(CCTest, devMgrCtrlBroadcastCpDisCntEvent)
{
    devMgrCtrlBroadcastCpDisCntEvent(123456789, NULL);
}

TEST_F(CCTest, devMgrCtrlBroadcastCpDisCntEvent1)
{
    IMirroringCtrlStubCbAdapter* mirCtrlCbs =
        new IMirroringCtrlStubCbAdapter(NULL);
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    devMgrCtrlBroadcastCpDisCntEvent(123456789, NULL);
    cout<<"devMgrCtrlBroadcastCpDisCntEvent++++++: "<<mirCtrlCbs<<"\n";
    devMgrCtrlBroadcastCpDisCntEvent(123456789, &mirrctrl);
    delete mirCtrlCbs;
    delete oOsalMock;
}

TEST_F(CCTest, devMgrCtrlBroadcastCpCntEvent)
{
    mirroring::MRM_DevInfo sDevInfo;
    devMgrCtrlBroadcastCpCntEvent(sDevInfo, NULL);
}

TEST_F(CCTest, vEnabledCaps)
{
    ALES_BOOL bRet = false;
    IMRMMirrorControlCallbacks* mirCtrlCbs = new IMirroringCtrlStubCbAdapter(NULL);
    sDevMgrDeviceInfo *psDeviceInfoInfo = (sDevMgrDeviceInfo*)
        malloc(sizeof(sDevMgrDeviceInfo));
    // MRM_DevInfoInternal sDevInfoInternal = {};
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    ProcessHandle* processhandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    bRet = vEnabledCaps(processhandle, psDeviceInfoInfo, NULL);
    EXPECT_FALSE(bRet);

    psDeviceInfoInfo->eDeviceType = DEV_MGR_DEVICE_TYPE_CARLIFE;
    psDeviceInfoInfo->eTransportInfo = DEV_MGR_TRANSPORT_USB;
    psDeviceInfoInfo->devHandle = (DeviceHandle)123456789;
    strcpy(psDeviceInfoInfo->sSerialNumber.acString, "360eed33a64e6e334847761a20b3aa752a1da213");
            cout<<"DEBUG +++++++++ mirCtrlCbs: \n"<<mirCtrlCbs;
    bRet = vEnabledCaps(processhandle, psDeviceInfoInfo, &mirrctrl);
    EXPECT_TRUE(bRet);
    delete oOsalMock;
    free(psDeviceInfoInfo);
    delete mirCtrlCbs;
    free(processhandle);
}

TEST_F(CCTest, vDisabledCaps)
{
    ALES_BOOL bRet = false;
    IMRMMirrorControlCallbacks* mirCtrlCbs = new IMirroringCtrlStubCbAdapter(NULL);
    sDevMgrDeviceInfo *psDeviceInfoInfo = (sDevMgrDeviceInfo*)
        malloc(sizeof(sDevMgrDeviceInfo));
    sDevMgrDetachCause *psDetachCause = (sDevMgrDetachCause *)
        malloc(sizeof(sDevMgrDetachCause));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    ProcessHandle* processhandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));
    psDetachCause->uiUserData = 1234;
    psDetachCause->eDetachType = DEV_MGR_DETACH_USER;
    bRet = vDisabledCaps(processhandle, psDeviceInfoInfo, NULL, NULL);
    EXPECT_FALSE(bRet);

    psDeviceInfoInfo->eDeviceType = DEV_MGR_DEVICE_TYPE_CARLIFE;
    psDeviceInfoInfo->eTransportInfo = DEV_MGR_TRANSPORT_USB;
    psDeviceInfoInfo->devHandle = (DeviceHandle)123456789;
    strcpy(psDeviceInfoInfo->sSerialNumber.acString, "360eed33a64e6e334847761a20b3aa752a1da213");
    bRet = vEnabledCaps(processhandle, psDeviceInfoInfo, &mirrctrl);
    EXPECT_TRUE(bRet);
    /*setting device handle which will indicate session is started*/
    mirrctrl.aSessionInfo[mirroring::MRM_SessionType::E_MRM_CL].ulDevHandle = (mirroring::DEVHANDLE)psDeviceInfoInfo->devHandle;

    bRet = vDisabledCaps(processhandle, psDeviceInfoInfo, psDetachCause, &mirrctrl);
    EXPECT_TRUE(bRet);
    delete oOsalMock;
    free(psDetachCause);
    free(psDeviceInfoInfo);
    free(mirCtrlCbs);
    free(processhandle);
}

TEST_F(CCTest, CpCtrlClientConnectEvent)
{
    ALES_BOOL bRet = false;
    IMRMMirrorControlCallbacks* mirCtrlCbs = new IMirroringCtrlStubCbAdapter(NULL);
    sDevMgrDeviceInfo *psDeviceInfoInfo = (sDevMgrDeviceInfo*)
        malloc(sizeof(sDevMgrDeviceInfo));
    sDevMgrDetachCause *psDetachCause = (sDevMgrDetachCause *)
        malloc(sizeof(sDevMgrDetachCause));
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Invoke(mutex_lock));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Invoke(mutex_unlock));
    MirroringCtrlServer mirrctrl(mirCtrlCbs);
    ProcessHandle* processhandle = reinterpret_cast<ProcessHandle*>
        (ales_osal_mem_calloc(1, sizeof(ProcessHandle)));

    psDeviceInfoInfo->eDeviceType = DEV_MGR_DEVICE_TYPE_CARPLAY;
    psDeviceInfoInfo->eTransportInfo = DEV_MGR_TRANSPORT_USB;
    psDeviceInfoInfo->devHandle = (DeviceHandle)123456789;
    strcpy(psDeviceInfoInfo->sSerialNumber.acString, "360eed33a64e6e334847761a20b3aa752a1da213");
    bRet = vEnabledCaps(processhandle, psDeviceInfoInfo, &mirrctrl);
    EXPECT_TRUE(bRet);
    mirroring::MRM_DevInfo sDevInfo;
    sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB;
    bRet = CpCtrlClientConnectEvent(sDevInfo, *(reinterpret_cast<ALES_UINT64 *>(psDeviceInfoInfo)), &mirrctrl);
    EXPECT_FALSE(bRet);
    psDetachCause->uiUserData = 1234;
    psDetachCause->eDetachType = DEV_MGR_DETACH_USER;
   /* bRet = vDisabledCaps(processhandle, psDeviceInfoInfo, psDetachCause, &mirrctrl);
    printf("\n\n 5 \n\n");
    EXPECT_TRUE(bRet);*/
    sDevInfo.iTransportType = mirroring::MRM_DeviceTransportType::E_MRM_DEVICETYPE_USB;
    bRet = CpCtrlClientConnectEvent(sDevInfo, *(reinterpret_cast<ALES_UINT64 *>(psDeviceInfoInfo)), &mirrctrl);
    EXPECT_FALSE(bRet);
    delete oOsalMock;
    free(psDetachCause);
    free(psDeviceInfoInfo);
    free(mirCtrlCbs);
    free(processhandle);
}

TEST_F(CCTest, getServiceHandleRegisterCbParams)
{
    mirroring::SERVICE_HANDLE handle;
    OsalMock *oOsalMock = OsalMock::getInstance();
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    handle = MRMUtils::getServiceHandleRegisterCbParams(0, "");
    EXPECT_EQ(0, handle);
    handle = MRMUtils::getServiceHandleRegisterCbParams(0, MIRRCTRLSERVINST);
    EXPECT_EQ(0, handle);
    handle = MRMUtils::getServiceHandleRegisterCbParams(0, MEDIASERVINST);
    EXPECT_EQ(0, handle);
    handle = MRMUtils::getServiceHandleRegisterCbParams(0, NAVISERVINST);
    EXPECT_EQ(0, handle);
    handle = MRMUtils::getServiceHandleRegisterCbParams(0, TELESERVINST);
    EXPECT_EQ(0, handle);
    handle = MRMUtils::getServiceHandleRegisterCbParams(0, "TESTINST");
    EXPECT_EQ(0, handle);
    delete oOsalMock;
}

TEST_F(CCTest, MRMUtilPlay)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 123456789;
    mirroring::MRM_RetType eRetType;
    eRetType = MRMUtils::MRMUtilPlay(123456789);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_CTRL_SESSION_NOT_CREATED, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(123456789,
        456987123, mirroring::MRM_SessionType::E_MRM_AAP, 777777);
    eRetType = MRMUtils::MRMUtilPlay(0);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SESSION_NOT_STARTED, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

TEST_F(CCTest, MRMUtilPause)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 123456789;
    mirroring::MRM_RetType eRetType;
    eRetType = MRMUtils::MRMUtilPause(123456789);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_CTRL_SESSION_NOT_CREATED, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(123456789,
        456987123, mirroring::MRM_SessionType::E_MRM_AAP, 777777);
    eRetType = MRMUtils::MRMUtilPause(0);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SESSION_NOT_STARTED, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

TEST_F(CCTest, MRMUtilNextsong)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 123456789;
    mirroring::MRM_RetType eRetType;
    eRetType = MRMUtils::MRMUtilNextsong(123456789);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_CTRL_SESSION_NOT_CREATED, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(123456789,
        456987123, mirroring::MRM_SessionType::E_MRM_AAP, 777777);
    eRetType = MRMUtils::MRMUtilNextsong(0);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SESSION_NOT_STARTED, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

TEST_F(CCTest, MRMUtilPrevsong)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 123456789;
    mirroring::MRM_RetType eRetType;
    eRetType = MRMUtils::MRMUtilPrevsong(123456789);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_CTRL_SESSION_NOT_CREATED, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(123456789,
        456987123, mirroring::MRM_SessionType::E_MRM_AAP, 777777);
    eRetType = MRMUtils::MRMUtilPrevsong(0);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SESSION_NOT_STARTED, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

TEST_F(CCTest, getHandleParams)
{
    MRM_HandleParams vHandleParams;
    size_t handle = 123456789;
    vHandleParams = MRMUtils::getHandleParams(&handle, E_MRM_MIRRORING_SESSION_HANDLE);
    EXPECT_EQ(0, vHandleParams.ulMirrCtrlHandle);
    vHandleParams = MRMUtils::getHandleParams(&handle, E_MRM_MEDIA_SESSION_HANDLE);
    EXPECT_EQ(0, vHandleParams.ulMediaHandle);
    vHandleParams = MRMUtils::getHandleParams(&handle, E_MRM_NAVI_HANDLE);
    vHandleParams = MRMUtils::getHandleParams(&handle, E_MRM_TELE_HANDLE);
}

TEST_F(CCTest, getMediaDevicesInfoList)
{
    std::vector<mirroring::MRM_MediaDevInfo> vMediaDevInfo;
    // OsalMock *oOsalMock = OsalMock::getInstance();

    // EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
    //     .WillRepeatedly(Return(0));
    // EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
    //     .WillRepeatedly(Return(0));
    vMediaDevInfo = MRMUtils::getMediaDevicesInfoList();
    EXPECT_EQ(0, vMediaDevInfo.size());
    // MRMUtils::setResetStubHandle(myService, "MRM_MirrCtrlService1",
    //     true);

    // vMediaDevInfo = MRMUtils::getMediaDevicesInfoList();
    // EXPECT_EQ(0, vMediaDevInfo.size());
    // delete oOsalMock;
}

TEST_F(CCTest, getNavDevicesInfoList)
{
    std::vector<mirroring::MRM_NAVDevInfo> vNavDevInfo;
    // OsalMock *oOsalMock = OsalMock::getInstance();

    // EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
    //     .WillRepeatedly(Return(0));
    // EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
    //     .WillRepeatedly(Return(0));
    vNavDevInfo = MRMUtils::getNavDevicesInfoList();
    EXPECT_EQ(0, vNavDevInfo.size());
    // MRMUtils::setResetStubHandle(myService, "MRM_MirrCtrlService1",
    //     true);

    // vMediaDevInfo = MRMUtils::getNavDevicesInfoList();
    // EXPECT_EQ(0, vMediaDevInfo.size());
    // delete oOsalMock;
}

TEST_F(CCTest, getTelephonyDevicesInfoList)
{
    std::vector<mirroring::MRM_TelDevInfo> vTelDevInfo;
    // OsalMock *oOsalMock = OsalMock::getInstance();

    // EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
    //     .WillRepeatedly(Return(0));
    // EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
    //     .WillRepeatedly(Return(0));
    vTelDevInfo = MRMUtils::getTelephonyDevicesInfoList();
    EXPECT_EQ(0, vTelDevInfo.size());
    // MRMUtils::setResetStubHandle(myService, "MRM_MirrCtrlService1",
    //     true);

    // vMediaDevInfo = MRMUtils::getTelephonyDevicesInfoList();
    // EXPECT_EQ(0, vMediaDevInfo.size());
    // delete oOsalMock;
}

TEST_F(CCTest, MRMUtilNaviLaunch)
{
    mirroring::MRM_RetType eRetType;
    eRetType = MRMUtils::MRMUtilNaviLaunch(12343435);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_CTRL_SESSION_NOT_CREATED,
        eRetType);
}

TEST_F(CCTest, TelephonyAcceptCall)
{
    mirroring::MRM_RetType eRetType;
    eRetType = MRMUtils::TelephonyAcceptCall(12343435);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_CTRL_SESSION_NOT_CREATED,
        eRetType);
}

TEST_F(CCTest, TelephonyRejectCall)
{
    mirroring::MRM_RetType eRetType;
    eRetType = MRMUtils::TelephonyRejectCall(12343435);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_CTRL_SESSION_NOT_CREATED,
        eRetType);
}

TEST_F(CCTest, TelephonyLaunch)
{
    mirroring::MRM_RetType eRetType;
    eRetType = MRMUtils::TelephonyLaunch(12343435);
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_CTRL_SESSION_NOT_CREATED,
        eRetType);
}

TEST_F(CCTest, registerNaviState)
{
    MRMUtils::registerNaviState(NULL, NULL);
}

TEST_F(CCTest, registerTelephonyState)
{
    MRMUtils::registerTelephonyState(NULL, NULL);
}

TEST_F(CCTest, getCPSessionHandleParams)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 123456789;
    MRMUtils::getCPSessionHandleParams();
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(123456789,
        456987123, mirroring::MRM_SessionType::E_MRM_AAP, 777777);
    MRMUtils::getCPSessionHandleParams();
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

TEST_F(CCTest, registerGetActiveDevList)
{
    mirroring::MRM_RetType eRetType;
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    eRetType = MRMUtils::registerGetActiveDevList(NULL, NULL, "");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, eRetType);
    delete oOsalMock;
}

TEST_F(CCTest, registerDevCtrlService)
{
    mirroring::MRM_RetType eRetType;
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    eRetType = MRMUtils::registerDevCtrlService(NULL, NULL, "");
    EXPECT_EQ(mirroring::MRM_RetType::E_MRM_NOT_REGISTERED, eRetType);
    delete oOsalMock;
}

TEST_F(CCTest, deregisterDevCtrlService)
{
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    MRMUtils::deregisterDevCtrlService(NULL);
    delete oOsalMock;
}

TEST_F(CCTest, broadcastSessionState)
{
    sDevMgrDeviceInfo *psDeviceInfoInfo = (sDevMgrDeviceInfo*)
        malloc(sizeof(sDevMgrDeviceInfo));
    psDeviceInfoInfo->eDeviceType = DEV_MGR_DEVICE_TYPE_CARPLAY;
    psDeviceInfoInfo->eTransportInfo = DEV_MGR_TRANSPORT_USB;
    psDeviceInfoInfo->devHandle = (DeviceHandle)123456789;
    strcpy(psDeviceInfoInfo->sSerialNumber.acString, "360eed33a64e6e334847761a20b3aa752a1da213");
    mirroring::MRM_DevInfo sDevInfo = {};
    ALES_BOOL bRet = false;
    bRet = MRMUtils::broadcastSessionState(sDevInfo, *(reinterpret_cast<ALES_UINT64 *>(psDeviceInfoInfo)),
            E_MRM_DEVMGR_ENABLED);
    EXPECT_FALSE(bRet);
}

TEST_F(CCTest, setMediaSharedPtr)
{
    EXPECT_EQ(nullptr, MRMUtils::setMediaSharedPtr(0));
}

TEST_F(CCTest, registerMirrCtrlMediacallback)
{
    MRMUtils::registerMirrCtrlMediacallback(0, NULL);
}

TEST_F(CCTest, registerMirrCtrlNavicallback)
{
    MRMUtils::registerMirrCtrlNavicallback(NULL, 0);
}

TEST_F(CCTest, registerMirrCtrlTelephonycallback)
{
    MRMUtils::registerMirrCtrlTelephonycallback(NULL, 0);
}

TEST_F(CCTest, setMediaSessionHandle)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 0;
    MRMUtils::setMediaSessionHandle(0, 0, 1234, NULL);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    sDevInfo.ulDevHandle = 123456789;
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::setMediaSessionHandle(123456789, 0, 1234, NULL);
    MRMUtils::setMediaSessionHandle(123456789, 2341, 1234, NULL);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

TEST_F(CCTest, setNaviSessionHandle)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 0;
    MRMUtils::updateDevHandleList(sDevInfo, true);
    sDevInfo.ulDevHandle = 123456789;
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::setNaviSessionHandle(123456789, 1234);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

TEST_F(CCTest, setTelephonySessionHandle)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 0;
    MRMUtils::updateDevHandleList(sDevInfo, true);
    sDevInfo.ulDevHandle = 123456789;
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::setTelephonySessionHandle(123456789, 1234);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

/* MRM_UTILS_PLATFORM.CPP functions */
TEST_F(CCTest, sendTurnByTurn)
{
    mirroring::MRM_RetType eRetType;
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 1234;
    mirroring::MRM_NaviTBTInfo sNaviTBTInfo;
    NaviCtrlServer sNaviCtrlServer;
    mirroring::MRM_Response sResponseObj;
    std::shared_ptr<INavigationStubImpl> myService =
        std::make_shared<INavigationStubImpl>();
    sNaviTBTInfo.eMesgTye = (mirroring::MRM_NavMessageType) 987;
    //eRetType = MRMUtils::sendTurnByTurn(1234, sNaviTBTInfo);
    MRMUtils::sendTurnByTurn(1234, sNaviTBTInfo);
    //EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(1234,
        456987123, mirroring::MRM_SessionType::E_MRM_AAP, 12356342);
    MRMUtils::setResetStubHandle(myService, NAVISERVINST,
        true);
    sResponseObj = sNaviCtrlServer.Register(12356342, NAVISERVINST);
    //eRetType = MRMUtils::sendTurnByTurn(1234, sNaviTBTInfo);
    MRMUtils::sendTurnByTurn(1234, sNaviTBTInfo);
    /*EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);
    eRetType = sNaviCtrlServer.openNavSession(sResponseObj.handle, (mirroring::DEVHANDLE)1234);
    eRetType = MRMUtils::sendTurnByTurn(1234, sNaviTBTInfo);*/
    MRMUtils::sendTurnByTurn(1234, sNaviTBTInfo);
    //EXPECT_EQ(mirroring::MRM_RetType::E_MRM_INVALID_ARGS, eRetType);
    MRMUtils::setResetStubHandle(myService, NAVISERVINST,
        false);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}


TEST_F(CCTest, removeMirrCntlSessionHandle)
{
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 1234;
    MRMUtils::removeMirrCntlSessionHandle((mirroring::MRM_MIRRORING_HANDLE) 90777656);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::removeMirrCntlSessionHandle((mirroring::MRM_MIRRORING_HANDLE) 0);
    MRMUtils::updateDevHandleList(sDevInfo, false);
}

TEST_F(CCTest, OnMirroringCtrlSessionCompleted)
{
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    MRMUtils::OnMirroringCtrlSessionCompleted(123456789,
        456987123, mirroring::MRM_SessionType::E_MRM_CP, 777777);
    delete oOsalMock;
}

TEST_F(CCTest, MRMUtilsEnabledCaps)
{
    sDevMgrDeviceInfo sdeviceInfo = {};

    sdeviceInfo.eDeviceType = DEV_MGR_DEVICE_TYPE_IAP_BT;
    mirroring::MRM_RetType eRetType;
    sServiceCallbacks sCallbackStr = {};
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillOnce(SaveArgPointee<2>(&sCallbackStr))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlRegister(NULL);

    sCallbackStr.psClbMirror->vDevMgrEnabledCap_Callback(NULL, nullptr, NULL);
    sCallbackStr.psClbMirror->vDevMgrEnabledCap_Callback(NULL, &sdeviceInfo, NULL);
    sdeviceInfo.eDeviceType = (eDevMgrDeviceType) 123;
    sCallbackStr.psClbMirror->vDevMgrEnabledCap_Callback(NULL, &sdeviceInfo, NULL);
    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, MRMUtilsDisabledCaps)
{
    sDevMgrDeviceInfo sdeviceInfo = {};
    mirroring::MRM_RetType eRetType;
    sdeviceInfo.eDeviceType = DEV_MGR_DEVICE_TYPE_IAP_BT;
    sDevMgrDetachCause sDetachCause;
    sServiceCallbacks sCallbackStr = {};
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillOnce(SaveArgPointee<2>(&sCallbackStr))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlRegister(NULL);

    sCallbackStr.psClbMirror->vDevMgrDisabledCap_Callback(NULL, nullptr, nullptr, NULL);
    sCallbackStr.psClbMirror->vDevMgrDisabledCap_Callback(NULL, &sdeviceInfo, nullptr, NULL);
    sDetachCause.eDetachType = DEV_MGR_DETACH_USER;
    sCallbackStr.psClbMirror->vDevMgrDisabledCap_Callback(NULL, &sdeviceInfo, &sDetachCause, NULL);
    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, MRMUtilsNowPlayingUpdates)
{
    sDevMgrDeviceInfo sdeviceInfo = {};

    sdeviceInfo.eDeviceType = DEV_MGR_DEVICE_TYPE_IAP_BT;
    mirroring::MRM_RetType eRetType;
    sMMCallbacks sMRMCbs = {};
    sDevMgr_MediaItemInfo sMediaitemInfo = {};
    sDevMgr_PlaybackInfo sPlaybackInfo = {};
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 9877664123;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();
    OsalMock *oOsalMock = OsalMock::getInstance();
    std::shared_ptr<IMirroringCtrlStubImpl> myService =
        std::make_shared<IMirroringCtrlStubImpl>();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_malloc(_) )
        .WillRepeatedly(Invoke(mem_malloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillOnce(SaveArgPointee<1>(&sMRMCbs))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlRegister(NULL);

    sMediaitemInfo.validInfo = BIT(6U);
    strcpy(sMediaitemInfo.album.pValue, "Tst Album");
    sMediaitemInfo.validInfo |= BIT(12U);
    strcpy(sMediaitemInfo.artist.pValue, "Tst Artist");
    sMediaitemInfo.validInfo |= BIT(14U);
    strcpy(sMediaitemInfo.albumArtist.pValue, "Tst AlbumArtist");
    sPlaybackInfo.validInfo = BIT(0U);
    sPlaybackInfo.shuffleMode = (sDevMgr_ShuffleType) 2;/*E_IAP_SHUFFLE_ALBUMS*/
    sPlaybackInfo.status = (sDevMgr_PlayStatusType) 0;/*E_IAP_PLAY_STATUS_STOPPED*/
    sMRMCbs.vNowPlayingUpdates(NULL, NULL, nullptr, nullptr);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(9877664123,
        45698712313, mirroring::MRM_SessionType::E_MRM_CP, 12356342);
    MRMUtils::setResetStubHandle(myService, MIRRCTRLSERVINST, true);
    sMRMCbs.vNowPlayingUpdates(NULL, NULL, &sMediaitemInfo, &sPlaybackInfo);
    MRMUtils::updateDevHandleList(sDevInfo, false);
    MRMUtils::setResetStubHandle(myService, MIRRCTRLSERVINST, false);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, MRMUtilsCommunicationsUpdate)
{
    MRMUtilsCommunicationsUpdate(NULL, NULL, nullptr);
}

TEST_F(CCTest, MRMUtilsCallStateUpdate)
{
    OsalMock *oOsalMock = OsalMock::getInstance();
    mirroring::MRM_RetType eRetType;
    std::shared_ptr<IMirroringCtrlStubImpl> myService =
        std::make_shared<IMirroringCtrlStubImpl>();
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_malloc(_) )
        .WillRepeatedly(Invoke(mem_malloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlRegister(NULL);

    sDevMgr_CallStateUpdate sCallStateUpdate = {};
    mirroring::MRM_CallInfo callInfo = {};
    sCallStateUpdate.status = (eDevMgr_CallStateStatus) 5; /*E_IAP2_HELD*/
    sCallStateUpdate.validInfo |= BIT(2); /*IAP2_CALLSTATE_STATUS*/
    sCallStateUpdate.validInfo |= BIT(9);/*IAP2_CALLSTATE_ISCONFERENCED*/
    MRMUtilsCallStateUpdate(NULL, NULL, nullptr);
    MRMUtils::setResetStubHandle(myService, MIRRCTRLSERVINST, true);
    MRMUtilsCallStateUpdate(NULL, NULL, &sCallStateUpdate);
    MRMUtils::setResetStubHandle(myService, MIRRCTRLSERVINST, false);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}


TEST_F(CCTest, startVehicleSensorData)
{
    mirroring::MRM_RetType eRetType;
    OsalMock *oOsalMock = OsalMock::getInstance();
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 123456789;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();

    std::shared_ptr<IMirroringCtrlStubImpl> myService =
        std::make_shared<IMirroringCtrlStubImpl>();
    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_malloc(_) )
        .WillRepeatedly(Invoke(mem_malloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));

    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlRegister(NULL);

    //eRetType = MRMUtils::startVehicleSensorData(654897321, 31564);
    MRMUtils::startVehicleSensorData(E_MRM_IAP_START_VEHICLE, 31564);
   // EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(123456789,
        456987123, mirroring::MRM_SessionType::E_MRM_AAP, 777777);
    MRMUtils::setResetStubHandle(myService, MIRRCTRLSERVINST, true);
    MRMUtils::startVehicleSensorData(E_MRM_IAP_START_VEHICLE, 31564);
    //eRetType = MRMUtils::startVehicleSensorData(654897321, 31564);
    //EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);
    MRMUtils::setResetStubHandle(myService, MIRRCTRLSERVINST, false);
    MRMUtils::updateDevHandleList(sDevInfo, false);

    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;

}

TEST_F(CCTest, stopVehicleSensorData)
{
    mirroring::MRM_RetType eRetType;
    OsalMock *oOsalMock = OsalMock::getInstance();

   // init_osal_mocks(oOsalMock);
    std::shared_ptr<IMirroringCtrlStubImpl> myService =
        std::make_shared<IMirroringCtrlStubImpl>();
    mirroring::MRM_DevInfo sDevInfo = {};
    sDevInfo.ulDevHandle = 123456789;
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_malloc(_) )
        .WillRepeatedly(Invoke(mem_malloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );

    eRetType = devMgrCtrlRegister(NULL);

    //MRMUtils::stopVehicleSensorData(654897321, 31564);
    MRMUtils::stopVehicleSensorData(E_MRM_IAP_STOP_VEHICLE);
    //EXPECT_EQ(mirroring::MRM_RetType::E_MRM_FAILURE, eRetType);
    MRMUtils::updateDevHandleList(sDevInfo, true);
    MRMUtils::OnMirroringCtrlSessionCompleted(123456789,
        456987123, mirroring::MRM_SessionType::E_MRM_AAP, 777777);
    MRMUtils::setResetStubHandle(myService, MIRRCTRLSERVINST, true);
    //eRetType = MRMUtils::stopVehicleSensorData(654897321, 31564);
    MRMUtils::stopVehicleSensorData(E_MRM_IAP_STOP_LOCATION);
    //EXPECT_EQ(mirroring::MRM_RetType::E_MRM_SUCCESS, eRetType);
    MRMUtils::setResetStubHandle(myService, MIRRCTRLSERVINST, false);
    MRMUtils::updateDevHandleList(sDevInfo, false);
    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}

TEST_F(CCTest, setResetStubHandle)
{
    mirroring::MRM_RetType eRetType;
    std::vector<mirroring::MRM_TelDevInfo> vTelDevInfo;
    OsalMock *oOsalMock = OsalMock::getInstance();
    devMgrInterfaceMock *odevMgrInterfaceMock = devMgrInterfaceMock::getInstance();

    EXPECT_CALL(*oOsalMock, ales_osal_mem_calloc(_,_) )
        .WillRepeatedly(Invoke(mem_calloc));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_malloc(_) )
        .WillRepeatedly(Invoke(mem_malloc));
    EXPECT_CALL(*oOsalMock, ales_osal_memset(_,_,_) )
        .WillRepeatedly(Invoke(mem_memset));
    EXPECT_CALL(*oOsalMock, ales_osal_mem_free(_) )
        .WillRepeatedly(Invoke(mem_free));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_create(_) )
        .WillRepeatedly(Invoke(mutex_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_condition_create(_) )
        .WillRepeatedly(Invoke(thread_condition_create));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_create(_,_,_) )
        .WillRepeatedly(Invoke(semaphore_create));
    EXPECT_CALL(*oOsalMock, ales_osal_thread_create(_,_,_,_) )
        .WillRepeatedly(Invoke(thread_create));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_lock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_mutex_unlock(_) )
        .WillRepeatedly(Return(0));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_wait(_) )
        .WillRepeatedly(Invoke(semaphore_wait));
    EXPECT_CALL(*oOsalMock, ales_osal_semaphore_post(_) )
        .WillRepeatedly(Invoke(semaphore_post));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterProcess(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS));
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterMirroringCallback(_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterEAPCallback(_,_))
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    EXPECT_CALL(*odevMgrInterfaceMock, eDevMgrRegisterDeviceApplication(_,_,_,_) )
        .WillRepeatedly(Return(DEV_MGR_SUCCESS) );
    eRetType = devMgrCtrlRegister(NULL);
    std::shared_ptr<IMediaStubImpl> myMediaService =
        std::make_shared<IMediaStubImpl>();
    std::shared_ptr<IMirroringCtrlStubImpl> myMirrCtrlService =
        std::make_shared<IMirroringCtrlStubImpl>();
    std::shared_ptr<INavigationStubImpl> myNavService =
        std::make_shared<INavigationStubImpl>();
    std::shared_ptr<ITelephonyStubImpl> myTeleService =
        std::make_shared<ITelephonyStubImpl>();
    std::shared_ptr<IMirroringCtrlStubImpl> myMirrCtrlService1 =
        std::make_shared<IMirroringCtrlStubImpl>();
    std::shared_ptr<IMediaStubImpl> myMediaService1 =
        std::make_shared<IMediaStubImpl>();
    MRMUtils::setResetStubHandle(myMirrCtrlService, "",
        true);
    MRMUtils::setResetStubHandle(NULL, MIRRCTRLSERVINST,
        true);
    MRMUtils::setResetStubHandle(NULL, MEDIASERVINST,
        true);
    MRMUtils::setResetStubHandle(NULL, NAVISERVINST,
        true);
    MRMUtils::setResetStubHandle(NULL, TELESERVINST,
        true);
    MRMUtils::setResetStubHandle(myMirrCtrlService, MIRRCTRLSERVINST,
        false);
    MRMUtils::setResetStubHandle(myMediaService, MEDIASERVINST,
        false);
    MRMUtils::setResetStubHandle(myNavService, NAVISERVINST,
        false);
    MRMUtils::setResetStubHandle(myTeleService, TELESERVINST,
        false);
    MRMUtils::setResetStubHandle(myMediaService, "TESTIFACE",
        true);
    MRMUtils::setResetStubHandle(myMirrCtrlService, "MRM_MirrCtrlService1",
        true);
    MRMUtils::setResetStubHandle(myMediaService, "MRM_MediaService1",
        true);
    MRMUtils::setResetStubHandle(myMirrCtrlService1, MIRRCTRLSERVINST,
        false);
    MRMUtils::setResetStubHandle(myMediaService1, MEDIASERVINST,
        false);
    MRMUtils::setResetStubHandle(myMediaService, "MRM_MediaService1",
        false);
    delete odevMgrInterfaceMock;
    delete oOsalMock;
    free(MRMUtils::devCtrlHandle);
    MRMUtils::devCtrlHandle = nullptr;
}
#endif
int main(int argc, char **argv)
{
    int iRet = 0;
    ::testing::InitGoogleMock(&argc, argv);
    iRet = RUN_ALL_TESTS();
    (void)iRet;
#ifdef GCOV_FLUSH
    __gcov_flush();
#endif /* ifdef GCOV_FLUSH */
    printf("Exiting main\n");
    return 0;
}
