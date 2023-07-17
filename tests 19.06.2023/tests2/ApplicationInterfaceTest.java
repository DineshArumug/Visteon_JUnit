package com.allgo.devicecontrolmanager;

import com.allgo.DevCtrlMgrIface.*;
import com.allgo.devicecontrolmanager.ApplicationInterface;
import android.os.RemoteException;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.test.suitebuilder.annotation.MediumTest;
import java.lang.reflect.Field;
import android.util.Log;

import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.*;

@MediumTest
@RunWith(AndroidJUnit4.class)
public class ApplicationInterfaceTest {

    private ApplicationInterface mApplicationInterface;
    
    @Mock
    private RemotableEventType mMockremotableEventType;
    @Mock
    private RemotableDevInfo mMockremotableDevInfo;
    @Mock
    private IDevCtrlManagerHMICb mMockHMICb;
    @Mock
    private DeviceStateManager mMockdeviceStateManager;


    @Before
    public void setup()
    {
        MockitoAnnotations.initMocks(this);
        mApplicationInterface = new ApplicationInterface(mMockHMICb, mMockdeviceStateManager);
    }

    @Test
    public void onDeviceConnected_RemotableDevInfoNull_ReturnSuccessStatus() throws RemoteException
    {
	//Arrange
        int SUCCESS = 0;
        RemotableDevInfo remotableDevInfo = null;
	mMockHMICb = mApplicationInterface.getCallbackObject();
	
	//Act
        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, mMockremotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, mMockremotableEventType);

	//Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(remotableDevInfo,mMockremotableEventType);
	assertNotNull(mMockHMICb);

    }

    @Test
    public void onDeviceConnected_RemotableEventTypeNull_ReturnSuccessStatus() throws RemoteException
    {
        //Arrange
        int SUCCESS = 0;
        RemotableEventType remotableEventType = null;
	mMockHMICb = mApplicationInterface.getCallbackObject();

        //Act
        doNothing().when(mMockHMICb).onDeviceConnected(mMockremotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(mMockremotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(mMockremotableDevInfo,remotableEventType);
	assertNotNull(mMockHMICb);

    }

    @Test
    public void onDeviceConnected_BothObjectNonNull_ReturnSuccessStatus() throws RemoteException
    {
        //Arrange
        int SUCCESS = 0;
        RemotableEventType remotableEventType = mMockremotableEventType;
        RemotableDevInfo remotableDevInfo = mMockremotableDevInfo;
	mMockHMICb = mApplicationInterface.getCallbackObject();
        //Act
        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(remotableDevInfo,remotableEventType);
	assertNotNull(mMockHMICb);
    }

    @Test
    public void onDeviceConnected_BothObjectWithNull_ReturnSuccessStatus() throws RemoteException
    {
        //Arrange
        int SUCCESS = 0;
        RemotableDevInfo remotableDevInfo = null;
        RemotableEventType remotableEventType = null;
	mMockHMICb = mApplicationInterface.getCallbackObject();

        //Act
        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(remotableDevInfo,remotableEventType);
	assertNotNull(mMockHMICb);
    }

    @Test
    public void onDeviceConnected_mCallbackWithNull_ReturnInvalidStatus()
    {
        //Arrange
        int INVALID_STATUS = -1;

        RemotableEventType remotableEventType = mMockremotableEventType;
        RemotableDevInfo remotableDevInfo = mMockremotableDevInfo;
  	mApplicationInterface = new ApplicationInterface(null,mMockdeviceStateManager);
	mMockHMICb = mApplicationInterface.getCallbackObject();

        //Act
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(INVALID_STATUS,result);
	assertNull(mMockHMICb);

    }

    @Test
    public void onDeviceConnected_mCallbackWithoutNull_ReturnSucessStatus() throws RemoteException
    {
        //Arrange
        int SUCCESS = 0;
        RemotableDevInfo remotableDevInfo = mMockremotableDevInfo;
        RemotableEventType remotableEventType = mMockremotableEventType;
	mMockHMICb = mApplicationInterface.getCallbackObject();

        //Act
        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(remotableDevInfo,remotableEventType);
	assertNotNull(mMockHMICb);
    }

    @Test
    public void onDeviceConnected_RemotableDevInfoWithMaxValues() throws RemoteException
    {
	    int SUCCESS = 0;
	    RemotableEventType remotableEventType = mMockremotableEventType;
	    RemotableDevInfo devInfo = new RemotableDevInfo();
	    devInfo.mDevHandle = 234234389898L;
	    devInfo.mTransports = Integer.MAX_VALUE;
	    devInfo.mActiveTransport = Integer.MAX_VALUE;
	    devInfo.mDevType = Integer.MAX_VALUE;
	    devInfo.mActiveCapability = Integer.MAX_VALUE;
	    devInfo.mCapabilities = Integer.MAX_VALUE;
	    devInfo.mDevName = null;
	    devInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
	    devInfo.mIsActiveDevice = false;
	    devInfo.mVendorId = 0;
	    devInfo.mProductId = 0;
	    devInfo.mPortNum = 0;
	    devInfo.mDeviceAlias = 0;
	    devInfo.mDevNode = null;
	    devInfo.mDevState = 0;

	    doNothing().when(mMockHMICb).onDeviceConnected(devInfo, remotableEventType);
	    int result = mApplicationInterface.onDeviceConnected(devInfo, remotableEventType);

	    assertEquals(SUCCESS,result);
	    verify(mMockHMICb,times(1)).onDeviceConnected(devInfo,remotableEventType);

    }
}
