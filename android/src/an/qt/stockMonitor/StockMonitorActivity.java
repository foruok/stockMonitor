package an.qt.stockMonitor;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.location.LocationManager;
import android.location.Criteria;
import android.provider.Settings;
import android.os.Bundle;
import android.os.Environment;
import java.io.File;

public class StockMonitorActivity extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static NotificationManager m_notificationManager;
    private static StockMonitorActivity m_instance;
    private final static String TAG = "StockMonitor";

    public StockMonitorActivity(){
        m_instance = this;
    }

    public static int networkState(){
        ConnectivityManager conMan = (ConnectivityManager) m_instance.getSystemService(Context.CONNECTIVITY_SERVICE);
        return conMan.getActiveNetworkInfo() == null ? 0 : 1;
    }

    public static void notify(String s){
        if (m_notificationManager == null) {
            m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
        }

        String title = m_instance.getString(R.string.notify_title);
        Notification n = new Notification(R.drawable.icon, title, System.currentTimeMillis());
        n.flags = Notification.FLAG_AUTO_CANCEL;
        Intent i = new Intent(m_instance, StockMonitorActivity.class);
        i.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP | Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);

        PendingIntent contentIntent = PendingIntent.getActivity(
            m_instance,
            R.string.app_name,
            i,
            PendingIntent.FLAG_UPDATE_CURRENT);

        n.setLatestEventInfo(
            m_instance,
            title,
            s,
            contentIntent);
        m_notificationManager.notify(R.string.app_name, n);
    }
    
    public static void launchActivity(String action){
        m_instance.startActivity(new Intent(action));
    }
}
