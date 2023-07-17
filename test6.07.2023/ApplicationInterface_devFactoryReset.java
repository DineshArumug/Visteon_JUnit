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
public class ApplicationInterface_devFactoryReset {
	private ApplicationInterface mapplicationInterface;
	@Mock
	private DeviceStateManager mMockDeviceStateManager;
	@Mock
	private IDevCtrlManagerHMICb mMockIDevCtrlManagerHMICb;

	@Before
	public void setup()
	{
		MockitoAnnotations.initMocks(this);
		mapplicationInterface = new ApplicationInterface(mMockIDevCtrlManagerHMICb,mMockDeviceStateManager);
	}

	@Test
	public void devFactoryReset_DeviceStateManagerWithNull_ReturnInvalidResult()
	{
		DeviceStateManager deviceStateManager = null;
		ApplicationInterface applicationInterface = new ApplicationInterface(mMockIDevCtrlManagerHMICb,deviceStateManager);
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = applicationInterface.getDevCtrlManagerHMIBinder();
		int result = devCtrlManagerHMIBinder.devFactoryReset();
		assertEquals(result,applicationInterface.INVALID_RESULT);

		deviceStateManager = devCtrlManagerHMIBinder.getDeviceStateManager();
		assertNull(deviceStateManager);
	}

	@Test
	public void devFactoryReset_DeviceStateManagerWithNonNull_ReturnSuccessResult()
	{
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mapplicationInterface.getDevCtrlManagerHMIBinder();
		when(mMockDeviceStateManager.devFactoryReset()).thenReturn(0);
		
		int result = devCtrlManagerHMIBinder.devFactoryReset();
		assertEquals(result,mapplicationInterface.SUCCESS);

		verify(mMockDeviceStateManager).devFactoryReset();
	}
}
