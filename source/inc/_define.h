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

//
//  CLA
//
struct CPU_CLA_DATA
{
    Uint16 flag;        //  packet label
    Uint32 output_case; //  ������� �״�� ����. relu ó�� ���� ����
    Uint32 input_count; //  input data size �˸�
    float32 bias;       //  �ʿ�� bias ���
    float32 input[256]; //  input data
    float32 weight[256];//  weight data
};

struct CLA_CPU_DATA
{
    Uint16 flag;        //  packet label
    float32 output;     //  CLA > CPU�� ���� �� ��� ���
};

//  IPC
struct CPU_SHARE_DATA_MASTER_TO_SLAVE
{
    Uint16 flag;                //  packet label
    Uint32 target;         //  data ���� ��
    Uint32 out_case;         //  output ó�� ���
    Uint32 input_count;         //  input data size �˸�
    float32 bias[32];               //  �ʿ�� bias ���
    float32 input[256];         //  input data
    float32 weight[256];        //  weight data
};

struct CPU_SHARE_DATA_SLAVE_TO_MASTER
{
    Uint16 flag;        //  packet label
    Uint16 debug;        //  packet label
    float32 output[32];     //  CLA > CPU�� ���� �� ��� ���
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
