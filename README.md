# Entropy-Harvester

# Hardware & Kernel Entropy Harvester

Built a dual-engine C/C++ entropy harvester capturing bare-metal CPU anomalies (thermal noise, voltage ripple, clock jitter) and Linux OS context-switch delays. Piped these raw hardware and software chaos streams through SHA hashing to derive secure 256-bit encryption keys.

## The Architecture

* **`entropy_engine_threads.c`**: Utilizes POSIX threads (`pthreads`) to force a hardware race condition, extracting entropy from physical CPU anomalies.
* **`entropy_engine_kernel.cpp`**: Utilizes `__rdtscp()` and `usleep(0)` to force rapid Linux kernel context switches, extracting entropy from OS scheduling jitter.

---

# Step-by-Step Execution Pipeline

## 1. Prerequisites

This engine is built for Linux environments and requires standard C/C++ compilers and the OpenSSL toolkit.

### Requirements

* `gcc`
* `g++`
* `openssl`

---

## 2. Compile the Engines

Compile the hardware-thread engine (C) and the kernel-interrupt engine (C++) into executable binaries:

```bash
gcc entropy_engine_threads.c -o harvester_hw -pthread
g++ entropy_engine_kernel.cpp -o harvester_os
```

---

## 3. Harvest the Chaos

Run either engine. It will execute for exactly **5 seconds**, capturing real-time environmental noise and streaming the raw binary directly into a vault file.

```bash
./harvester_hw

# This creates a file named 'raw_chaos.key'
# filled with trapped hardware entropy.
```

---

## 4. Derive the Cryptographic Key

The raw binary stream is mathematically compressed into a perfect **256-bit (64-character) symmetric key** using SHA-256.

```bash
sha256sum raw_chaos.key
```

---

## 5. Create a Classified Payload

Create a standard text file containing a secret message to test the encryption.

```bash
echo "This is a highly classified hardware payload." > secret_message.txt
```

---

## 6. Execute AES-256 Encryption

Pipe the 256-bit hardware key directly into OpenSSL to lock the text file.

```bash
openssl enc -aes-256-cbc -pbkdf2 \
-in secret_message.txt \
-out locked.enc \
-pass pass:$(cat raw_chaos.key)
```

The file **`locked.enc`** is now fully encrypted and unreadable.

---

## 7. Decrypt and Verify

Reverse the pipeline using the same symmetric key to unlock the file and reveal the original message.

```bash
openssl enc -d -aes-256-cbc -pbkdf2 \
-in locked.enc \
-out unlocked.txt \
-pass pass:$(cat raw_chaos.key)

cat unlocked.txt
```

Expected output:

```text
This is a highly classified hardware payload.
```

---

# Project Flow

```text
CPU Race Conditions
        +
Kernel Scheduling Jitter
        |
        v
 Raw Entropy Stream
        |
        v
     SHA-256
        |
        v
  256-bit AES Key
        |
        v
 Encrypted Payload
```

---

# Components

| File | Purpose |
|---|---|
| `entropy_engine_threads.c` | Captures CPU hardware-level timing anomalies using pthread races |
| `entropy_engine_kernel.cpp` | Captures Linux scheduling jitter using timestamp counters |
| `raw_chaos.key` | Generated entropy vault |
| `secret_message.txt` | Test plaintext payload |
| `locked.enc` | AES-256 encrypted output |
| `unlocked.txt` | Verified decrypted payload |

---

# Notes

* Built specifically for Linux systems.
* Uses CPU timing instability and kernel scheduling behavior as entropy sources.
* Final cryptographic material is derived through SHA-256 compression.
* AES-256-CBC encryption is handled through OpenSSL.
