package com.allgo.devicecontrolmanager;

import com.allgo.DevCtrlMgrIface.*;
import com.allgo.devicecontrolmanager.*;
import com.allgo.devicecontolmanager.TestUtils;
import com.allgo.devicecontroller.DeviceController;
import android.os.RemoteException;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.test.suitebuilder.annotation.MediumTest;
import java.util.ArrayList;
import java.util.List;
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
public class ApplicationInterface_getDeviceList {

	private ApplicationInterface mApplicationInterface;
	@Mock
	private IDevCtrlManagerHMICb mMockIDevCtrlManagerHMICb;
	@Mock
	private DeviceStateManager mMockDeviceStateManager;
	@Mock
	private RemotableDevInfo mMockRemotableDevInfo;

	@Before
	public void setup()
	{
		MockitoAnnotations.initMocks(this);
		mApplicationInterface = new ApplicationInterface(mMockIDevCtrlManagerHMICb,mMockDeviceStateManager);
	}

	@Test
	public void testGetDeviceList_ValidCapabilityMask() throws RemoteException {

		int capMask = RemotableCapability.DEVCAPS_CARPLAY;
		List<RemotableDevInfo> expectedList = new ArrayList<>();
		RemotableDevInfo remotableDevInfo = TestUtils.getRemotableDevInfo(2L, RemotableTransport.TRANSPORT_BT, RemotableCapability.DEVCAPS_BT_MIRRORING);
		expectedList.add(remotableDevInfo);
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mApplicationInterface.getDevCtrlManagerHMIBinder();

		when(mMockDeviceStateManager.getDeviceList(capMask)).thenReturn(expectedList);


		List<RemotableDevInfo> actualList = devCtrlManagerHMIBinder.getDeviceList(capMask);


		assertNotNull(actualList);
		assertEquals(expectedList, actualList);
	}

	@Test
	public void testGetDeviceList_InvalidCapabilityMask() throws RemoteException {

		int capMask = 0; // invalid capability mask
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mApplicationInterface.getDevCtrlManagerHMIBinder();
		when(mMockDeviceStateManager.getDeviceList(capMask)).thenReturn(null);


		List<RemotableDevInfo> actualList = devCtrlManagerHMIBinder.getDeviceList(capMask);


		assertNull(actualList);
	}

	@Test
	public void testGetDeviceList_EmptyDeviceList() throws RemoteException {

		int capMask = RemotableCapability.DEVCAPS_BT_MIRRORING;
		List<RemotableDevInfo> expectedList = new ArrayList<>();
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mApplicationInterface.getDevCtrlManagerHMIBinder();
		when(mMockDeviceStateManager.getDeviceList(capMask)).thenReturn(expectedList);


		List<RemotableDevInfo> actualList = devCtrlManagerHMIBinder.getDeviceList(capMask);


		assertNotNull(actualList);
		assertTrue(actualList.isEmpty());
	}

	@Test
	public void testGetDeviceList_NullDeviceStateManager() throws RemoteException {


		ApplicationInterface applicationInterface = new ApplicationInterface(mMockIDevCtrlManagerHMICb,null);
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = applicationInterface.getDevCtrlManagerHMIBinder();
		int capMask = RemotableCapability.DEVCAPS_ALL;

		List<RemotableDevInfo> actualList = devCtrlManagerHMIBinder.getDeviceList(capMask);

		assertNull(actualList);
	}

	@Test
	public void testGetDeviceList_NullCallback() throws RemoteException {

		ApplicationInterface applicationInterface = new ApplicationInterface(null,mMockDeviceStateManager);
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = applicationInterface.getDevCtrlManagerHMIBinder();
		int capMask = RemotableCapability.DEVCAPS_BT_MIRRORING;
		List<RemotableDevInfo> expectedList = new ArrayList<>();
		RemotableDevInfo remotableDevInfo = TestUtils.getRemotableDevInfo(5476376655091210960L, DeviceController.TRANSPORT_BT, DeviceController.DEVCAPS_WIRELESS_AAP);
		expectedList.add(remotableDevInfo);
		when(mMockDeviceStateManager.getDeviceList(capMask)).thenReturn(expectedList);

		List<RemotableDevInfo> actualList = devCtrlManagerHMIBinder.getDeviceList(capMask);


		assertNotNull(actualList);
		assertEquals(expectedList, actualList);
	}	

}
