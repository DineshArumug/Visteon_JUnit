package com.example.project1;

import static org.junit.Assert.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import com.example.project1.RemotableDevInfo;


import org.junit.Before;
import org.junit.Test;
import org.mockito.Mockito;

public class ApplicationInterfaceTest {

    ApplicationInterface unit;
    private RemotableDevInfo devInfo;
    private RemotableEventType event ;

    DeviceStateManager deviceStateManager;


    @Before
    public void setUp(){
//        callback = mock(IDevCtrlManagerHMICb.class);
//        unit = new ApplicationInterface(callback,deviceStateManager);
        devInfo = new RemotableDevInfo();
       event = new RemotableEventType();
    }

    @Test
    public void test0() {
        int result = unit.onDeviceDisconnected(devInfo,event);
//        Mockito.doNothing().when(callback).onDeviceDisconnected(devInfo,event);
        assertEquals(0,result);
    }

    @Test
    public void test1() {
        int result = unit.onDeviceDisconnected(devInfo,event);
        assertEquals(0,result);
    }

    @Test
    public void test2() {
        int result = unit.onDeviceDisconnected(null,event);
        assertEquals(0,result);
    }

    @Test
    public void test3() {
        int result = unit.onDeviceDisconnected(devInfo,null);
        assertEquals(0,result);
    }

    @Test
    public void test4() {
        int result = unit.onDeviceDisconnected(null,null);
        assertEquals(0,result);
    }


  //  @Test
  //  public void testmcall(){

}

//    private RemotableDevInfo devInfo;
//    private RemotableEventType event;
//    ApplicationInterface applicationInterface;
//
//    @Before
//    public void setUp() throws Exception {
//        applicationInterface = new ApplicationInterface();
//        devInfo = new RemotableDevInfo();
//        event = new RemotableEventType();
//
//    }
//
//    @Test
//    public void name() {
//        int result = applicationInterface.onDeviceDisconnected(devInfo,event);
//        assertEquals(-1,result);
//    }
