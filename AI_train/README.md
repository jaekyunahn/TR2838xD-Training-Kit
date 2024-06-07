# TR2838xD-Training-Kit
싱크웍스 TR2838xD-Training-Kit Base project

  train_app 이 폴더는 AI 모델학습하는 부분으로 train.py를 통해 MNIST 데이터를 학습하여 my_model.h5를 생성, 이후 가중치만 뽑아서 정리하는 model_extract_sizeDiv.py로 구성되어 있습니다.
  학습이 끝난 후 모델파일에서 가중치를 추출하면 data폴더에 여러 파일들이 만들어지게 되는데요. 이들은 4바이트 크기 소수점인 float 데이터들의 row형태이며 각 파일명의 의미는 다음과 같습니다
  
  히든레이어 파일. h<히든레이어번호>_<w(가중치) 또는 b(바이어스)>
  출력레이어 파일. out_<w(가중치) 또는 b(바이어스)>

  여기서 추출 된 모델 파일들은 SD-Card에 home/data/ 에 넣어주면 됩니다. 
  
  ex_app은 손 글씨를 이진화 시켜서 파일로 만드는 앱으로 data 폴더에 x.in으로 출력 된 파일을 sd-card의 home/ex/ 부분에 넣어주면 ai 실행 시 불러와서 분류 작업을 하게 되는 구조로 되어 있습니다.