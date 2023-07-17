/********************************************************************
*
*   VISTEON TECHNICAL & SERVICES CENTRE CONFIDENTIAL PROPRIETARY
*
*
*   FILENAME       - TestUtils.java
*
*
*********************************************************************
*   CHANGE HISTORY
*   -----------------------------------------------------------------
*   DATE        REVISION      AUTHOR             COMMENTS
*   -----------------------------------------------------------------
*   03/05/23    1.0           Neeraj Nama        Initial version
*********************************************************************
*  File Description
*  ---------------------
* General helper Unit Test function class
********************************************************************/

 

package com.allgo.devicecontolmanager;

 

import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.*;

 

import android.content.Intent;
import android.content.res.Resources;
import android.os.Handler;
import android.os.Looper;

 

import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import android.test.suitebuilder.annotation.MediumTest;

 

import com.allgo.devicecontroller.DeviceInfo;
import com.allgo.devicecontroller.DeviceController;

 

import org.junit.Assert;
import org.mockito.ArgumentCaptor;
import org.mockito.internal.util.MockUtil;

 

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;
import com.allgo.DevCtrlMgrIface.*;

 

/**
* A set of methods useful in DCM instrumentation tests
*/
public class TestUtils {
    private static final int SERVICE_TOGGLE_TIMEOUT_MS = 1000;    // 1s

 


    /**
     * Utility method to replace obj.fieldName with newValue where obj is of type c
     *
     * @param c type of obj
     * @param fieldName field name to be replaced
     * @param obj instance of type c whose fieldName is to be replaced, null for static fields
     * @param newValue object used to replace fieldName
     * @return the old value of fieldName that got replaced, caller is responsible for restoring
     *         it back to obj
     * @throws NoSuchFieldException when fieldName is not found in type c
     * @throws IllegalAccessException when fieldName cannot be accessed in type c
     */
    public static Object replaceField(final Class c, final String fieldName, final Object obj,
            final Object newValue) throws NoSuchFieldException, IllegalAccessException {
        Field field = c.getDeclaredField(fieldName);
        field.setAccessible(true);

 

        Object oldValue = field.get(obj);
        field.set(obj, newValue);
        return oldValue;
    }

 

    /**
     * Wait and verify that an intent has been received.
     *
     * @param timeoutMs the time (in milliseconds) to wait for the intent
     * @param queue the queue for the intent
     * @return the received intent
     */
    public static Intent waitForIntent(int timeoutMs, BlockingQueue<Intent> queue) {
        try {
            Intent intent = queue.poll(timeoutMs, TimeUnit.MILLISECONDS);
            Assert.assertNotNull(intent);
            return intent;
        } catch (InterruptedException e) {
            Assert.fail("Cannot obtain an Intent from the queue: " + e.getMessage());
        }
        return null;
    }

 

    /**
     * Wait and verify that no intent has been received.
     *
     * @param timeoutMs the time (in milliseconds) to wait and verify no intent
     * has been received
     * @param queue the queue for the intent
     * @return the received intent. Should be null under normal circumstances
     */
    public static Intent waitForNoIntent(int timeoutMs, BlockingQueue<Intent> queue) {
        try {
            Intent intent = queue.poll(timeoutMs, TimeUnit.MILLISECONDS);
            Assert.assertNull(intent);
            return intent;
        } catch (InterruptedException e) {
            Assert.fail("Cannot obtain an Intent from the queue: " + e.getMessage());
        }
        return null;
    }

 

    /**
     * Wait for looper to finish its current task and all tasks schedule before this
     *
     * @param looper looper of interest
     */
    public static void waitForLooperToFinishScheduledTask(Looper looper) {
        runOnLooperSync(looper, () -> {
            // do nothing, just need to make sure looper finishes current task
        });
    }

 

    /**
     * Run synchronously a runnable action on a looper.
     * The method will return after the action has been execution to completion.
     *
     * Example:
     * <pre>
     * {@code
     * TestUtils.runOnMainSync(new Runnable() {
     *       public void run() {
     *           Assert.assertTrue(mA2dpService.stop());
     *       }
     *   });
     * }
     * </pre>
     *
     * @param looper the looper used to run the action
     * @param action the action to run
     */
    public static void runOnLooperSync(Looper looper, Runnable action) {
        if (Looper.myLooper() == looper) {
            // requested thread is the same as the current thread. call directly.
            action.run();
        } else {
            Handler handler = new Handler(looper);
            SyncRunnable sr = new SyncRunnable(action);
            handler.post(sr);
            sr.waitForComplete();
        }
    }

 

    /**
     * Helper class used to run synchronously a runnable action on a looper.
     */
    private static final class SyncRunnable implements Runnable {
        private final Runnable mTarget;
        private volatile boolean mComplete = false;

 

        SyncRunnable(Runnable target) {
            mTarget = target;
        }

 

        @Override
        public void run() {
            mTarget.run();
            synchronized (this) {
                mComplete = true;
                notifyAll();
            }
        }

 

        public void waitForComplete() {
            synchronized (this) {
                while (!mComplete) {
                    try {
                        wait();
                    } catch (InterruptedException e) {
                    }
                }
            }
        }
    }

 

    public static DeviceInfo getDeviceInfo(long handle, int transport, int capability){
        DeviceInfo deviceInfo = new DeviceInfo();
        deviceInfo.mHandle = handle;
        deviceInfo.mType = DeviceController.DEVTYPE_ANDROID;
        deviceInfo.mTransport = transport;
        deviceInfo.mActiveCapability = capability ;
        deviceInfo.mCapabilities = capability ;
        deviceInfo.mName = "RUI";
        return deviceInfo;
    }

 

    public static RemotableDevInfo getRemotableDevInfo(long handle, int transport, int capability){
        RemotableDevInfo remotableDevInfo =new RemotableDevInfo();
        remotableDevInfo.mDevHandle = handle;
        remotableDevInfo.mTransports = transport;
        remotableDevInfo.mActiveTransport =  capability;
        remotableDevInfo.mDevType = DeviceController.DEVTYPE_ANDROID;
        remotableDevInfo.mActiveCapability = 6;
        remotableDevInfo.mCapabilities = capability ;
        remotableDevInfo.mDevName = "RUI";
        remotableDevInfo.mBtMacAddress = "A8:AB:B5:DE:10:03";
        remotableDevInfo.mSerialId = "12345";
        return remotableDevInfo;
    }
}
