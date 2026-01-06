# Hurd on XNU

> **Running GNU Hurd userspace on Apple's XNU kernel**

[![Status](https://img.shields.io/badge/status-v0.3--alpha-green)](https://github.com/J-x-Z/hurd-xnu)
[![Tests](https://img.shields.io/badge/tests-27%2F27%20passed-brightgreen)](https://github.com/J-x-Z/hurd-xnu)
[![License](https://img.shields.io/badge/license-GPL--2.0-blue)](LICENSE)

## Overview

This project ports GNU Hurd's microkernel-based userspace to run on Apple's XNU kernel, combining:
- **XNU**: Apple's hybrid kernel (Mach + BSD) with modern hardware support
- **Hurd**: GNU's microkernel OS with translator-based architecture

## Current Status (v0.3-alpha) 🎉

### Core Servers (5)
| Server | Status | Description |
|--------|--------|-------------|
| auth | ✅ Working | Authentication with root handle |
| proc | ✅ Working | Process management, PID/task conversion |
| exec | ✅ Working | ELF execution (stubbed) |
| pfinet | ✅ Working | Network (BSD socket wrapper) |
| ext2fs | ✅ Working | Filesystem (POSIX wrapper) |

### Translators (4)
| Translator | Status | Description |
|------------|--------|-------------|
| term | ✅ Working | PTY + console (/dev/ttys*) |
| symlink | ✅ Working | Symbolic links |
| devnull | ✅ Working | /dev/null, zero, full, random, urandom |
| tmpfs | ✅ Working | In-memory filesystem (64MB) |

### Bootstrap & Init
| Component | Status | Description |
|-----------|--------|-------------|
| startup | ✅ Working | Essential task registration, shutdown notify |
| init | ✅ Working | Runlevel 0-3, service ordering |
| bootstrap | ✅ Working | Server startup management |

### Driver Layer
| Component | Status | Description |
|-----------|--------|-------------|
| rumpuser | ✅ Working | NetBSD Rump hypercalls for XNU |
| machdev | ✅ Working | Mach device layer compatibility |
| rumpdisk | ✅ Working | Block devices (wd0, wd1) |
| rumpnet | ✅ Working | Network interfaces (eth0, lo0) |
| LKL bridge | ✅ Working | Linux Kernel Library integration |

### Integration Testing
```
═══════════════════════════════════════════════════════════
   TEST SUMMARY
═══════════════════════════════════════════════════════════
   Passed: 27
   Failed: 0
   Total:  27
═══════════════════════════════════════════════════════════
🎉 ALL USERSPACE TESTS PASSED!
```

## Quick Start

### Prerequisites
- macOS (tested on Sequoia 15.x, M4 Mac)
- Xcode Command Line Tools
- QEMU (optional, for boot testing)

### Build & Test
```bash
# Clone
git clone --recursive https://github.com/J-x-Z/hurd-xnu
cd hurd-xnu

# Build all tests
make tests

# Run userspace integration test (27 tests)
./tests/userspace_test

# Run individual translator tests
./compat/term_test
./compat/tmpfs_test
./compat/devnull_test
```

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                  Hurd Userspace Tests                   │
│    (shell, coreutils, network, multi-user, PTY)        │
├─────────────────────────────────────────────────────────┤
│                     Translators                         │
│      term │ symlink │ devnull │ tmpfs                   │
├─────────────────────────────────────────────────────────┤
│                    Core Servers                         │
│    auth │ proc │ exec │ pfinet │ ext2fs │ startup      │
├─────────────────────────────────────────────────────────┤
│               Bootstrap / Init                          │
│         hurd_bootstrap.c │ init_xnu.c                  │
├─────────────────────────────────────────────────────────┤
│                   Driver Layer                          │
│    rumpuser │ machdev │ LKL bridge                     │
├─────────────────────────────────────────────────────────┤
│              Compatibility Layer                        │
│    libports │ MIG stubs │ mach.h │ ports.h             │
├─────────────────────────────────────────────────────────┤
│                    XNU Kernel                           │
│         (Mach microkernel + BSD + IOKit)               │
└─────────────────────────────────────────────────────────┘
```

## Directory Structure

```
hurd-xnu/
├── compat/                 # XNU compatibility layer (~6000 lines)
│   ├── auth_xnu.c          # Auth server
│   ├── proc_xnu.c          # Proc server
│   ├── exec_xnu.c          # Exec server
│   ├── pfinet_xnu.c        # Network server
│   ├── ext2fs_xnu.c        # Filesystem translator
│   ├── startup_xnu.c       # Startup server
│   ├── init_xnu.c          # Init process
│   ├── hurd_bootstrap.c    # Bootstrap manager
│   ├── term_xnu.c          # Terminal translator
│   ├── symlink_xnu.c       # Symlink translator
│   ├── devnull_xnu.c       # /dev/null, zero, random
│   ├── tmpfs_xnu.c         # In-memory filesystem
│   ├── rumpuser_xnu.c      # Rump hypercalls
│   ├── machdev_xnu.c       # Mach device layer
│   ├── lkl_hurd_bridge.c   # LKL integration
│   ├── mach.h              # Mach type definitions
│   ├── ports.h             # libports interface
│   └── hurd/               # MIG .defs files
├── tests/                  # Test programs
│   ├── hurd_userspace_test.c  # Integration test (27 tests)
│   └── rump_integration_test.c
├── hurd/                   # Original Hurd source (submodule)
├── xnu/                    # XNU kernel source (submodule)
└── qemu/                   # QEMU boot infrastructure
```

## Roadmap

- [x] **Phase 1-5**: Environment, Mach IPC, libports, QEMU boot ✅
- [x] **Phase 6**: Core Hurd servers (auth, proc, exec, pfinet, ext2fs) ✅
- [x] **Phase 7**: Rump Kernels (rumpdisk, rumpnet) ✅
- [x] **Phase 8**: Bootstrap + Startup + Init ✅
- [x] **Phase 9**: Translators (term, symlink, devnull, tmpfs) ✅
- [x] **Phase 10**: Userspace integration (27/27 tests) ✅
- [ ] **Phase 11**: Bare metal Hurd on XNU (launchd, Kext, boot image)

## Code Statistics

| Category | Files | Lines |
|----------|-------|-------|
| Core Servers | 6 | ~1,400 |
| Translators | 4 | ~950 |
| Bootstrap/Init | 3 | ~720 |
| Driver Layer | 5 | ~1,600 |
| Compatibility | 10+ | ~1,500 |
| Tests | 5+ | ~800 |
| **Total** | **30+** | **~7,000** |

## Related Projects

- [GNU Hurd](https://www.gnu.org/software/hurd/)
- [XNU Source](https://github.com/apple-oss-distributions/xnu)
- [PureDarwin](https://www.puredarwin.org/)
- [NetBSD Rump Kernel](https://rumpkernel.org/)
- [Linux Kernel Library (LKL)](https://github.com/lkl/linux)

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
- NetBSD Rump and LKL projects
