package org.cvlod.recomp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;

/**
 * Shows the splash artwork for a fixed duration before handing off to MainActivity.
 *
 * A dedicated activity is needed because the theme's windowBackground alone is covered by SDL's
 * surface within ~200ms of launch, which is too brief to read. Orientation is deliberately left
 * unlocked here (MainActivity locks to landscape) so the portrait artwork is used when the device
 * is held upright.
 */
public class SplashActivity extends Activity {
    /** How long the artwork stays on screen before MainActivity is started. */
    private static final long SPLASH_DURATION_MS = 2500;

    private final Handler handler = new Handler(Looper.getMainLooper());
    private boolean handedOff = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        handler.postDelayed(this::handOff, SPLASH_DURATION_MS);
    }

    private void handOff() {
        if (handedOff) {
            return;
        }
        handedOff = true;
        startActivity(new Intent(this, MainActivity.class));
        // No transition animation, so the splash cuts straight to the game surface.
        overridePendingTransition(0, 0);
        finish();
    }

    @Override
    protected void onDestroy() {
        handler.removeCallbacksAndMessages(null);
        super.onDestroy();
    }
}
