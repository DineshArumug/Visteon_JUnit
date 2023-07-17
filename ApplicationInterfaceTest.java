package com.allgo.devicecontrolmanager;

import com.allgo.DevCtrlMgrIface.*;
import com.allgo.devicecontrolmanager.ApplicationInterface;


import static org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.verify;

@RunWith(JUnit4.class)
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
    public void setup() throws Exception
    {
        MockitoAnnotations.initMocks(this);
        mApplicationInterface = new ApplicationInterface(mMockHMICb, mMockdeviceStateManager);
        doNothing().when(mMockHMICb).onDeviceConnected(mMockremotableDevInfo, mMockremotableEventType);
    }

    @Test
    public void onDeviceConnected_RemotableDevInfoNull_ReturnSuccessStatus() 
    {

        int SUCCESS = 0;
        RemotableDevInfo remotableDevInfo = null;

//        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, mMockremotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, mMockremotableEventType);

        assertEquals(SUCCESS,result);
        try{
        verify(mMockHMICb).onDeviceConnected(remotableDevInfo, mMockremotableEventType);}
        catch(Exception e){
        e.printStackTrace();
        }

    }

    @Test
    public void onDeviceConnected_RemotableEventTypeNull_ReturnSuccessStatus() 
    {
        //Arrange
        int SUCCESS = 0;
        RemotableEventType remotableEventType = null;

        //Act
//        doNothing().when(mMockHMICb).onDeviceConnected(mMockremotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(mMockremotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        try{
        verify(mMockHMICb).onDeviceConnected(mMremotableDevInfo, remotableEventType);}
        catch(Exception e){
        e.printStackTrace();
        }

    }

    @Test
    public void onDeviceConnected_BothObjectNonNull_ReturnSuccessStatus() 
    {
        //Arrange
        int SUCCESS = 0;
        RemotableEventType remotableEventType = mMockremotableEventType;
        RemotableDevInfo remotableDevInfo = mMockremotableDevInfo;

        //Act
//        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        try{
        verify(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        }
        catch(Exception e){
        e.printStackTrace();
        }

    }

    @Test
    public void onDeviceConnected_BothObjectWithNull_ReturnSuccessStatus() 
    {
        //Arrange
        int SUCCESS = 0;
        RemotableDevInfo remotableDevInfo = null;
        RemotableEventType remotableEventType = null;

        //Act
 //       doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
 	try{
        verify(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        }
        catch(Exception e){
        e.printStackTrace();
        }
    }

    @Test
    public void onDeviceConnected_mCallbackWithNull_ReturnInvalidStatus()
    {
        //Arrange
        int INVALID_STATUS = -1;

        RemotableEventType remotableEventType = mMockremotableEventType;
        RemotableDevInfo remotableDevInfo = mMockremotableDevInfo;
        mApplicationInterface = new ApplicationInterface(null,mMockdeviceStateManager);

        //Act
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(INVALID_STATUS,result);
    }

    @Test
    public void onDeviceConnected_mCallbackWithoutNull_ReturnSucessStatus() 
    {
        //Arrange
        int SUCCESS = 0;
        RemotableDevInfo remotableDevInfo = mMockremotableDevInfo;
        RemotableEventType remotableEventType = mMockremotableEventType;

        //Act
//        doNothing().when(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        int result = mApplicationInterface.onDeviceConnected(remotableDevInfo, remotableEventType);

        //Assert
        assertEquals(SUCCESS,result);
        try{
       	verify(mMockHMICb).onDeviceConnected(remotableDevInfo, remotableEventType);
        }
        catch(Exception e){
        e.printStackTrace();
        }
    }
}

