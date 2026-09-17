// Single source of truth for the version: the repo VERSION file, so the APK can never disagree
// with the launcher text. versionCode must increase monotonically for in-place upgrades to work.
val lodVersionName = rootProject.file("../VERSION").readText().trim()
val lodVersionParts = lodVersionName.split(".").map { it.toInt() }
val lodVersionCode = lodVersionParts[0] * 10000 + lodVersionParts[1] * 100 + lodVersionParts[2]

plugins {
    id("com.android.application")
}

// The shared runtime assets live at the repo root (assets/). Desktop builds copy them next to the
// executable via the LodRecompAssets CMake target, which never applies to an APK, so stage them into
// the packaged assets under "assets/" and let MainActivity extract them to filesDir on launch.
val gameAssetsRoot = layout.buildDirectory.dir("generated/lodAssets")

// Fixes the Android port needs live inside submodules we do not control (rt64, and plume nested
// inside it), so they are vendored as diffs in patches/ and re-applied here. Without plume.patch the
// game crashes on "Start game" on Adreno GPUs, so this runs before every build rather than being
// left as a manual step someone can forget after a fresh submodule checkout.
val applySubmodulePatches = tasks.register<Exec>("applySubmodulePatches") {
    workingDir = rootProject.file("..")
    commandLine("python", "tools/apply_submodule_patches.py")
    isIgnoreExitValue = false
}

val stageGameAssets = tasks.register<Sync>("stageGameAssets") {
    from(rootProject.file("../assets"))
    into(gameAssetsRoot.map { it.dir("assets") })
}

android {
    namespace = "org.cvlod.recomp"
    compileSdk = 34
    ndkVersion = "26.2.11394342"



    defaultConfig {
        applicationId = "org.cvlod.recomp"
        minSdk = 28
        targetSdk = 34
        versionCode = lodVersionCode
        versionName = lodVersionName

        externalNativeBuild {
            cmake {
                arguments(
                    "-DANDROID_STL=c++_shared",
                    "-DLOD_USE_ZELDA_MENU=ON",
                    "-DRT64_STATIC=TRUE",
                    "-DRT64_SDL_WINDOW_VULKAN=TRUE"
                )
                targets("LodRecomp")
            }
        }

        ndk {
            abiFilters.addAll(setOf("arm64-v8a", "x86_64"))
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
            // The recompiled game functions are only optimized in a release native build, which is
            // the difference between running at ~40% speed and full speed. Sign with the debug key
            // so that optimized build is installable for testing without extra key setup.
            signingConfig = signingConfigs.getByName("debug")
        }
    }

    externalNativeBuild {
        cmake {
            path = file("../../CMakeLists.txt")
            version = "3.22.1"
        }
    }

    sourceSets["main"].assets.srcDir(gameAssetsRoot)

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.13.1")
    implementation("androidx.appcompat:appcompat:1.7.0")
    implementation("com.google.android.material:material:1.12.0")
    implementation("androidx.constraintlayout:constraintlayout:2.1.4")
    implementation("androidx.documentfile:documentfile:1.0.1")
}

tasks.withType<JavaCompile>().configureEach {
    options.encoding = "UTF-8"
}

tasks.named("preBuild") {
    dependsOn(applySubmodulePatches, stageGameAssets)
}
