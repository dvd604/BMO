package pw.dvd604.bmocontroller;

import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.format.Formatter;
import android.util.Log;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com._8rine.upnpdiscovery.UPnPDevice;
import com._8rine.upnpdiscovery.UPnPDiscovery;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    boolean found = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        final boolean[] found = {false};
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ((WebView) findViewById(R.id.webview1)).setWebViewClient(new WebViewClient());
        ((WebView) findViewById(R.id.webview1)).loadUrl("http://printrworks.co.uk/bmo/loading");

        final RequestQueue queue = Volley.newRequestQueue(this);
        Log.e("TEST", getLocalIpAddress(true));
        final String address = getLocalIpAddress(true).substring(0, getLocalIpAddress(true).lastIndexOf(".")) + ".";
        Log.e("TEST", address);

        for(int ip = 1; ip < 255; ip++) {
            final int finalIp = ip;
            StringRequest stringRequest = new StringRequest(Request.Method.GET, "http://" + address + ip,
                    new Response.Listener<String>() {
                        @Override
                        public void onResponse(String response) {
                            // Display the first 500 characters of the response string.
                            if(response.contains("BMO")){
                                found[0] = true;
                                ((WebView) findViewById(R.id.webview1)).loadUrl("http://"  + address + finalIp);
                                queue.cancelAll("BMO");

                            }
                        }
                    }, new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {

                }
            });

// Add the request to the RequestQueue.
            stringRequest.setTag("BMO");
            queue.add(stringRequest);
            queue.addRequestFinishedListener(new RequestQueue.RequestFinishedListener<Object>() {
                @Override
                public void onRequestFinished(Request<Object> request) {
                    if(request.getUrl().contains(address + "100") && !found[0]){
                        Log.e("test","Finished");
                        ((WebView) findViewById(R.id.webview1)).loadUrl("http://printrworks.co.uk/bmo/nobmo");
                    }
                }
            });
        }

       /* //while(!found) {
            //SCAN NETWORK
            UPnPDiscovery.discoveryDevices(this, new UPnPDiscovery.OnDiscoveryListener() {
                @Override
                public void OnStart() {
                    Log.d("UPnPDiscovery", "Starting discovery");
                }

                @Override
                public void OnFoundNewDevice(UPnPDevice device) {
                    Log.d("UPnPDiscovery", "Found new device: " + device.getFriendlyName());
                }

                @Override
                public void OnFinish(HashSet<UPnPDevice> devices) {
                    Log.d("UPnPDiscovery", "Finish discovery");

                    try {
                        for (UPnPDevice d : devices) {
                            if (d.getFriendlyName().contains("BMO")) {
                                ((WebView) findViewById(R.id.webview1)).loadUrl("http://" + d.getHostAddress());
                                Log.e("BMO", "FOUND BMO");
                                found = true;
                            }
                        }
                    } catch(Exception ignored){}

                    if(!found){
                        ((WebView) findViewById(R.id.webview1)).loadUrl("http://printrworks.co.uk/bmo/nobmo");
                    }

                }

                @Override
                public void OnError(Exception e) {
                    Log.d("UPnPDiscovery", "Error: " + e.getLocalizedMessage());
                }
            });*/
        // }

    }

    public static String getLocalIpAddress(boolean useIPv4) {
        try {
            List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface intf : interfaces) {
                List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
                for (InetAddress addr : addrs) {
                    if (!addr.isLoopbackAddress()) {
                        String sAddr = addr.getHostAddress();
                        //boolean isIPv4 = InetAddressUtils.isIPv4Address(sAddr);
                        boolean isIPv4 = sAddr.indexOf(':')<0;

                        if (useIPv4) {
                            if (isIPv4)
                                return sAddr;
                        } else {
                            if (!isIPv4) {
                                int delim = sAddr.indexOf('%'); // drop ip6 zone suffix
                                return delim<0 ? sAddr.toUpperCase() : sAddr.substring(0, delim).toUpperCase();
                            }
                        }
                    }
                }
            }
        } catch (Exception ignored) { } // for now eat exceptions
        return "";
    }
}
