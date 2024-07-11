/*
 * app_ai.c
 *
 *  Created on: 2024. 5. 18.
 *      Author: User
 */
#include "main.h"
#ifdef APP
//  �ӽ÷� ������ ��������
#define MAX_TEMP_LAYER_COUNT    1024
//  SD-Card���� �ѹ��� ���� ������ ���� ũ��
#define SDCARD_READ_BUFFER      8192

//  Ai ���̾���
#define LAYERCOUNT              6

//  �� ������ �� �ۼ�Ʈ�� ����
#define INPUT_LAYER             784
#define HIDDEN0_LAYER           256
#define HIDDEN1_LAYER           256
#define HIDDEN2_LAYER           256
#define HIDDEN3_LAYER           64
#define OUTPUT_LAYER            10

//  ���̾��� ����
#define HEDDEN_LAYER_TYPE   0
#define OUTPUT_LAYER_TYPE   1

//  SD card buffer
Uint16 read_data_sdcard_buffer[SDCARD_READ_BUFFER] = {0,};
#pragma DATA_SECTION(read_data_sdcard_buffer, "ramgs0");

//  ����� ���̾� ����
float32 input_layer[INPUT_LAYER] = {0.0,};
#pragma DATA_SECTION(input_layer, "ramgs0");
float32 output_layer[OUTPUT_LAYER] = {0.0,};
#pragma DATA_SECTION(output_layer, "ramgs0");

//  �߰� ó�� ������ - �ӽ� ���̾�
float32 temp_in_layer[MAX_TEMP_LAYER_COUNT] = {0.0,};
#pragma DATA_SECTION(temp_in_layer, "ramgs0");
float32 temp_out_layer[MAX_TEMP_LAYER_COUNT] = {0.0,};
#pragma DATA_SECTION(temp_out_layer, "ramgs0");

//  ����ġ �� �ӽ� ���̾ >> �޸𸮰� �������̶� �ۼ�Ʈ�� 1�� ���� �� ������ SD-card���� �ҷ����� ������ �ӽ������� ������ ����ġ�� ���̾�� ���̾
//  Dense Layer�� ����� �ۼ�Ʈ�� ���� ���̾�� 1���� �ο����� ����
float32 temp_weight[MAX_TEMP_LAYER_COUNT] = {0.0,};
#pragma DATA_SECTION(temp_weight, "ramgs0");
float32 temp_bias[MAX_TEMP_LAYER_COUNT] = {0.0,};
#pragma DATA_SECTION(temp_bias, "ramgs0");

//  CPU2�� ��ȯ�� ������

struct CPU_SHARE_DATA_MASTER_TO_SLAVE set_cpu2_cpu;
#pragma DATA_SECTION(set_cpu2_cpu, "ramgs0");
struct CPU_SHARE_DATA_MASTER_TO_SLAVE set_cpu2_cla;
#pragma DATA_SECTION(set_cpu2_cla, "ramgs0");
struct CPU_SHARE_DATA_SLAVE_TO_MASTER get_cpu2_cpu;
#pragma DATA_SECTION(get_cpu2_cpu, "ramgs0");
struct CPU_SHARE_DATA_SLAVE_TO_MASTER get_cpu2_cla;
#pragma DATA_SECTION(get_cpu2_cla, "ramgs0");


//  ���� ���� �ּ� ����
Uint16 read_file_address[32] = {0x00,};

//  ���� �ð� �������� ����
Uint16 timer_flag = 0;
Uint16 timer_count = 0;

//  �� �ھ� ���� �÷���
Uint16 cpu1_cla_end = 0;
Uint16 cpu2_end = 0;

//  ���̳ʸ�ȭ �� �����͵��� �迭���� ���� �����ͷ� �����ϱ� ���� unionó��
union Converter_data convertBuffer;

//  ó������ ���̾� �ε���
Uint16 Layer_index = 0;
void app_ai(void) {
    Uint32 layer_count = 0;
    Uint16 x = 0, y = 0;

    //  �ӽ� ����
    Uint16 temp[4];
    Uint16 float_data_string[16];

    //  �ӽ� �ε���
    Uint16 temp_index = 0;

    //  ���� ���� �ҷ����� �ε���
    Uint16 load_exmaple_index = 0;

    //  init buffer
    memset(read_data_sdcard_buffer,0x00,sizeof(read_data_sdcard_buffer));
    memset(read_file_address,0x00,sizeof(read_file_address));
    memset(input_layer,0.0,sizeof(input_layer));
    memset(output_layer,0.0,sizeof(output_layer));
    memset(temp_in_layer,0.0,sizeof(temp_in_layer));
    memset(temp_out_layer,0.0,sizeof(temp_out_layer));
    memset(temp_weight,0.0,sizeof(temp_weight));
    memset(temp_bias,0.0,sizeof(temp_bias));

    //  config file
    int16 res;
    memset(read_file_address, 0x00, sizeof(read_file_address));
    sprintf(read_file_address,"/home/data/config");
    res = read_file(read_file_address, read_data_sdcard_buffer, sizeof(read_data_sdcard_buffer), 0);

    //  config file �о ����� ������ �ܾ������ Ȯ��
    memcpy(temp,read_data_sdcard_buffer + temp_index,sizeof(temp));
    convert_c28_to_x86(convertBuffer.array_2byte, temp);
    layer_count = convertBuffer.int32;
    temp_index = temp_index + 4;

    UARTprintf("layer_count=%d\n",(Uint16)layer_count);

    //
    for(load_exmaple_index = 0;load_exmaple_index < 1 ;load_exmaple_index++) {
        //  <-- Timer0 initiation
        timer_count = 0;
        timer_flag = 1;
        //  Timer0 initiation -->
        //  ���� �б�
        memset(read_file_address, 0x00, sizeof(read_file_address));
        sprintf(read_file_address,"/home/ex/%d.in", load_exmaple_index);
        res = read_file(read_file_address, read_data_sdcard_buffer, sizeof(read_data_sdcard_buffer), 0);
        //  input_layer
        for (x = 0; x < 784 ; x++) {
            input_layer[x] = (float32)read_data_sdcard_buffer[x];
        }
#if 1
        //  �Էµ����� Ȯ��
        UARTprintf("----------------------------------------------------\n");
        for (y = 0; y < 28 ; y++) {
            for (x = 0; x < 28 ; x++) {
                UARTprintf("[%1d]",read_data_sdcard_buffer[(y*28)+x]);
            }
            scia_write('\n');
        }
#endif
        //  input data�� layer input buffer�� ����
        memcpy(temp_in_layer, input_layer, sizeof(input_layer));
        //  ���������� Input Layer�� ���� ó�� �� �� �����Ƿ� ����
        for(Layer_index = 0; Layer_index < LAYERCOUNT - 1; Layer_index++) {
            switch(Layer_index) {
            case 0: dence_layer( HEDDEN_LAYER_TYPE, Layer_index, INPUT_LAYER, HIDDEN0_LAYER ); break;
            case 1: dence_layer( HEDDEN_LAYER_TYPE, Layer_index, HIDDEN0_LAYER, HIDDEN1_LAYER ); break;
            case 2: dence_layer( HEDDEN_LAYER_TYPE, Layer_index, HIDDEN1_LAYER, HIDDEN2_LAYER ); break;
            case 3: dence_layer( HEDDEN_LAYER_TYPE, Layer_index, HIDDEN2_LAYER, HIDDEN3_LAYER ); break;
            case 4: dence_layer( OUTPUT_LAYER_TYPE, Layer_index, HIDDEN3_LAYER, OUTPUT_LAYER ); break;
            }
        }

        // ó�� ��� ���� ���
        Uint16 max_class = 0;
        float32 max_probability = output_layer[0];
        for (x = 1; x < OUTPUT_LAYER; x++) {
            sprintf(float_data_string,"%f",output_layer[x]);
            UARTprintf("%d[%s]\n",x , float_data_string);
            if (output_layer[x] > max_probability) {
                max_class = x;
                max_probability = output_layer[x];
            }
        }
        //  <-- Timer stop
        timer_flag = 0;
        //  Timer stop -->
        sprintf(float_data_string,"%f",max_probability);
        UARTprintf("    max:%d[%s]\n",max_class , float_data_string);
        UARTprintf("    time=%dms\n",timer_count);
        scia_write('\n');
    }

    //
    // Return success.
    //
    set_run_flag(0);
    return ;
}

//  �л� ó�� ũ��
Uint16 Distributed_processing_size = 0;
//  �Ϲ����� ���̾�, input, output, bias, weight, input_size, output_size
void dence_layer( Uint16 layer_case, Uint16 layer_index, Uint16 input_layer_count, Uint16 output_layer_count ) {
    //  �л� ó�� ���,
    //  0 : �ھ� �� 1�� �ۼ�Ʈ��
    //  1 : 1�� �ۼ�Ʈ�п� �� �ھ� ���
    Uint16 Distributed_processing_flag = 0;

    //  output perceptron ���� ��� ó������ ����.
    //  �켱 �ִ� 2����(�ھ�� 1��) >> �켱 input ������ ����Ͽ� �л� �� ���� ���ÿ� �������� ó���� �� �ֵ��� ���� �� ��
    Uint16 jump_index_size = 0;

    //Uint16 layer_temp_index = 0;
    int16 res;
    //Uint16 temp[4];
    Uint16 x = 0, y = 0;

#if 1
    memset(read_file_address, 0x00, sizeof(read_file_address));
    if(layer_case == HEDDEN_LAYER_TYPE)
        sprintf(read_file_address,"/home/data/h%d_b",layer_index);  //layer_case + layer_index
    if(layer_case == OUTPUT_LAYER_TYPE)
        sprintf(read_file_address,"/home/data/out_b");  //layer_case
    res = read_file(read_file_address, read_data_sdcard_buffer, sizeof(read_data_sdcard_buffer), 0);
#endif

    //  ���� ���̾��� bias�� �о� ���� �κ�
    convertBuffer_charTofloat(read_data_sdcard_buffer, temp_bias, output_layer_count);

    Distributed_processing_size = input_layer_count / 4;
    Distributed_processing_flag = 1;

    //  ���̾� ���� �ۼ�Ʈ�� �ε���
    for(x = 0 ; x < output_layer_count ; x = x + 1 ) {
        //  ���̾ �Է�
        temp_out_layer[x] = temp_bias[x];

        //  ����ġ �о����
#if 1
        memset(read_file_address, 0x00, sizeof(read_file_address));
        if(layer_case == HEDDEN_LAYER_TYPE){
            sprintf(read_file_address,"/home/data/h%d_w%d",layer_index,x);
        }
        if(layer_case == OUTPUT_LAYER_TYPE){
            sprintf(read_file_address,"/home/data/out_w%d",x);
        }
        res = read_file(read_file_address, read_data_sdcard_buffer, sizeof(read_data_sdcard_buffer), 0);
#endif
        //
        convertBuffer_charTofloat(read_data_sdcard_buffer, temp_weight, input_layer_count);
#if 1
        //----------------------------------------------------------------------------------------------------------------------------------------------------------------
        //  data �л�   Distributed_processing_size
        //  CPU1.CLA (input + weight)
        memcpy(cpu_to_cla.input, temp_in_layer + Distributed_processing_size * 1, sizeof(float32)*Distributed_processing_size);
        memcpy(cpu_to_cla.weight, temp_weight + Distributed_processing_size * 1, sizeof(float32)*Distributed_processing_size);
        cpu_to_cla.input_count = Distributed_processing_size;

        //  CPU2.CPU (input + weight)
        memcpy(shareBuffer.cpu_input, temp_in_layer + Distributed_processing_size * 2, sizeof(float32)*Distributed_processing_size);
        memcpy(shareBuffer.cpu_weight, temp_weight + Distributed_processing_size * 2, sizeof(float32)*Distributed_processing_size);
        //  CPU2.CLA (input + weight)
        memcpy(shareBuffer.cla_input, temp_in_layer + Distributed_processing_size * 3, sizeof(float32)*Distributed_processing_size);
        memcpy(shareBuffer.cla_weight, temp_weight + Distributed_processing_size * 3, sizeof(float32)*Distributed_processing_size);
        cpu1_to_cpu2.input_count = Distributed_processing_size;

        //  share memory contorl : cpu2
        share_cpu2_mode();

        //----------------------------------------------------------------------------------------------------------------------------------------------------------------
        //  ��Ƽ�ھ� ó�� ����
        CLA_start();
        cpu1_to_cpu2.flag = 1;

        //----------------------------------------------------------------------------------------------------------------------------------------------------------------
        //  CPU ó�� ����
        for(y = 0 ; y < Distributed_processing_size ; y=y+1 ){
            temp_out_layer[x] = temp_out_layer[x] + (temp_in_layer[y] * temp_weight[y]);
        }

        //----------------------------------------------------------------------------------------------------------------------------------------------------------------
        //  CLA ó�� ���
        while(1) {
            CLA_end();
            CPU2_end();
            if((cpu1_cla_end == 1)&&(cpu2_end == 1)) {
                clear_ipc_cpu1_to_cpu2();
                clear_ipc_cpu2_to_cpu1();
                //  IPC flag reset
                cpu1_cla_end = 0;
                cpu2_end = 0;
                //  share memory contorl : cpu1
                share_cpu1_mode();
                break;
            }
        }
        temp_out_layer[x] = temp_out_layer[x] + cla_to_cpu.output + cpu2_to_cpu1.output[0] + cpu2_to_cpu1.output[1];
#else
        for(y = 0 ; y < input_layer_count ; y=y+1 ){
            temp_out_layer[x] = temp_out_layer[x] + (temp_in_layer[y] * temp_weight[y]);
        }
#endif
        if(layer_case == HEDDEN_LAYER_TYPE){
            temp_out_layer[x] = relu(temp_out_layer[x]);
        }
    }

    if(layer_case == HEDDEN_LAYER_TYPE){
        UARTprintf("next layer\n");
        memcpy(temp_in_layer,temp_out_layer,sizeof(temp_out_layer));
    }
    if(layer_case == OUTPUT_LAYER_TYPE){
        UARTprintf("softmax\n");
        softmax(temp_out_layer, output_layer, OUTPUT_LAYER);
    }
}

//  send CLA processing start
void CLA_start(void) {
    //  CPU1.CLA start
    asm("  IACK  #0x0001");
    asm("  RPT #3 || NOP");
}

//  check CLA end Flag
void CLA_end(void) {
    //  CPU1.CLA end
    if (Cla1Regs.MIRUN.bit.INT1 != 1) {
        cpu1_cla_end = 1;
    }
}

//
//  SD-card���� ���� �����͸� float32 ���·� �����ϴ� �Լ�
//
void convertBuffer_charTofloat(Uint16 *source, float32 *output, Uint16 count) {
    Uint16 x = 0;
    Uint16 temp_index = 0;
    Uint16 output_index = 0;
    union Converter_data tmp_convertBuffer;
    for(x = 0 ; x < count ; x++) {
        convert_c28_to_x86(tmp_convertBuffer.array_2byte, source + temp_index);
        output[output_index] = tmp_convertBuffer.float32;
        temp_index = temp_index + 4; output_index++;
    }
}

//  Send CPU2 Processing Start
void CPU2_start(void) {
    cpu1_to_cpu2.flag = 1;
}

//  check CPU2 End Flag
void CPU2_end(void) {
    if(cpu2_to_cpu1.flag == 1) {
        cpu2_end = 1;
    }
}

#pragma CODE_SECTION(ai_tick_timer, ".TI.ramfunc");
void ai_tick_timer(void) {
    if (timer_flag == 1)
        timer_count++;
}

#endif

