package com.allgo.devicecontrolmanager;

import com.allgo.DevCtrlMgrIface.*;
import com.allgo.devicecontrolmanager.ApplicationInterface;
import android.os.RemoteException;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.test.suitebuilder.annotation.MediumTest;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import static org.mockito.Mockito.*;
import static org.junit.Assert.*;

@MediumTest
@RunWith(AndroidJUnit4.class)
public class ApplicationInterface_ClearUserPreference {
	private ApplicationInterface mapplicationInterface; 
	@Mock
	private DeviceStateManager mMockdeviceStateManager;
	@Mock 
	private RemotableDevInfo mMockRemotableDevInfo;
	@Mock 
	private IDevCtrlManagerHMICb mMockHMICb;

	@Before
	public void setup()
	{
		MockitoAnnotations.initMocks(this);
		mapplicationInterface = new ApplicationInterface(mMockHMICb,mMockdeviceStateManager);

	}

	@Test
	void ClearUserPreference_DeviceStateManagerWithNull_ResultInvalidResult()
	{
		int INVALIDRESULT = -1;
		DeviceStateManager deviceStateManager = null;
		RemotableDevInfo remotableDevInfo = mMockRemotableDevInfo;
		ApplicationInterface applicationInterface = new ApplicationInterface(mMockHMICb,deviceStateManager);
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = applicationInterface.getDevCtrlManagerHMIBinder();
		int result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);

		assertEquals(result,INVALIDRESULT);
		deviceStateManager = devCtrlManagerHMIBinder.getDeviceStateManager();
		assertNull(deviceStateManager);
	}

	@Test
	void ClearUserPreference_RemotableDevInfoWithNull_ResultInvalidResult()
	{
		int INVALIDRESULT = -1;
		RemotableDevInfo remotableDevInfo = null;
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mapplicationInterface.getDevCtrlManagerHMIBinder();
		
		int result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);

		assertEquals(result,INVALIDRESULT);
	}

	@Test
	void ClearUserPreference_mDevHandleAndmBtMacAddressWithNull_ResultInvalidResult()
	{
		int INVALIDRESULT = -1;
		RemotableDevInfo remotableDevInfo = new RemotableDevInfo();
		remotableDevInfo.mDevHandle = 0;
		remotableDevInfo.mBtMacAddress = null;
//		remotableDevInfo.mDevHandle = handle;
		remotableDevInfo.mTransports = 0;
		remotableDevInfo.mActiveTransport =  0;
		remotableDevInfo.mDevType = 0;
		remotableDevInfo.mActiveCapability = 0;
		remotableDevInfo.mCapabilities = 0;
		remotableDevInfo.mDevName = "ApplicationInterface";
//		remotableDevInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
		remotableDevInfo.mSerialId = "12345";
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mapplicationInterface.getDevCtrlManagerHMIBinder();

		int result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);
		assertEquals(result,INVALIDRESULT);
	}

	@Test
	void ClearUserPreference_mDevHandleWithMaxAndMinValue_ResultSuccessInvalid()
	{
		int SUCCESS = 0;
		RemotableDevInfo remotableDevInfo = new RemotableDevInfo();
		remotableDevInfo.mDevHandle = Long.MAX_VALUE;
		remotableDevInfo.mBtMacAddress = null;
		remotableDevInfo.mTransports = 0;
		remotableDevInfo.mActiveTransport =  0;
		remotableDevInfo.mDevType = 0;
		remotableDevInfo.mActiveCapability = 0;
		remotableDevInfo.mCapabilities = 0;
		remotableDevInfo.mDevName = "ApplicationInterface";
		remotableDevInfo.mSerialId = "12345";
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mapplicationInterface.getDevCtrlManagerHMIBinder();

                int result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);
                assertEquals(result,SUCCESS);

		remotableDevInfo.mDevHandle = Long.MIN_VALUE;
                result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);
                assertEquals(result,SUCCESS);

	}

}
	
