# Hurd on XNU

> **Running GNU Hurd userspace on Apple's XNU kernel**

[![Status](https://img.shields.io/badge/status-alpha-orange)](https://github.com/J-x-Z/hurd-xnu)
[![License](https://img.shields.io/badge/license-GPL--2.0-blue)](LICENSE)

## Overview

This project aims to port GNU Hurd's microkernel-based userspace to run on Apple's XNU kernel, combining:
- **XNU**: Apple's hybrid kernel (Mach + BSD) with modern hardware support
- **Hurd**: GNU's microkernel OS with translator-based architecture

## Current Status (v0.1-alpha)

| Component | Status | Description |
|-----------|--------|-------------|
| Mach IPC | ✅ Verified | `mach_msg`, port sets, memory objects work on XNU |
| libports | ✅ Compiled | Core Hurd port management library (27KB) |
| MIG Interfaces | ✅ Generated | io, fs, fsys, notify, interrupt stubs |
| Hello Translator | ✅ Working | Hurd-style server with `io_read`/`io_readable` |
| Rump Driver Layer | ✅ Ready | `rumpuser_xnu.c` (~480 lines) |
| LKL Support | ✅ Compiled | Linux Kernel Library for modern drivers |
| QEMU x86 Boot | ✅ POC | XNU kernel initialization verified |

## Quick Start

### Prerequisites
- macOS (tested on Sequoia 15.x)
- Xcode Command Line Tools
- QEMU (for boot testing)

### Build Hello Translator
```bash
cd /path/to/hurd-xnu
gcc -I./compat -I./compat/include \
    -o hello_translator hello_translator.c \
    -L./compat -lports_xnu -lpthread
./hello_translator &
# In another terminal:
cat /dev/hello  # (requires translator mount)
```

### Test QEMU Boot
```bash
cd qemu/OSX-KVM
./boot_xnu_tcg.sh
# Opens QEMU window with XNU boot (slow on ARM64 Macs)
```

## Architecture

```
┌─────────────────────────────────────────┐
│           Hurd Translators              │
│  (auth, proc, exec, ext2fs, pfinet)     │
├─────────────────────────────────────────┤
│         libports / libtrivfs            │
│              (ported)                   │
├─────────────────────────────────────────┤
│        Mach IPC (mach_msg)              │
│      ┌─────────┴─────────┐              │
│      │                   │              │
│   GNU Mach            XNU Mach          │
│   (original)          (this port)       │
├─────────────────────────────────────────┤
│              XNU Kernel                 │
│  (Mach microkernel + BSD + IOKit)       │
└─────────────────────────────────────────┘
```

## Directory Structure

```
hurd-xnu/
├── compat/              # XNU compatibility layer
│   ├── mach.h           # Mach type definitions
│   ├── ports.h          # libports interface
│   ├── rumpuser_xnu.c   # Rump kernel hypercall impl
│   └── hurd/            # MIG .defs files for XNU
├── hello_translator.c   # Working Hurd server demo
├── hurd/                # Original Hurd source (submodule)
├── xnu/                 # XNU kernel source (submodule)
├── qemu/                # QEMU boot infrastructure
│   └── OSX-KVM/         # OpenCore + OVMF + boot scripts
├── tests/               # Mach IPC verification tests
└── docs/                # Architecture documentation
```

## Roadmap

- [x] **Phase 1-3**: Mach IPC, libports, Hello Translator
- [x] **Phase 4**: Driver compatibility (Rump + LKL)
- [x] **Phase 5**: QEMU x86 boot POC
- [ ] **Phase 6**: Port `auth` server
- [ ] **Phase 7**: Port `proc` server  
- [ ] **Phase 8**: Port `exec` server
- [ ] **Phase 9**: Self-hosting Hurd on XNU

## Related Projects

- [GNU Hurd](https://www.gnu.org/software/hurd/)
- [XNU Source](https://github.com/apple-oss-distributions/xnu)
- [PureDarwin](https://www.puredarwin.org/)
- [NetBSD Rump Kernel](https://rumpkernel.org/)

## License

- Hurd components: GPL-2.0
- XNU: APSL-2.0
- This compatibility layer: GPL-2.0

## Contributing

This is an experimental research project. Contributions welcome!

1. Fork this repository
2. Create a feature branch
3. Submit a Pull Request

## Acknowledgments

- GNU Hurd developers
- Apple XNU team (open source kernel)
- OSX-KVM / DarwinKVM communities

## Clone with Submodules

```bash
# Clone with all submodules (hurd + xnu sources)
git clone --recursive https://github.com/J-x-Z/hurd-xnu

# Or if you already cloned:
git submodule update --init --recursive
```
