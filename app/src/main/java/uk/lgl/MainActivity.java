package uk.lgl;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.widget.Toast;

import uk.lgl.modmenu.FloatingModMenuService;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("MyLibName");
    }

    @Override
    protected void onCreate(
        Bundle savedInstanceState
    ) {
        super.onCreate(savedInstanceState);

        setContentView(
            R.layout.activity_main
        );

        Start(this);
    }

    public static void Start(
        final Context context
    ) {
        if (
            Build.VERSION.SDK_INT >=
                Build.VERSION_CODES.M &&
            !Settings.canDrawOverlays(
                context
            )
        ) {
            Toast.makeText(
                context,
                "Permita sobreposição para abrir o painel.",
                Toast.LENGTH_LONG
            ).show();

            Intent intent =
                new Intent(
                    Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                    Uri.parse(
                        "package:" +
                        context.getPackageName()
                    )
                );

            context.startActivity(intent);

            return;
        }

        new Handler().postDelayed(
            new Runnable() {
                @Override
                public void run() {
                    context.startService(
                        new Intent(
                            context,
                            FloatingModMenuService.class
                        )
                    );
                }
            },
            300
        );
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        if (
            Build.VERSION.SDK_INT <
                Build.VERSION_CODES.M ||
            Settings.canDrawOverlays(this)
        ) {
            Start(this);
        }
    }
}
