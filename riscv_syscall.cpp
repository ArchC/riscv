#include "riscv_syscall.H"

// FIXME: This file has been copied and pasted from the MIPS model.
// Change the implementation accordingly.

// 'using namespace' statement to allow access to all
// riscv-specific datatypes
using namespace riscv_parms;
unsigned procNumber = 0;

void riscv_syscall::get_buffer(int argn, unsigned char* buf, unsigned int size)
{
  unsigned int addr = RB[4+argn];
  for (unsigned int i = 0; i<size; i++, addr++) {
    buf[i] = DATA_PORT->read_byte(addr);
  }
}

void riscv_syscall::guest2hostmemcpy(unsigned char *dst, uint32_t src,
                                    unsigned int size) {
  for (unsigned int i = 0; i < size; i++) {
    dst[i] = DATA_PORT->read_byte(src++);
  }
}

void riscv_syscall::set_buffer(int argn, unsigned char* buf, unsigned int size)
{
  unsigned int addr = RB[4+argn];

  for (unsigned int i = 0; i<size; i++, addr++) {
    DATA_PORT->write_byte(addr, buf[i]);
  }
}

void riscv_syscall::host2guestmemcpy(uint32_t dst, unsigned char *src,
                                    unsigned int size) {
  for (unsigned int i = 0; i < size; i++) {
    DATA_PORT->write_byte(dst++, src[i]);
  }
}

void riscv_syscall::set_buffer_noinvert(int argn, unsigned char* buf, unsigned int size)
{
  unsigned int addr = RB[4+argn];

  for (unsigned int i = 0; i<size; i+=4, addr+=4) {
    DATA_PORT->write(addr, *(unsigned int *) &buf[i]);
    //printf("\nDATA_PORT_no[%d]=%d", addr, buf[i]);
  }
}

int riscv_syscall::get_int(int argn)
{
  return RB[4+argn];
}

void riscv_syscall::set_int(int argn, int val)
{
  RB[2+argn] = val;
}

void riscv_syscall::return_from_syscall()
{
  //ac_pc = RB[31];
  ac_pc = RB[1];
}

void riscv_syscall::set_prog_args(int argc, char **argv)
{
  // ac_argstr holds argument strings to be stored into guest memory
  char ac_argstr[512];
  // guest_stack represents pre-allocated values into the guest stack
  // passing information from kernel to userland (program args, etc)
  //
  // Memory diagram:
  //   higher addresses  .. auxv
  //                        envp pointers
  //                        argv pointers
  //   lower addresses   .. argc
  uint32_t guest_stack_size = argc + 6;
  uint32_t *guest_stack = (uint32_t *) malloc(sizeof(uint32_t) *
                                              guest_stack_size);
  uint32_t i = 0, j = 0;
  uint32_t strtable = AC_RAM_END - 512 - procNumber * 64 * 1024;
  guest_stack[i++] = argc;
  for (uint32_t k = 0; k < argc; ++k) {
    uint32_t len = strlen(argv[k]) + 1;
    guest_stack[i++] = strtable + j;
    if (j + len > 512) {
      fprintf(stderr, "Fatal: argv strings bigger than 512 bytes.\n");
      exit(EXIT_FAILURE);
    }
    memcpy(&ac_argstr[j], argv[k], len);
    j += len;
  }
  // Set argv end
  guest_stack[i++] = 0;
  // Set envp
  guest_stack[i++] = 0;
  // Set auxv
  guest_stack[i++] = 6; // AT_PAGESZ -
                    // see http://articles.manugarg.com/aboutelfauxiliaryvectors
  guest_stack[i++] = 4096; // page size
  guest_stack[i++] = 0; // AT_NULL

  RB[4] = strtable;
  set_buffer(0, (unsigned char*) ac_argstr, 512);   //$25 = $29(sp) - 4 (set_buffer adds 4)

  RB[4] = strtable - guest_stack_size * 4;
  set_buffer_noinvert(0, (unsigned char*) guest_stack, guest_stack_size * 4);

  RB[29] = strtable - guest_stack_size * 4;

  // FIXME: Necessary?
  RB[4] = argc;
  RB[5] = strtable - guest_stack_size * 4 + 4;

  procNumber ++;
}

void riscv_syscall::set_pc(unsigned val) {
  ac_pc.write(val);
}

void riscv_syscall::set_return(unsigned val) {
  //RB.write(31, val);
  RB.write(1, val);
}

unsigned riscv_syscall::get_return() {
  //return (unsigned) RB.read(31);
  return (unsigned) RB.read(1);
}

bool riscv_syscall::is_mmap_anonymous(uint32_t flags) {
  return flags & 0x800;
}

uint32_t riscv_syscall::convert_open_flags(uint32_t flags) {
  uint32_t dst = 0;
  dst |= (flags & 00000)? O_RDONLY : 0;
  dst |= (flags & 00001)? O_WRONLY : 0;
  dst |= (flags & 00002)? O_RDWR   : 0;
  dst |= (flags & 001000)? O_CREAT  : 0;
  dst |= (flags & 004000)? O_EXCL   : 0;
  dst |= (flags & 0100000)? O_NOCTTY   : 0;
  dst |= (flags & 02000)? O_TRUNC    : 0;
  dst |= (flags & 00010)? O_APPEND   : 0;
  dst |= (flags & 040000)? O_NONBLOCK : 0;
  dst |= (flags & 020000)? O_SYNC  : 0;
  // We don't know the mapping of these:
  //    dst |= (flags & 020000)? O_ASYNC   : 0;
  //    dst |= (flags & 0100000)? O_LARGEFILE  : 0;
  //    dst |= (flags & 0200000)? O_DIRECTORY  : 0;
  //    dst |= (flags & 0400000)? O_NOFOLLOW   : 0;
  //    dst |= (flags & 02000000)? O_CLOEXEC   : 0;
  //    dst |= (flags & 040000)? O_DIRECT      : 0;
  //    dst |= (flags & 01000000)? O_NOATIME   : 0;
  //    dst |= (flags & 010000000)? O_PATH     : 0;
  //    dst |= (flags & 010000)? O_DSYNC       : 0;
  return dst;
}


// RISCV syscalls mapping for process simulators
//

#define RISCV__NR_Linux			4000
#define RISCV__NR_syscall			(RISCV__NR_Linux +	0)
#define RISCV__NR_exit			(RISCV__NR_Linux +	1)
#define RISCV__NR_fork			(RISCV__NR_Linux +	2)
#define RISCV__NR_read			(RISCV__NR_Linux +	3)
#define RISCV__NR_write			(RISCV__NR_Linux +	4)
#define RISCV__NR_open			(RISCV__NR_Linux +	5)
#define RISCV__NR_close			(RISCV__NR_Linux +	6)
#define RISCV__NR_waitpid			(RISCV__NR_Linux +	7)
#define RISCV__NR_creat			(RISCV__NR_Linux +	8)
#define RISCV__NR_link			(RISCV__NR_Linux +	9)
#define RISCV__NR_unlink			(RISCV__NR_Linux +  10)
#define RISCV__NR_execve			(RISCV__NR_Linux +  11)
#define RISCV__NR_chdir			(RISCV__NR_Linux +  12)
#define RISCV__NR_time			(RISCV__NR_Linux +  13)
#define RISCV__NR_mknod			(RISCV__NR_Linux +  14)
#define RISCV__NR_chmod			(RISCV__NR_Linux +  15)
#define RISCV__NR_lchown			(RISCV__NR_Linux +  16)
#define RISCV__NR_break			(RISCV__NR_Linux +  17)
#define RISCV__NR_unused18			(RISCV__NR_Linux +  18)
#define RISCV__NR_lseek			(RISCV__NR_Linux +  19)
#define RISCV__NR_getpid			(RISCV__NR_Linux +  20)
#define RISCV__NR_mount			(RISCV__NR_Linux +  21)
#define RISCV__NR_umount			(RISCV__NR_Linux +  22)
#define RISCV__NR_setuid			(RISCV__NR_Linux +  23)
#define RISCV__NR_getuid			(RISCV__NR_Linux +  24)
#define RISCV__NR_stime			(RISCV__NR_Linux +  25)
#define RISCV__NR_ptrace			(RISCV__NR_Linux +  26)
#define RISCV__NR_alarm			(RISCV__NR_Linux +  27)
#define RISCV__NR_unused28			(RISCV__NR_Linux +  28)
#define RISCV__NR_pause			(RISCV__NR_Linux +  29)
#define RISCV__NR_utime			(RISCV__NR_Linux +  30)
#define RISCV__NR_stty			(RISCV__NR_Linux +  31)
#define RISCV__NR_gtty			(RISCV__NR_Linux +  32)
#define RISCV__NR_access			(RISCV__NR_Linux +  33)
#define RISCV__NR_nice			(RISCV__NR_Linux +  34)
#define RISCV__NR_ftime			(RISCV__NR_Linux +  35)
#define RISCV__NR_sync			(RISCV__NR_Linux +  36)
#define RISCV__NR_kill			(RISCV__NR_Linux +  37)
#define RISCV__NR_rename			(RISCV__NR_Linux +  38)
#define RISCV__NR_mkdir			(RISCV__NR_Linux +  39)
#define RISCV__NR_rmdir			(RISCV__NR_Linux +  40)
#define RISCV__NR_dup			(RISCV__NR_Linux +  41)
#define RISCV__NR_pipe			(RISCV__NR_Linux +  42)
#define RISCV__NR_times			(RISCV__NR_Linux +  43)
#define RISCV__NR_prof			(RISCV__NR_Linux +  44)
#define RISCV__NR_brk			(RISCV__NR_Linux +  45)
#define RISCV__NR_setgid			(RISCV__NR_Linux +  46)
#define RISCV__NR_getgid			(RISCV__NR_Linux +  47)
#define RISCV__NR_signal			(RISCV__NR_Linux +  48)
#define RISCV__NR_geteuid			(RISCV__NR_Linux +  49)
#define RISCV__NR_getegid			(RISCV__NR_Linux +  50)
#define RISCV__NR_acct			(RISCV__NR_Linux +  51)
#define RISCV__NR_umount2			(RISCV__NR_Linux +  52)
#define RISCV__NR_lock			(RISCV__NR_Linux +  53)
#define RISCV__NR_ioctl			(RISCV__NR_Linux +  54)
#define RISCV__NR_fcntl			(RISCV__NR_Linux +  55)
#define RISCV__NR_mpx			(RISCV__NR_Linux +  56)
#define RISCV__NR_setpgid			(RISCV__NR_Linux +  57)
#define RISCV__NR_ulimit			(RISCV__NR_Linux +  58)
#define RISCV__NR_unused59			(RISCV__NR_Linux +  59)
#define RISCV__NR_umask			(RISCV__NR_Linux +  60)
#define RISCV__NR_chroot			(RISCV__NR_Linux +  61)
#define RISCV__NR_ustat			(RISCV__NR_Linux +  62)
#define RISCV__NR_dup2			(RISCV__NR_Linux +  63)
#define RISCV__NR_getppid			(RISCV__NR_Linux +  64)
#define RISCV__NR_getpgrp			(RISCV__NR_Linux +  65)
#define RISCV__NR_setsid			(RISCV__NR_Linux +  66)
#define RISCV__NR_sigaction			(RISCV__NR_Linux +  67)
#define RISCV__NR_sgetmask			(RISCV__NR_Linux +  68)
#define RISCV__NR_ssetmask			(RISCV__NR_Linux +  69)
#define RISCV__NR_setreuid			(RISCV__NR_Linux +  70)
#define RISCV__NR_setregid			(RISCV__NR_Linux +  71)
#define RISCV__NR_sigsuspend			(RISCV__NR_Linux +  72)
#define RISCV__NR_sigpending			(RISCV__NR_Linux +  73)
#define RISCV__NR_sethostname		(RISCV__NR_Linux +  74)
#define RISCV__NR_setrlimit			(RISCV__NR_Linux +  75)
#define RISCV__NR_getrlimit			(RISCV__NR_Linux +  76)
#define RISCV__NR_getrusage			(RISCV__NR_Linux +  77)
#define RISCV__NR_gettimeofday		(RISCV__NR_Linux +  78)
#define RISCV__NR_settimeofday		(RISCV__NR_Linux +  79)
#define RISCV__NR_getgroups			(RISCV__NR_Linux +  80)
#define RISCV__NR_setgroups			(RISCV__NR_Linux +  81)
#define RISCV__NR_reserved82			(RISCV__NR_Linux +  82)
#define RISCV__NR_symlink			(RISCV__NR_Linux +  83)
#define RISCV__NR_unused84			(RISCV__NR_Linux +  84)
#define RISCV__NR_readlink			(RISCV__NR_Linux +  85)
#define RISCV__NR_uselib			(RISCV__NR_Linux +  86)
#define RISCV__NR_swapon			(RISCV__NR_Linux +  87)
#define RISCV__NR_reboot			(RISCV__NR_Linux +  88)
#define RISCV__NR_readdir			(RISCV__NR_Linux +  89)
#define RISCV__NR_mmap			(RISCV__NR_Linux +  90)
#define RISCV__NR_munmap			(RISCV__NR_Linux +  91)
#define RISCV__NR_truncate			(RISCV__NR_Linux +  92)
#define RISCV__NR_ftruncate			(RISCV__NR_Linux +  93)
#define RISCV__NR_fchmod			(RISCV__NR_Linux +  94)
#define RISCV__NR_fchown			(RISCV__NR_Linux +  95)
#define RISCV__NR_getpriority		(RISCV__NR_Linux +  96)
#define RISCV__NR_setpriority		(RISCV__NR_Linux +  97)
#define RISCV__NR_profil			(RISCV__NR_Linux +  98)
#define RISCV__NR_statfs			(RISCV__NR_Linux +  99)
#define RISCV__NR_fstatfs			(RISCV__NR_Linux + 100)
#define RISCV__NR_ioperm			(RISCV__NR_Linux + 101)
#define RISCV__NR_socketcall			(RISCV__NR_Linux + 102)
#define RISCV__NR_syslog			(RISCV__NR_Linux + 103)
#define RISCV__NR_setitimer			(RISCV__NR_Linux + 104)
#define RISCV__NR_getitimer			(RISCV__NR_Linux + 105)
#define RISCV__NR_stat			(RISCV__NR_Linux + 106)
#define RISCV__NR_lstat			(RISCV__NR_Linux + 107)
#define RISCV__NR_fstat			(RISCV__NR_Linux + 108)
#define RISCV__NR_unused109			(RISCV__NR_Linux + 109)
#define RISCV__NR_iopl			(RISCV__NR_Linux + 110)
#define RISCV__NR_vhangup			(RISCV__NR_Linux + 111)
#define RISCV__NR_idle			(RISCV__NR_Linux + 112)
#define RISCV__NR_vm86			(RISCV__NR_Linux + 113)
#define RISCV__NR_wait4			(RISCV__NR_Linux + 114)
#define RISCV__NR_swapoff			(RISCV__NR_Linux + 115)
#define RISCV__NR_sysinfo			(RISCV__NR_Linux + 116)
#define RISCV__NR_ipc			(RISCV__NR_Linux + 117)
#define RISCV__NR_fsync			(RISCV__NR_Linux + 118)
#define RISCV__NR_sigreturn			(RISCV__NR_Linux + 119)
#define RISCV__NR_clone			(RISCV__NR_Linux + 120)
#define RISCV__NR_setdomainname		(RISCV__NR_Linux + 121)
#define RISCV__NR_uname			(RISCV__NR_Linux + 122)
#define RISCV__NR_modify_ldt			(RISCV__NR_Linux + 123)
#define RISCV__NR_adjtimex			(RISCV__NR_Linux + 124)
#define RISCV__NR_mprotect			(RISCV__NR_Linux + 125)
#define RISCV__NR_sigprocmask		(RISCV__NR_Linux + 126)
#define RISCV__NR_create_module		(RISCV__NR_Linux + 127)
#define RISCV__NR_init_module		(RISCV__NR_Linux + 128)
#define RISCV__NR_delete_module		(RISCV__NR_Linux + 129)
#define RISCV__NR_get_kernel_syms		(RISCV__NR_Linux + 130)
#define RISCV__NR_quotactl			(RISCV__NR_Linux + 131)
#define RISCV__NR_getpgid			(RISCV__NR_Linux + 132)
#define RISCV__NR_fchdir			(RISCV__NR_Linux + 133)
#define RISCV__NR_bdflush			(RISCV__NR_Linux + 134)
#define RISCV__NR_sysfs			(RISCV__NR_Linux + 135)
#define RISCV__NR_personality		(RISCV__NR_Linux + 136)
#define RISCV__NR_afs_syscall		(RISCV__NR_Linux + 137) /* Syscall for Andrew File System */
#define RISCV__NR_setfsuid			(RISCV__NR_Linux + 138)
#define RISCV__NR_setfsgid			(RISCV__NR_Linux + 139)
#define RISCV__NR__llseek			(RISCV__NR_Linux + 140)
#define RISCV__NR_getdents			(RISCV__NR_Linux + 141)
#define RISCV__NR__newselect			(RISCV__NR_Linux + 142)
#define RISCV__NR_flock			(RISCV__NR_Linux + 143)
#define RISCV__NR_msync			(RISCV__NR_Linux + 144)
#define RISCV__NR_readv			(RISCV__NR_Linux + 145)
#define RISCV__NR_writev			(RISCV__NR_Linux + 146)
#define RISCV__NR_cacheflush			(RISCV__NR_Linux + 147)
#define RISCV__NR_cachectl			(RISCV__NR_Linux + 148)
#define RISCV__NR_sysriscv			(RISCV__NR_Linux + 149)
#define RISCV__NR_unused150			(RISCV__NR_Linux + 150)
#define RISCV__NR_getsid			(RISCV__NR_Linux + 151)
#define RISCV__NR_fdatasync			(RISCV__NR_Linux + 152)
#define RISCV__NR__sysctl			(RISCV__NR_Linux + 153)
#define RISCV__NR_mlock			(RISCV__NR_Linux + 154)
#define RISCV__NR_munlock			(RISCV__NR_Linux + 155)
#define RISCV__NR_mlockall			(RISCV__NR_Linux + 156)
#define RISCV__NR_munlockall			(RISCV__NR_Linux + 157)
#define RISCV__NR_sched_setparam		(RISCV__NR_Linux + 158)
#define RISCV__NR_sched_getparam		(RISCV__NR_Linux + 159)
#define RISCV__NR_sched_setscheduler		(RISCV__NR_Linux + 160)
#define RISCV__NR_sched_getscheduler		(RISCV__NR_Linux + 161)
#define RISCV__NR_sched_yield		(RISCV__NR_Linux + 162)
#define RISCV__NR_sched_get_priority_max	(RISCV__NR_Linux + 163)
#define RISCV__NR_sched_get_priority_min	(RISCV__NR_Linux + 164)
#define RISCV__NR_sched_rr_get_interval	(RISCV__NR_Linux + 165)
#define RISCV__NR_nanosleep			(RISCV__NR_Linux + 166)
#define RISCV__NR_mremap			(RISCV__NR_Linux + 167)
#define RISCV__NR_accept			(RISCV__NR_Linux + 168)
#define RISCV__NR_bind			(RISCV__NR_Linux + 169)
#define RISCV__NR_connect			(RISCV__NR_Linux + 170)
#define RISCV__NR_getpeername		(RISCV__NR_Linux + 171)
#define RISCV__NR_getsockname		(RISCV__NR_Linux + 172)
#define RISCV__NR_getsockopt			(RISCV__NR_Linux + 173)
#define RISCV__NR_listen			(RISCV__NR_Linux + 174)
#define RISCV__NR_recv			(RISCV__NR_Linux + 175)
#define RISCV__NR_recvfrom			(RISCV__NR_Linux + 176)
#define RISCV__NR_recvmsg			(RISCV__NR_Linux + 177)
#define RISCV__NR_send			(RISCV__NR_Linux + 178)
#define RISCV__NR_sendmsg			(RISCV__NR_Linux + 179)
#define RISCV__NR_sendto			(RISCV__NR_Linux + 180)
#define RISCV__NR_setsockopt			(RISCV__NR_Linux + 181)
#define RISCV__NR_shutdown			(RISCV__NR_Linux + 182)
#define RISCV__NR_socket			(RISCV__NR_Linux + 183)
#define RISCV__NR_socketpair			(RISCV__NR_Linux + 184)
#define RISCV__NR_setresuid			(RISCV__NR_Linux + 185)
#define RISCV__NR_getresuid			(RISCV__NR_Linux + 186)
#define RISCV__NR_query_module		(RISCV__NR_Linux + 187)
#define RISCV__NR_poll			(RISCV__NR_Linux + 188)
#define RISCV__NR_nfsservctl			(RISCV__NR_Linux + 189)
#define RISCV__NR_setresgid			(RISCV__NR_Linux + 190)
#define RISCV__NR_getresgid			(RISCV__NR_Linux + 191)
#define RISCV__NR_prctl			(RISCV__NR_Linux + 192)
#define RISCV__NR_rt_sigreturn		(RISCV__NR_Linux + 193)
#define RISCV__NR_rt_sigaction		(RISCV__NR_Linux + 194)
#define RISCV__NR_rt_sigprocmask		(RISCV__NR_Linux + 195)
#define RISCV__NR_rt_sigpending		(RISCV__NR_Linux + 196)
#define RISCV__NR_rt_sigtimedwait		(RISCV__NR_Linux + 197)
#define RISCV__NR_rt_sigqueueinfo		(RISCV__NR_Linux + 198)
#define RISCV__NR_rt_sigsuspend		(RISCV__NR_Linux + 199)
#define RISCV__NR_pread64			(RISCV__NR_Linux + 200)
#define RISCV__NR_pwrite64			(RISCV__NR_Linux + 201)
#define RISCV__NR_chown			(RISCV__NR_Linux + 202)
#define RISCV__NR_getcwd			(RISCV__NR_Linux + 203)
#define RISCV__NR_capget			(RISCV__NR_Linux + 204)
#define RISCV__NR_capset			(RISCV__NR_Linux + 205)
#define RISCV__NR_sigaltstack		(RISCV__NR_Linux + 206)
#define RISCV__NR_sendfile			(RISCV__NR_Linux + 207)
#define RISCV__NR_getpmsg			(RISCV__NR_Linux + 208)
#define RISCV__NR_putpmsg			(RISCV__NR_Linux + 209)
#define RISCV__NR_mmap2			(RISCV__NR_Linux + 210)
#define RISCV__NR_truncate64			(RISCV__NR_Linux + 211)
#define RISCV__NR_ftruncate64		(RISCV__NR_Linux + 212)
#define RISCV__NR_stat64			(RISCV__NR_Linux + 213)
#define RISCV__NR_lstat64			(RISCV__NR_Linux + 214)
#define RISCV__NR_fstat64			(RISCV__NR_Linux + 215)
#define RISCV__NR_pivot_root			(RISCV__NR_Linux + 216)
#define RISCV__NR_mincore			(RISCV__NR_Linux + 217)
#define RISCV__NR_madvise			(RISCV__NR_Linux + 218)
#define RISCV__NR_getdents64			(RISCV__NR_Linux + 219)
#define RISCV__NR_fcntl64			(RISCV__NR_Linux + 220)
#define RISCV__NR_reserved221		(RISCV__NR_Linux + 221)
#define RISCV__NR_gettid			(RISCV__NR_Linux + 222)
#define RISCV__NR_readahead			(RISCV__NR_Linux + 223)
#define RISCV__NR_setxattr			(RISCV__NR_Linux + 224)
#define RISCV__NR_lsetxattr			(RISCV__NR_Linux + 225)
#define RISCV__NR_fsetxattr			(RISCV__NR_Linux + 226)
#define RISCV__NR_getxattr			(RISCV__NR_Linux + 227)
#define RISCV__NR_lgetxattr			(RISCV__NR_Linux + 228)
#define RISCV__NR_fgetxattr			(RISCV__NR_Linux + 229)
#define RISCV__NR_listxattr			(RISCV__NR_Linux + 230)
#define RISCV__NR_llistxattr			(RISCV__NR_Linux + 231)
#define RISCV__NR_flistxattr			(RISCV__NR_Linux + 232)
#define RISCV__NR_removexattr		(RISCV__NR_Linux + 233)
#define RISCV__NR_lremovexattr		(RISCV__NR_Linux + 234)
#define RISCV__NR_fremovexattr		(RISCV__NR_Linux + 235)
#define RISCV__NR_tkill			(RISCV__NR_Linux + 236)
#define RISCV__NR_sendfile64			(RISCV__NR_Linux + 237)
#define RISCV__NR_futex			(RISCV__NR_Linux + 238)
#define RISCV__NR_sched_setaffinity		(RISCV__NR_Linux + 239)
#define RISCV__NR_sched_getaffinity		(RISCV__NR_Linux + 240)
#define RISCV__NR_io_setup			(RISCV__NR_Linux + 241)
#define RISCV__NR_io_destroy			(RISCV__NR_Linux + 242)
#define RISCV__NR_io_getevents		(RISCV__NR_Linux + 243)
#define RISCV__NR_io_submit			(RISCV__NR_Linux + 244)
#define RISCV__NR_io_cancel			(RISCV__NR_Linux + 245)
#define RISCV__NR_exit_group			(RISCV__NR_Linux + 246)
#define RISCV__NR_lookup_dcookie		(RISCV__NR_Linux + 247)
#define RISCV__NR_epoll_create		(RISCV__NR_Linux + 248)
#define RISCV__NR_epoll_ctl			(RISCV__NR_Linux + 249)
#define RISCV__NR_epoll_wait			(RISCV__NR_Linux + 250)
#define RISCV__NR_remap_file_pages		(RISCV__NR_Linux + 251)
#define RISCV__NR_set_tid_address		(RISCV__NR_Linux + 252)
#define RISCV__NR_restart_syscall		(RISCV__NR_Linux + 253)
#define RISCV__NR_fadvise64			(RISCV__NR_Linux + 254)
#define RISCV__NR_statfs64			(RISCV__NR_Linux + 255)
#define RISCV__NR_fstatfs64			(RISCV__NR_Linux + 256)
#define RISCV__NR_timer_create		(RISCV__NR_Linux + 257)
#define RISCV__NR_timer_settime		(RISCV__NR_Linux + 258)
#define RISCV__NR_timer_gettime		(RISCV__NR_Linux + 259)
#define RISCV__NR_timer_getoverrun		(RISCV__NR_Linux + 260)
#define RISCV__NR_timer_delete		(RISCV__NR_Linux + 261)
#define RISCV__NR_clock_settime		(RISCV__NR_Linux + 262)
#define RISCV__NR_clock_gettime		(RISCV__NR_Linux + 263)
#define RISCV__NR_clock_getres		(RISCV__NR_Linux + 264)
#define RISCV__NR_clock_nanosleep		(RISCV__NR_Linux + 265)
#define RISCV__NR_tgkill			(RISCV__NR_Linux + 266)
#define RISCV__NR_utimes			(RISCV__NR_Linux + 267)
#define RISCV__NR_mbind			(RISCV__NR_Linux + 268)
#define RISCV__NR_get_mempolicy		(RISCV__NR_Linux + 269)
#define RISCV__NR_set_mempolicy		(RISCV__NR_Linux + 270)
#define RISCV__NR_mq_open			(RISCV__NR_Linux + 271)
#define RISCV__NR_mq_unlink			(RISCV__NR_Linux + 272)
#define RISCV__NR_mq_timedsend		(RISCV__NR_Linux + 273)
#define RISCV__NR_mq_timedreceive		(RISCV__NR_Linux + 274)
#define RISCV__NR_mq_notify			(RISCV__NR_Linux + 275)
#define RISCV__NR_mq_getsetattr		(RISCV__NR_Linux + 276)
#define RISCV__NR_vserver			(RISCV__NR_Linux + 277)
#define RISCV__NR_waitid			(RISCV__NR_Linux + 278)
/* #define RISCV__NR_sys_setaltroot		(RISCV__NR_Linux + 279) */
#define RISCV__NR_add_key			(RISCV__NR_Linux + 280)
#define RISCV__NR_request_key		(RISCV__NR_Linux + 281)
#define RISCV__NR_keyctl			(RISCV__NR_Linux + 282)
#define RISCV__NR_set_thread_area		(RISCV__NR_Linux + 283)
#define RISCV__NR_inotify_init		(RISCV__NR_Linux + 284)
#define RISCV__NR_inotify_add_watch		(RISCV__NR_Linux + 285)
#define RISCV__NR_inotify_rm_watch		(RISCV__NR_Linux + 286)
#define RISCV__NR_migrate_pages		(RISCV__NR_Linux + 287)
#define RISCV__NR_openat			(RISCV__NR_Linux + 288)
#define RISCV__NR_mkdirat			(RISCV__NR_Linux + 289)
#define RISCV__NR_mknodat			(RISCV__NR_Linux + 290)
#define RISCV__NR_fchownat			(RISCV__NR_Linux + 291)
#define RISCV__NR_futimesat			(RISCV__NR_Linux + 292)
#define RISCV__NR_fstatat64			(RISCV__NR_Linux + 293)
#define RISCV__NR_unlinkat			(RISCV__NR_Linux + 294)
#define RISCV__NR_renameat			(RISCV__NR_Linux + 295)
#define RISCV__NR_linkat			(RISCV__NR_Linux + 296)
#define RISCV__NR_symlinkat			(RISCV__NR_Linux + 297)
#define RISCV__NR_readlinkat			(RISCV__NR_Linux + 298)
#define RISCV__NR_fchmodat			(RISCV__NR_Linux + 299)
#define RISCV__NR_faccessat			(RISCV__NR_Linux + 300)
#define RISCV__NR_pselect6			(RISCV__NR_Linux + 301)
#define RISCV__NR_ppoll			(RISCV__NR_Linux + 302)
#define RISCV__NR_unshare			(RISCV__NR_Linux + 303)
#define RISCV__NR_splice			(RISCV__NR_Linux + 304)
#define RISCV__NR_sync_file_range		(RISCV__NR_Linux + 305)
#define RISCV__NR_tee			(RISCV__NR_Linux + 306)
#define RISCV__NR_vmsplice			(RISCV__NR_Linux + 307)
#define RISCV__NR_move_pages			(RISCV__NR_Linux + 308)
#define RISCV__NR_set_robust_list		(RISCV__NR_Linux + 309)
#define RISCV__NR_get_robust_list		(RISCV__NR_Linux + 310)
#define RISCV__NR_kexec_load			(RISCV__NR_Linux + 311)
#define RISCV__NR_getcpu			(RISCV__NR_Linux + 312)
#define RISCV__NR_epoll_pwait		(RISCV__NR_Linux + 313)
#define RISCV__NR_ioprio_set			(RISCV__NR_Linux + 314)
#define RISCV__NR_ioprio_get			(RISCV__NR_Linux + 315)
#define RISCV__NR_utimensat			(RISCV__NR_Linux + 316)
#define RISCV__NR_signalfd			(RISCV__NR_Linux + 317)
#define RISCV__NR_timerfd			(RISCV__NR_Linux + 318)
#define RISCV__NR_eventfd			(RISCV__NR_Linux + 319)
#define RISCV__NR_fallocate			(RISCV__NR_Linux + 320)
#define RISCV__NR_timerfd_create		(RISCV__NR_Linux + 321)
#define RISCV__NR_timerfd_gettime		(RISCV__NR_Linux + 322)
#define RISCV__NR_timerfd_settime		(RISCV__NR_Linux + 323)
#define RISCV__NR_signalfd4			(RISCV__NR_Linux + 324)
#define RISCV__NR_eventfd2			(RISCV__NR_Linux + 325)
#define RISCV__NR_epoll_create1		(RISCV__NR_Linux + 326)
#define RISCV__NR_dup3			(RISCV__NR_Linux + 327)
#define RISCV__NR_pipe2			(RISCV__NR_Linux + 328)
#define RISCV__NR_inotify_init1		(RISCV__NR_Linux + 329)
#define RISCV__NR_preadv			(RISCV__NR_Linux + 330)
#define RISCV__NR_pwritev			(RISCV__NR_Linux + 331)
#define RISCV__NR_rt_tgsigqueueinfo		(RISCV__NR_Linux + 332)
#define RISCV__NR_perf_event_open		(RISCV__NR_Linux + 333)
#define RISCV__NR_accept4			(RISCV__NR_Linux + 334)
#define RISCV__NR_recvmmsg			(RISCV__NR_Linux + 335)
#define RISCV__NR_fanotify_init		(RISCV__NR_Linux + 336)
#define RISCV__NR_fanotify_mark		(RISCV__NR_Linux + 337)
#define RISCV__NR_prlimit64			(RISCV__NR_Linux + 338)
#define RISCV__NR_name_to_handle_at		(RISCV__NR_Linux + 339)
#define RISCV__NR_open_by_handle_at		(RISCV__NR_Linux + 340)
#define RISCV__NR_clock_adjtime		(RISCV__NR_Linux + 341)
#define RISCV__NR_syncfs			(RISCV__NR_Linux + 342)
#define RISCV__NR_sendmmsg			(RISCV__NR_Linux + 343)
#define RISCV__NR_setns			(RISCV__NR_Linux + 344)
#define RISCV__NR_process_vm_readv		(RISCV__NR_Linux + 345)
#define RISCV__NR_process_vm_writev		(RISCV__NR_Linux + 346)
#define RISCV__NR_kcmp			(RISCV__NR_Linux + 347)
#define RISCV__NR_finit_module		(RISCV__NR_Linux + 348)

/*
 * Offset of the last Linux o32 flavoured syscall
 */
#define RISCV__NR_Linux_syscalls		348


int *riscv_syscall::get_syscall_table() {
  static int syscall_table[] = {
    RISCV__NR_restart_syscall,
    RISCV__NR_exit,
    RISCV__NR_fork,
    RISCV__NR_read,
    RISCV__NR_write,
    RISCV__NR_open,
    RISCV__NR_close,
    RISCV__NR_creat,
    RISCV__NR_time,
    RISCV__NR_lseek,
    RISCV__NR_getpid,
    RISCV__NR_access,
    RISCV__NR_kill,
    RISCV__NR_dup,
    RISCV__NR_times,
    RISCV__NR_brk,
    RISCV__NR_mmap,
    RISCV__NR_munmap,
    RISCV__NR_stat,
    RISCV__NR_lstat,
    RISCV__NR_fstat,
    RISCV__NR_uname,
    RISCV__NR__llseek,
    RISCV__NR_readv,
    RISCV__NR_writev,
    RISCV__NR_mmap2,
    RISCV__NR_stat64,
    RISCV__NR_lstat64,
    RISCV__NR_fstat64,
    RISCV__NR_getuid,
    RISCV__NR_getgid,
    RISCV__NR_geteuid,
    RISCV__NR_getegid,
    RISCV__NR_fcntl64,
    RISCV__NR_exit_group,
    RISCV__NR_socketcall,
    RISCV__NR_gettimeofday,
    RISCV__NR_settimeofday,
    RISCV__NR_clock_gettime
  };
  return syscall_table;
}
