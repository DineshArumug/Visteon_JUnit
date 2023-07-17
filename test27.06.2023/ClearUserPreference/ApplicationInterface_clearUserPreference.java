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
public class ApplicationInterface_clearUserPreference {

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
	public void ClearUserPreference_DeviceStateManagerWithNull_ReturnInvalidStatus()
	{
		DeviceStateManager deviceStateManager = null;
		ApplicationInterface applicationInterface = new ApplicationInterface(mMockIDevCtrlManagerHMICb,deviceStateManager);
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = applicationInterface.getDevCtrlManagerHMIBinder();
		
		int result = devCtrlManagerHMIBinder.clearUserPreference(mMockRemotableDevInfo);
		assertEquals(result,applicationInterface.INVALID_RESULT);

	}

	@Test
	public void ClearUserPreference_DeviceStateManagerWithNonNull_ReturnSuccessStatus()
	{
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mApplicationInterface.getDevCtrlManagerHMIBinder();
		when(mMockDeviceStateManager.clearUserPreference(mMockRemotableDevInfo)).thenReturn(0);

		int result = devCtrlManagerHMIBinder.clearUserPreference(mMockRemotableDevInfo);
		assertEquals(result,mApplicationInterface.SUCCESS);

		verify(mMockDeviceStateManager).clearUserPreference(mMockRemotableDevInfo);

	}

	@Test
	public void ClearUserPreference_RemotableWithNull_ReturnInvalidStatus()
	{
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mApplicationInterface.getDevCtrlManagerHMIBinder();
		RemotableDevInfo remotableDevInfo = null;

		int result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);
		assertEquals(result,mApplicationInterface.INVALID_RESULT);

	}

	@Test
	public void ClearUserPreference_RemotableDevInfoWithMinValue_ReturnSuccessStatus()
	{
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mApplicationInterface.getDevCtrlManagerHMIBinder();
		RemotableDevInfo remotableDevInfo = new RemotableDevInfo();
		remotableDevInfo.mDevHandle = Long.MIN_VALUE;
		remotableDevInfo.mTransports = Integer.MIN_VALUE;
		remotableDevInfo.mActiveTransport = Integer.MIN_VALUE;
		remotableDevInfo.mDevType = Integer.MIN_VALUE;
		remotableDevInfo.mActiveCapability = Integer.MIN_VALUE;
		remotableDevInfo.mCapabilities = Integer.MIN_VALUE;
		remotableDevInfo.mDevName = "AppInterface";
		remotableDevInfo.mBtMacAddress = "10.121.198.100";
		remotableDevInfo.mSerialId = "10258963147";
		remotableDevInfo.mIsActiveDevice = true;
		remotableDevInfo.mVendorId = Integer.MIN_VALUE;
		remotableDevInfo.mProductId = Integer.MIN_VALUE;
		remotableDevInfo.mPortNum = Integer.MIN_VALUE;
		remotableDevInfo.mDeviceAlias = Integer.MIN_VALUE;
		remotableDevInfo.mDevNode = "asd";
		remotableDevInfo.mDevState = Integer.MIN_VALUE;
		remotableDevInfo.mManufacturerName = "JLR";
		remotableDevInfo.mModelName = "Major[0,1]";
		remotableDevInfo.mDeviceVersion = "infoUpdate";
		remotableDevInfo.mStatus = Integer.MIN_VALUE;
		remotableDevInfo.mWCPFeatureStatus = Integer.MIN_VALUE;
		remotableDevInfo.mWAAPStatus = Integer.MIN_VALUE;
		remotableDevInfo.mRoleSwitchStatus = Integer.MIN_VALUE;
		remotableDevInfo.mDevFSType = Integer.MIN_VALUE;
		remotableDevInfo.mMountPath = "USB";
		remotableDevInfo.mSupportedCapabilities = Integer.MIN_VALUE;
		remotableDevInfo.mHMIDeviceAlias = "same device";
		remotableDevInfo.mWifiConnectStatus = Integer.MIN_VALUE;
		remotableDevInfo.mErrorMsgHint = "Error_Wifi_Connect_status";

		int result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);
		assertEquals(result,mApplicationInterface.SUCCESS);


	}

	@Test
	public void ClearUserPreference_RemotableDevInfoWithMaxValue_ReturnSuccessStatus()
	{
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mApplicationInterface.getDevCtrlManagerHMIBinder();
		RemotableDevInfo remotableDevInfo = new RemotableDevInfo();
		remotableDevInfo.mDevHandle = Long.MAX_VALUE;
		remotableDevInfo.mTransports = Integer.MAX_VALUE;
		remotableDevInfo.mActiveTransport = Integer.MAX_VALUE;
		remotableDevInfo.mDevType = Integer.MAX_VALUE;
		remotableDevInfo.mActiveCapability = Integer.MAX_VALUE;
		remotableDevInfo.mCapabilities = Integer.MAX_VALUE;
		remotableDevInfo.mDevName = "AppInterface";
		remotableDevInfo.mBtMacAddress = "10.121.198.100";
		remotableDevInfo.mSerialId = "10258963147";
		remotableDevInfo.mIsActiveDevice = true;
		remotableDevInfo.mVendorId = Integer.MAX_VALUE;
		remotableDevInfo.mProductId = Integer.MAX_VALUE;
		remotableDevInfo.mPortNum = Integer.MAX_VALUE;
		remotableDevInfo.mDeviceAlias = Integer.MAX_VALUE;
		remotableDevInfo.mDevNode = "asd";
		remotableDevInfo.mDevState = Integer.MAX_VALUE;
		remotableDevInfo.mManufacturerName = "JLR";
		remotableDevInfo.mModelName = "Major[0,1]";
		remotableDevInfo.mDeviceVersion = "infoUpdate";
		remotableDevInfo.mStatus = Integer.MAX_VALUE;
		remotableDevInfo.mWCPFeatureStatus = Integer.MAX_VALUE;
		remotableDevInfo.mWAAPStatus = Integer.MAX_VALUE;
		remotableDevInfo.mRoleSwitchStatus = Integer.MAX_VALUE;
		remotableDevInfo.mDevFSType = Integer.MAX_VALUE;
		remotableDevInfo.mMountPath = "USB";
		remotableDevInfo.mSupportedCapabilities = Integer.MAX_VALUE;
		remotableDevInfo.mHMIDeviceAlias = "same device";
		remotableDevInfo.mWifiConnectStatus = Integer.MAX_VALUE;
		remotableDevInfo.mErrorMsgHint = "Error_Wifi_Connect_status";

		int result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);
		assertEquals(result,mApplicationInterface.SUCCESS);


	}

	@Test
	public void ClearUserPreference_RemotableDevInfoWithZero_ReturnSuccessStatus()
	{
		ApplicationInterface.DevCtrlManagerHMIBinder devCtrlManagerHMIBinder = mApplicationInterface.getDevCtrlManagerHMIBinder();
		RemotableDevInfo remotableDevInfo = new RemotableDevInfo();
		remotableDevInfo.mDevHandle = 0;
		remotableDevInfo.mTransports = 0;
		remotableDevInfo.mActiveTransport = 0;
		remotableDevInfo.mDevType = 0;
		remotableDevInfo.mActiveCapability = 0;
		remotableDevInfo.mCapabilities = 0;
		remotableDevInfo.mDevName = null;
		remotableDevInfo.mBtMacAddress = null;
		remotableDevInfo.mSerialId = null;
		remotableDevInfo.mIsActiveDevice = false;
		remotableDevInfo.mVendorId = 0;
		remotableDevInfo.mProductId = 0;
		remotableDevInfo.mPortNum = 0;
		remotableDevInfo.mDeviceAlias = 0;
		remotableDevInfo.mDevNode = null;
		remotableDevInfo.mDevState = 0;
		remotableDevInfo.mManufacturerName = null;
		remotableDevInfo.mModelName = null;
		remotableDevInfo.mDeviceVersion = null;
		remotableDevInfo.mStatus = 0;
		remotableDevInfo.mWCPFeatureStatus = 0;
		remotableDevInfo.mWAAPStatus = 0;
		remotableDevInfo.mRoleSwitchStatus = 0;
		remotableDevInfo.mDevFSType = 0;
		remotableDevInfo.mMountPath = null;
		remotableDevInfo.mSupportedCapabilities = 0;
		remotableDevInfo.mHMIDeviceAlias = null;
		remotableDevInfo.mWifiConnectStatus = 0;
		remotableDevInfo.mErrorMsgHint = null;

		int result = devCtrlManagerHMIBinder.clearUserPreference(remotableDevInfo);
		assertEquals(result,mApplicationInterface.SUCCESS);

	}

}
