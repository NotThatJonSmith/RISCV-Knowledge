#pragma once

#include <cstdint>
#include <type_traits>
#include <array>
#include <string>

namespace RISCV {

// -- Facts about the RISC-V specification this file describes --

constexpr const char* PrivilegedSpecDate = "20190608";
constexpr const char* UnprivilegedSpecDate = "20191213";

// -- Relevant enums --

enum PrivilegeMode {
    User=0,
    Supervisor=1,
    Hypervisor=2,
    Machine=3
};

enum XlenMode {
    None = 0,
    XL32 = 1,
    XL64 = 2,
    XL128 = 3
};

enum FloatingPointState {
    Off = 0,
    Initial = 1,
    Clean = 2,
    Dirty = 3
};

enum ExtensionState {
    AllOff = 0,
    NoneDirtyNoneClean = 1,
    NoneDirtySomeClean = 2,
    SomeDirty = 3
};

enum PagingMode {
    Bare = 0, Sv32 = 1, Sv39 = 8, Sv48 = 9, Sv57 = 10, Sv64 = 11
};

enum PTEBit {
    V = 0b00000001,
    R = 0b00000010,
    W = 0b00000100,
    X = 0b00001000,
    U = 0b00010000,
    G = 0b00100000,
    A = 0b01000000,
    D = 0b10000000
};

enum tvecMode {
    Direct = 0,
    Vectored = 1
};

enum TrapCause {
    
    NONE = -1,

    // Interrupts
    USER_SOFTWARE_INTERRUPT = 0,
    SUPERVISOR_SOFTWARE_INTERRUPT = 1,
    // HYPERVISOR_SOFTWARE_INTERRUPT = 2,
    MACHINE_SOFTWARE_INTERRUPT = 3,
    USER_TIMER_INTERRUPT = 4,
    SUPERVISOR_TIMER_INTERRUPT = 5,
    // HYPERVISOR_TIMER_INTERRUPT = 6,
    MACHINE_TIMER_INTERRUPT = 7,
    USER_EXTERNAL_INTERRUPT = 8,
    SUPERVISOR_EXTERNAL_INTERRUPT = 9,
    // HYPERVISOR_EXTERNAL_INTERRUPT = 10,
    MACHINE_EXTERNAL_INTERRUPT = 11,

    // Exceptions
    INSTRUCTION_ADDRESS_MISALIGNED = 0,
    INSTRUCTION_ACCESS_FAULT = 1,
    ILLEGAL_INSTRUCTION = 2,
    BREAKPOINT = 3,
    LOAD_ADDRESS_MISALIGNED = 4,
    LOAD_ACCESS_FAULT = 5,
    STORE_AMO_ADDRESS_MISALIGNED = 6,
    STORE_AMO_ACCESS_FAULT = 7,
    ECALL_FROM_U_MODE = 8,
    ECALL_FROM_S_MODE = 9,
    // ECALL_FROM_H_MODE = 10,
    ECALL_FROM_M_MODE = 11,
    INSTRUCTION_PAGE_FAULT = 12,
    LOAD_PAGE_FAULT = 13,
    STORE_AMO_PAGE_FAULT = 15
};

enum CSRAddress {
    USTATUS = 0X000, UIE = 0X004, UTVEC = 0X005, USCRATCH = 0X040, UEPC = 0X041,
    UCAUSE = 0X042, UTVAL = 0X043, UIP = 0X044, FFLAGS = 0X001, FRM = 0X002,
    FCSR = 0X003, CYCLE = 0XC00, TIME = 0XC01, INSTRET = 0XC02,
    HPMCOUNTER3 = 0XC03, HPMCOUNTER4 = 0XC04, HPMCOUNTER5 = 0XC05,
    HPMCOUNTER6 = 0XC06, HPMCOUNTER7 = 0XC07, HPMCOUNTER8 = 0XC08,
    HPMCOUNTER9 = 0XC09, HPMCOUNTER10 = 0XC0A, HPMCOUNTER11 = 0XC0B,
    HPMCOUNTER12 = 0XC0C, HPMCOUNTER13 = 0XC0D, HPMCOUNTER14 = 0XC0E,
    HPMCOUNTER15 = 0XC0F, HPMCOUNTER16 = 0XC10, HPMCOUNTER17 = 0XC11,
    HPMCOUNTER18 = 0XC12, HPMCOUNTER19 = 0XC13, HPMCOUNTER20 = 0XC14,
    HPMCOUNTER21 = 0XC15, HPMCOUNTER22 = 0XC16, HPMCOUNTER23 = 0XC17,
    HPMCOUNTER24 = 0XC18, HPMCOUNTER25 = 0XC19, HPMCOUNTER26 = 0XC1A,
    HPMCOUNTER27 = 0XC1B, HPMCOUNTER28 = 0XC1C, HPMCOUNTER29 = 0XC1D,
    HPMCOUNTER30 = 0XC1E, HPMCOUNTER31 = 0XC1F, CYCLEH = 0XC80, TIMEH = 0XC81,
    INSTRETH = 0XC82, HPMCOUNTER3H = 0XC83, HPMCOUNTER4H = 0XC84,
    HPMCOUNTER5H = 0XC85, HPMCOUNTER6H = 0XC86, HPMCOUNTER7H = 0XC87,
    HPMCOUNTER8H = 0XC88, HPMCOUNTER9H = 0XC89, HPMCOUNTER10H = 0XC8A,
    HPMCOUNTER11H = 0XC8B, HPMCOUNTER12H = 0XC8C, HPMCOUNTER13H = 0XC8D,
    HPMCOUNTER14H = 0XC8E, HPMCOUNTER15H = 0XC8F, HPMCOUNTER16H = 0XC90,
    HPMCOUNTER17H = 0XC91, HPMCOUNTER18H = 0XC92, HPMCOUNTER19H = 0XC93,
    HPMCOUNTER20H = 0XC94, HPMCOUNTER21H = 0XC95, HPMCOUNTER22H = 0XC96,
    HPMCOUNTER23H = 0XC97, HPMCOUNTER24H = 0XC98, HPMCOUNTER25H = 0XC99,
    HPMCOUNTER26H = 0XC9A, HPMCOUNTER27H = 0XC9B, HPMCOUNTER28H = 0XC9C,
    HPMCOUNTER29H = 0XC9D, HPMCOUNTER30H = 0XC9E, HPMCOUNTER31H = 0XC9F,
    SSTATUS = 0X100, SEDELEG = 0X102, SIDELEG = 0X103, SIE = 0X104,
    STVEC = 0X105, SCOUNTEREN = 0X106, SSCRATCH = 0X140, SEPC = 0X141,
    SCAUSE = 0X142, STVAL = 0X143, SIP = 0X144, SATP = 0X180, MVENDORID = 0XF11,
    MARCHID = 0XF12, MIMPID = 0XF13, MHARTID = 0XF14, MSTATUS = 0X300,
    MISA = 0X301, MEDELEG = 0X302, MIDELEG = 0X303, MIE = 0X304, MTVEC = 0X305,
    MCOUNTEREN = 0X306, MSCRATCH = 0X340, MEPC = 0X341, MCAUSE = 0X342,
    MTVAL = 0X343, MIP = 0X344, PMPCFG0 = 0X3A0, PMPCFG1 = 0X3A1,
    PMPCFG2 = 0X3A2, PMPCFG3 = 0X3A3, PMPADDR0 = 0X3B0, PMPADDR1 = 0X3B1,
    PMPADDR2 = 0X3B2, PMPADDR3 = 0X3B3, PMPADDR4 = 0X3B4, PMPADDR5 = 0X3B5,
    PMPADDR6 = 0X3B6, PMPADDR7 = 0X3B7, PMPADDR8 = 0X3B8, PMPADDR9 = 0X3B9,
    PMPADDR10 = 0X3BA, PMPADDR11 = 0X3BB, PMPADDR12 = 0X3BC, PMPADDR13 = 0X3BD,
    PMPADDR14 = 0X3BE, PMPADDR15 = 0X3BF, MCYCLE = 0XB00, MINSTRET = 0XB02,
    MHPMCOUNTER3 = 0XB03, MHPMCOUNTER4 = 0XB04, MHPMCOUNTER31 = 0XB1F,
    MCYCLEH = 0XB80, MINSTRETH = 0XB82, MHPMCOUNTER3H = 0XB83,
    MHPMCOUNTER4H = 0XB84, MHPMCOUNTER5H = 0XB85, MHPMCOUNTER6H = 0XB86,
    MHPMCOUNTER7H = 0XB87, MHPMCOUNTER8H = 0XB88, MHPMCOUNTER9H = 0XB89,
    MHPMCOUNTER10H = 0XB8A, MHPMCOUNTER11H = 0XB8B, MHPMCOUNTER12H = 0XB8C,
    MHPMCOUNTER13H = 0XB8D, MHPMCOUNTER14H = 0XB8E, MHPMCOUNTER15H = 0XB8F,
    MHPMCOUNTER16H = 0XB90, MHPMCOUNTER17H = 0XB91, MHPMCOUNTER18H = 0XB92,
    MHPMCOUNTER19H = 0XB93, MHPMCOUNTER20H = 0XB94, MHPMCOUNTER21H = 0XB95,
    MHPMCOUNTER22H = 0XB96, MHPMCOUNTER23H = 0XB97, MHPMCOUNTER24H = 0XB98,
    MHPMCOUNTER25H = 0XB99, MHPMCOUNTER26H = 0XB9A, MHPMCOUNTER27H = 0XB9B,
    MHPMCOUNTER28H = 0XB9C, MHPMCOUNTER29H = 0XB9D, MHPMCOUNTER30H = 0XB9E,
    MHPMCOUNTER31H = 0XB9F, MCOUNTINHIBIT = 0X320, MHPMEVENT3 = 0X323,
    MHPMEVENT4 = 0X324, MHPMEVENT5 = 0X325, MHPMEVENT6 = 0X326,
    MHPMEVENT7 = 0X327, MHPMEVENT8 = 0X328, MHPMEVENT9 = 0X329,
    MHPMEVENT10 = 0X32A, MHPMEVENT11 = 0X32B, MHPMEVENT12 = 0X32C,
    MHPMEVENT13 = 0X32D, MHPMEVENT14 = 0X32E, MHPMEVENT15 = 0X32F,
    MHPMEVENT16 = 0X330, MHPMEVENT17 = 0X331, MHPMEVENT18 = 0X332,
    MHPMEVENT19 = 0X333, MHPMEVENT20 = 0X334, MHPMEVENT21 = 0X335,
    MHPMEVENT22 = 0X336, MHPMEVENT23 = 0X337, MHPMEVENT24 = 0X338,
    MHPMEVENT25 = 0X339, MHPMEVENT26 = 0X33A, MHPMEVENT27 = 0X33B,
    MHPMEVENT28 = 0X33C, MHPMEVENT29 = 0X33D, MHPMEVENT30 = 0X33E,
    MHPMEVENT31 = 0X33F, TSELECT = 0X7A0, TDATA1 = 0X7A1, TDATA2 = 0X7A2,
    TDATA3 = 0X7A3, DCSR = 0X7B0, DPC = 0X7B1, DSCRATCH0 = 0X7B2,
    DSCRATCH1 = 0X7B3, INVALID_CSR = 0x1000
};


// -- Facts about general purpose registers --

constexpr unsigned int NumRegs = 32;

constexpr std::array<const char*, NumRegs> registerAbiNames = {
    "zero", "ra", "sp", "gp", "tp",
    "t0", "t1", "t2",
    "s0", "s1",
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
    "t3", "t4", "t5", "t6"
};

constexpr std::array<const char*, NumRegs> registerFlatNames = {
    "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11",
    "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "x20", "x21", "x22",
    "x23", "x24", "x25", "x26", "x27", "x28", "x29", "x30", "x31"
};

inline std::string regName(unsigned int regNum, bool flat=false) {
    if (regNum >= NumRegs)
        return "(invalid register #" + std::to_string(regNum) + ")";
    if (flat)
        return registerFlatNames[regNum];
    return registerAbiNames[regNum];
}

// -- Facts about RISC-V extension vectors --

constexpr inline __uint32_t stringToExtensions(const char *isa) {
    __uint32_t vec = 0;
    for (; *isa; isa++)
        vec |= (1 << (*isa - (((*isa >= 'a') && (*isa <= 'z')) ? 'a' : 'A')));
    return vec;
}

constexpr inline bool vectorHasExtension(__uint32_t vector, char extension) {
    return vector & (1 <<  (extension - 'A'));
}

template<typename XLEN_t>
constexpr XlenMode xlenTypeToMode() {
    if (std::is_same<XLEN_t, __uint32_t>())
        return XlenMode::XL32;
    if (std::is_same<XLEN_t, __uint64_t>())
        return XlenMode::XL64;
    if (std::is_same<XLEN_t, __uint128_t>())
        return XlenMode::XL128;
    return XlenMode::None;
}

// -- Facts about RISC-V instruction encodings --

enum OpcodeQuadrant {
    Q0           = 0b00, Q1           = 0b01,
    Q2           = 0b10, UNCOMPRESSED = 0b11
};

enum MajorOpcode {
    LOAD       = 0b00000,    LOAD_FP    = 0b00001,    CUSTOM_0   = 0b00010,
    MISC_MEM   = 0b00011,    OP_IMM     = 0b00100,    AUIPC      = 0b00101,
    OP_IMM_32  = 0b00110,    LONG_48B_1 = 0b00111,    STORE      = 0b01000,
    STORE_FP   = 0b01001,    CUSTOM_1   = 0b01010,    AMO        = 0b01011,
    OP         = 0b01100,    LUI        = 0b01101,    OP_32      = 0b01110,
    LONG_64B   = 0b01111,    MADD       = 0b10000,    MSUB       = 0b10001,
    NMSUB      = 0b10010,    NMADD      = 0b10011,    OP_FP      = 0b10100,
    RESERVED_0 = 0b10101,    CUSTOM_2   = 0b10110,    LONG_48B_2 = 0b10111,
    BRANCH     = 0b11000,    JALR       = 0b11001,    RESERVED_1 = 0b11010,
    JAL        = 0b11011,    SYSTEM     = 0b11100,    RESERVED_2 = 0b11101,
    CUSTOM_3   = 0b11110,    LONG_80B   = 0b11111
};

enum MinorOpcode {

    // OP
    ADD = 0, SLL = 1, SLT = 2, SLTU = 3, XOR = 4, SRL = 5, OR = 6, AND = 7, SUB = 256, SRA = 261,

    // OP-IMM
    ADDI = 0, SLLI = 1, SLTI = 2, SLTIU = 3, XORI = 4, SRI = 5, ORI = 6, ANDI = 7,

    // MULDIV
    MUL = 8, MULH = 9, MULHSU = 10, MULHU = 11, DIV = 12, DIVU = 13, REM = 14, REMU = 15,

    // BRANCH
    BEQ = 0, BNE = 1, BLT = 4, BGE = 5, BLTU = 6, BGEU = 7,

    // LOAD
    LB = 0, LH = 1, LW = 2, LBU = 4, LHU = 5,

    // STORE
    SB = 0, SH = 1, SW = 2,

    // SYSTEM
    PRIV = 0, CSRRW = 1, CSRRS = 2, CSRRC = 3, CSRRWI = 5, CSRRSI = 6, CSRRCI = 7, 

    // MISC-MEM
    FENCE = 0, FENCE_I = 1,

    // Atomic operations standard extension
    AMOADD = 0, AMOSWAP = 1, LR = 2, SC = 3, AMOXOR = 4, AMOOR = 8, AMOAND = 12, AMOMIN = 16, AMOMAX = 20, AMOMINU = 24, AMOMAXU = 28
};

enum SubMinorOpcode {

    // OP-IMM / SRI
    SRLI = 0b0000000, SRAI = 0b0100000,

    // SYSTEM / PRIV
    ECALL_EBREAK_URET = 0, SRET_WFI = 8, MRET = 24, SFENCE_VMA = 9

};

enum SubSubMinorOpcode {

    // SYSTEM / PRIV / ECALL_EBREAK_URET
    ECALL = 0, EBREAK = 1, URET = 2,

    // SYSTEM / PRIV / SRET_WFI
    SRET = 2, WFI = 5

};

enum AmoWidth {
    AMO_W = 2,
    AMO_D = 3
};

constexpr bool isCompressed(__uint32_t encodedInstruction) {
    return (encodedInstruction & 0x00000003) != 0x00000003;
}

constexpr unsigned int instructionLength(__uint32_t encodedInstruction) {
    return isCompressed(encodedInstruction) ? 2 : 4; // TODO extended lengths
}

// -- Facts about Configuration & Status Registers --

constexpr unsigned int NumCSRs = 0x1000;

constexpr std::array<const char*, NumCSRs> getCSRNameTable() {
    std::array<const char*, NumCSRs> table = {0};
    table[CSRAddress::USTATUS] = "ustatus";
    table[CSRAddress::UIE] = "uie";
    table[CSRAddress::UTVEC] = "utvec";
    table[CSRAddress::USCRATCH] = "uscratch";
    table[CSRAddress::UEPC] = "uepc";
    table[CSRAddress::UCAUSE] = "ucause";
    table[CSRAddress::UTVAL] = "utval";
    table[CSRAddress::UIP] = "uip";
    table[CSRAddress::FFLAGS] = "fflags";
    table[CSRAddress::FRM] = "frm";
    table[CSRAddress::FCSR] = "fcsr";
    table[CSRAddress::CYCLE] = "cycle";
    table[CSRAddress::TIME] = "time";
    table[CSRAddress::INSTRET] = "instret";
    table[CSRAddress::HPMCOUNTER3] = "hpmcounter3";
    table[CSRAddress::HPMCOUNTER4] = "hpmcounter4";
    table[CSRAddress::HPMCOUNTER5] = "hpmcounter5";
    table[CSRAddress::HPMCOUNTER6] = "hpmcounter6";
    table[CSRAddress::HPMCOUNTER7] = "hpmcounter7";
    table[CSRAddress::HPMCOUNTER8] = "hpmcounter8";
    table[CSRAddress::HPMCOUNTER9] = "hpmcounter9";
    table[CSRAddress::HPMCOUNTER10] = "hpmcounter10";
    table[CSRAddress::HPMCOUNTER11] = "hpmcounter11";
    table[CSRAddress::HPMCOUNTER12] = "hpmcounter12";
    table[CSRAddress::HPMCOUNTER13] = "hpmcounter13";
    table[CSRAddress::HPMCOUNTER14] = "hpmcounter14";
    table[CSRAddress::HPMCOUNTER15] = "hpmcounter15";
    table[CSRAddress::HPMCOUNTER16] = "hpmcounter16";
    table[CSRAddress::HPMCOUNTER17] = "hpmcounter17";
    table[CSRAddress::HPMCOUNTER18] = "hpmcounter18";
    table[CSRAddress::HPMCOUNTER19] = "hpmcounter19";
    table[CSRAddress::HPMCOUNTER20] = "hpmcounter20";
    table[CSRAddress::HPMCOUNTER21] = "hpmcounter21";
    table[CSRAddress::HPMCOUNTER22] = "hpmcounter22";
    table[CSRAddress::HPMCOUNTER23] = "hpmcounter23";
    table[CSRAddress::HPMCOUNTER24] = "hpmcounter24";
    table[CSRAddress::HPMCOUNTER25] = "hpmcounter25";
    table[CSRAddress::HPMCOUNTER26] = "hpmcounter26";
    table[CSRAddress::HPMCOUNTER27] = "hpmcounter27";
    table[CSRAddress::HPMCOUNTER28] = "hpmcounter28";
    table[CSRAddress::HPMCOUNTER29] = "hpmcounter29";
    table[CSRAddress::HPMCOUNTER30] = "hpmcounter30";
    table[CSRAddress::HPMCOUNTER31] = "hpmcounter31";
    table[CSRAddress::CYCLEH] = "cycleh";
    table[CSRAddress::TIMEH] = "timeh";
    table[CSRAddress::INSTRETH] = "instreth";
    table[CSRAddress::HPMCOUNTER3H] = "hpmcounter3h";
    table[CSRAddress::HPMCOUNTER4H] = "hpmcounter4h";
    table[CSRAddress::HPMCOUNTER5H] = "hpmcounter5h";
    table[CSRAddress::HPMCOUNTER6H] = "hpmcounter6h";
    table[CSRAddress::HPMCOUNTER7H] = "hpmcounter7h";
    table[CSRAddress::HPMCOUNTER8H] = "hpmcounter8h";
    table[CSRAddress::HPMCOUNTER9H] = "hpmcounter9h";
    table[CSRAddress::HPMCOUNTER10H] = "hpmcounter10h";
    table[CSRAddress::HPMCOUNTER11H] = "hpmcounter11h";
    table[CSRAddress::HPMCOUNTER12H] = "hpmcounter12h";
    table[CSRAddress::HPMCOUNTER13H] = "hpmcounter13h";
    table[CSRAddress::HPMCOUNTER14H] = "hpmcounter14h";
    table[CSRAddress::HPMCOUNTER15H] = "hpmcounter15h";
    table[CSRAddress::HPMCOUNTER16H] = "hpmcounter16h";
    table[CSRAddress::HPMCOUNTER17H] = "hpmcounter17h";
    table[CSRAddress::HPMCOUNTER18H] = "hpmcounter18h";
    table[CSRAddress::HPMCOUNTER19H] = "hpmcounter19h";
    table[CSRAddress::HPMCOUNTER20H] = "hpmcounter20h";
    table[CSRAddress::HPMCOUNTER21H] = "hpmcounter21h";
    table[CSRAddress::HPMCOUNTER22H] = "hpmcounter22h";
    table[CSRAddress::HPMCOUNTER23H] = "hpmcounter23h";
    table[CSRAddress::HPMCOUNTER24H] = "hpmcounter24h";
    table[CSRAddress::HPMCOUNTER25H] = "hpmcounter25h";
    table[CSRAddress::HPMCOUNTER26H] = "hpmcounter26h";
    table[CSRAddress::HPMCOUNTER27H] = "hpmcounter27h";
    table[CSRAddress::HPMCOUNTER28H] = "hpmcounter28h";
    table[CSRAddress::HPMCOUNTER29H] = "hpmcounter29h";
    table[CSRAddress::HPMCOUNTER30H] = "hpmcounter30h";
    table[CSRAddress::HPMCOUNTER31H] = "hpmcounter31h";
    table[CSRAddress::SSTATUS] = "sstatus";
    table[CSRAddress::SEDELEG] = "sedeleg";
    table[CSRAddress::SIDELEG] = "sideleg";
    table[CSRAddress::SIE] = "sie";
    table[CSRAddress::STVEC] = "stvec";
    table[CSRAddress::SCOUNTEREN] = "scounteren";
    table[CSRAddress::SSCRATCH] = "sscratch";
    table[CSRAddress::SEPC] = "sepc";
    table[CSRAddress::SCAUSE] = "scause";
    table[CSRAddress::STVAL] = "stval";
    table[CSRAddress::SIP] = "sip";
    table[CSRAddress::SATP] = "satp";
    table[CSRAddress::MVENDORID] = "mvendorid";
    table[CSRAddress::MARCHID] = "marchid";
    table[CSRAddress::MIMPID] = "mimpid";
    table[CSRAddress::MHARTID] = "mhartid";
    table[CSRAddress::MSTATUS] = "mstatus";
    table[CSRAddress::MISA] = "misa";
    table[CSRAddress::MEDELEG] = "medeleg";
    table[CSRAddress::MIDELEG] = "mideleg";
    table[CSRAddress::MIE] = "mie";
    table[CSRAddress::MTVEC] = "mtvec";
    table[CSRAddress::MCOUNTEREN] = "mcounteren";
    table[CSRAddress::MSCRATCH] = "mscratch";
    table[CSRAddress::MEPC] = "mepc";
    table[CSRAddress::MCAUSE] = "mcause";
    table[CSRAddress::MTVAL] = "mtval";
    table[CSRAddress::MIP] = "mip";
    table[CSRAddress::PMPCFG0] = "pmpcfg0";
    table[CSRAddress::PMPCFG1] = "pmpcfg1";
    table[CSRAddress::PMPCFG2] = "pmpcfg2";
    table[CSRAddress::PMPCFG3] = "pmpcfg3";
    table[CSRAddress::PMPADDR0] = "pmpaddr0";
    table[CSRAddress::PMPADDR1] = "pmpaddr1";
    table[CSRAddress::PMPADDR2] = "pmpaddr2";
    table[CSRAddress::PMPADDR3] = "pmpaddr3";
    table[CSRAddress::PMPADDR4] = "pmpaddr4";
    table[CSRAddress::PMPADDR5] = "pmpaddr5";
    table[CSRAddress::PMPADDR6] = "pmpaddr6";
    table[CSRAddress::PMPADDR7] = "pmpaddr7";
    table[CSRAddress::PMPADDR8] = "pmpaddr8";
    table[CSRAddress::PMPADDR9] = "pmpaddr9";
    table[CSRAddress::PMPADDR10] = "pmpaddr10";
    table[CSRAddress::PMPADDR11] = "pmpaddr11";
    table[CSRAddress::PMPADDR12] = "pmpaddr12";
    table[CSRAddress::PMPADDR13] = "pmpaddr13";
    table[CSRAddress::PMPADDR14] = "pmpaddr14";
    table[CSRAddress::PMPADDR15] = "pmpaddr15";
    table[CSRAddress::MCYCLE] = "mcycle";
    table[CSRAddress::MINSTRET] = "minstret";
    table[CSRAddress::MHPMCOUNTER3] = "mhpmcounter3";
    table[CSRAddress::MHPMCOUNTER4] = "mhpmcounter4";
    table[CSRAddress::MHPMCOUNTER31] = "mhpmcounter31";
    table[CSRAddress::MCYCLEH] = "mcycleh";
    table[CSRAddress::MINSTRETH] = "minstreth";
    table[CSRAddress::MHPMCOUNTER3H] = "mhpmcounter3h";
    table[CSRAddress::MHPMCOUNTER4H] = "mhpmcounter4h";
    table[CSRAddress::MHPMCOUNTER5H] = "mhpmcounter5h";
    table[CSRAddress::MHPMCOUNTER6H] = "mhpmcounter6h";
    table[CSRAddress::MHPMCOUNTER7H] = "mhpmcounter7h";
    table[CSRAddress::MHPMCOUNTER8H] = "mhpmcounter8h";
    table[CSRAddress::MHPMCOUNTER9H] = "mhpmcounter9h";
    table[CSRAddress::MHPMCOUNTER10H] = "mhpmcounter10h";
    table[CSRAddress::MHPMCOUNTER11H] = "mhpmcounter11h";
    table[CSRAddress::MHPMCOUNTER12H] = "mhpmcounter12h";
    table[CSRAddress::MHPMCOUNTER13H] = "mhpmcounter13h";
    table[CSRAddress::MHPMCOUNTER14H] = "mhpmcounter14h";
    table[CSRAddress::MHPMCOUNTER15H] = "mhpmcounter15h";
    table[CSRAddress::MHPMCOUNTER16H] = "mhpmcounter16h";
    table[CSRAddress::MHPMCOUNTER17H] = "mhpmcounter17h";
    table[CSRAddress::MHPMCOUNTER18H] = "mhpmcounter18h";
    table[CSRAddress::MHPMCOUNTER19H] = "mhpmcounter19h";
    table[CSRAddress::MHPMCOUNTER20H] = "mhpmcounter20h";
    table[CSRAddress::MHPMCOUNTER21H] = "mhpmcounter21h";
    table[CSRAddress::MHPMCOUNTER22H] = "mhpmcounter22h";
    table[CSRAddress::MHPMCOUNTER23H] = "mhpmcounter23h";
    table[CSRAddress::MHPMCOUNTER24H] = "mhpmcounter24h";
    table[CSRAddress::MHPMCOUNTER25H] = "mhpmcounter25h";
    table[CSRAddress::MHPMCOUNTER26H] = "mhpmcounter26h";
    table[CSRAddress::MHPMCOUNTER27H] = "mhpmcounter27h";
    table[CSRAddress::MHPMCOUNTER28H] = "mhpmcounter28h";
    table[CSRAddress::MHPMCOUNTER29H] = "mhpmcounter29h";
    table[CSRAddress::MHPMCOUNTER30H] = "mhpmcounter30h";
    table[CSRAddress::MHPMCOUNTER31H] = "mhpmcounter31h";
    table[CSRAddress::MCOUNTINHIBIT] = "mcountinhibit";
    table[CSRAddress::MHPMEVENT3] = "mhpmevent3";
    table[CSRAddress::MHPMEVENT4] = "mhpmevent4";
    table[CSRAddress::MHPMEVENT5] = "mhpmevent5";
    table[CSRAddress::MHPMEVENT6] = "mhpmevent6";
    table[CSRAddress::MHPMEVENT7] = "mhpmevent7";
    table[CSRAddress::MHPMEVENT8] = "mhpmevent8";
    table[CSRAddress::MHPMEVENT9] = "mhpmevent9";
    table[CSRAddress::MHPMEVENT10] = "mhpmevent10";
    table[CSRAddress::MHPMEVENT11] = "mhpmevent11";
    table[CSRAddress::MHPMEVENT12] = "mhpmevent12";
    table[CSRAddress::MHPMEVENT13] = "mhpmevent13";
    table[CSRAddress::MHPMEVENT14] = "mhpmevent14";
    table[CSRAddress::MHPMEVENT15] = "mhpmevent15";
    table[CSRAddress::MHPMEVENT16] = "mhpmevent16";
    table[CSRAddress::MHPMEVENT17] = "mhpmevent17";
    table[CSRAddress::MHPMEVENT18] = "mhpmevent18";
    table[CSRAddress::MHPMEVENT19] = "mhpmevent19";
    table[CSRAddress::MHPMEVENT20] = "mhpmevent20";
    table[CSRAddress::MHPMEVENT21] = "mhpmevent21";
    table[CSRAddress::MHPMEVENT22] = "mhpmevent22";
    table[CSRAddress::MHPMEVENT23] = "mhpmevent23";
    table[CSRAddress::MHPMEVENT24] = "mhpmevent24";
    table[CSRAddress::MHPMEVENT25] = "mhpmevent25";
    table[CSRAddress::MHPMEVENT26] = "mhpmevent26";
    table[CSRAddress::MHPMEVENT27] = "mhpmevent27";
    table[CSRAddress::MHPMEVENT28] = "mhpmevent28";
    table[CSRAddress::MHPMEVENT29] = "mhpmevent29";
    table[CSRAddress::MHPMEVENT30] = "mhpmevent30";
    table[CSRAddress::MHPMEVENT31] = "mhpmevent31";
    table[CSRAddress::TSELECT] = "tselect";
    table[CSRAddress::TDATA1] = "tdata1";
    table[CSRAddress::TDATA2] = "tdata2";
    table[CSRAddress::TDATA3] = "tdata3";
    table[CSRAddress::DCSR] = "dcsr";
    table[CSRAddress::DPC] = "dpc";
    table[CSRAddress::DSCRATCH0] = "dscratch0";
    table[CSRAddress::DSCRATCH1] = "dscratch1";
    return table;
};

constexpr std::array<const char*, NumCSRs> csrNames = getCSRNameTable();

inline std::string csrName(unsigned int address) {
    if (address >= NumCSRs)
        return "(invalid CSR #" + std::to_string(address) + ")";
    return csrNames[address];
}

inline constexpr PrivilegeMode csrRequiredPrivilege(CSRAddress addr) {
    return (PrivilegeMode) ((addr & 0b001100000000) >> 8);
}

inline bool csrIsReadOnly(CSRAddress addr) {
    return (addr & 0b110000000000) == 0b110000000000;
}

// -- Facts about interrupts, exceptions, and traps --

struct TrapCSRs {
    CSRAddress status, cause, epc, tvec, tval, ip, ie, ideleg, edeleg;
};

constexpr TrapCSRs MachineTrapCSRs = {
    CSRAddress::MSTATUS,
    CSRAddress::MCAUSE,
    CSRAddress::MEPC,
    CSRAddress::MTVEC,
    CSRAddress::MTVAL,
    CSRAddress::MIP,
    CSRAddress::MIE,
    CSRAddress::MIDELEG, // TODO what if no S/U modes?
    CSRAddress::MEDELEG  // TODO what if no S/U modes?
};

constexpr TrapCSRs HypervisorTrapCSRs = {
    CSRAddress::INVALID_CSR,
    CSRAddress::INVALID_CSR,
    CSRAddress::INVALID_CSR,
    CSRAddress::INVALID_CSR,
    CSRAddress::INVALID_CSR,
    CSRAddress::INVALID_CSR,
    CSRAddress::INVALID_CSR,
    CSRAddress::INVALID_CSR,
    CSRAddress::INVALID_CSR
};

constexpr TrapCSRs SupervisorTrapCSRs = {
    CSRAddress::SSTATUS,
    CSRAddress::SCAUSE,
    CSRAddress::SEPC,
    CSRAddress::STVEC,
    CSRAddress::STVAL,
    CSRAddress::SIP,
    CSRAddress::SIE,
    CSRAddress::SIDELEG, // What if no N extension for U mode? Or no U mode?
    CSRAddress::SEDELEG  // What if no N extension for U mode? Or no U mode?
};

constexpr TrapCSRs UserTrapCSRs = {
    CSRAddress::USTATUS,
    CSRAddress::UCAUSE,
    CSRAddress::UEPC,
    CSRAddress::UTVEC,
    CSRAddress::UTVAL,
    CSRAddress::UIP,
    CSRAddress::UIE,
    CSRAddress::INVALID_CSR, // CSRAddress::UIDELEG does not exist
    CSRAddress::INVALID_CSR  // CSRAddress::UEDELEG does not exist
};

constexpr TrapCSRs trapCSRsForPrivilege[4] = {
    UserTrapCSRs,
    SupervisorTrapCSRs,
    HypervisorTrapCSRs,
    MachineTrapCSRs
};

constexpr unsigned int tvecModeMask = 0x00000003;
constexpr unsigned int tvecBaseMask = 0xfffffffc;

constexpr __uint32_t uieMask  = 0b00000000000000000000001;
constexpr __uint32_t sieMask  = 0b00000000000000000000010;
constexpr __uint32_t hieMask  = 0b00000000000000000000100;
constexpr __uint32_t mieMask  = 0b00000000000000000001000;
constexpr __uint32_t upieMask = 0b00000000000000000010000;
constexpr __uint32_t spieMask = 0b00000000000000000100000;
constexpr __uint32_t hpieMask = 0b00000000000000001000000;
constexpr __uint32_t mpieMask = 0b00000000000000010000000;
constexpr __uint32_t sppMask  = 0b00000000000000100000000; constexpr __uint32_t sppShift = 8;
constexpr __uint32_t hppMask  = 0b00000000000011000000000; constexpr __uint32_t hppShift = 9;
constexpr __uint32_t mppMask  = 0b00000000001100000000000; constexpr __uint32_t mppShift = 11;
constexpr __uint32_t fsMask   = 0b00000000110000000000000; constexpr __uint32_t fsShift  = 13;
constexpr __uint32_t xsMask   = 0b00000011000000000000000; constexpr __uint32_t xsShift  = 15;
constexpr __uint32_t mprvMask = 0b00000100000000000000000;
constexpr __uint32_t sumMask  = 0b00001000000000000000000;
constexpr __uint32_t mxrMask  = 0b00010000000000000000000;
constexpr __uint32_t tvmMask  = 0b00100000000000000000000;
constexpr __uint32_t twMask   = 0b01000000000000000000000;
constexpr __uint32_t tsrMask  = 0b10000000000000000000000;

constexpr __uint32_t  sdMask32  = 0x80000000;
constexpr __uint64_t  sdMask64  = 0x8000000000000000;
// constexpr __uint128_t sdMask128 = 0x80000000000000000000000000000000;

constexpr __uint64_t uxlMask = 0x300000000; constexpr __uint32_t uxlShift = 32;
constexpr __uint64_t sxlMask = 0xC00000000; constexpr __uint32_t sxlShift = 34;
// constexpr __uint64_t hxlMask = 0x3000000000; << 32; constexpr __uint32_t hxlShift = 36;

template<typename XLEN_t>
PrivilegeMode DestinedPrivilegeForCause(RISCV::TrapCause cause, XLEN_t mdeleg, XLEN_t sdeleg, __uint32_t extensions) {

    XLEN_t causeMask = 1 << cause;

    // Without U mode, there is no S mode either, so M mode takes it.
    if (!vectorHasExtension(extensions, 'U')) {
        return PrivilegeMode::Machine;
    }

    // If M mode hasn't delegated the trap, it takes it.
    if (!(mdeleg & causeMask)) {
        return PrivilegeMode::Machine;
    }

    // If there is no S mode, U mode takes it.
    if (!vectorHasExtension(extensions, 'S')) {
        return PrivilegeMode::User;
    }

    // If S mode hasn't delegated the trap, it takes it.
    if (!(sdeleg & causeMask)) {
        return PrivilegeMode::Supervisor;
    }

    // M mode has delegated, and S mode has either also delegated or doesn't
    // exist, so U mode takes it.
    return PrivilegeMode::User;
}

template<typename XLEN_t>
TrapCause highestPriorityInterrupt(XLEN_t interruptsToService) {
    // The order in the spec is: MEI MSI MTI SEI SSI STI UEI USI UTI
    if (interruptsToService & (1 << RISCV::TrapCause::MACHINE_EXTERNAL_INTERRUPT)) {
        return RISCV::TrapCause::MACHINE_EXTERNAL_INTERRUPT;
    } else if (interruptsToService & (1 << RISCV::TrapCause::MACHINE_SOFTWARE_INTERRUPT)) {
        return RISCV::TrapCause::MACHINE_SOFTWARE_INTERRUPT;
    } else if (interruptsToService & (1 << RISCV::TrapCause::MACHINE_TIMER_INTERRUPT)) {
        return RISCV::TrapCause::MACHINE_TIMER_INTERRUPT;
    } else if (interruptsToService & (1 << RISCV::TrapCause::SUPERVISOR_EXTERNAL_INTERRUPT)) {
        return RISCV::TrapCause::SUPERVISOR_EXTERNAL_INTERRUPT;
    } else if (interruptsToService & (1 << RISCV::TrapCause::SUPERVISOR_SOFTWARE_INTERRUPT)) {
        return RISCV::TrapCause::SUPERVISOR_SOFTWARE_INTERRUPT;
    } else if (interruptsToService & (1 << RISCV::TrapCause::SUPERVISOR_TIMER_INTERRUPT)) {
        return RISCV::TrapCause::SUPERVISOR_TIMER_INTERRUPT;
    } else if (interruptsToService & (1 << RISCV::TrapCause::USER_EXTERNAL_INTERRUPT)) {
        return RISCV::TrapCause::USER_EXTERNAL_INTERRUPT;
    } else if (interruptsToService & (1 << RISCV::TrapCause::USER_SOFTWARE_INTERRUPT)) {
        return RISCV::TrapCause::USER_SOFTWARE_INTERRUPT;
    } else if (interruptsToService & (1 << RISCV::TrapCause::USER_TIMER_INTERRUPT)) {
        return RISCV::TrapCause::USER_TIMER_INTERRUPT;
    } else {
        // This indicates custom interrupts... TODO ?
        return RISCV::TrapCause::NONE;
    }
}

} // namespace RISCV
