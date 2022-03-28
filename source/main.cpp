#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

#include <switch.h>
#include <cstring>

#include "mp3.h"
#include "util.h"

#include <sstream>
#include <iostream>
#include <string>

#include "misc/config.h"
#include "misc/misc.h"

using namespace std;

// bool onboot = false;

#define ERPT_SAVE_ID 0x80000000000000D1
#define TITLE_ID 0x00FF0000000002AA
#define INNER_HEAP_SIZE 0x40000
size_t nx_inner_heap_size = INNER_HEAP_SIZE;
char nx_inner_heap[INNER_HEAP_SIZE];

extern "C"
{
    void __libnx_initheap(void)
    {
        void *addr = nx_inner_heap;
        size_t size = nx_inner_heap_size;

        // Newlib
        extern char *fake_heap_start;
        extern char *fake_heap_end;

        fake_heap_start = (char *)addr;
        fake_heap_end = (char *)addr + size;
    }

    // Init/exit services, update as needed.
    void __attribute__((weak)) __appInit(void)
    {
        Result rc;

        // Initialize default services.
        rc = smInitialize();
        // if (R_FAILED(rc))
        // fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));

        // Enable this if you want to use HID.
        rc = hidInitialize();
        // if (R_FAILED(rc))
        // fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));

        // Enable this if you want to use time.
        /*rc = timeInitialize();
        if (R_FAILED(rc))
            fatalLater(MAKERESULT(Module_Libnx, LibnxError_InitFail_Time));
        __libnx_init_time();*/

        rc = fsInitialize();
        // if (R_FAILED(rc))
        // fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));

        fsdevMountSdmc();
    }

    void __attribute__((weak)) userAppExit(void);

    void __attribute__((weak)) __appExit(void)
    {
        // Cleanup default services.
        fsdevUnmountAll();
        fsExit();
        // timeExit();//Enable this if you want to use time.
        hidExit(); // Enable this if you want to use HID.
        smExit();
    }
}

// Main program entrypoint
int main(int argc, char *argv[])
{
    // create a configuration named "BOOTSOUND_CFG" in a writable directory (BootSoundNX Config Dir)
    // auto config = new Config("BOOTSOUND_CFG", "sdmc://config/BootSoundNX/config.cfg");

    // add a "config group"
    // configUtils::addConfigGroup(config);

    // load the configuration from file, overwriting default values (added in addConfigGroup)
    // if (!config->load())
    //{
    //    // file doesn't exist or is malformed, (re)create it
    //    config->save();
    //}

    // const string bootSoundNameStr = config->getOption("BOOT_SOUND_NX", "SOUND_NAME")->getString();

    // int soundEnabled = config->getOption("BOOT_SOUND_NX", "SOUND_ENABLED")->getInteger(); // 1 for on, 0 for off.
    // int experimentalFeaturesEnabled = config->getOption("BOOT_SOUND_NX", "EXPERIMENTAL_FEATURES_ON")->getInteger(); // 1 for on, 0 for off.

    // char c[bootSoundNameStr.size() + 1];
    // const char *bootSoundName = strcpy(c, bootSoundNameStr.c_str());

    // if (soundEnabled == 1 && experimentalFeaturesEnabled == 1)
    //{
    // FILE *Soundfile = fopen("sdmc://config/BootSoundNX/sound/" + *bootSoundName, "r+");

    // if (Soundfile != NULL)
    //{
    // playMp3("sdmc://config/BootSoundNX/sound/bootsound.mp3");
    // fclose(Soundfile);
    //}
    //}
    // else
    FILE *Soundfile = fopen("sdmc://config/BootSoundNX/sound/bootsound.mp3", "r+");

    if (Soundfile != NULL)
    {
        playMp3("sdmc://config/BootSoundNX/sound/bootsound.mp3");
        fclose(Soundfile);
    }
    printf("Sound not enabled... Continuing...");
    return 0;
}