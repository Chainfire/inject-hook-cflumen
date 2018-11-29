/* Excerpts:
   Copyright (c) 2008, The Android Open Source Project

   Modifications:
   Copyright (c) 2015, Simone 'evilsocket' Margaritelli
   Copyright (c) 2015, Jorrit 'Chainfire' Jongma

   See LICENSE file for details */

#ifndef LINKER_H_
#define LINKER_H_

#include <elf.h>
#include <dlfcn.h>

#define ANDROID_ARM_LINKER 1

#define FLAG_LINKED     0x00000001
#define FLAG_EXE        0x00000004
#define FLAG_LINKER     0x00000010
#define FLAG_GNU_HASH   0x00000040
#define FLAG_NEW_SOINFO 0x40000000

#define SOINFO_NAME_LEN 128

struct link_map {
    uintptr_t l_addr;
    char * l_name;
    uintptr_t l_ld;
    struct link_map * l_next;
    struct link_map * l_prev;
};

#define R_ARM_ABS32      	2
#define R_ARM_GLOB_DAT   	21
#define R_ARM_JUMP_SLOT  	22

#define R_AARCH64_ABS64		257
#define R_AARCH64_GLOB_DAT	1025
#define R_AARCH64_JUMP_SLOT	1026

#define R_386_32			1
#define R_386_GLOB_DAT		6
#define R_386_JUMP_SLOT		7

#define R_X86_64_64			1
#define R_X86_64_GLOB_DAT	6
#define R_X86_64_JUMP_SLOT	7

typedef void (*linker_function_t)();

#if __LP64__
#define ElfW(type) Elf64_ ## type
#else
#define ElfW(type) Elf32_ ## type
#endif

#if defined(__LP64__)
#define ELFW(what) ELF64_ ## what
#else
#define ELFW(what) ELF32_ ## what
#endif

// mips64 interprets Elf64_Rel structures' r_info field differently.
// bionic (like other C libraries) has macros that assume regular ELF files,
// but the dynamic linker needs to be able to load mips64 ELF files.
#if defined(__mips__) && defined(__LP64__)
#undef ELF64_R_SYM
#undef ELF64_R_TYPE
#undef ELF64_R_INFO
#define ELF64_R_SYM(info)   (((info) >> 0) & 0xffffffff)
#define ELF64_R_SSYM(info)  (((info) >> 32) & 0xff)
#define ELF64_R_TYPE3(info) (((info) >> 40) & 0xff)
#define ELF64_R_TYPE2(info) (((info) >> 48) & 0xff)
#define ELF64_R_TYPE(info)  (((info) >> 56) & 0xff)
#endif

#if defined(__aarch64__) || defined(__x86_64__)
#define USE_RELA 1
#endif

// constructors, methods, and typedefs are stripped out, not part of the data representation
struct soinfo_common {
    unsigned flags;

    const char* strtab;
    ElfW(Sym)* symtab;

    size_t nbucket;
    size_t nchain;
    unsigned* bucket;
    unsigned* chain;

#if defined(__mips__) || !defined(__LP64__)
    // This is only used by mips and mips64, but needs to be here for
    // all 32-bit architectures to preserve binary compatibility.
    ElfW(Addr)** plt_got;
#endif

#if defined(USE_RELA)
    ElfW(Rela)* plt_rela;
    size_t plt_rela_count;

    ElfW(Rela)* rela;
    size_t rela_count;
#else
    ElfW(Rel)* plt_rel;
    size_t plt_rel_count;

    ElfW(Rel)* rel;
    size_t rel_count;
#endif

    linker_function_t* preinit_array;
    size_t preinit_array_count;

    linker_function_t* init_array;
    size_t init_array_count;
    linker_function_t* fini_array;
    size_t fini_array_count;

    linker_function_t init_func;
    linker_function_t fini_func;

#if defined(__arm__)
    // ARM EABI section used for stack unwinding.
    unsigned* ARM_exidx;
    size_t ARM_exidx_count;
#elif defined(__mips__)
    unsigned mips_symtabno;
    unsigned mips_local_gotno;
    unsigned mips_gotsym;
#endif

    size_t ref_count;
    link_map link_map_head;

    bool constructors_called;

    // When you read a virtual address from the ELF file, add this
    // value to get the corresponding address in the process' address space.
    ElfW(Addr) load_bias;

#if !defined(__LP64__)
    bool has_text_relocations;
#endif
    bool has_DT_SYMBOLIC;

    // This part of the structure is only available
    // when FLAG_NEW_SOINFO is set in this->flags.
    uint32_t version;

    // version >= 0
    dev_t st_dev;
    ino_t st_ino;

    // dependency graph
    void* children[2]; //soinfo_list_t children; void* head, void* tail
    void* parents[2]; //soinfo_list_t parents;

    // version >= 1
    off64_t file_offset;
    uint32_t rtld_flags;
    uint32_t dt_flags_1_;
    size_t strtab_size;

    // version >= 2
    size_t gnu_nbucket_;
    uint32_t* gnu_bucket_;
    uint32_t* gnu_chain_;
    uint32_t gnu_maskwords_;
    uint32_t gnu_shift2_;
    ElfW(Addr)* gnu_bloom_filter_;

    void* local_group_root_; //soinfo*

    uint8_t* android_relocs_;
    size_t android_relocs_size_;
};

struct soinfo_compat { // old style compatibility
    char name[SOINFO_NAME_LEN]; // DO NOT USE, maintained for compatibility.
    const ElfW(Phdr)* phdr;
    size_t phnum;
    ElfW(Addr) entry;
    ElfW(Addr) base;
    size_t size;

#if !defined(__LP64__)
    uint32_t unused1; // DO NOT USE, maintained for compatibility.
#endif

    ElfW(Dyn)* dynamic;

#if !defined(__LP64__)
    uint32_t unused2; // DO NOT USE, maintained for compatibility
    uint32_t unused3; // DO NOT USE, maintained for compatibility
#endif

    void* next; //soinfo*
    struct soinfo_common common;
};

struct soinfo_compact { // new style (64-bit only?)
    const ElfW(Phdr)* phdr;
    size_t phnum;
    ElfW(Addr) entry;
    ElfW(Addr) base;
    size_t size;

    ElfW(Dyn)* dynamic;

    void* next; //soinfo*
    struct soinfo_common common;
};

struct soinfo_compact2 { // even newer style (64-bit only?)
    const ElfW(Phdr)* phdr;
    size_t phnum;
    ElfW(Addr) base;
    size_t size;

    ElfW(Dyn)* dynamic;

    void* next; //soinfo*
    struct soinfo_common common;
};

class sleb128_decoder {
public:
    sleb128_decoder(const uint8_t* buffer, size_t count) :
            current_(buffer), end_(buffer + count) {
    }

    size_t pop_front() {
        size_t value = 0;
        static const size_t size = CHAR_BIT * sizeof(value);

        size_t shift = 0;
        uint8_t byte;

        do {
            if (current_ >= end_) {
                return 0;
            }
            byte = *current_++;
            value |= (static_cast<size_t>(byte & 127) << shift);
            shift += 7;
        } while (byte & 128);

        if (shift < size && (byte & 64)) {
            value |= -(static_cast<size_t>(1) << shift);
        }

        return value;
    }

private:
    const uint8_t* current_;
    const uint8_t* const end_;
};

const size_t RELOCATION_GROUPED_BY_INFO_FLAG = 1;
const size_t RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG = 2;
const size_t RELOCATION_GROUPED_BY_ADDEND_FLAG = 4;
const size_t RELOCATION_GROUP_HAS_ADDEND_FLAG = 8;

template<typename decoder_t>
class packed_reloc_iterator {
#if defined(USE_RELA)
    typedef ElfW(Rela) rel_t;
#else
    typedef ElfW(Rel) rel_t;
#endif
public:
    explicit packed_reloc_iterator(decoder_t&& decoder) :
            decoder_(decoder) {
        // initialize fields
        memset(&reloc_, 0, sizeof(reloc_));
        relocation_count_ = decoder_.pop_front();
        reloc_.r_offset = decoder_.pop_front();
        relocation_index_ = 0;
        relocation_group_index_ = 0;
        group_size_ = 0;
    }

    bool has_next() const {
        return relocation_index_ < relocation_count_;
    }

    rel_t* next() {
        if (relocation_group_index_ == group_size_) {
            if (!read_group_fields()) {
                // Iterator is inconsistent state; it should not be called again
                // but in case it is let's make sure has_next() returns false.
                relocation_index_ = relocation_count_ = 0;
                return nullptr;
            }
        }

        if (is_relocation_grouped_by_offset_delta()) {
            reloc_.r_offset += group_r_offset_delta_;
        } else {
            reloc_.r_offset += decoder_.pop_front();
        }

        if (!is_relocation_grouped_by_info()) {
            reloc_.r_info = decoder_.pop_front();
        }

#if defined(USE_RELA)
        if (is_relocation_group_has_addend() &&
                !is_relocation_grouped_by_addend()) {
            reloc_.r_addend += decoder_.pop_front();
        }
#endif

        relocation_index_++;
        relocation_group_index_++;

        return &reloc_;
    }
private:
    bool read_group_fields() {
        group_size_ = decoder_.pop_front();
        group_flags_ = decoder_.pop_front();

        if (is_relocation_grouped_by_offset_delta()) {
            group_r_offset_delta_ = decoder_.pop_front();
        }

        if (is_relocation_grouped_by_info()) {
            reloc_.r_info = decoder_.pop_front();
        }

        if (is_relocation_group_has_addend()
                && is_relocation_grouped_by_addend()) {
#if defined(USE_RELA)
            reloc_.r_addend += decoder_.pop_front();
        } else if (!is_relocation_group_has_addend()) {
            reloc_.r_addend = 0;
#endif
        }

        relocation_group_index_ = 0;
        return true;
    }

    bool is_relocation_grouped_by_info() {
        return (group_flags_ & RELOCATION_GROUPED_BY_INFO_FLAG) != 0;
    }

    bool is_relocation_grouped_by_offset_delta() {
        return (group_flags_ & RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG) != 0;
    }

    bool is_relocation_grouped_by_addend() {
        return (group_flags_ & RELOCATION_GROUPED_BY_ADDEND_FLAG) != 0;
    }

    bool is_relocation_group_has_addend() {
        return (group_flags_ & RELOCATION_GROUP_HAS_ADDEND_FLAG) != 0;
    }

    decoder_t decoder_;
    size_t relocation_count_;
    size_t group_size_;
    size_t group_flags_;
    size_t group_r_offset_delta_;
    size_t relocation_index_;
    size_t relocation_group_index_;
    rel_t reloc_;
};

#endif
