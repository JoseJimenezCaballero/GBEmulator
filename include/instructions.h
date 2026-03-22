
#include <common.h>
#include <cpu.h>

// will contain the map definition for the opcodes and the fucntions too for
// each opcode

// defining a function pointer type for cpu instructions
typedef void (*InstructionFunc)();
// same but for cb instructions
typedef void (*InstructionFuncCB)();

// instruction lookup table(one per opcode)
extern InstructionFunc instruction_table[256];
// same but for cb
extern InstructionFuncCB instruction_tableCB[256];

// to init instruction table
void init_instruction_table();
void init_instruction_tableCB();

// fucntion prototypes for CPU instructions go here:

void instr_nop();       // 00
void ld_bc_d16();       // 01
void instr_ld_bc_a();   // 02
void inc_bc();          // 03
void inc_b();           // 04
void instr_dec_b();     // 05
void instr_ld_b_d8();   // 06
void instr_rlca();      // 07
void instr_ld_a16_sp(); // 08
void instr_add_hl_bc(); // 09
void instr_ld_a_bc();   // 0A
void instr_dec_bc();    // 0B
void instr_inc_c();     // 0C
void instr_dec_c();     // 0D
void instr_ld_c_d8();   // 0E
void instr_rrca();      // 0F

void stop_0();        // 10
void ld_de_d16();     // 11
void ld_de_a();       // 12
void instr_inc_de();  // 13
void instr_inc_d();   // 14
void instr_dec_d();   // 15
void instr_ld_d_d8(); // 16
void instr_rla();     // 17
void jr_r8();         // 18
void add_hl_de();     // 19
void instr_ld_a_de(); // 1A
void instr_dec_de();  // 1B
void instr_inc_e();   // 1C
void instr_dec_e();   // 1D
void instr_ld_e_d8(); // 1E
void instr_rra();     // 1F

void instr_nz_r8();       // 20
void instr_ld_hl_d16();   // 21
void instr_ld_hlplus_a(); // 22
void instr_inc_hl();      // 23
void instr_inc_h();       // 24
void instr_dec_h();       // 25
void instr_ld_h_d8();     // 26
void instr_daa();         // 27
void instr_jr_z_r8();     // 28
void add_hl_hl();         // 29
void instr_ld_a_hlplus(); // 2A
void instr_dec_hl();      // 2B
void instr_inc_l();       // 2C
void instr_dec_l();       // 2D
void instr_ld_l_d8();     // 2E
void instr_cpl();         // 2F

void instr_nc_r8();        // 30
void instr_ld_sp_d16();    // 31
void instr_ld_hlminus_a(); // 32
void instr_inc_sp();       // 33
void instr_inc_8bithl();   // 34
void instr_dec_hl8bit();   // 35
void instr_ld_hl_d8();     // 36
void instr_scf();          // 37
void instr_jr_c_r8();      // 38
void instr_add_hl_sp();    // 39
void instr_ld_a_hlminus(); // 3A
void instr_dec_sp();       // 3B
void instr_inc_a();        // 3C
void instr_dec_a();        // 3D
void ld_a_d8();            // 3E
void instr_ccf();          // 3F

void instr_ld_b_b();  // 40
void instr_ld_b_c();  // 41
void instr_ld_b_d();  // 42
void instr_ld_b_e();  // 43
void instr_ld_b_h();  // 44
void instr_ld_b_l();  // 45
void instr_ld_b_hl(); // 46;
void instr_ld_b_a();  // 47
void instr_ld_c_b();  // 48
void instr_ld_c_c();  // 49
void instr_ld_c_d();  // 4A
void instr_ld_c_e();  // 4B
void instr_ld_c_h();  // 4C
void instr_ld_c_l();  // 4D
void instr_ld_c_hl(); // 4E
void instr_ld_c_a();  // 4F

void instr_ld_d_b();  // 50
void instr_ld_d_c();  // 51
void instr_ld_d_d();  // 52
void instr_ld_d_e();  // 53
void instr_ld_d_h();  // 54
void instr_ld_d_l();  // 55
void instr_ld_d_hl(); // 56
void instr_ld_d_a();  // 57
void instr_ld_e_b();  // 58
void instr_ld_e_c();  // 59
void instr_ld_e_d();  // 5A
void instr_ld_e_e();  // 5B
void instr_ld_e_h();  // 5C
void instr_ld_e_l();  // 5D
void instr_ld_e_hl(); // 5E
void instr_ld_e_a();  // 5F

void instr_ld_h_b();  // 60
void instr_ld_h_c();  // 61
void instr_ld_h_d();  // 62
void instr_ld_h_e();  // 63
void instr_ld_h_h();  // 64
void instr_ld_h_l();  // 65
void instr_ld_h_hl(); // 66
void instr_ld_h_a();  // 67
void instr_ld_l_b();  // 68
void instr_ld_l_c();  // 69
void instr_ld_l_d();  // 6A
void instr_ld_l_e();  // 6B
void instr_ld_l_h();  // 6C
void instr_ld_l_l();  // 6D
void instr_ld_l_hl(); // 6E
void instr_ld_la();   // 6F

void instr_ld_hl();   // 70-75, 77
void instr_halt();    // 76
void instr_ld_ab();   // 78
void instr_ld_ac();   // 79
void instr_ld_ad();   // 7A
void instr_ld_ae();   // 7B
void instr_ld_ah();   // 7C
void instr_ld_al();   // 7D
void instr_ld_a_hl(); // 7E
void instr_ld_aa();   // 7F

void instr_add_ab();   // 80
void instr_add_ac();   // 81
void instr_add_ad();   // 82
void instr_add_ae();   // 83
void instr_add_ah();   // 84
void instr_add_al();   // 85
void instr_add_a_hl(); // 86
void instr_add_aa();   // 87
void instr_adc_ab();   // 88
void instr_adc_ac();   // 89
void instr_adc_ad();   // 8A
void instr_adc_ae();   // 8B
void instr_adc_ah();   // 8C
void instr_adc_al();   // 8D
void instr_adc_a_hl(); // 8E
void instr_adc_aa();   // 8F

void instr_sub_ab();   // 90
void instr_sub_ac();   // 91
void instr_sub_ad();   // 92
void instr_sub_ae();   // 93
void instr_sub_ah();   // 94
void instr_sub_al();   // 95
void instr_sub_a_hl(); // 96
void instr_sub_aa();   // 97
void instr_sbc_ab();   // 98
void instr_sbc_ac();   // 99
void instr_sbc_ad();   // 9A
void instr_sbc_ae();   // 9B
void instr_sbc_ah();   // 9C
void instr_sbc_al();   // 9D
void instr_sbc_a_hl(); // 9E
void instr_sbc_aa();   // 9F

void instr_and_ab();   // A0
void instr_and_ac();   // A1
void instr_and_ad();   // A2
void instr_and_ae();   // A3
void instr_and_ah();   // A4
void instr_and_al();   // A5
void instr_and_a_hl(); // A6
void instr_and_aa();   // A7

void instr_xor_ab();   // A8
void instr_xor_ac();   // A9
void instr_xor_ad();   // AA
void instr_xor_ae();   // AB
void instr_xor_ah();   // AC
void instr_xor_al();   // AD
void instr_xor_a_hl(); // AE
void instr_xor_a();    // AF

void instr_jp_a16();    // C3
void instr_prefix_cb(); // CB
void instr_call_a16();  // CD

void instr_ldh_a8_a(); // E0
void instr_ld_a16_a(); // EA

void instr_ldh_a_a8(); // F0
void instr_di();       // F3
void instr_ld_a_a16(); // FA
void instr_cp_d8();    // FE

// fucntion prototypes for CPU CB instructions go here:

void cb_instr_res_0_a(); // 87