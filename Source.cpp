#include <jni.h>
#include <string>
#include <sstream>
#include <stdint.h>

uint64_t get_main_id_register() {
    uint64_t register_value;
    asm volatile(
            "mrs %0, MIDR_EL1\n"
            : "=r" (register_value)
            );
    return register_value;
}

uint64_t get_multiprocessor_affinity_register() {
    uint64_t register_value;
    asm volatile(
            "mrs %0, MPIDR_EL1\n"
            : "=r" (register_value)
            );
    return register_value;
}

uint64_t get_revision_id_register() {
    uint64_t register_value;
    asm volatile(
            "mrs %0, REVIDR_EL1\n"
            : "=r" (register_value)
            );
    return register_value;
}

uint64_t get_processor_feature_register_0() {
    uint64_t register_value;
    asm volatile(
            "mrs %0, ID_AA64PFR0_EL1\n"
            : "=r" (register_value)
            );
    return register_value;
}

uint64_t get_instruction_set_attribute_register_0() {
    uint64_t register_value;
    asm volatile(
            "mrs %0, ID_AA64ISAR0_EL1\n"
            : "=r" (register_value)
            );
    return register_value;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_arm_info_MainActivity_stringFromJNI(
        JNIEnv* environment,
        jobject /* java_this_object */) {

    std::stringstream output_stream;

    output_stream << "--- ARM Hardware Information ---\n\n";

    uint64_t midr_value = get_main_id_register();
    output_stream << "MIDR_EL1 (Main ID Register): 0x" << std::hex << midr_value << std::dec << "\n";
    unsigned char implementer_code = (midr_value >> 24) & 0xFF;
    output_stream << "  Implementer Code (0x" << std::hex << (unsigned int)implementer_code << std::dec << "): ";
    switch (implementer_code) {
        case 0x41: output_stream << "ARM Limited\n"; break;
        case 0x42: output_stream << "Broadcom Corporation\n"; break;
        case 0x43: output_stream << "Cavium Inc.\n"; break;
        case 0x44: output_stream << "Digital Equipment Corporation\n"; break;
        case 0x46: output_stream << "Fujitsu Ltd.\n"; break;
        case 0x49: output_stream << "Infineon Technologies AG\n"; break;
        case 0x4D: output_stream << "Motorola or Freescale Semiconductor Inc.\n"; break;
        case 0x4E: output_stream << "NVIDIA Corporation\n"; break;
        case 0x50: output_stream << "Applied Micro Circuits Corporation\n"; break;
        case 0x51: output_stream << "Qualcomm Inc.\n"; break;
        case 0x56: output_stream << "Marvell International Ltd.\n"; break;
        case 0x69: output_stream << "Intel Corporation\n"; break;
        case 0xC0: output_stream << "Ampere Computing\n"; break;
        default: output_stream << "Unknown/Reserved (or not publicly documented)\n"; break;
    }
    unsigned int part_number = (midr_value >> 4) & 0xFFF;
    output_stream << "  Part Number: 0x" << std::hex << part_number << std::dec << "\n";
    unsigned int revision_number = midr_value & 0xF;
    output_stream << "  Revision: 0x" << std::hex << revision_number << std::dec << "\n";
    output_stream << "\n";

    uint64_t mpidr_value = get_multiprocessor_affinity_register();
    output_stream << "MPIDR_EL1 (Multiprocessor Affinity Register): 0x" << std::hex << mpidr_value << std::dec << "\n";
    unsigned int core_identifier = mpidr_value & 0xFF;
    output_stream << "  Core ID (Aff0): " << core_identifier << "\n";
    unsigned int cluster_identifier = (mpidr_value >> 8) & 0xFF;
    output_stream << "  Cluster ID (Aff1): " << cluster_identifier << "\n";
    output_stream << "\n";

    uint64_t revidr_value = get_revision_id_register();
    output_stream << "REVIDR_EL1 (Revision ID Register): 0x" << std::hex << revidr_value << std::dec << "\n";
    output_stream << "\n";

    uint64_t pfr0_value = get_processor_feature_register_0();
    output_stream << "ID_AA64PFR0_EL1 (AArch64 Processor Feature Register 0): 0x" << std::hex << pfr0_value << std::dec << "\n";
    unsigned int fp_support_level = (pfr0_value >> 16) & 0xF;
    output_stream << "  FP (Floating Point) support: ";
    if (fp_support_level == 0x0) {
        output_stream << "Not implemented\n";
    } else if (fp_support_level == 0x1) {
        output_stream << "Implemented\n";
    } else {
        output_stream << "Reserved/Unknown (0x" << std::hex << fp_support_level << std::dec << ")\n";
    }

    unsigned int asimd_support_level = (pfr0_value >> 20) & 0xF;
    output_stream << "  ASIMD (Advanced SIMD) support: ";
    if (asimd_support_level == 0x0) {
        output_stream << "Not implemented\n";
    } else if (asimd_support_level == 0x1) {
        output_stream << "Implemented\n";
    } else {
        output_stream << "Reserved/Unknown (0x" << std::hex << asimd_support_level << std::dec << ")\n";
    }
    output_stream << "\n";

    uint64_t isar0_value = get_instruction_set_attribute_register_0();
    output_stream << "ID_AA64ISAR0_EL1 (AArch64 Instruction Set Attribute Register 0): 0x" << std::hex << isar0_value << std::dec << "\n";
    unsigned int aes_support_level = (isar0_value >> 4) & 0xF;
    output_stream << "  AES support: ";
    if (aes_support_level == 0x0) {
        output_stream << "Not implemented\n";
    } else if (aes_support_level == 0x1) {
        output_stream << "Implemented\n";
    } else if (aes_support_level == 0x2) {
        output_stream << "Implemented with PMULL\n";
    } else {
        output_stream << "Reserved/Unknown (0x" << std::hex << aes_support_level << std::dec << ")\n";
    }

    unsigned int sha1_support_level = (isar0_value >> 8) & 0xF;
    output_stream << "  SHA1 support: ";
    if (sha1_support_level == 0x0) {
        output_stream << "Not implemented\n";
    } else if (sha1_support_level == 0x1) {
        output_stream << "Implemented\n";
    } else {
        output_stream << "Reserved/Unknown (0x" << std::hex << sha1_support_level << std::dec << ")\n";
    }
    output_stream << "\n";

    std::string final_result_string = output_stream.str();

    return environment->NewStringUTF(final_result_string.c_str());
}
