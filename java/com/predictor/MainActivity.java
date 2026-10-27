package com.predictor;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.widget.Toast;

public class MainActivity extends Activity {
    private static final int OVERLAY_PERMISSION_REQ_CODE = 1002;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Check for overlay drawing permissions immediately at boot
        checkOverlayPermission();
    }

    private void checkOverlayPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!Settings.canDrawOverlays(this)) {
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                        Uri.parse("package:" + getPackageName()));
                startActivityForResult(intent, OVERLAY_PERMISSION_REQ_CODE);
            } else {
                initializeUnconditionalSandbox();
            }
        } else {
            initializeUnconditionalSandbox();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == OVERLAY_PERMISSION_REQ_CODE) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && Settings.canDrawOverlays(this)) {
                initializeUnconditionalSandbox();
            } else {
                Toast.makeText(this, "Overlay permission is required to draw trajectory lines.", Toast.LENGTH_LONG).show();
                finish();
            }
        }
    }

    private void initializeUnconditionalSandbox() {
        // BYPASS: Admin panel login or licensing key check logic is entirely removed here.
        Toast.makeText(this, "Licensing bypass active. Initializing engine...", Toast.LENGTH_SHORT).show();
        
        // Start the background tracking service to render lines
        Intent overlayIntent = new Intent(this, OverlayService.class);
        startService(overlayIntent);
        
        // Boot Carrom Pool package mapping natively within our memory container
        bootVirtualEnvironment("com.miniclip.carrom");
    }

    private void bootVirtualEnvironment(String targetPackage) {
        try {
            Intent launchIntent = getPackageManager().getLaunchIntentForPackage(targetPackage);
            if (launchIntent != null) {
                startActivity(launchIntent);
            }
        } catch (Exception e) {
            Toast.makeText(this, "Target game instance not found inside container environment.", Toast.LENGTH_LONG).show();
        }
    }
}
