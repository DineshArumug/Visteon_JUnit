package com.allgo.devicecontrolmanager;

import com.allgo.DevCtrlMgrIface.*;
import com.allgo.devicecontrolmanager.ApplicationInterface;
import android.os.RemoteException;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.test.suitebuilder.annotation.MediumTest;

import java.util.ArrayList;
import java.util.List;
import android.util.Log;
import java.lang.reflect.Field;

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
public class ApplicationInterface_unmountDevice {
	private ApplicationInterface mapplicationInterface;
	private ApplicationInterface.DevCtrlManagerHMIBinder mDevCtrlManagerHMIBinder; 
	@Mock
	private IDevCtrlManagerHMICb mMockHMICb;
	@Mock
	private DeviceStateManager mMockdeviceStateManager;
//	@Mock
//	private Logger mMockLogger;
	@Before
	public void setup()
	{
		MockitoAnnotations.initMocks(this);
		mapplicationInterface = new ApplicationInterface(mMockHMICb,mMockdeviceStateManager);
		mDevCtrlManagerHMIBinder = new ApplicationInterface.DevCtrlManagerHMIBinder(mMockdeviceStateManager);		
		//mDevCtrlHMIBinder = new ApplicationInterface.DevCtrlHMIBinder(mMockdeviceStateManager);
//		try{
//			Field mLog = ApplicationInterface.class.getDeclaredField("mLog");
//			mLog.setAccessible(true);
//			mLog.set(mapplicationInterface,mMockLog);
//		}catch(Exception e){
//			Log.e("DeviceStateMananagerTest","Exception Occured while Setting the field :"+e.getMessage());
//		}
	}
	@Test
	public void unmountDevice_deviceStateManagerWithNull_StatusInvalidResult()
	{
		int INVALID_RESULT = -1;
		long value = 123L;
		DeviceStateManager deviceStateManager = null;
		mDevCtrlManagerHMIBinder = new ApplicationInterface.DevCtrlManagerHMIBinder(deviceStateManager);

		int status = mDevCtrlManagerHMIBinder.unmountDevice(value);
		assertEquals(status,INVALID_RESULT);
//		assertNull(mDevCtrlManagerHMIBinder.mDeviceStateManager);
//		verify(mMockLogger).printError(eq("mDeviceStateManager is null"));		
	}
	@Test
	public void unmountDevice_deviceHandleWithZero_StatusSuccessResult()
	{
		int SUCCESS = 0;
		long value = 0;
		DeviceStateManager deviceStateManager = mMockdeviceStateManager;

		int status = mDevCtrlManagerHMIBinder.unmountDevice(value);
		assertEquals(status,SUCCESS);		

	}
	@Test
	public void unmountDevice_deviceHandleWithMaxValue_StatusSuccessResult()
        {
                int SUCCESS = 0;
                long value = Long.MAX_VALUE;
                DeviceStateManager deviceStateManager = mMockdeviceStateManager;

                int status = mDevCtrlManagerHMIBinder.unmountDevice(value);
                assertEquals(status,SUCCESS);

        }

	@Test
	public void unmountDevice_deviceHandleWithMinValue_StatusSuccessResult()
        {
                int SUCCESS = 0;
                long value = Long.MIN_VALUE;
                DeviceStateManager deviceStateManager = mMockdeviceStateManager;

                int status = mDevCtrlManagerHMIBinder.unmountDevice(value);
                assertEquals(status,SUCCESS);

        }

}
