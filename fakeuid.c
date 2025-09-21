#define _GNU_SOURCE
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static uid_t termux_uid = 0;
static gid_t termux_gid = 0;
static gid_t termux_groups[5];
static int termux_ngroups = 0;

// initialize once by stat() on /data/data/com.termux
static void init_termux_ids() {
    if (termux_uid != 0) return;

    struct stat st;
    if (stat("/data/data/com.termux", &st) == 0) {
        termux_uid = st.st_uid;
        termux_gid = st.st_gid;

        // common Termux groups: inet, everybody, cache, all_X
        termux_groups[0] = termux_gid;
        termux_groups[1] = 3003;   // inet
        termux_groups[2] = 9997;   // everybody
        termux_groups[3] = 20000 + (termux_uid % 10000); // cache group
        termux_groups[4] = 50000 + (termux_uid % 10000); // all_X group
        termux_ngroups = 5;
    } else {
        termux_uid = getuid();
        termux_gid = getgid();
        termux_groups[0] = termux_gid;
        termux_ngroups = 1;
    }
}

uid_t getuid(void)   { init_termux_ids(); return termux_uid; }
uid_t geteuid(void)  { init_termux_ids(); return termux_uid; }
gid_t getgid(void)   { init_termux_ids(); return termux_gid; }
gid_t getegid(void)  { init_termux_ids(); return termux_gid; }

int getgroups(int size, gid_t list[]) {
    init_termux_ids();
    if (size < termux_ngroups) return termux_ngroups; // tell caller how many
    for (int i = 0; i < termux_ngroups; i++) list[i] = termux_groups[i];
    return termux_ngroups;
}
