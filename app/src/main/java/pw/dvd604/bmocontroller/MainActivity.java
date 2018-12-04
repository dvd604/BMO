package pw.dvd604.bmocontroller;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import com._8rine.upnpdiscovery.UPnPDevice;
import com._8rine.upnpdiscovery.UPnPDiscovery;

import java.util.HashSet;

public class MainActivity extends AppCompatActivity {

    String BMOIP = "";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ((WebView)findViewById(R.id.webview1)).setWebViewClient(new WebViewClient() {

            public void onPageFinished(WebView view, String url) {
                //http://192.168.1.9/
                if(url.split("/").length > 1 && !url.contains("printrworks")){
                    view.loadUrl(BMOIP);
                }
            }
        });

        ((WebView)findViewById(R.id.webview1)).loadUrl("http://printrworks.co.uk/bmo/loading");

        //SCAN NETWORK
        UPnPDiscovery.discoveryDevices(this, new UPnPDiscovery.OnDiscoveryListener() {
            @Override
            public void OnStart() {
                Log.d("UPnPDiscovery", "Starting discovery");
            }

            @Override
            public void OnFoundNewDevice(UPnPDevice device) {
                Log.d("UPnPDiscovery", "Found new device: " + device.toString());
                // String friendlyName = device.getFriendlyName();
                // ... see UPnPDevice description below
                if(device.getFriendlyName().contains("BMO")){
                    Log.i("UPNP", "FOUND BMO!");
                    BMOIP = device.getHostAddress();
                    ((WebView)findViewById(R.id.webview1)).loadUrl(BMOIP);
                }
            }

            @Override
            public void OnFinish(HashSet<UPnPDevice> devices) {
                Log.d("UPnPDiscovery", "Finish discovery");
                if(BMOIP.equals("")){
                    ((WebView)findViewById(R.id.webview1)).loadUrl("http://printrworks.co.uk/bmo/nobmo");
                }
            }

            @Override
            public void OnError(Exception e) {
                Log.d("UPnPDiscovery", "Error: " + e.getLocalizedMessage());
                if(BMOIP.equals("")){
                    ((WebView)findViewById(R.id.webview1)).loadUrl("http://printrworks.co.uk/bmo/nobmo");
                }
            }
        });


    }
}
