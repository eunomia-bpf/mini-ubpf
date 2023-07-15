# ubpf-mini

UBPF-Mini is a minimal, yet portable, library intended to support eBPF (Extended Berkeley Packet Filter). The primary goal of this project is simplicity, providing just enough features to get you started with eBPF. It offers support for a variety of platforms including JIT ARM32, x86, ARM64, and more.

This is a modify version of https://github.com/iovisor/ubpf, keep it simple and easy to use.

## Features

* **Simple and Lightweight:** As the name suggests, UBPF-Mini has been designed with simplicity and minimalism in mind. It provides just enough functionality to work with eBPF, without the complexities of larger libraries.
* **Portable:** UBPF-Mini is designed to be portable and flexible, with support for a wide range of platforms including JIT ARM32, x86, ARM64, and others.

## Getting Started

Follow the steps below to get started with UBPF-Mini.

### install all requirements

Tested on Ubuntu 22.04.

1. install pahole (>=v1.22)

    ```bash
    sudo apt-get install dwarves
    pahole --version
    v1.22
    ```

2. install arm32 arm64 build and qemu

    arm32:

    ```bash
    sudo apt-get install -y gcc-arm-linux-gnueabi qemu-user
    ```

arm64:

```bash
sudo apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu
```

### build and run

arm32

```sh
make build-arm32
make run-arm32
```

arm64

```sh
make build-arm64
make run-arm64
```

on the current platform

```sh
make build
build/bin/Debug/libebpf
```

### compile bpf insts  

install pyelftools

```sh
pip3 install pyelftools
```

run

```bash
python3 runtime/tools/compile_code.py -s example/bpf/test1.bpf.c 
```

### test for core jit and vm

Use python3.8 and pytest

```sh
python3.8 -m venv test
source test/bin/activate
sudo apt install python3-pytest
pip install -r test/requirements.txt
make build # or build-arm32 build-arm64
make test
```

- https://github.com/iovisor/ubpf
