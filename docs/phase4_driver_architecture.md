# Phase 4: 驱动兼容层架构设计

## 目标
在 Hurd/XNU 混合系统上提供 Linux/BSD 驱动兼容性，实现裸机硬件支持。

## 候选技术方案对比

| 方案 | 来源 | 特点 | 适用场景 |
|------|------|------|----------|
| **Rump Kernel** | NetBSD | 用户态内核组件，anykernel 架构 | 文件系统、网络栈、USB |
| **libuinet** | FreeBSD 9.1 | 用户态 TCP/IP，支持 1M+ 连接 | 高性能网络 |
| **LKL** | Linux | 完整 Linux 内核作为库 | 任意 Linux 驱动 |
| **libzpool** | OpenZFS | 用户态 ZFS 存储池 | 现代文件系统 |

## 推荐架构

```
┌─────────────────────────────────────────────┐
│           Hurd Translators (用户态)           │
│  ┌─────┐  ┌─────┐  ┌──────┐  ┌─────────────┐│
│  │ext2fs│  │pfinet│  │devnode│  │ ZFS Translator ││
│  └──┬──┘  └──┬──┘  └───┬──┘  └──────┬──────┘│
└─────┼───────┼────────┼───────────┼────────┘
      │       │        │           │
┌─────▼───────▼────────▼───────────▼────────┐
│            DDE Layer (用户态驱动框架)          │
│  ┌────────┐  ┌────────┐  ┌─────────────────┐│
│  │ Rump   │  │ LKL    │  │  libzpool (ZFS) ││
│  │ Kernel │  │ Drivers│  │                 ││
│  └───┬────┘  └───┬────┘  └────────┬────────┘│
└──────┼───────────┼─────────────────┼────────┘
       │           │                 │
┌──────▼───────────▼─────────────────▼────────┐
│        Hardware Access Shim (XNU Kext)       │
│    • MMIO 映射                                │
│    • 中断转发 (IRQ → Mach IPC)                │
│    • I/O 端口访问                              │
└─────────────────────────────────────────────┘
                    │
            ┌───────▼───────┐
            │   XNU Kernel   │
            └───────────────┘
```

## 实施优先级

### 阶段 4.1: 网络驱动 (首选 Rump/libuinet)
- [ ] 移植 Rump Kernel 的网络栈到 Hurd/XNU
- [ ] 或者：移植 libuinet 替代 Hurd pfinet
- [ ] 封装为 Hurd 设备 translator

### 阶段 4.2: 存储驱动 (LKL 或 Rump)
- [ ] 移植 LKL 块设备驱动 (NVMe, AHCI)
- [ ] 评估 libzpool 集成

### 阶段 4.3: XNU Kext (硬件访问)
- [ ] 实现 MMIO/端口映射
- [ ] 实现 Mach 中断通知机制

## 技术细节

### Rump Kernel 集成
```c
// rumpuser hypercall 接口 - 需适配 XNU
int rumpuser_malloc(size_t len, int alignment, void **memp);
void *rumpuser_anonmmap(size_t size);
int rumpuser_thread_create(...);
```

### LKL 集成
```c
// LKL 初始化 - 需适配 Mach IPC
lkl_host_ops.mem_alloc = xnu_mem_alloc;
lkl_host_ops.thread_create = pthread_create;
lkl_start_kernel(&lkl_host_ops, "mem=64M");
```

## 下一步行动
1. 克隆 Rump Kernel 源码
2. 分析 rumpuser 接口
3. 创建 XNU 兼容的 rumpuser 实现
