# Linux 驱动移植策略对比分析

## 问题背景
Hurd 只支持老旧硬件，要在现代 x86 机器上裸机运行需要：
- NVMe/AHCI 存储驱动
- 现代网卡 (Intel i225, Realtek 8125)
- 可选：GPU (AMDGPU, i915)

## 方案对比

| 方案 | 原理 | 驱动覆盖 | 复杂度 | 推荐度 |
|------|------|----------|--------|--------|
| **LKL** | Linux 内核编译成库 | 块设备/网络/VFS | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| **LinuxKPI** | FreeBSD 内核层兼容 | GPU/WiFi | ⭐⭐⭐⭐ | ⭐⭐⭐ |
| **Rump Kernel** | NetBSD 驱动用户态化 | 块/网/USB | ⭐⭐ | ⭐⭐⭐⭐ |
| **手动移植** | 逐个驱动适配 | 任意 | ⭐⭐⭐⭐⭐ | ⭐⭐ |
| **NDISwrapper** | Windows NDIS 封装 | WiFi 网卡 | ⭐⭐ | ⭐ (太旧) |

## 推荐方案：LKL + Rump 混合架构

```
┌─────────────────────────────────────────┐
│          Hurd Translators               │
│  ext2fs  │  pfinet  │  rumpdisk         │
└────┬─────┴────┬─────┴────┬──────────────┘
     │          │          │
┌────▼────┐ ┌───▼────┐ ┌───▼──────────────┐
│ Rump FS │ │ LKL    │ │ Rump Block       │ 
│ (NetBSD)│ │ Network│ │ (NVMe via LKL)   │
└────┬────┘ └───┬────┘ └───┬──────────────┘
     │          │          │
     └──────────┴──────────┘
                │
     ┌──────────▼──────────┐
     │  XNU Hardware Shim  │
     │  (Mach IPC for IRQ) │
     └─────────────────────┘
```

### 阶段 1: LKL 块设备 (优先)
```c
// LKL 提供块设备接口
struct lkl_disk disk = {
    .ops = &lkl_dev_blk_ops,
    .handle = fd,  // 底层硬件文件描述符
};
lkl_disk_add(&disk);
```
- 使用 LKL 的 NVMe/AHCI 驱动 (Linux 完整支持)
- 通过 Hurd rumpdisk 模式暴露给 ext2fs

### 阶段 2: 网络驱动
- **选项 A:** LKL 网络栈 (完整 Linux TCP/IP)
- **选项 B:** Rump 网络栈 (NetBSD, Hurd 已有 rumpnet)
- **选项 C:** FreeBSD libuinet (用户态 TCP/IP)

### 阶段 3: GPU (可选)
- 不影响基础系统启动
- 可后续通过 LinuxKPI 方案添加

## 为什么不用纯 LinuxKPI？

1. LinuxKPI 是 **内核级** 兼容层 - 需要在 XNU 内核中实现
2. LKL/Rump 是 **用户态** - 与 Hurd 微内核哲学一致
3. LinuxKPI 主要为 GPU/WiFi 优化，块设备支持不完整

## 具体实施步骤

1. [x] 实现 rumpuser_xnu.c (基础完成)
2. [ ] 将 LKL 编译为 liblkl.a (macOS 交叉编译 x86)
3. [ ] 创建 LKL→Hurd 设备桥接
4. [ ] 在 QEMU 中测试 NVMe 访问
5. [ ] 移植 Hurd rumpdisk 使用 LKL 后端
