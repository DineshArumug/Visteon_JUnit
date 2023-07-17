package com.allgo.devicecontrolmanager;

import android.os.RemoteException;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.List;
import android.util.Log;
import java.lang.reflect.Field;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import com.allgo.DevCtrlMgrIface.IDevCtrlManagerRUI;
import com.allgo.DevCtrlMgrIface.IDevCtrlManagerRUICb;

import com.allgo.DevCtrlMgrIface.RemotableDevInfo;
import com.allgo.DevCtrlMgrIface.RemotableSessionType;
import com.allgo.DevCtrlMgrIface.RemotableSessionStatus;
import com.allgo.DevCtrlMgrIface.RemotableTransport;
import com.allgo.DevCtrlMgrIface.RemotableReason;
import com.allgo.DevCtrlMgrIface.RemotablePairingStatus;
import com.allgo.DevCtrlMgrIface.RemotableSessionErrReason;


public class RemoteUIInterfaceTest {

    @Mock
    private DeviceStateManager mockDeviceStateManager;

    @Mock
    private IDevCtrlManagerRUICb mockCallback;

    @Mock
    private Logger mockLog;

    private RemoteUIInterface remoteUIInterface;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        remoteUIInterface = new RemoteUIInterface(mockCallback, mockDeviceStateManager);

        try{
            Field mLog = RemoteUIInterface.class.getDeclaredField("mLog");
            mLog.setAccessible(true);
            mLog.set(remoteUIInterface,mockLog);
        }catch(Exception e){
            Log.e("DeviceStateMananagerTest","Exception Occured while Setting the field :"+e.getMessage());
        }
    }


    @Test

    public void testSessionStateChange_NullSessionType_LogsError()  throws RemoteException{


        long deviceHandle = 12345L;

        RemotableSessionType sessionType = null;

        RemotableSessionStatus sessionStatus = new RemotableSessionStatus(RemotableSessionStatus.SESSION_STARTING);

        RemotableTransport transport = new RemotableTransport(RemotableTransport.TRANSPORT_BT);

        RemotableSessionErrReason errorReason = new RemotableSessionErrReason(RemotableSessionErrReason.ERR_UNKNOWN);


        remoteUIInterface.mIDevCtrlManagerRUI.sessionStateChange(deviceHandle, sessionType, sessionStatus, transport, errorReason);


        verify(mockLog).printError(eq("sessionType is null"));

        verify(mockDeviceStateManager, never()).sessionStateChange(anyLong(), any(RemotableSessionType.class), any(RemotableSessionStatus.class), any(RemotableTransport.class), any(RemotableSessionErrReason.class));

    }


    @Test

    public void testSessionStateChange_NullSessionStatus_LogsError()  throws RemoteException{


        long deviceHandle = 12345L;

        RemotableSessionType sessionType = new RemotableSessionType(RemotableSessionType.SESSION_TYPE_ANDROIDAUTO);

        RemotableSessionStatus sessionStatus = null;

        RemotableTransport transport = new RemotableTransport(RemotableTransport.TRANSPORT_BT);

        RemotableSessionErrReason errorReason = new RemotableSessionErrReason(RemotableSessionErrReason.ERR_UNKNOWN);


        remoteUIInterface.mIDevCtrlManagerRUI.sessionStateChange(deviceHandle, sessionType, sessionStatus, transport, errorReason);


        verify(mockLog).printError(eq("sessionStatus is null"));

        verify(mockDeviceStateManager, never()).sessionStateChange(anyLong(), any(RemotableSessionType.class), any(RemotableSessionStatus.class), any(RemotableTransport.class), any(RemotableSessionErrReason.class));

    }


    @Test

    public void testSessionStateChange_NullTransport_LogsError()  throws RemoteException{


        long deviceHandle = 12345L;

        RemotableSessionType sessionType = new RemotableSessionType(RemotableSessionType.SESSION_TYPE_ANDROIDAUTO);

        RemotableSessionStatus sessionStatus = new RemotableSessionStatus(RemotableSessionStatus.SESSION_STARTED);

        RemotableTransport transport = null;

        RemotableSessionErrReason errorReason = new RemotableSessionErrReason(RemotableSessionErrReason.ERR_UNKNOWN);


        remoteUIInterface.mIDevCtrlManagerRUI.sessionStateChange(deviceHandle, sessionType, sessionStatus, transport, errorReason);


        verify(mockLog).printError(eq("transport is null"));

        verify(mockDeviceStateManager, never()).sessionStateChange(anyLong(), any(RemotableSessionType.class), any(RemotableSessionStatus.class), any(RemotableTransport.class), any(RemotableSessionErrReason.class));

    }


    @Test

    public void testSessionStateChange_NullErrorReason_LogsError()  throws RemoteException{


        long deviceHandle = 12345L;

        RemotableSessionType sessionType = new RemotableSessionType(RemotableSessionType.SESSION_TYPE_CARPLAY);

        RemotableSessionStatus sessionStatus = new RemotableSessionStatus(RemotableSessionStatus.SESSION_STARTED);

        RemotableTransport transport = new RemotableTransport(RemotableTransport.TRANSPORT_BT);

        RemotableSessionErrReason errorReason = null;


        remoteUIInterface.mIDevCtrlManagerRUI.sessionStateChange(deviceHandle, sessionType, sessionStatus, transport, errorReason);


        verify(mockLog).printError(eq("errorReason is null"));

        verify(mockDeviceStateManager, never()).sessionStateChange(anyLong(), any(RemotableSessionType.class), any(RemotableSessionStatus.class), any(RemotableTransport.class), any(RemotableSessionErrReason.class));

    }


    @Test

    public void testSessionStateChange_ValidInput_NullDeviceStateManager_LogsError()  throws RemoteException{


        remoteUIInterface = new RemoteUIInterface(null, null);

        long deviceHandle = 12345L;

        RemotableSessionType sessionType = new RemotableSessionType(RemotableSessionType.SESSION_TYPE_CARPLAY);

        RemotableSessionStatus sessionStatus = new RemotableSessionStatus(RemotableSessionStatus.SESSION_FAILED);

        RemotableTransport transport = new RemotableTransport(RemotableTransport.TRANSPORT_BT);

        RemotableSessionErrReason errorReason = new RemotableSessionErrReason(RemotableSessionErrReason.ERR_PING_RESPONSE_TIMEOUT);


        remoteUIInterface.mIDevCtrlManagerRUI.sessionStateChange(deviceHandle, sessionType, sessionStatus, transport, errorReason);


        verify(mockLog).printError(eq("mDeviceStateManager is null"));

        verify(mockDeviceStateManager, never()).sessionStateChange(anyLong(), any(RemotableSessionType.class), any(RemotableSessionStatus.class), any(RemotableTransport.class), any(RemotableSessionErrReason.class));

    }


    @Test

    public void testSessionStateChange_ValidInput_DeviceStateManagerNotNull_CallsSessionStateChange()  throws RemoteException{


        long deviceHandle = 12345L;

        RemotableSessionType sessionType = new RemotableSessionType(RemotableSessionType.SESSION_TYPE_CARPLAY);

        RemotableSessionStatus sessionStatus = new RemotableSessionStatus(RemotableSessionStatus.SESSION_STARTED);

        RemotableTransport transport = new RemotableTransport(RemotableTransport.TRANSPORT_BT);

        RemotableSessionErrReason errorReason = new RemotableSessionErrReason(RemotableSessionErrReason.ERR_UNKNOWN);

        doNothing().when(mockDeviceStateManager).sessionStateChange(anyLong(), any(RemotableSessionType.class), any(RemotableSessionStatus.class), any(RemotableTransport.class), any(RemotableSessionErrReason.class));


        remoteUIInterface.mIDevCtrlManagerRUI.sessionStateChange(deviceHandle, sessionType, sessionStatus, transport, errorReason);


        verify(mockDeviceStateManager).sessionStateChange(eq(deviceHandle), eq(sessionType), eq(sessionStatus), eq(transport), eq(errorReason));

        verify(mockLog, never()).printError(anyString());

    }


    @Test

    public void testSessionStateChange_ValidInput_DeviceStateManagerNull_LogsError()  throws RemoteException{


        remoteUIInterface = new RemoteUIInterface(mockCallback, null);

        long deviceHandle = 12345L;

        RemotableSessionType sessionType = new RemotableSessionType(RemotableSessionType.SESSION_TYPE_CARPLAY);

        RemotableSessionStatus sessionStatus = new RemotableSessionStatus(RemotableSessionStatus.SESSION_STARTED);

        RemotableTransport transport = new RemotableTransport(RemotableTransport.TRANSPORT_BT);

        RemotableSessionErrReason errorReason = new RemotableSessionErrReason(RemotableSessionErrReason.ERR_UNKNOWN);


        remoteUIInterface.mIDevCtrlManagerRUI.sessionStateChange(deviceHandle, sessionType, sessionStatus, transport, errorReason);


        verify(mockLog).printError(eq("mDeviceStateManager is null"));

        verify(mockDeviceStateManager, never()).sessionStateChange(anyLong(), any(RemotableSessionType.class), any(RemotableSessionStatus.class), any(RemotableTransport.class), any(RemotableSessionErrReason.class));

    }
}