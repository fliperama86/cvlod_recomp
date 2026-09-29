#ifdef __ANDROID__
#include <jni.h>
#include <string>
#include <filesystem>
#include <android/log.h>

#define LOG_TAG "LodRecompNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static JavaVM* g_jvm = nullptr;
static jclass g_main_activity_class = nullptr;

extern void start_rom_validation_thread(std::filesystem::path rom_path, bool persist_selected_path, const char* source_label);

#ifdef LOD_USE_ZELDA_MENU
namespace lod::android {
    void on_rom_picker_result(std::string path, bool copy_failed);
}
#endif



#include <SDL_system.h>

extern "C" JNIEXPORT void JNICALL
Java_org_cvlod_recomp_MainActivity_nativeOnRomSelected(JNIEnv* env, jobject thiz, jstring romPath) {
    if (romPath == nullptr) {
#ifdef LOD_USE_ZELDA_MENU
        lod::android::on_rom_picker_result({}, false);
#endif
        return;
    }
    const char* path_str = env->GetStringUTFChars(romPath, nullptr);
    if (path_str == nullptr) {
        return;
    }
    std::filesystem::path selected_rom_path = path_str;
    LOGI("Native received selected ROM path: %s", path_str);
    env->ReleaseStringUTFChars(romPath, path_str);

#ifdef LOD_USE_ZELDA_MENU
    lod::android::on_rom_picker_result(selected_rom_path.string(), selected_rom_path.empty());
#else
    if (!selected_rom_path.empty()) {
        start_rom_validation_thread(selected_rom_path, true, "Android SAF ROM Picker");
    }
#endif
}

namespace lod::android {

// Invokes a no-arg static void method on MainActivity.
//
// Deliberately avoids FindClass(): on threads attached from native code (the render thread, which is
// where the launcher document is built) FindClass resolves against the system class loader and cannot
// see app classes. Deriving the class from the activity instance works on every thread.
static void call_main_activity_static_void(const char* method_name) {
    JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
    if (env == nullptr) {
        LOGE("SDL_AndroidGetJNIEnv returned null (%s)", method_name);
        return;
    }

    jobject activity = static_cast<jobject>(SDL_AndroidGetActivity());
    if (activity == nullptr) {
        LOGE("SDL_AndroidGetActivity returned null (%s)", method_name);
        return;
    }

    jclass cls = env->GetObjectClass(activity);
    if (cls == nullptr) {
        LOGE("GetObjectClass failed (%s)", method_name);
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
        env->DeleteLocalRef(activity);
        return;
    }

    jmethodID mid = env->GetStaticMethodID(cls, method_name, "()V");
    if (mid == nullptr) {
        LOGE("Could not find static method %s", method_name);
        // A failed lookup leaves a pending exception that would break later JNI calls.
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
    } else {
        env->CallStaticVoidMethod(cls, mid);
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
    }

    env->DeleteLocalRef(cls);
    env->DeleteLocalRef(activity);
}

// MediaPlayer bypasses the SDL audio device, so the in-app master volume and mute have to be
// pushed across explicitly or menu music would ignore the Sound tab entirely.
void set_menu_music_gain(float gain) {
    JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
    if (env == nullptr) {
        return;
    }
    jobject activity = static_cast<jobject>(SDL_AndroidGetActivity());
    if (activity == nullptr) {
        return;
    }
    jclass cls = env->GetObjectClass(activity);
    if (cls != nullptr) {
        jmethodID mid = env->GetStaticMethodID(cls, "setMenuMusicGain", "(F)V");
        if (mid != nullptr) {
            env->CallStaticVoidMethod(cls, mid, static_cast<jfloat>(gain));
        }
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
        env->DeleteLocalRef(cls);
    }
    env->DeleteLocalRef(activity);
}

void start_menu_music() {
    call_main_activity_static_void("startMenuMusic");
}

void stop_menu_music() {
    call_main_activity_static_void("stopMenuMusic");
}

void request_rom_picker_from_java() {
    // Same FindClass hazard as above: this only ever worked because it happens to be called from the
    // Java-originated main thread. Route it through the thread-safe helper.
    call_main_activity_static_void("requestRomPicker");
    LOGI("Triggered requestRomPicker on MainActivity");
}

// Returns the app's private filesDir path (e.g. /data/user/0/org.cvlod.recomp/files).
// This is where copyUriToCache writes rom.z64 after a SAF ROM pick, so
// discover_rom_path can find it on subsequent launches or after a successful pick.
// Returns an empty string if the JNI call fails.
std::string get_files_dir() {
    JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
    if (env == nullptr) {
        return {};
    }

    jobject activity = static_cast<jobject>(SDL_AndroidGetActivity());
    if (activity == nullptr) {
        return {};
    }

    jclass cls = env->GetObjectClass(activity);
    std::string result;
    if (cls != nullptr) {
        jmethodID mid = env->GetStaticMethodID(cls, "getInternalStoragePath", "()Ljava/lang/String;");
        if (mid != nullptr) {
            jstring jstr = static_cast<jstring>(env->CallStaticObjectMethod(cls, mid));
            if (jstr != nullptr) {
                const char* chars = env->GetStringUTFChars(jstr, nullptr);
                if (chars != nullptr) {
                    result = chars;
                    env->ReleaseStringUTFChars(jstr, chars);
                }
                env->DeleteLocalRef(jstr);
            }
        }
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
        env->DeleteLocalRef(cls);
    }
    env->DeleteLocalRef(activity);
    return result;
}

} // namespace lod::android

// NFD Stubs for Android
typedef int nfdresult_t;
#define NFD_ERROR 0
#define NFD_OKAY 1
#define NFD_CANCEL 2

extern "C" {
    nfdresult_t NFD_Init() { return NFD_OKAY; }
    void NFD_Quit() {}
    const char* NFD_GetError() { return "NFD not supported on Android"; }
    nfdresult_t NFD_OpenDialogU8(char** outPath, const void* filters, int filterCount, const char* defaultPath) { return NFD_CANCEL; }
    void NFD_FreePathU8(char* outPath) {}
    nfdresult_t NFD_PickFolderN(char** outPath, const char* defaultPath) { return NFD_CANCEL; }
    void NFD_FreePathN(char* outPath) {}
    nfdresult_t NFD_OpenDialogN(char** outPath, const void* filters, int filterCount, const char* defaultPath) { return NFD_CANCEL; }
    nfdresult_t NFD_SaveDialogN(char** outPath, const void* filters, int filterCount, const char* defaultPath, const char* defaultName) { return NFD_CANCEL; }
    nfdresult_t NFD_OpenDialogMultipleU8(const void** outPaths, const void* filters, int filterCount, const char* defaultPath) { return NFD_CANCEL; }
    size_t NFD_PathSet_GetCount(const void* pathSet) { return 0; }
    nfdresult_t NFD_PathSet_GetPathU8(const void* pathSet, size_t index, char** outPath) { return NFD_CANCEL; }
    void NFD_PathSet_FreePathU8(char* outPath) {}
    void NFD_PathSet_Free(const void* pathSet) {}
}

#endif
