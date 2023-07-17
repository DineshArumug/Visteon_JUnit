package com.allgo.devicecontrolmanager;

import android.os.IBinder;
import android.os.RemoteException;
import android.content.Context;
import com.allgo.DevCtrlMgrIface.IDevCtrlManagerCommonCb;
import com.allgo.DevCtrlMgrIface.IDevCtrlManagerHMICb;
import com.allgo.DevCtrlMgrIface.IDevCtrlManagerRUICb;
import com.allgo.DevCtrlMgrIface.IDeviceControlManager;
import com.allgo.DevCtrlMgrIface.RemotableApplicationType;



import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.test.suitebuilder.annotation.MediumTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.mockito.Mockito.*;
import org.junit.runner.RunWith;

@MediumTest
@RunWith(AndroidJUnit4.class)
public class DeviceControlManagerTest {
	
    @Mock 
	private Context mMockContext;
	
	private android.app.Application mApplication;
    @Mock
    private IDevCtrlManagerHMICb mMockHmiCallback;
    @Mock
    private IDevCtrlManagerRUICb mMockRuiCallback;
    @Mock
    private IDevCtrlManagerCommonCb mMockCommonCallback;
    @Mock
    private IBinder mMockHmiBinder;
    @Mock
    private IBinder mMockRuiBinder;
    @Mock
   private IBinder mMockCommonBinder;
   

    private DeviceControlManager mDeviceControlManager;

    @Before
    public void setUp() {
		
        MockitoAnnotations.initMocks(this);
		
		 InstrumentationRegistry.getInstrumentation().runOnMainSync(new Runnable() {
            @Override
            public void run() {
				mDeviceControlManager = new DeviceControlManager();
            }
        });
		
		    // Attach a context to the service for permission checks.
        mDeviceControlManager.attach(mMockContext, null, null, null, mApplication, null);

        mDeviceControlManager.onCreate();

    }
	
	@Test
	public void testRegisterUnRegisterHmiApplication() throws RemoteException {
		when(mMockHmiCallback.asBinder()).thenReturn(mMockHmiBinder);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		IBinder result = mProxy.register(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));

		assertEquals(mMockHmiBinder, result);
		verify(mMockHmiBinder).linkToDeath(any(), eq(0));

		// Unregister sequence
		int status = mProxy.unregister(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));

		assertEquals(mDeviceControlManager.SUCCESS, status);
		verify(mMockHmiBinder).unlinkToDeath(any(), eq(0));
		assertNull(mDeviceControlManager.getApplicationInterface());
	}

	@Test
	public void testRegisterUnRegisterRuiApplication() throws RemoteException {
		when(mMockRuiCallback.asBinder()).thenReturn(mMockRuiBinder);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		IBinder result = mProxy.register(mMockRuiBinder, new RemotableApplicationType(RemotableApplicationType.RUI_APPLICATION));

		assertEquals(mMockRuiBinder, result);
		verify(mMockRuiBinder).linkToDeath(any(), eq(0));

		// Unregister sequence
		int status = mProxy.unregister(mMockRuiBinder, new RemotableApplicationType(RemotableApplicationType.RUI_APPLICATION));

		assertEquals(mDeviceControlManager.SUCCESS, status);
		verify(mMockRuiBinder).unlinkToDeath(any(), eq(0));
		assertNull(mDeviceControlManager.getRemoteUIInterface());
	}

	@Test
	public void testRegisterUnRegisterCommonApplication() throws RemoteException {
		when(mMockCommonCallback.asBinder()).thenReturn(mMockCommonBinder);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		IBinder result = mProxy.register(mMockCommonBinder, new RemotableApplicationType(RemotableApplicationType.COMMON_APPLICATION));

		assertEquals(mMockCommonBinder, result);
		verify(mMockCommonBinder).linkToDeath(any(), eq(0));

		// Unregister sequence
		int status = mProxy.unregister(mMockCommonBinder, new RemotableApplicationType(RemotableApplicationType.COMMON_APPLICATION));

		assertEquals(mDeviceControlManager.SUCCESS, status);
		verify(mMockCommonBinder).unlinkToDeath(any(), eq(0));
	}
	
	@Test
	public void testRegisterHmiApplicationTwice() throws RemoteException {
		when(mMockHmiCallback.asBinder()).thenReturn(mMockHmiBinder);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		// First registration
		IBinder result1 = mProxy.register(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));
		assertEquals(mMockHmiBinder, result1);
		verify(mMockHmiBinder).linkToDeath(any(), eq(0));

		// Second registration
		IBinder result2 = mProxy.register(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));
		assertNull(result2); // Expect null as the registration should fail

		// Unregister sequence
		int status = mProxy.unregister(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));

		assertEquals(mDeviceControlManager.SUCCESS, status);
		verify(mMockHmiBinder).unlinkToDeath(any(), eq(0));
		assertNull(mDeviceControlManager.getApplicationInterface());
	}
	
		 @Test
	public void testUnregisterHmiApplicationTwice() throws RemoteException {
		when(mMockHmiCallback.asBinder()).thenReturn(mMockHmiBinder);
		when(mMockHmiBinder == mDeviceControlManager.getApplicationInterface().getCallbackObject()).thenReturn(true);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		// Register
		IBinder result = mProxy.register(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));
		assertEquals(mMockHmiBinder, result);
		verify(mMockHmiBinder).linkToDeath(any(), eq(0));

		// First unregister
		int status1 = mProxy.unregister(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));
		assertEquals(mDeviceControlManager.SUCCESS, status1);
		verify(mMockHmiBinder).unlinkToDeath(any(), eq(0));
		assertNull(mDeviceControlManager.getApplicationInterface());

		// Second unregister
		int status2 = mProxy.unregister(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));
		assertEquals(mDeviceControlManager.INVALID_RESULT, status2);
		verifyNoMoreInteractions(mMockHmiBinder);
	}

	@Test
	public void testUnregisterHmiApplicationFailure() throws RemoteException {
		when(mMockHmiCallback.asBinder()).thenReturn(mMockHmiBinder);
		when(mMockHmiBinder == mDeviceControlManager.getApplicationInterface().getCallbackObject()).thenReturn(false);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		int status = mProxy.unregister(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));

		assertEquals(mDeviceControlManager.INVALID_RESULT, status);
		verify(mMockHmiBinder, never()).unlinkToDeath(any(), eq(0));
	}

	@Test
	public void testUnregisterRuiApplicationFailure() throws RemoteException {
		when(mMockRuiCallback.asBinder()).thenReturn(mMockRuiBinder);
		when(mMockRuiBinder == mDeviceControlManager.getRemoteUIInterface().getCallbackObject()).thenReturn(false);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		int status = mProxy.unregister(mMockRuiBinder, new RemotableApplicationType(RemotableApplicationType.RUI_APPLICATION));

		assertEquals(mDeviceControlManager.INVALID_RESULT, status);
		verify(mMockRuiBinder, never()).unlinkToDeath(any(), eq(0));
	}

	@Test
	public void testUnregisterCommonApplicationFailure() throws RemoteException {
		when(mMockCommonCallback.asBinder()).thenReturn(mMockCommonBinder);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		int status = mProxy.unregister(mMockCommonBinder, new RemotableApplicationType(RemotableApplicationType.COMMON_APPLICATION));

		assertEquals(mDeviceControlManager.INVALID_RESULT, status);
		verify(mMockCommonBinder, never()).unlinkToDeath(any(), eq(0));
	}
	
		@Test
	public void testRegisterInvalidApplicationType() throws RemoteException {
		when(mMockHmiCallback.asBinder()).thenReturn(mMockHmiBinder);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		IBinder result = mProxy.register(mMockHmiBinder, null);

		assertNull(result);
		verifyNoMoreInteractions(mMockHmiBinder);
	}


	@Test
	public void testUnregisterUnknownApplication() throws RemoteException {
		IDevCtrlManagerHMICb unknownCallback = mock(IDevCtrlManagerHMICb.class);
		IBinder ibinder = mock(IBinder.class);
		when(unknownCallback.asBinder()).thenReturn(ibinder);
		IDeviceControlManager.Stub mProxy = mDeviceControlManager.getProxy();

		// Register
		IBinder result = mProxy.register(mMockHmiBinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));
		assertEquals(mMockHmiBinder, result);
		verify(mMockHmiBinder).linkToDeath(any(), eq(0));

		// Unregister unknown application
		int status = mProxy.unregister(ibinder, new RemotableApplicationType(RemotableApplicationType.HMI_APPLICATION));

		assertEquals(mDeviceControlManager.INVALID_RESULT, status);
		verifyNoMoreInteractions(ibinder);
	}

}
