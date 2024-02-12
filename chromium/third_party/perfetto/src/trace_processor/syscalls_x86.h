/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_TRACE_PROCESSOR_SYSCALLS_X86_H_
#define SRC_TRACE_PROCESSOR_SYSCALLS_X86_H_

namespace perfetto {
namespace trace_processor {

// See tools/extract_linux_syscall_tables .
constexpr const char* kSyscalls_x86[] = {
    "sys_restart_syscall",         // 0
    "sys_exit",                    // 1
    "sys_fork",                    // 2
    "sys_read",                    // 3
    "sys_write",                   // 4
    "sys_open",                    // 5
    "sys_close",                   // 6
    "sys_waitpid",                 // 7
    "sys_creat",                   // 8
    "sys_link",                    // 9
    "sys_unlink",                  // 10
    "sys_execve",                  // 11
    "sys_chdir",                   // 12
    "sys_time",                    // 13
    "sys_mknod",                   // 14
    "sys_chmod",                   // 15
    "sys_lchown",                  // 16
    "sys_break",                   // 17
    "sys_oldstat",                 // 18
    "sys_lseek",                   // 19
    "sys_getpid",                  // 20
    "sys_mount",                   // 21
    "sys_umount",                  // 22
    "sys_setuid",                  // 23
    "sys_getuid",                  // 24
    "sys_stime",                   // 25
    "sys_ptrace",                  // 26
    "sys_alarm",                   // 27
    "sys_oldfstat",                // 28
    "sys_pause",                   // 29
    "sys_utime",                   // 30
    "sys_stty",                    // 31
    "sys_gtty",                    // 32
    "sys_access",                  // 33
    "sys_nice",                    // 34
    "sys_ftime",                   // 35
    "sys_sync",                    // 36
    "sys_kill",                    // 37
    "sys_rename",                  // 38
    "sys_mkdir",                   // 39
    "sys_rmdir",                   // 40
    "sys_dup",                     // 41
    "sys_pipe",                    // 42
    "sys_times",                   // 43
    "sys_prof",                    // 44
    "sys_brk",                     // 45
    "sys_setgid",                  // 46
    "sys_getgid",                  // 47
    "sys_signal",                  // 48
    "sys_geteuid",                 // 49
    "sys_getegid",                 // 50
    "sys_acct",                    // 51
    "sys_umount2",                 // 52
    "sys_lock",                    // 53
    "sys_ioctl",                   // 54
    "sys_fcntl",                   // 55
    "sys_mpx",                     // 56
    "sys_setpgid",                 // 57
    "sys_ulimit",                  // 58
    "sys_oldolduname",             // 59
    "sys_umask",                   // 60
    "sys_chroot",                  // 61
    "sys_ustat",                   // 62
    "sys_dup2",                    // 63
    "sys_getppid",                 // 64
    "sys_getpgrp",                 // 65
    "sys_setsid",                  // 66
    "sys_sigaction",               // 67
    "sys_sgetmask",                // 68
    "sys_ssetmask",                // 69
    "sys_setreuid",                // 70
    "sys_setregid",                // 71
    "sys_sigsuspend",              // 72
    "sys_sigpending",              // 73
    "sys_sethostname",             // 74
    "sys_setrlimit",               // 75
    "sys_getrlimit",               // 76
    "sys_getrusage",               // 77
    "sys_gettimeofday",            // 78
    "sys_settimeofday",            // 79
    "sys_getgroups",               // 80
    "sys_setgroups",               // 81
    "sys_select",                  // 82
    "sys_symlink",                 // 83
    "sys_oldlstat",                // 84
    "sys_readlink",                // 85
    "sys_uselib",                  // 86
    "sys_swapon",                  // 87
    "sys_reboot",                  // 88
    "sys_readdir",                 // 89
    "sys_mmap",                    // 90
    "sys_munmap",                  // 91
    "sys_truncate",                // 92
    "sys_ftruncate",               // 93
    "sys_fchmod",                  // 94
    "sys_fchown",                  // 95
    "sys_getpriority",             // 96
    "sys_setpriority",             // 97
    "sys_profil",                  // 98
    "sys_statfs",                  // 99
    "sys_fstatfs",                 // 100
    "sys_ioperm",                  // 101
    "sys_socketcall",              // 102
    "sys_syslog",                  // 103
    "sys_setitimer",               // 104
    "sys_getitimer",               // 105
    "sys_stat",                    // 106
    "sys_lstat",                   // 107
    "sys_fstat",                   // 108
    "sys_olduname",                // 109
    "sys_iopl",                    // 110
    "sys_vhangup",                 // 111
    "sys_idle",                    // 112
    "sys_vm86old",                 // 113
    "sys_wait4",                   // 114
    "sys_swapoff",                 // 115
    "sys_sysinfo",                 // 116
    "sys_ipc",                     // 117
    "sys_fsync",                   // 118
    "sys_sigreturn",               // 119
    "sys_clone",                   // 120
    "sys_setdomainname",           // 121
    "sys_uname",                   // 122
    "sys_modify_ldt",              // 123
    "sys_adjtimex",                // 124
    "sys_mprotect",                // 125
    "sys_sigprocmask",             // 126
    "sys_create_module",           // 127
    "sys_init_module",             // 128
    "sys_delete_module",           // 129
    "sys_get_kernel_syms",         // 130
    "sys_quotactl",                // 131
    "sys_getpgid",                 // 132
    "sys_fchdir",                  // 133
    "sys_bdflush",                 // 134
    "sys_sysfs",                   // 135
    "sys_personality",             // 136
    "sys_afs_syscall",             // 137
    "sys_setfsuid",                // 138
    "sys_setfsgid",                // 139
    "sys__llseek",                 // 140
    "sys_getdents",                // 141
    "sys__newselect",              // 142
    "sys_flock",                   // 143
    "sys_msync",                   // 144
    "sys_readv",                   // 145
    "sys_writev",                  // 146
    "sys_getsid",                  // 147
    "sys_fdatasync",               // 148
    "sys__sysctl",                 // 149
    "sys_mlock",                   // 150
    "sys_munlock",                 // 151
    "sys_mlockall",                // 152
    "sys_munlockall",              // 153
    "sys_sched_setparam",          // 154
    "sys_sched_getparam",          // 155
    "sys_sched_setscheduler",      // 156
    "sys_sched_getscheduler",      // 157
    "sys_sched_yield",             // 158
    "sys_sched_get_priority_max",  // 159
    "sys_sched_get_priority_min",  // 160
    "sys_sched_rr_get_interval",   // 161
    "sys_nanosleep",               // 162
    "sys_mremap",                  // 163
    "sys_setresuid",               // 164
    "sys_getresuid",               // 165
    "sys_vm86",                    // 166
    "sys_query_module",            // 167
    "sys_poll",                    // 168
    "sys_nfsservctl",              // 169
    "sys_setresgid",               // 170
    "sys_getresgid",               // 171
    "sys_prctl",                   // 172
    "sys_rt_sigreturn",            // 173
    "sys_rt_sigaction",            // 174
    "sys_rt_sigprocmask",          // 175
    "sys_rt_sigpending",           // 176
    "sys_rt_sigtimedwait",         // 177
    "sys_rt_sigqueueinfo",         // 178
    "sys_rt_sigsuspend",           // 179
    "sys_pread64",                 // 180
    "sys_pwrite64",                // 181
    "sys_chown",                   // 182
    "sys_getcwd",                  // 183
    "sys_capget",                  // 184
    "sys_capset",                  // 185
    "sys_sigaltstack",             // 186
    "sys_sendfile",                // 187
    "sys_getpmsg",                 // 188
    "sys_putpmsg",                 // 189
    "sys_vfork",                   // 190
    "sys_ugetrlimit",              // 191
    "sys_mmap2",                   // 192
    "sys_truncate64",              // 193
    "sys_ftruncate64",             // 194
    "sys_stat64",                  // 195
    "sys_lstat64",                 // 196
    "sys_fstat64",                 // 197
    "sys_lchown32",                // 198
    "sys_getuid32",                // 199
    "sys_getgid32",                // 200
    "sys_geteuid32",               // 201
    "sys_getegid32",               // 202
    "sys_setreuid32",              // 203
    "sys_setregid32",              // 204
    "sys_getgroups32",             // 205
    "sys_setgroups32",             // 206
    "sys_fchown32",                // 207
    "sys_setresuid32",             // 208
    "sys_getresuid32",             // 209
    "sys_setresgid32",             // 210
    "sys_getresgid32",             // 211
    "sys_chown32",                 // 212
    "sys_setuid32",                // 213
    "sys_setgid32",                // 214
    "sys_setfsuid32",              // 215
    "sys_setfsgid32",              // 216
    "sys_pivot_root",              // 217
    "sys_mincore",                 // 218
    "sys_madvise",                 // 219
    "sys_getdents64",              // 220
    "sys_fcntl64",                 // 221
    "",                            // 222
    "",                            // 223
    "sys_gettid",                  // 224
    "sys_readahead",               // 225
    "sys_setxattr",                // 226
    "sys_lsetxattr",               // 227
    "sys_fsetxattr",               // 228
    "sys_getxattr",                // 229
    "sys_lgetxattr",               // 230
    "sys_fgetxattr",               // 231
    "sys_listxattr",               // 232
    "sys_llistxattr",              // 233
    "sys_flistxattr",              // 234
    "sys_removexattr",             // 235
    "sys_lremovexattr",            // 236
    "sys_fremovexattr",            // 237
    "sys_tkill",                   // 238
    "sys_sendfile64",              // 239
    "sys_futex",                   // 240
    "sys_sched_setaffinity",       // 241
    "sys_sched_getaffinity",       // 242
    "sys_set_thread_area",         // 243
    "sys_get_thread_area",         // 244
    "sys_io_setup",                // 245
    "sys_io_destroy",              // 246
    "sys_io_getevents",            // 247
    "sys_io_submit",               // 248
    "sys_io_cancel",               // 249
    "sys_fadvise64",               // 250
    "",                            // 251
    "sys_exit_group",              // 252
    "sys_lookup_dcookie",          // 253
    "sys_epoll_create",            // 254
    "sys_epoll_ctl",               // 255
    "sys_epoll_wait",              // 256
    "sys_remap_file_pages",        // 257
    "sys_set_tid_address",         // 258
    "sys_timer_create",            // 259
    "sys_timer_settime",           // 260
    "sys_timer_gettime",           // 261
    "sys_timer_getoverrun",        // 262
    "sys_timer_delete",            // 263
    "sys_clock_settime",           // 264
    "sys_clock_gettime",           // 265
    "sys_clock_getres",            // 266
    "sys_clock_nanosleep",         // 267
    "sys_statfs64",                // 268
    "sys_fstatfs64",               // 269
    "sys_tgkill",                  // 270
    "sys_utimes",                  // 271
    "sys_fadvise64_64",            // 272
    "sys_vserver",                 // 273
    "sys_mbind",                   // 274
    "sys_get_mempolicy",           // 275
    "sys_set_mempolicy",           // 276
    "sys_mq_open",                 // 277
    "sys_mq_unlink",               // 278
    "sys_mq_timedsend",            // 279
    "sys_mq_timedreceive",         // 280
    "sys_mq_notify",               // 281
    "sys_mq_getsetattr",           // 282
    "sys_kexec_load",              // 283
    "sys_waitid",                  // 284
    "",                            // 285
    "sys_add_key",                 // 286
    "sys_request_key",             // 287
    "sys_keyctl",                  // 288
    "sys_ioprio_set",              // 289
    "sys_ioprio_get",              // 290
    "sys_inotify_init",            // 291
    "sys_inotify_add_watch",       // 292
    "sys_inotify_rm_watch",        // 293
    "sys_migrate_pages",           // 294
    "sys_openat",                  // 295
    "sys_mkdirat",                 // 296
    "sys_mknodat",                 // 297
    "sys_fchownat",                // 298
    "sys_futimesat",               // 299
    "sys_fstatat64",               // 300
    "sys_unlinkat",                // 301
    "sys_renameat",                // 302
    "sys_linkat",                  // 303
    "sys_symlinkat",               // 304
    "sys_readlinkat",              // 305
    "sys_fchmodat",                // 306
    "sys_faccessat",               // 307
    "sys_pselect6",                // 308
    "sys_ppoll",                   // 309
    "sys_unshare",                 // 310
    "sys_set_robust_list",         // 311
    "sys_get_robust_list",         // 312
    "sys_splice",                  // 313
    "sys_sync_file_range",         // 314
    "sys_tee",                     // 315
    "sys_vmsplice",                // 316
    "sys_move_pages",              // 317
    "sys_getcpu",                  // 318
    "sys_epoll_pwait",             // 319
    "sys_utimensat",               // 320
    "sys_signalfd",                // 321
    "sys_timerfd_create",          // 322
    "sys_eventfd",                 // 323
    "sys_fallocate",               // 324
    "sys_timerfd_settime",         // 325
    "sys_timerfd_gettime",         // 326
    "sys_signalfd4",               // 327
    "sys_eventfd2",                // 328
    "sys_epoll_create1",           // 329
    "sys_dup3",                    // 330
    "sys_pipe2",                   // 331
    "sys_inotify_init1",           // 332
    "sys_preadv",                  // 333
    "sys_pwritev",                 // 334
    "sys_rt_tgsigqueueinfo",       // 335
    "sys_perf_event_open",         // 336
    "sys_recvmmsg",                // 337
    "sys_fanotify_init",           // 338
    "sys_fanotify_mark",           // 339
    "sys_prlimit64",               // 340
    "sys_name_to_handle_at",       // 341
    "sys_open_by_handle_at",       // 342
    "sys_clock_adjtime",           // 343
    "sys_syncfs",                  // 344
    "sys_sendmmsg",                // 345
    "sys_setns",                   // 346
    "sys_process_vm_readv",        // 347
    "sys_process_vm_writev",       // 348
    "sys_kcmp",                    // 349
    "sys_finit_module",            // 350
    "sys_sched_setattr",           // 351
    "sys_sched_getattr",           // 352
    "sys_renameat2",               // 353
    "sys_seccomp",                 // 354
    "sys_getrandom",               // 355
    "sys_memfd_create",            // 356
    "sys_bpf",                     // 357
    "sys_execveat",                // 358
    "sys_socket",                  // 359
    "sys_socketpair",              // 360
    "sys_bind",                    // 361
    "sys_connect",                 // 362
    "sys_listen",                  // 363
    "sys_accept4",                 // 364
    "sys_getsockopt",              // 365
    "sys_setsockopt",              // 366
    "sys_getsockname",             // 367
    "sys_getpeername",             // 368
    "sys_sendto",                  // 369
    "sys_sendmsg",                 // 370
    "sys_recvfrom",                // 371
    "sys_recvmsg",                 // 372
    "sys_shutdown",                // 373
    "sys_userfaultfd",             // 374
    "sys_membarrier",              // 375
    "sys_mlock2",                  // 376
    "sys_copy_file_range",         // 377
    "sys_preadv2",                 // 378
    "sys_pwritev2",                // 379
    "sys_pkey_mprotect",           // 380
    "sys_pkey_alloc",              // 381
    "sys_pkey_free",               // 382
    "sys_statx",                   // 383
    "sys_arch_prctl",              // 384
    "sys_io_pgetevents",           // 385
    "sys_rseq",                    // 386
};

}  // namespace trace_processor
}  // namespace perfetto

#endif  // SRC_TRACE_PROCESSOR_SYSCALLS_X86_H_