package org.cvlod.recomp;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings;
import android.util.Log;
import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

public class MainActivity extends SDLActivity {
    private static final String TAG = "MainActivity";
    private static final int REQUEST_CODE_ROM_PICK = 1001;
    private static final String ASSET_DIR_NAME = "assets";
    private static final String ASSET_STAMP_NAME = "assets.stamp";
    private static MainActivity instance = null;

    public static MainActivity getInstance() {
        return instance;
    }

    public static void requestRomPicker() {
        if (instance != null) {
            instance.runOnUiThread(() -> instance.openRomPicker());
        }
    }

    public static String getInternalStoragePath() {
        if (instance != null && instance.getApplicationContext() != null) {
            return instance.getApplicationContext().getFilesDir().getAbsolutePath();
        }
        return "";
    }

    @Override
    protected String[] getLibraries() {
        return new String[] {
            "c++_shared",
            "LodRecomp"
        };
    }

    @Override
    protected String getMainFunction() {
        return "SDL_main";
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        instance = this;
        // Must run before super.onCreate(), which starts the native thread that reads these files.
        extractBundledAssets();
        super.onCreate(savedInstanceState);
        handleIntent(getIntent());
    }

    /**
     * The UI (fonts, .rml documents, .rcss stylesheets) is loaded by absolute path from
     * filesDir/assets. Unpack the copy packaged in the APK there, refreshing it whenever the
     * installed package changes so an app update cannot leave stale documents behind.
     */
    private void extractBundledAssets() {
        File destRoot = new File(getFilesDir(), ASSET_DIR_NAME);
        File stampFile = new File(getFilesDir(), ASSET_STAMP_NAME);
        String stamp = currentPackageStamp();

        if (destRoot.isDirectory() && stamp.equals(readStamp(stampFile))) {
            return;
        }

        Log.i(TAG, "Extracting bundled assets to " + destRoot.getAbsolutePath());
        deleteRecursively(destRoot);
        try {
            copyAsset(ASSET_DIR_NAME, destRoot);
            writeStamp(stampFile, stamp);
            Log.i(TAG, "Bundled assets extracted");
        } catch (IOException e) {
            // Leave the stamp unwritten so the next launch retries instead of running with a
            // half-populated asset tree.
            Log.e(TAG, "Failed to extract bundled assets: " + e);
        }
    }

    /** Recursively copies an APK asset entry (file or directory) to {@code dest}. */
    private void copyAsset(String assetPath, File dest) throws IOException {
        String[] children = getAssets().list(assetPath);
        if (children == null || children.length == 0) {
            File parent = dest.getParentFile();
            if (parent != null) {
                parent.mkdirs();
            }
            try (InputStream in = getAssets().open(assetPath);
                 OutputStream out = new FileOutputStream(dest)) {
                byte[] buffer = new byte[8192];
                int bytesRead;
                while ((bytesRead = in.read(buffer)) != -1) {
                    out.write(buffer, 0, bytesRead);
                }
            }
            return;
        }

        dest.mkdirs();
        for (String child : children) {
            copyAsset(assetPath + "/" + child, new File(dest, child));
        }
    }

    private String currentPackageStamp() {
        try {
            PackageInfo info = getPackageManager().getPackageInfo(getPackageName(), 0);
            return info.versionName + "/" + info.lastUpdateTime;
        } catch (PackageManager.NameNotFoundException e) {
            return "unknown";
        }
    }

    private static String readStamp(File stampFile) {
        try (InputStream in = new FileInputStream(stampFile)) {
            byte[] buffer = new byte[256];
            int bytesRead = in.read(buffer);
            return bytesRead > 0 ? new String(buffer, 0, bytesRead, StandardCharsets.UTF_8) : "";
        } catch (IOException e) {
            return "";
        }
    }

    private static void writeStamp(File stampFile, String stamp) throws IOException {
        try (OutputStream out = new FileOutputStream(stampFile)) {
            out.write(stamp.getBytes(StandardCharsets.UTF_8));
        }
    }

    private static void deleteRecursively(File file) {
        File[] children = file.listFiles();
        if (children != null) {
            for (File child : children) {
                deleteRecursively(child);
            }
        }
        file.delete();
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
        handleIntent(intent);
    }

    private void handleIntent(Intent intent) {
        if (intent != null && "pick_rom".equals(intent.getStringExtra("action"))) {
            openRomPicker();
        }
    }

    public void openRomPicker() {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");
        startActivityForResult(intent, REQUEST_CODE_ROM_PICK);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE_ROM_PICK && resultCode == Activity.RESULT_OK) {
            if (data != null && data.getData() != null) {
                String cachedPath = copyUriToCache(data.getData());
                if (cachedPath != null) {
                    nativeOnRomSelected(cachedPath);
                }
            }
        }
    }

    private String copyUriToCache(Uri uri) {
        try {
            InputStream inputStream = getContentResolver().openInputStream(uri);
            File destFile = new File(getApplicationContext().getFilesDir(), "rom.z64");
            if (inputStream != null) {
                FileOutputStream outputStream = new FileOutputStream(destFile);
                byte[] buffer = new byte[8192];
                int bytesRead;
                while ((bytesRead = inputStream.read(buffer)) != -1) {
                    outputStream.write(buffer, 0, bytesRead);
                }
                outputStream.close();
                inputStream.close();
                return destFile.getAbsolutePath();
            }
        } catch (Exception e) {
            Log.e(TAG, "Failed to copy selected ROM: " + e.getMessage());
        }
        return null;
    }

    private native void nativeOnRomSelected(String romPath);

    // ── Menu music ──────────────────────────────────────────────────────────────
    // Played through MediaPlayer rather than the game's SDL audio device: the OS decodes the mp3
    // and mixes it, so menu music cannot perturb the emulator's audio queue.

    private static MediaPlayer menuMusic = null;
    private static boolean menuMusicWanted = false;
    // Mirrors the in-app master volume/mute so menu music obeys the Sound tab even though it plays
    // through MediaPlayer rather than the SDL device the slider controls.
    private static float menuMusicGain = 1.0f;

    /** Called from native code when the launcher menu appears. */
    public static void startMenuMusic() {
        if (instance != null) {
            instance.runOnUiThread(() -> instance.startMenuMusicInternal());
        }
    }

    /** Called from native code whenever the in-app master volume or mute changes. */
    public static void setMenuMusicGain(float gain) {
        final float clamped = gain < 0.0f ? 0.0f : (gain > 1.0f ? 1.0f : gain);
        if (instance == null) {
            menuMusicGain = clamped;
            return;
        }
        instance.runOnUiThread(() -> {
            menuMusicGain = clamped;
            if (menuMusic != null) {
                try {
                    menuMusic.setVolume(clamped, clamped);
                } catch (IllegalStateException e) {
                    // Player was released between the post and here; nothing to do.
                }
            }
        });
    }

    /** Called from native code when the game starts. */
    public static void stopMenuMusic() {
        if (instance != null) {
            instance.runOnUiThread(() -> instance.stopMenuMusicInternal());
        }
    }

    private void startMenuMusicInternal() {
        menuMusicWanted = true;
        if (menuMusic != null) {
            if (!menuMusic.isPlaying()) {
                menuMusic.start();
            }
            return;
        }
        try {
            menuMusic = MediaPlayer.create(this, R.raw.menu_music);
            if (menuMusic == null) {
                Log.e(TAG, "MediaPlayer.create returned null for menu music");
                return;
            }
            menuMusic.setLooping(true);
            menuMusic.setVolume(menuMusicGain, menuMusicGain);
            menuMusic.start();
            Log.i(TAG, "Menu music started");
        } catch (Exception e) {
            Log.e(TAG, "Failed to start menu music: " + e);
            releaseMenuMusic();
        }
    }

    private void stopMenuMusicInternal() {
        menuMusicWanted = false;
        releaseMenuMusic();
    }

    private void releaseMenuMusic() {
        if (menuMusic == null) {
            return;
        }
        try {
            menuMusic.stop();
        } catch (IllegalStateException e) {
            // Already stopped; release below regardless.
        }
        menuMusic.release();
        menuMusic = null;
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (menuMusic != null && menuMusic.isPlaying()) {
            menuMusic.pause();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (menuMusicWanted && menuMusic != null && !menuMusic.isPlaying()) {
            menuMusic.start();
        }
    }

    @Override
    protected void onDestroy() {
        releaseMenuMusic();
        super.onDestroy();
    }
}
