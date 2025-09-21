# TermuxInAdbRoot

Run a full Termux shell inside `adb shell` (with root) by faking the Termux app UID using `LD_PRELOAD`.

---

## 📂 Project structure

```
.
├── lib
│   ├── arm/              # prebuilt .so for 32-bit ARM (optional)
│   └── arm64/            # prebuilt .so for 64-bit ARM
│       └── fakeuid.so
└── src
    └── fakeuid.c         # source code for building fakeuid.so
```

---

## 🚀 Usage

### 1. Push `fakeuid.so` to device
```sh
adb push lib/arm64/fakeuid.so /data/local/tmp/fakeuid.so
```

*(use `lib/arm/fakeuid.so` if your device is 32-bit)*

---

### 2. Open adb shell
```sh
adb shell
```

---

### 3. Start Termux login shell with fake UID

```sh
su -c "exec /data/data/com.termux/files/usr/bin/login"
```

Then set environment variables:

```sh
export SHELL=/data/data/com.termux/files/usr/bin/bash
export COLORTERM=truecolor
export HISTCONTROL=ignoreboth
export PREFIX=/data/data/com.termux/files/usr
export SHELL_CMD__RUNNER_NAME=terminal-session
export TERMUX_APP__PACKAGE_NAME=com.termux
export TERMUX_APP__IS_INSTALLED_ON_EXTERNAL_STORAGE=false
export PWD=/data/data/com.termux/files/home
export TERMUX__HOME=/data/data/com.termux/files/home
export TERMUX_VERSION=0.119.0-beta.3
export TERMUX_APP__SE_INFO=default:targetSdkVersion=28:complete
export TERMUX__ROOTFS_DIR=/data/data/com.termux/files
export EXTERNAL_STORAGE=/sdcard
export HOME=/data/data/com.termux/files/home
export LANG=en_US.UTF-8
export TERMUX_APP__PID=16693
export TERMUX_APP__APP_VERSION_NAME=0.119.0-beta.3
export ANDROID_DATA=/data
export TERMUX__APPS_DIR=/data/user/0/com.termux/termux/apps
export SHELL_CMD__SHELL_ID=1
export TERMUX__PREFIX=/data/data/com.termux/files/usr
export TERMUX_APP__SE_FILE_CONTEXT=u:object_r:app_data_file:s0:c143,c257,c512,c768
export ANDROID_STORAGE=/storage
export TERM=xterm-256color
export TERMUX_APP__IS_DEBUGGABLE_BUILD=false
export TERMUX_APP__APP_VERSION_CODE=1022
export TERMUX_APP__LEGACY_DATA_DIR=/data/data/com.termux
export SHLVL=1
export SHELL_CMD__APP_TERMINAL_SESSION_NUMBER_SINCE_APP_START=0
export TERMUX_APP__APK_RELEASE=F_DROID
export TERMUX__SE_PROCESS_CONTEXT=u:r:untrusted_app_27:s0:c143,c257,c512,c768
export SHELL_CMD__PACKAGE_NAME=com.termux
export PATH=/data/data/com.termux/files/usr/bin
export TERMUX_APP__DATA_DIR=/data/user/0/com.termux
export TERMUX_APP__TARGET_SDK=28
export TERMUX__UID=10399
export TERMUX__USER_ID=0
export SHELL_CMD__APP_TERMINAL_SESSION_NUMBER_SINCE_BOOT=1
export _=/data/data/com.termux/files/usr/bin/env
export LD_PRELOAD="/data/local/tmp/fakeuid.so:/data/data/com.termux/files/usr/lib/libtermux-exec-ld-preload.so"

clear
```

---

### 4. Update Termux packages

Now you can run:

```sh
pkg update
apt update
```

Both will believe you are the Termux app user (UID ≈ 10399) instead of root.

---

## 🔧 Building from source

If you want to rebuild `fakeuid.so` yourself:

```sh
cd src
gcc -shared -fPIC -o fakeuid.so fakeuid.c
```

Then copy to the right place in `lib/arm64/` or `lib/arm/` before pushing to device.

---

## ⚠️ Notes

- Requires a rooted device (KernelSU, Magisk, etc).  
- `LD_PRELOAD` tricks user/group detection but does not truly drop root privileges.  
- Some apps or scripts may still detect root through SELinux context.  
- Use at your own risk — this project is for experimentation.

---

## 📜 License

MIT License (or your choice)
