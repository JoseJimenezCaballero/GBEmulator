
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

void instr_xor_a(); // AF

void instr_jp_a16();    // C3
void instr_prefix_cb(); // CB
void insr_call_a16();   // CD

void instr_ldh_a8_a(); // E0
void instr_ld_a16_a(); // EA

void instr_ldh_a_a8(); // F0
void instr_di();       // F3
void instr_ld_a_a16(); // FA
void instr_cp_d8();    // FE

// fucntion prototypes for CPU CB instructions go here:

void cb_instr_res_0_a(); // 87