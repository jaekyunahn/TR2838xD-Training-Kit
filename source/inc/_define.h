/*
 * _define.h
 *
 *  Created on: 2023. 3. 18.
 *      Author: ajg10
 */

#ifndef USER_INC__DEFINE_H_
#define USER_INC__DEFINE_H_

#include "F28x_Project.h"

#define CMD_BUF_SIZE    64
#define PATH_BUF_SIZE   64

#define NAME_TOO_LONG_ERROR     1
#define OPENDIR_ERROR           2

//  IPC target Address
#define CPU2_CPU    1
#define CPU2_CLA    2
#define CM          3

//  Bank0 Sector start addresses
#define FlashStartAddress           0x80000
#define Bzero_Sector0_start         0x80000
#define Bzero_Sector1_start         0x82000
#define Bzero_Sector2_start         0x84000
#define Bzero_Sector3_start         0x86000
#define Bzero_Sector4_start         0x88000
#define Bzero_Sector5_start         0x90000
#define Bzero_Sector6_start         0x98000
#define Bzero_Sector7_start         0xA0000
#define Bzero_Sector8_start         0xA8000
#define Bzero_Sector9_start         0xB0000
#define Bzero_Sector10_start        0xB8000
#define Bzero_Sector11_start        0xBA000
#define Bzero_Sector12_start        0xBC000
#define Bzero_Sector13_start        0xBE000
#define FlashEndAddress             0xBFFFF
//  Sector length in number of 32bits
#define Sector16KB_u32length   0x1000
#define Sector64KB_u32length   0x4000

//
//  CLA
//
struct CPU_CLA_DATA
{
    Uint16 flag;        //  packet label
    Uint32 output_case; //  결과물을 그대로 낼지. relu 처리 할지 결정
    Uint32 input_count; //  input data size 알림
    float32 bias;       //  필요시 bias 사용
    float32 input[256]; //  input data
    float32 weight[256];//  weight data
};

struct CLA_CPU_DATA
{
    Uint16 flag;        //  packet label
    float32 output;     //  CLA > CPU로 보낼 때 결과 사용
};

//  IPC
struct CPU_SHARE_DATA_MASTER_TO_SLAVE
{
    Uint16 flag;                //  packet label
    Uint32 target;         //  data 받을 놈
    Uint32 out_case;         //  output 처리 방식
    Uint32 input_count;         //  input data size 알림
    float32 bias[32];               //  필요시 bias 사용
    float32 input[256];         //  input data
    float32 weight[256];        //  weight data
};

struct CPU_SHARE_DATA_SLAVE_TO_MASTER
{
    Uint16 flag;        //  packet label
    Uint16 debug;        //  packet label
    float32 output[32];     //  CLA > CPU로 보낼 때 결과 사용
};

struct CPU_SHARE_DATA_buffer
{
    float32 cpu_input[256];         //  input data
    float32 cpu_weight[256];        //  cpu weight data
    float32 cla_input[256];         //  input data
    float32 cla_weight[256];        //  cpu weight data
};

union Converter_data {
    int32 int32;
    float32 float32;
    Uint16 array_2byte[2];
};

#endif /* USER_INC__DEFINE_H_ */
