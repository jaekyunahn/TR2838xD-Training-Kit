import sys
from contextlib import contextmanager
from io import StringIO
import tensorflow as tf
import struct
import re

dir_name = "./data/"

# 바이너리로 변환하여 파일에 쓰기
#with open("float_data.bin", "wb") as file:
#    binary_data = struct.pack('f', float_data)
#    file.write(binary_data)

@contextmanager
def capture_stdout():
    old_stdout = sys.stdout
    sys.stdout = StringIO()
    try:
        yield sys.stdout
    finally:
        sys.stdout = old_stdout

# 모델 로드
model = tf.keras.models.load_model("my_model.h5")

# 모델 요약 정보를 문자열로 가져오기
with capture_stdout() as stdout:
    model.summary()

summary_string = stdout.getvalue()
print(summary_string)

# 정규표현식을 사용하여 Output Shape 정보 추출
output_shapes = re.findall(r'\((\w+),\s*(\d+)\)', summary_string)

print(output_shapes)
print(type(output_shapes))

LayerCount=len(output_shapes)
print("Layer Count:",LayerCount)

ai_layer_count = len(output_shapes)

byte_count = 0

# 가장 큰 레이어를 기준으로 임시 버퍼 크기 결정하기 위한 max값
max_layer_size = 0

# model data 정보 
with open(dir_name + "config", "wb") as file:

    int32_format = struct.pack('i', int(ai_layer_count))
    file.write(int32_format)
    #print("int32_format:", int32_format)
    #byte_count = byte_count + 4

    #   정보 파일에 저장
    for y, layer_type_list in enumerate(output_shapes):
        print("----------------------------------------------------------------------")
        #print(str(layer_type_list))
        #print(layer_type_list)
        shape, size = layer_type_list
        print("Shape:", shape)
        print("Size:", size)
        #print("Shape type:", type(shape))
        #print("Size type:", type(int(size)))
        int32_format = struct.pack('i', int(size))
        file.write(int32_format)
        #print("int32_format:", int32_format)
        #byte_count = byte_count + 4
        if max_layer_size < int(size):
            max_layer_size = int(size)

# 모델 레이어 별 데이터 저장
# 레이어명 규칙은 히든n, 출력을 각각 hx, out 으로 지정하고 바이어스는 뒤에 _b, 가중치는 뒤에 _w라고 기입 한다.
hidden_layer_count = 0

# 재조립할 레이어 개수
remap_layerCount = (LayerCount - 1)
# 히든레이어 인덱스
remap_hidden_layer_count = 0
# 각 레이어 크기
remap_layerSize = [0] * (LayerCount - 1)

# 2차 배열 초기화
rows, cols = max_layer_size, max_layer_size
temp_wegBuffer = [[0.0] * cols for _ in range(rows)]
#temp_wegBuffer[0][0] = 1

print("rows size:", rows)
print("cols size:", cols)
print("temp_wegBuffer size:", len(temp_wegBuffer))

# 인덱싱
temp_perceptronIndex = 0
temp_perceptronInputIndex = 0

# 재조립시 사용할 변수
# 레이어당 퍼셉트론
temp_layerPerPerceptron = 0
# 퍼셉트론 당 input
temp_PerceptronPerInput = 0

hidden_layer_count = -1

for layer in model.layers:
    print("----------------------------------------------------------------------")
    weights = layer.get_weights()
    print("Layer 타입:", type(layer))
    print("Weights 타입:", type(weights))
    #print("Weights:", weights)
    
    # 히든레이어인지 출력레이어인지 구분하기 위한 값
    LayerCount = LayerCount - 1
    print("Layer count:", LayerCount)
    
    #with open(dir_name + "model_config.bin", "wb") as file:
    for i, weight_array in enumerate(weights):
        print(f"Weight 배열 {i}의 크기:", weight_array.shape)
        #print("Weight 배열 내용:")
        #print(weight_array)
        if i == 0 : # 가중치
            for x, weight_array_temp in enumerate(weight_array):
                temp_PerceptronPerInput = temp_PerceptronPerInput + 1
                temp_layerPerPerceptron = 0
                for count_temp in range(0,weight_array_temp.shape[0]):
                    temp_layerPerPerceptron = temp_layerPerPerceptron + 1
                    float_data = weight_array_temp[count_temp]
                    binary_data = struct.pack('f', float_data)
                    #file.write(binary_data)
                    #print("temp_perceptronIndex:", temp_perceptronIndex)
                    #print("temp_perceptronInputIndex:", temp_perceptronInputIndex)
                    temp_wegBuffer[temp_perceptronIndex][temp_perceptronInputIndex] = binary_data
                    temp_perceptronIndex = temp_perceptronIndex + 1
                    byte_count = byte_count + 4
                temp_perceptronInputIndex = temp_perceptronInputIndex + 1
                temp_perceptronIndex = 0
            temp_perceptronInputIndex = 0
            
            print("temp_PerceptronPerInput:", temp_PerceptronPerInput)
            print("temp_layerPerPerceptron:", temp_layerPerPerceptron)
            # 가중치 재 배치
            # 퍼셉트론 개수
            for x in range(0,temp_layerPerPerceptron):
                # 여기서 파일 생성
                if LayerCount == 0:
                    filename_string = "out_w" + str(x)
                else:
                    filename_string = "h" + str(hidden_layer_count) + "_w" + str(x)
                with open(dir_name + filename_string, "wb") as file:
                    # 퍼셉트론 input
                    for y in range(0,temp_PerceptronPerInput):
                        tmp=temp_wegBuffer[x][y]
                        #binary_data = struct.pack('f', tmp)
                        file.write(tmp)
                
            # 초기화
            temp_PerceptronPerInput = 0
            temp_layerPerPerceptron = 0
            
        elif i == 1: # 바이어스
            if LayerCount == 0:
                filename_string = "out_b"
            else:
                filename_string = "h" + str(hidden_layer_count) + "_b"
            with open(dir_name + filename_string, "wb") as file:
                for count in range(0,weight_array.shape[0]):
                    float_data = weight_array[count]
                    binary_data = struct.pack('f', float_data)
                    file.write(binary_data)
                    byte_count = byte_count + 4
                
    hidden_layer_count = hidden_layer_count + 1 
                
print("byte_count:"+str(byte_count) + "Byte")
# 필요에 따라 가중치를 전역 변수에 할당할 수 있습니다.
