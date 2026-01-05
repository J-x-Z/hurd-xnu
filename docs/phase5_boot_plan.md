# Phase 5: x86 裸机启动计划

## 当前状态
- ✅ QEMU 10.2.0 已安装
- ✅ OSX-KVM 项目已克隆 (包含 OpenCore 启动脚本)
- 🔄 需要下载: OVMF 固件, OpenCore EFI, macOS BaseSystem

## 启动架构
```
┌─────────────────────────────────────────┐
│           QEMU x86_64 虚拟机             │
│  ┌─────────────────────────────────────┐│
│  │ OVMF (UEFI固件)                      ││
│  │   ↓                                 ││
│  │ OpenCore (Hackintosh bootloader)    ││
│  │   ↓                                 ││
│  │ XNU Kernel (macOS 内核)              ││
│  │   ↓                                 ││
│  │ Hurd Servers (替换 macOS userspace) ││
│  └─────────────────────────────────────┘│
└─────────────────────────────────────────┘
```

## macOS HVF vs Linux KVM
- OSX-KVM 脚本使用 `-enable-kvm` (Linux)
- macOS 需要使用 `-accel hvf` (Hypervisor.framework)
- 但 HVF 不支持嵌套虚拟化，可能需要在 Linux VM 中运行

## 下一步
1. 下载 OVMF 固件
2. 下载 OpenCore EFI
3. 下载 macOS BaseSystem.img (或 recoveryOS)
4. 修改启动脚本适配 macOS 主机
5. 测试 XNU 内核启动
