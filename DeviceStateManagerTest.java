/********************************************************************
 *
 *   VISTEON TECHNICAL & SERVICES CENTRE CONFIDENTIAL PROPRIETARY
 *
 *
 *   FILENAME       - DeviceControlManagerServiceTest.java
 *
 *
 *********************************************************************
 *   CHANGE HISTORY
 *   -----------------------------------------------------------------
 *   DATE        REVISION      AUTHOR             COMMENTS
 *   -----------------------------------------------------------------
 *   22/12/22    1.0           Neeraj Nama        Initial version
 *********************************************************************
 *  File Description
 *  ---------------------
 * RemoteUIService Unit Test
 ********************************************************************/

package com.allgo.devicecontrolmanager;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.content.SharedPreferences;
import android.media.AudioManager;
import android.os.Binder;
import android.os.Bundle;
import android.os.Looper;
import android.os.Process;
import android.os.SystemProperties;
import android.os.UserManager;

import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.test.suitebuilder.annotation.MediumTest;
import android.test.mock.MockContentResolver;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.concurrent.CopyOnWriteArrayList;
import java.util.logging.Logger;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import android.util.Log;
import com.allgo.devicecontroller.DeviceInfo;
import com.allgo.devicecontrolmanager.DeviceStateManager;
import com.allgo.DevCtrlMgrIface.*;
import static org.junit.Assert.*;

@MediumTest
@RunWith(AndroidJUnit4.class)
public class DeviceStateManagerTest {
    private DeviceStateManager mDeviceStateManager;

    private @Mock Context mMockContext;
    private @Mock android.app.Application mApplication;
    private @Mock ApplicationInfo mMockApplicationInfo;
    private @Mock Resources mMockResources;
    private @Mock Binder mBinder;
    private @Mock AudioManager mAudioManager;
    private @Mock UserManager mUserManager;
    @Mock
    private BluetoothInterface mockBluetoothInterface;
    @Mock
    private ApplicationInterface mockApplicationInterface;
    @Mock
    private RemoteUIInterface mockRemoteUIInterface;
    @Mock
    private DeviceControlService mockDeviceControlService;
    @Mock
    private CommonInterface mockCommonInterface;
    @Mock
    private PluginInterface mockPluginInterface;

    @Mock
    private RemotableTransport transport;
    @Mock
    private RemotableCapability capability;


    private static final int CONTEXT_SWITCH_MS = 100;
    private static final int ONE_SECOND_MS = 1000;
    private static final int NATIVE_INIT_MS = 8000;

    private PackageManager mMockPackageManager;
    private MockContentResolver mMockContentResolver;

    @Before
    public void setUp() throws PackageManager.NameNotFoundException {
        if (Looper.myLooper() == null) {
            Looper.prepare();

        }
        Assert.assertNotNull(Looper.myLooper());


        mMockPackageManager = mock(PackageManager.class);
        mMockContentResolver = new MockContentResolver(mMockContext);
        MockitoAnnotations.initMocks(this);

        when(mMockContext.getApplicationInfo()).thenReturn(mMockApplicationInfo);
        when(mMockContext.getContentResolver()).thenReturn(mMockContentResolver);
        when(mMockContext.getApplicationContext()).thenReturn(mMockContext);
        when(mMockContext.getResources()).thenReturn(mMockResources);
        when(mMockContext.getUserId()).thenReturn(Process.SYSTEM_UID);
        when(mMockContext.getPackageManager()).thenReturn(mMockPackageManager);
        when(mMockContext.getSystemService(Context.AUDIO_SERVICE)).thenReturn(mAudioManager);
        when(mMockContext.getSystemService(Context.USER_SERVICE)).thenReturn(mUserManager);
        when(mUserManager.isUserUnlocked()).thenReturn(true);

        final SharedPreferences sharedPrefs = mock(SharedPreferences.class);
        when(mMockContext.getSharedPreferences((java.io.File)any(),anyInt())).thenReturn(sharedPrefs);
        when(mMockContext.createDeviceProtectedStorageContext()).thenReturn(mMockContext);



        doReturn(Process.SYSTEM_UID).when(mMockPackageManager)
                .getPackageUidAsUser(any(),anyInt(),anyInt());

        InstrumentationRegistry.getInstrumentation().runOnMainSync(new Runnable() {
            @Override
            public void run() {
                mDeviceStateManager = new DeviceStateManager(mMockContext);
            }
        });
    }


    @Test
    public void testRegisterInterface_WithValidObjectAndInterfaceType_ShouldSetInterface() {
// Arrange
        Object bluetoothInterfaceObj = mockBluetoothInterface;
        int interfaceType = DeviceControlManager.BLUETOOTH_INTERFACE;

// Act
        mDeviceStateManager.registerInterface(bluetoothInterfaceObj, interfaceType);

// Assert
        assertEquals(mockBluetoothInterface, mDeviceStateManager.getBluetoothInterface());
// Verify that the other interface objects remain null
        assertNull(mDeviceStateManager.getApplicationInterface());
        assertNull(mDeviceStateManager.getRemoteUIInterface());
        assertNull(mDeviceStateManager.getDeviceControlService());
        assertNull(mDeviceStateManager.getCommonInterface());
        assertNull(mDeviceStateManager.getPluginInterface());
    }

    @Test
    public void testRegisterInterface_WithNullObject_ShouldNotSetInterface() {
// Arrange
        Object nullObj = null;
        int interfaceType = DeviceControlManager.APPLICATION_INTERFACE;

// Act
        mDeviceStateManager.registerInterface(nullObj, interfaceType);

// Assert
// Verify that none of the interface objects are set
        assertNull(mDeviceStateManager.getBluetoothInterface());
        assertNull(mDeviceStateManager.getApplicationInterface());
        assertNull(mDeviceStateManager.getRemoteUIInterface());
        assertNull(mDeviceStateManager.getDeviceControlService());
        assertNull(mDeviceStateManager.getCommonInterface());
        assertNull(mDeviceStateManager.getPluginInterface());
    }

    @Test
    public void testRegisterInterface_WithInvalidInterfaceType_ShouldNotSetInterface() {
// Arrange
        Object applicationInterfaceObj = mockApplicationInterface;
        int invalidInterfaceType = 999;

// Act
        mDeviceStateManager.registerInterface(applicationInterfaceObj, invalidInterfaceType);

// Assert
// Verify that none of the interface objects are set
        assertNull(mDeviceStateManager.getBluetoothInterface());
        assertNull(mDeviceStateManager.getApplicationInterface());
        assertNull(mDeviceStateManager.getRemoteUIInterface());
        assertNull(mDeviceStateManager.getDeviceControlService());
        assertNull(mDeviceStateManager.getCommonInterface());
        assertNull(mDeviceStateManager.getPluginInterface());
    }

    @Test
    public void testRegisterInterface_WithExistingInterface_ShouldReplaceInterface() {
// Arrange
        Object newBluetoothInterfaceObj = mock(BluetoothInterface.class);
        int interfaceType = DeviceControlManager.BLUETOOTH_INTERFACE;
        mDeviceStateManager.registerInterface(mockBluetoothInterface, interfaceType);

// Act
        mDeviceStateManager.registerInterface(newBluetoothInterfaceObj, interfaceType);

// Assert
        assertEquals(newBluetoothInterfaceObj, mDeviceStateManager.getBluetoothInterface());
// Verify that the other interface objects remain null
        assertNull(mDeviceStateManager.getApplicationInterface());
        assertNull(mDeviceStateManager.getRemoteUIInterface());
        assertNull(mDeviceStateManager.getDeviceControlService());
        assertNull(mDeviceStateManager.getCommonInterface());
        assertNull(mDeviceStateManager.getPluginInterface());
    }

    @Test
    public void testRegisterInterface_WithMultipleInterfaces_ShouldSetAllInterfaces() {

        Object bluetoothInterfaceObj = mock(BluetoothInterface.class);
        Object applicationInterfaceObj = mock(ApplicationInterface.class);
        Object remoteUIInterfaceObj = mock(RemoteUIInterface.class);
        int bluetoothInterfaceType = DeviceControlManager.BLUETOOTH_INTERFACE;
        int applicationInterfaceType = DeviceControlManager.APPLICATION_INTERFACE;
        int remoteUIInterfaceType = DeviceControlManager.REMOTE_UI_INTERFACE;

// Act
        mDeviceStateManager.registerInterface(bluetoothInterfaceObj, bluetoothInterfaceType);
        mDeviceStateManager.registerInterface(applicationInterfaceObj, applicationInterfaceType);
        mDeviceStateManager.registerInterface(remoteUIInterfaceObj, remoteUIInterfaceType);

// Assert
        assertEquals(bluetoothInterfaceObj, mDeviceStateManager.getBluetoothInterface());
        assertEquals(applicationInterfaceObj, mDeviceStateManager.getApplicationInterface());
        assertEquals(remoteUIInterfaceObj, mDeviceStateManager.getRemoteUIInterface());
// Verify that the other interface objects remain null
        assertNull(mDeviceStateManager.getDeviceControlService());
        assertNull(mDeviceStateManager.getCommonInterface());
        assertNull(mDeviceStateManager.getPluginInterface());
    }


    @Test
    public void testSetDeviceMode_WhenDeviceExistsAndInValidSettings_ReturnsExpectedStatus() {
        // Arrange
        long devHandle = 5476376655091210960L;
        int devTransport = 2;
        int devCapability = 131072;
        int expectedStatus = DeviceStateManager.INVALID_RESULT;

        // Create a mock HashMap for mHashDeviceList
        Map<Long, DeviceStateManager.CompositeDeviceInfo> mockDeviceList = new HashMap<>();


        DeviceInfo deviceInfo=new DeviceInfo();
        deviceInfo.mHandle =5476376655091210960L;
        deviceInfo.mType =3;
        deviceInfo.mTransport = 2;
        deviceInfo.mActiveCapability = 2;
        deviceInfo.mCapabilities = 131072 ;
        deviceInfo.mName = "RUI";
        deviceInfo.mVendorId = 0;
        deviceInfo.mProductId = 0;
        deviceInfo.mPortNum = 0;
        deviceInfo.mDeviceAlias = 0;


        RemotableDevInfo remotableDevInfo =new RemotableDevInfo();
        remotableDevInfo.mDevHandle =5476376655091210960L;
        remotableDevInfo.mTransports = 2;
        remotableDevInfo.mActiveTransport = 2;
        remotableDevInfo.mDevType = 2;
        remotableDevInfo.mActiveCapability = 6;
        remotableDevInfo.mCapabilities = 131072 ;
        remotableDevInfo.mDevName = "RUI";
        remotableDevInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
        remotableDevInfo.mIsActiveDevice = false;
        remotableDevInfo.mVendorId = 0;
        remotableDevInfo.mProductId = 0;
        remotableDevInfo.mPortNum = 0;
        remotableDevInfo.mDeviceAlias = 0;
        remotableDevInfo.mDevNode = null;
        remotableDevInfo.mDevState = 0;


        DeviceStateManager.CompositeDeviceInfo compositeDeviceInfo = mDeviceStateManager.new CompositeDeviceInfo(deviceInfo);

        mockDeviceList.put(devHandle, compositeDeviceInfo);

        // Mock the necessary methods and objects

        mDeviceStateManager.setHashDeviceList(mockDeviceList);
        Configuration mockConfiguration = mock(Configuration.class);
        mockConfiguration.mSoftReset= false;
        mockConfiguration.mResetDeviceSerialID = "A8:AB:B5:DE:10:03";
        try{
            java.lang.reflect.Field field = DeviceStateManager.class.getDeclaredField("mConfiguration");
            field.setAccessible(true);
            field.set(mDeviceStateManager,mockConfiguration);
        }catch(Exception e){
            Log.e("DeviceStateMananagerTest","Exception Occured while Setting the field :"+e.getMessage());
        }
        when(mockConfiguration.setDeviceMode(deviceInfo, devTransport, devCapability)).thenReturn(expectedStatus);
        // Act
        int result = mDeviceStateManager.setDeviceMode(remotableDevInfo, transport, capability);

        // Assert
        assertEquals(expectedStatus, result);


    }


  @Test
    public void testSetDeviceMode_WhenDeviceDoesNotExist_ReturnsFailureStatus() {
        // Arrange
        long devHandle = 5476376655091210960L;
        int devTransport = 2;
        int devCapability = 131072;
        int expectedStatus = DeviceStateManager.INVALID_RESULT;

        // Create an empty mock HashMap for mHashDeviceList
        Map<Long, DeviceStateManager.CompositeDeviceInfo> mockDeviceList = new HashMap<>();

        mDeviceStateManager.setHashDeviceList(mockDeviceList);

        RemotableDevInfo remotableDevInfo =new RemotableDevInfo();
        remotableDevInfo.mDevHandle =5476376655091210960L;
        remotableDevInfo.mTransports = 2;
        remotableDevInfo.mActiveTransport = 2;
        remotableDevInfo.mDevType = 2;
        remotableDevInfo.mActiveCapability = 6;
        remotableDevInfo.mCapabilities = 131072 ;
        remotableDevInfo.mDevName = "RUI";
        remotableDevInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
        remotableDevInfo.mIsActiveDevice = false;
        remotableDevInfo.mVendorId = 0;
        remotableDevInfo.mProductId = 0;
        remotableDevInfo.mPortNum = 0;
        remotableDevInfo.mDeviceAlias = 0;
        remotableDevInfo.mDevNode = null;
        remotableDevInfo.mDevState = 0;

        Configuration mockConfiguration = mock(Configuration.class);
        mockConfiguration.mSoftReset= false;
        mockConfiguration.mResetDeviceSerialID = "A8:AB:B5:DE:10:03";
        try{
            java.lang.reflect.Field field = DeviceStateManager.class.getDeclaredField("mConfiguration");
            field.setAccessible(true);
            field.set(mDeviceStateManager,mockConfiguration);
        }catch(Exception e){
            Log.e("DeviceStateMananagerTest","Exception Occured while Setting the field :"+e.getMessage());
        }

        // Act
        int result = mDeviceStateManager.setDeviceMode(remotableDevInfo, transport, capability);

        // Assert
        assertEquals(expectedStatus, result);

    }

    @Test
    public void testSetDeviceMode_WhenInvalidTransport_ReturnsFailureStatus() {
        // Arrange
        long devHandle = 5476376655091210960L;
        int devTransport = -1; // Invalid transport value
        int devCapability = 131072;
        int expectedStatus = DeviceStateManager.INVALID_RESULT;

        // Create a mock HashMap for mHashDeviceList
        Map<Long, DeviceStateManager.CompositeDeviceInfo> mockDeviceList = new HashMap<>();

        DeviceInfo deviceInfo=new DeviceInfo();
        deviceInfo.mHandle =5476376655091210960L;
        deviceInfo.mType =3;
        deviceInfo.mTransport = -1;
        deviceInfo.mActiveCapability = 2;
        deviceInfo.mCapabilities = 131072 ;
        deviceInfo.mName = "RUI";
        deviceInfo.mVendorId = 0;
        deviceInfo.mProductId = 0;
        deviceInfo.mPortNum = 0;
        deviceInfo.mDeviceAlias = 0;


        RemotableDevInfo remotableDevInfo =new RemotableDevInfo();
        remotableDevInfo.mDevHandle =5476376655091210960L;
        remotableDevInfo.mTransports = -1;
        remotableDevInfo.mActiveTransport = -1;
        remotableDevInfo.mDevType = 2;
        remotableDevInfo.mActiveCapability = 6;
        remotableDevInfo.mCapabilities = 131072 ;
        remotableDevInfo.mDevName = "RUI";
        remotableDevInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
        remotableDevInfo.mIsActiveDevice = false;
        remotableDevInfo.mVendorId = 0;
        remotableDevInfo.mProductId = 0;
        remotableDevInfo.mPortNum = 0;
        remotableDevInfo.mDeviceAlias = 0;
        remotableDevInfo.mDevNode = null;
        remotableDevInfo.mDevState = 0;


        DeviceStateManager.CompositeDeviceInfo compositeDeviceInfo = mDeviceStateManager.new CompositeDeviceInfo(deviceInfo);

        mockDeviceList.put(devHandle, compositeDeviceInfo);

        // Mock the necessary methods and objects

        mDeviceStateManager.setHashDeviceList(mockDeviceList);
        Configuration mockConfiguration = mock(Configuration.class);
        mockConfiguration.mSoftReset= false;
        mockConfiguration.mResetDeviceSerialID = "A8:AB:B5:DE:10:03";
        try{
            java.lang.reflect.Field field = DeviceStateManager.class.getDeclaredField("mConfiguration");
            field.setAccessible(true);
            field.set(mDeviceStateManager,mockConfiguration);
        }catch(Exception e){
            Log.e("DeviceStateMananagerTest","Exception Occured while Setting the field :"+e.getMessage());
        }
        when(mockConfiguration.setDeviceMode(deviceInfo, devTransport, devCapability)).thenReturn(expectedStatus);
        // Act
        int result = mDeviceStateManager.setDeviceMode(remotableDevInfo, transport, capability);

        // Assert
        assertEquals(expectedStatus, result);
    }

    @Test
    public void testSetDeviceMode_WhenDeviceListIsNull_ReturnsFailureStatus() {
        // Arrange
        long devHandle = 5476376655091210960L;
        int devTransport = 2;
        int devCapability = 2;
        int expectedStatus = DeviceStateManager.INVALID_RESULT;

        // Set mHashDeviceList to null here it was failing
        mDeviceStateManager.setHashDeviceList(null);

        RemotableDevInfo remotableDevInfo =new RemotableDevInfo();
        remotableDevInfo.mDevHandle =5476376655091210960L;
        remotableDevInfo.mTransports = 2;
        remotableDevInfo.mActiveTransport = 2;
        remotableDevInfo.mDevType = 2;
        remotableDevInfo.mActiveCapability = 6;
        remotableDevInfo.mCapabilities = devCapability;
        remotableDevInfo.mDevName = "RUI";
        remotableDevInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
        remotableDevInfo.mIsActiveDevice = false;
        remotableDevInfo.mVendorId = 0;
        remotableDevInfo.mProductId = 0;
        remotableDevInfo.mPortNum = 0;
        remotableDevInfo.mDeviceAlias = 0;
        remotableDevInfo.mDevNode = null;
        remotableDevInfo.mDevState = 0;

        Configuration mockConfiguration = mock(Configuration.class);
        mockConfiguration.mSoftReset= false;
        mockConfiguration.mResetDeviceSerialID = "A8:AB:B5:DE:10:03";
        try{
            java.lang.reflect.Field field = DeviceStateManager.class.getDeclaredField("mConfiguration");
            field.setAccessible(true);
            field.set(mDeviceStateManager,mockConfiguration);
        }catch(Exception e){
            Log.e("DeviceStateMananagerTest","Exception Occured while Setting the field :"+e.getMessage());
        }

        // Act
        int result = mDeviceStateManager.setDeviceMode(remotableDevInfo, transport, capability);

        // Assert
        assertEquals(expectedStatus, result);
    }

    @Test
    public void testSetDeviceMode_WhenInvalidCapability_ReturnsFailureStatus() {
        // Arrange
        long devHandle = 5476376655091210960L;
        int devTransport = 2;
        int devCapability = -1; // Invalid capability value
        int expectedStatus = DeviceStateManager.INVALID_RESULT;

        Map<Long, DeviceStateManager.CompositeDeviceInfo> mockDeviceList = new HashMap<>();

        RemotableDevInfo remotableDevInfo =new RemotableDevInfo();
        remotableDevInfo.mDevHandle =5476376655091210960L;
        remotableDevInfo.mTransports = 2;
        remotableDevInfo.mActiveTransport = 2;
        remotableDevInfo.mDevType = 2;
        remotableDevInfo.mActiveCapability = -1;
        remotableDevInfo.mCapabilities = devCapability;
        remotableDevInfo.mDevName = "RUI";
        remotableDevInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
        remotableDevInfo.mIsActiveDevice = false;
        remotableDevInfo.mVendorId = 0;
        remotableDevInfo.mProductId = 0;
        remotableDevInfo.mPortNum = 0;
        remotableDevInfo.mDeviceAlias = 0;
        remotableDevInfo.mDevNode = null;
        remotableDevInfo.mDevState = 0;

        mDeviceStateManager.setHashDeviceList(mockDeviceList);
        Configuration mockConfiguration = mock(Configuration.class);
        mockConfiguration.mSoftReset= false;
        mockConfiguration.mResetDeviceSerialID = "A8:AB:B5:DE:10:03";
        try{
            java.lang.reflect.Field field = DeviceStateManager.class.getDeclaredField("mConfiguration");
            field.setAccessible(true);
            field.set(mDeviceStateManager,mockConfiguration);
        }catch(Exception e){
            Log.e("DeviceStateMananagerTest","Exception Occured while Setting the field :"+e.getMessage());
        }

        // Act
        int result = mDeviceStateManager.setDeviceMode(remotableDevInfo, transport, capability);

        // Assert
        assertEquals(expectedStatus, result);

    }

  /*  @Test
    public void testSetDeviceMode_WhenDeviceModeIsAlreadySet_ReturnsSuccessStatus() {
        // Arrange
        long devHandle = 1234L;
        int devTransport = 1;
        int devCapability = 2;
        int expectedStatus = DeviceStateManager.DEVMGR_SUCCESS;

        // Create a mock HashMap for mHashDeviceList
        Map<Long, DeviceStateManager.CompositeDeviceInfo> mockDeviceList = new HashMap<>();

        // Create a mock DeviceInfoProvider
        DeviceInfoProvider mockDeviceInfoProvider = mock(DeviceInfoProvider.class);
        when(mockDeviceInfoProvider.getTransport()).thenReturn(devTransport);
        when(mockDeviceInfoProvider.getCapabilities()).thenReturn(devCapability);

        DeviceStateManager.CompositeDeviceInfo compositeDeviceInfo = mock(DeviceStateManager.CompositeDeviceInfo.class);
        when(compositeDeviceInfo.getDeviceInfo()).thenReturn(mockDeviceInfoProvider);
        mockDeviceList.put(devHandle, compositeDeviceInfo);

        // Mock the necessary methods and objects
        when(remotableDevInfo.getDevHandle()).thenReturn(devHandle);
        when(transport.getTransport()).thenReturn(devTransport);
        when(capability.getCapability()).thenReturn(devCapability);
        mDeviceStateManager.setHashDeviceList(mockDeviceList);

        // Act
        int result = mDeviceStateManager.setDeviceMode(remotableDevInfo, transport, capability);

        // Assert
        assertEquals(expectedStatus, result);
        verify(remotableDevInfo).getDevHandle();
        verify(transport).getTransport();
        verify(capability).getCapability();
    }*/

  /*  @Test
    public void testSetDeviceMode_WhenDeviceExistsAndModeIsSet_ReturnsSuccessStatus() {
        // Arrange
        long devHandle = 1234L;
        int devTransport = 1;
        int devCapability = 2;
        int expectedStatus = DeviceStateManager.DEVMGR_SUCCESS;

        // Create a mock HashMap for mHashDeviceList
        Map<Long, DeviceStateManager.CompositeDeviceInfo> mockDeviceList = new HashMap<>();

        // Create a mock DeviceInfoProvider
        DeviceInfoProvider mockDeviceInfoProvider = mock(DeviceInfoProvider.class);
        when(mockDeviceInfoProvider.getTransport()).thenReturn(devTransport);
        when(mockDeviceInfoProvider.getCapabilities()).thenReturn(devCapability);

        DeviceStateManager.CompositeDeviceInfo compositeDeviceInfo = mock(DeviceStateManager.CompositeDeviceInfo.class);
        when(compositeDeviceInfo.getDeviceInfo()).thenReturn(mockDeviceInfoProvider);
        mockDeviceList.put(devHandle, compositeDeviceInfo);

        // Mock the necessary methods and objects
        when(remotableDevInfo.getDevHandle()).thenReturn(devHandle);
        when(transport.getTransport()).thenReturn(devTransport);
        when(capability.getCapability()).thenReturn(devCapability);
        mDeviceStateManager.setHashDeviceList(mockDeviceList);

        // Act
        int result = mDeviceStateManager.setDeviceMode(remotableDevInfo, transport, capability);

        // Assert
        assertEquals(expectedStatus, result);
        verify(remotableDevInfo).getDevHandle();
        verify(transport).getTransport();
        verify(capability).getCapability();
    }*/

    @Test
    public void testSetDeviceMode_WhenDeviceListIsEmpty_ReturnsFailureStatus() {
        // Arrange
          
        long devHandle = 5476376655091210960L;
        int devTransport = 2;
        int devCapability = 131072;
        int expectedStatus = DeviceStateManager.INVALID_RESULT;

        Map<Long, DeviceStateManager.CompositeDeviceInfo> mockDeviceList = new HashMap<>();
        // Set mHashDeviceList to an empty list
        mDeviceStateManager.setHashDeviceList(mockDeviceList);
        
     
        RemotableDevInfo remotableDevInfo =new RemotableDevInfo();
        remotableDevInfo.mDevHandle =5476376655091210960L;
        remotableDevInfo.mTransports = 2;
        remotableDevInfo.mActiveTransport = 2;
        remotableDevInfo.mDevType = 2;
        remotableDevInfo.mActiveCapability = 6;
        remotableDevInfo.mCapabilities = 131072 ;
        remotableDevInfo.mDevName = "RUI";
        remotableDevInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
        remotableDevInfo.mIsActiveDevice = false;
        remotableDevInfo.mVendorId = 0;
        remotableDevInfo.mProductId = 0;
        remotableDevInfo.mPortNum = 0;
        remotableDevInfo.mDeviceAlias = 0;
        remotableDevInfo.mDevNode = null;
        remotableDevInfo.mDevState = 0;

        Configuration mockConfiguration = mock(Configuration.class);
        mockConfiguration.mSoftReset= false;
        mockConfiguration.mResetDeviceSerialID = "A8:AB:B5:DE:10:03";
        try{
            java.lang.reflect.Field field = DeviceStateManager.class.getDeclaredField("mConfiguration");
            field.setAccessible(true);
            field.set(mDeviceStateManager,mockConfiguration);
        }catch(Exception e){
            Log.e("DeviceStateMananagerTest","Exception Occured while Setting the field :"+e.getMessage());
        }
        // Act
        int result = mDeviceStateManager.setDeviceMode(remotableDevInfo, transport, capability);

        // Assert
        assertEquals(expectedStatus, result);
    
    }

}