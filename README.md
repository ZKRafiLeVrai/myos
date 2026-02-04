MyOS - build via GitHub Actions

This repository contains a 32-bit hobby OS. Building on Windows without installing a native toolchain is possible by using the included GitHub Actions workflow which produces `myos.bin` and `myos.iso` as artifacts.

How to build (no local install required):

1. Push this repository to GitHub (new repo or existing).
2. Open the Actions tab for the repo and run the `Build MyOS (i386)` workflow or push to `main`/`master`.
3. After the workflow finishes, download the `myos-bin` and/or `myos-iso` artifacts from the workflow run.

Quick test with QEMU (locally after downloading artifact):

```powershell
qemu-system-i386 -m 512 -kernel myos.bin
# or if you downloaded the ISO:
qemu-system-i386 -m 512 -cdrom myos.iso
```

Notes:
- The workflow installs `gcc-multilib`, `nasm`, `binutils` and `grub-mkrescue` on Ubuntu to build 32-bit ELF binaries and create the ISO.
- If you prefer a portable toolchain with no network download, tell me and I will add an alternative `build_myos.bat` that downloads a cross-toolchain archive into the repository.
