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
public class ApplicationInterface_unmountDevice {
	private ApplicationInterface mapplicationInterface;
	@Mock
	private IDevCtrlManagerHMICb mMockHMICb;
	@Mock
	private DeviceStateManager mMockdeviceStateManager;

	@Before
	public void setup()
	{
		MockitoAnnotations.initMocks(this);
		mapplicationInterface = new ApplicationInterface(mMockHMICb,mMockdeviceStateManager);
	}
	
	@Test
	public void unmountDevice_deviceStateManagerWithNull_StatusInvalidResult()
	{
		int INVALID_RESULT = -1;
		long value = 123L;
		DeviceStateManager deviceStateManager = null;
		ApplicationInterface applicationInterface = new ApplicationInterface(mMockHMICb,deviceStateManager);
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = applicationInterface.getDevCtrlManagerHMIBinder();

		int status = devCtrlManagerHMIBinder.unmountDevice(value);
		assertEquals(status,INVALID_RESULT);
		
		DeviceStateManager devicestatemanager = devCtrlManagerHMIBinder.getDeviceStateManager();
		assertNull(devicestatemanager);
	}
	
	@Test
	public void unmountDevice_deviceHandleWithZero_StatusSuccessResult()
	{
		int SUCCESS = 0;
		long value = 0;
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mapplicationInterface.getDevCtrlManagerHMIBinder();

		int status = devCtrlManagerHMIBinder.unmountDevice(value);
		assertEquals(status,SUCCESS);		
		verify(mMockdeviceStateManager,times(1)).unmountDevice(value);

	}
	
	@Test
	public void unmountDevice_deviceHandleWithMaxValue_StatusSuccessResult()
        {
                int SUCCESS = 0;
                long value = Long.MAX_VALUE;
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mapplicationInterface.getDevCtrlManagerHMIBinder();
  

                int status = devCtrlManagerHMIBinder.unmountDevice(value);
                assertEquals(status,SUCCESS);
		verify(mMockdeviceStateManager,times(1)).unmountDevice(value);

        }

	@Test
	public void unmountDevice_deviceHandleWithMinValue_StatusSuccessResult()
        {
                int SUCCESS = 0;
                long value = Long.MIN_VALUE;
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mapplicationInterface.getDevCtrlManagerHMIBinder();

                int status = devCtrlManagerHMIBinder.unmountDevice(value);
                assertEquals(status,SUCCESS);
		verify(mMockdeviceStateManager,times(1)).unmountDevice(value);

        }

	@Test
	public void unmountDevice_deviceHandleWithNegativeValue_statusSucessResult()
	{
		int SUCCESS = 0;
		long value = -10;
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mapplicationInterface.getDevCtrlManagerHMIBinder();

		int status = devCtrlManagerHMIBinder.unmountDevice(value);
		assertEquals(status,SUCCESS);	
		verify(mMockdeviceStateManager,times(1)).unmountDevice(value);
		
	}
}
