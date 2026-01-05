# Phase 2: glibc Mach/Hurd Sysdeps 分析

## 关键发现

### Hurd 已有 Rump 集成！
在 `/hurd/rumpdisk/` 和 `/hurd/rumpnet/` 中发现：
- Hurd 官方已经使用 NetBSD Rump Kernel 作为驱动层
- 这验证了我们的 Phase 4 方向是正确的

### rumpdisk 实现分析 (612 行代码)

**关键 API 使用：**
```c
#include <rump/rump.h>
#include <rump/rump_syscalls.h>
#include <rump/rumperrno2host.h>

rump_init();                    // 初始化 Rump 内核
rump_sys_open("/dev/rwd0d", ...); // 打开块设备
rump_sys_ioctl(fd, DIOCGMEDIASIZE, &size); // ioctl
rump_sys_pread/pwrite(fd, buf, len, off);  // 读写
rump_errno2host(errno);         // 错误码转换
```

**链接库：**
```
-lrump -lrumpuser -lrumpdev -lrumpdev_disk -lrumpvfs
-lrumpdev_pci -lrumpdev_ahcisata -lrumpdev_ata
```

**Hurd 库依赖：**
```
-lmachdev -lports -ltrivfs -lshouldbeinclibc -liohelp -lihash -lfshelp -lirqhelp
```

## glibc sysdeps/mach/hurd 需要适配的文件

| 目录/文件 | 功能 | XNU 兼容性 |
|-----------|------|------------|
| `mach/mig-reply.c` | MIG 回复端口管理 | ✅ 兼容 |
| `hurd/hurd.h` | Hurd 核心类型 | ⚠️ 需要 shim |
| `hurd/hurdsig.c` | Hurd 信号处理 | ❌ 需要重写 |
| `hurd/port-io.c` | 端口 I/O 封装 | ⚠️ 需要适配 |
| `hurd/hurdmsg.c` | Hurd 消息传递 | ✅ 可复用 |

## 适配策略

1. **已完成:** 
   - XNU compat headers (mach.h, hurd.h, refcount.h)
   - MIG interface generation (io, fs, fsys, notify, interrupt)
   - libports 编译 (libports_xnu.a)

2. **Rump 方向已验证:**
   - Hurd 官方 rumpdisk 证明 Rump 可行
   - 我们的 rumpuser_xnu.c 是正确路径
   - 需要继续完善 rumpuser 实现

3. **后续工作:**
   - 完善 rumpuser_xnu.c (bio, iovec, sync)
   - 尝试在 XNU 上编译 hurd/rumpdisk
   - 分析 glibc hurd 信号处理
