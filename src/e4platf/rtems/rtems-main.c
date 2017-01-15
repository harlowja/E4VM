//
// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
#include <sys/time.h>

#include <rtems.h>
#include <rtems/bdpart.h>
#include <rtems/bdbuf.h>
//#include <rtems/error.h>
#include <rtems/fsmount.h>
#include <rtems/dosfs.h>

#include <rtems/shell.h>
#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS
#include <rtems/shellconfig.h>

#include <stdlib.h>
#include <errno.h>

rtems_task Init(rtems_task_argument argument);
extern void e4_rtems_main();
void mount_filesystems();

void shell();
//const rtems_bdbuf_config rtems_bdbuf_configuration = {
//        .max_read_ahead_blocks = 5,
//        .max_write_blocks = 5,
//        .swapout_priority = 15,
//        .swapout_period = 250,
//        .swap_block_hold = 1000,
//        .swapout_workers = 0,
//        .swapout_worker_priority = 15,
//        .size = 1024 * 1024,
//        .buffer_min = 512,
//        .buffer_max = 4096
//};

#define HDA_DEV_NAME "/dev/ide0"
static const rtems_fstab_entry fstab[] = {
    {
        .source = HDA_DEV_NAME,
        .target = "/mnt",
        .type = "dosfs",
        .options = RTEMS_FILESYSTEM_READ_WRITE,
        .report_reasons = RTEMS_FSTAB_ANY,
        .abort_reasons = RTEMS_FSTAB_OK
    }
};

void mount_filesystems() {
    int rv = 0;
    size_t abort_index = 0;

    mkdir("/mnt", 0777);

//    rtems_status_code sc = rtems_bdpart_register_from_disk(HDA_DEV_NAME);
//    if (sc != RTEMS_SUCCESSFUL) {
//        printf("E4 mount_fs: read partition table failed: %s\n",
//               rtems_status_text(sc));
//        exit(1);
//    }

//    rv = rtems_fsmount(fstab, sizeof(fstab) / sizeof(fstab[0]), &abort_index);
//    if (rv != 0) {
//        printf("E4 mount_fs: failed %s\n", strerror(errno));
//        exit(2);
//    }

    if (mount(HDA_DEV_NAME, "/mnt", "msdos",
              RTEMS_FILESYSTEM_READ_ONLY, NULL) < 0) {
        fprintf(stderr, "E4 mount: failed: %s\n", strerror(errno));
        exit(1);
    }
    printf("E4 mount_fs: mount aborted at %zu\n", abort_index);
}

void start_shell(void) {
    printf("E4 shell: starting\n");
    rtems_shell_init(
            "SHLL", // task name
            RTEMS_MINIMUM_STACK_SIZE * 4, // task stack size
            100, // task priority
            "/dev/vgacons", // device name
            false, // run forever
            true, // wait for shell to terminate
            NULL // rtems_shell_login_check
    );
}

rtems_task Init(rtems_task_argument argument) {
//    start_shell();
    mount_filesystems();
    //e4_rtems_main();
}

// configuration information
#include <bsp.h>

//
// Configure drivers
//
#define CONFIGURE_MAXIMUM_DRIVERS                  10
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_IDE_DRIVER

//
// Configure file system and libblock
//
#define CONFIGURE_FILESYSTEM_IMFS
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS   20
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

#define CONFIGURE_MAXIMUM_PTYS              1
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS             4
#define CONFIGURE_EXTRA_TASK_STACKS         512000
//#define CONFIGURE_MAXIMUM_POSIX_THREADS   2

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
