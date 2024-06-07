# TR2838xD-Training-Kit
싱크웍스 TR2838xD-Training-Kit Base 

	이 폴더는 싱크웍스 TR2838xD-Training-Kit( TMS320F28384D 버전 ) 기준으로 작성된 프로젝트입니다.
	
	개발 환경은 다음과 같이 준비하였습니다.
	
	CCS version : 12.6
	compiler version : 22.6.1 LTS
	
	프로젝트 소프트웨어의 주요 내용은 다음과 같습니다.
	
	1. FreeRTOS : V11.1
	2. Fatfs : R0.15
	3. AI (MNIST 분류) : DNN 계열 자체 소스코드구현 및 라이브러리 사용 (일부 알고리즘에 대해 라이브러리 제작 시험)
		input layer 	: 784	(Relu)
		hidden 0 layer 	: 512   (Relu)
		hidden 1 layer 	: 256   (Relu)
		hidden 2 layer 	: 128   (Relu)
		hidden 3 layer 	: 64    (Relu)
		hidden 4 layer 	: 32    (Relu)
		hidden 5 layer 	: 16	(Relu)
		output layer 	: 10	(softmax)

	주요 폴더 트리는 다음과 같습니다.
	
		cla : CLA에 동작 시킬 소스코드 / 헤더파일 
		cmd : 각종 CMD 파일
		driver / driverlib : TI에서 배포한 드라이버 와 드라이버라이브러리 소스코드 / 헤더파일 
		inc : 유저가 작성한 헤더파일 
		kernel : FreeRTOS에서 제공하거나 유저가 일부 수정 또는 추가한 소스코드 / 헤더파일 
		lib : TI나 유저가 작성한 라이브러리와 해당 헤더파일
		src : 유저가 작성한 소스코드파일
		tergetConfig : 에뮬레이터에 동작시킬 정보 파일
		third_party : 서드파티에서 제공하거나 유저가 일부 수정 또는 추가한 소스코드 / 헤더파일
		TI_Util : TI에서 제공한 유용한 기능을 포함한 소스코드 / 헤더파일
		
	여기서 주로 건드리는게 cla, inc, src, lib, kernel 나 third_party 일부, TI_Util 수정 정도가 되고 cmd / driver / driverlib 이쪽은 거의 건드릴 필요가 없을겁니다.
	
	추가로 메모리맵 정보 가지고 있는 cmd들도 상황에 따라 조정하시길 권장 드립니다. Ai와 freeRTOS 사용하면서 일부 RAM 영역이 엄청 빡빡하게 할당 된 상태입니다.
	
	AI는 훈련 기능이 없고 분류만 동작합니다. 훈련 자체는 처리과정이 복잡하고 시간이 걸려 우선 컴퓨터에서 텐서플로우로 학습 시키고 모델만 가져오는게 방식을 취하게 됩니다. 추가로 모델 자체도 현 MCU RAM 고려하면 무거운 편이라 미리 읽지 못하고 SD-Card에서 일일이 꺼내는 실정입니다. 이건 SDRAM을 추가 하지 않는 이상 어쩔수 없고... 학습 관련 코드는 상위 train에 배포하였습니다.