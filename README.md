# ARMFingerprint
Code to extracts stable ARM CPU register values directly from system hardware for fingerprinting device.

## Features

* Reads essential ARMv8-A system registers:
  * `MIDR_EL1` (Main ID Register): Provides implementer, part number, and revision.
  * `MPIDR_EL1` (Multiprocessor Affinity Register): Shows core and cluster IDs.
  * `REVIDR_EL1` (Revision ID Register): Displays processor revision ID.
  * `ID_AA64PFR0_EL1` (AArch64 Processor Feature Register 0): Indicates support for Floating Point (FP) and Advanced SIMD (ASIMD).
  * `ID_AA64ISAR0_EL1` (AArch64 Instruction Set Attribute Register 0): Details support for cryptographic extensions like AES and SHA1.
* Formats the raw register values into human-readable strings.
* Integrates seamlessly into an Android Java application via JNI.
* **Enables device fingerprinting using register values, as these values generally remain consistent even after reboot or factory reset.**

## Important Notes

* **This code accesses system registers that are typically read-only from user space. The values obtained are inherent to the CPU hardware and cannot be modified by software.**
* The `asm volatile` instructions are specific to ARMv8-A (AArch64) architecture. This code will only function correctly on 64-bit ARM devices.
