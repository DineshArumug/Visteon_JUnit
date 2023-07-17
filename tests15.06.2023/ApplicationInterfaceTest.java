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
	
	//Act
        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, mMockremotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, mMockremotableEventType);

	//Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(remotableDevInfo,mMockremotableEventType);

    }

    @Test
    public void onDeviceConnected_RemotableEventTypeNull_ReturnSuccessStatus() throws RemoteException
    {
        //Arrange
        int SUCCESS = 0;
        RemotableEventType remotableEventType = null;

        //Act
        doNothing().when(mMockHMICb).onDeviceConnected(mMockremotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(mMockremotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(mMockremotableDevInfo,remotableEventType);

    }

    @Test
    public void onDeviceConnected_BothObjectNonNull_ReturnSuccessStatus() throws RemoteException
    {
        //Arrange
        int SUCCESS = 0;
        RemotableEventType remotableEventType = mMockremotableEventType;
        RemotableDevInfo remotableDevInfo = mMockremotableDevInfo;

        //Act
        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(remotableDevInfo,remotableEventType);
    }

    @Test
    public void onDeviceConnected_BothObjectWithNull_ReturnSuccessStatus() throws RemoteException
    {
        //Arrange
        int SUCCESS = 0;
        RemotableDevInfo remotableDevInfo = null;
        RemotableEventType remotableEventType = null;

        //Act
        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(remotableDevInfo,remotableEventType);
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
/*	try{
	Field privateField = ApplicationInterface.class.getDeclaredField("mCallback");
	privateField.setAccessible(true);
	privateField.set(privateField,null);
	}
	catch(Exception e)
	{
		Log.e("ApplicationInterfaceTest","Exception : "+e.getMessage());
	}
*/

        //Act
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(INVALID_STATUS,result);
//	assertNull(mApplicationInterface.mCallback);
	assertNull(mMockHMICb);

    }

    @Test
    public void onDeviceConnected_mCallbackWithoutNull_ReturnSucessStatus() throws RemoteException
    {
        //Arrange
        int SUCCESS = 0;
        RemotableDevInfo remotableDevInfo = mMockremotableDevInfo;
        RemotableEventType remotableEventType = mMockremotableEventType;

        //Act
        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        verify(mMockHMICb,times(1)).onDeviceConnected(remotableDevInfo,remotableEventType);
    }
}
