#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"


void thumb_lsl(struct GBA_Core* gba, uint16_t opcode);
void thumb_lsr(struct GBA_Core* gba, uint16_t opcode);
void thumb_asr(struct GBA_Core* gba, uint16_t opcode);

void thumb_cmp_rd_rs(struct GBA_Core* gba, uint16_t opcode);
void thumb_mov_rd_rs(struct GBA_Core* gba, uint16_t opcode);
void thumb_mov_hd_rs(struct GBA_Core* gba, uint16_t opcode);
void thumb_mov_hd_hs(struct GBA_Core* gba, uint16_t opcode);

void thumb_bx_rs(struct GBA_Core* gba, uint16_t opcode);
void thumb_bx_hs(struct GBA_Core* gba, uint16_t opcode);
void thumb_cond(struct GBA_Core* gba, uint16_t opcode);

void thumb_str_imm(struct GBA_Core* gba, uint16_t opcode);

void thumb_pop(struct GBA_Core* gba, uint16_t opcode);
void thumb_push(struct GBA_Core* gba, uint16_t opcode);
void thumb_stmia(struct GBA_Core* gba, uint16_t opcode);
void thumb_ldmia(struct GBA_Core* gba, uint16_t opcode);

void thumb_add_imm(struct GBA_Core* gba, uint16_t opcode);
void thumb_sub_imm(struct GBA_Core* gba, uint16_t opcode);
void thumb_cmp_imm(struct GBA_Core* gba, uint16_t opcode);
void thumb_mov_imm(struct GBA_Core* gba, uint16_t opcode);

void thumb_bic(struct GBA_Core* gba, uint16_t opcode);
void thumb_add_reg(struct GBA_Core* gba, uint16_t opcode);
void thumb_sub_reg(struct GBA_Core* gba, uint16_t opcode);

void thumb_pc_relative_load(struct GBA_Core* gba, uint16_t opcode);

void thumb_bl_high(struct GBA_Core* gba, uint16_t opcode);
void thumb_bl_low(struct GBA_Core* gba, uint16_t opcode);

#ifdef __cplusplus
}
#endif
