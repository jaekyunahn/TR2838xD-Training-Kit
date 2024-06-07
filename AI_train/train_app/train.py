import tensorflow as tf
from tensorflow.keras.datasets import mnist
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Flatten, Dense, Conv2D, MaxPooling2D, Dropout

# MNIST 데이터셋 로드
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()

# 데이터 전처리
train_images = train_images / 200.0
test_images = test_images / 200.0

# 모델 구축
model = Sequential([
    Flatten(input_shape=(28, 28)),
    Dense(256, activation='relu'),
    Dense(128, activation='relu'),
    Dense(64, activation='relu'),
    Dense(32, activation='relu'),
    Dense(10, activation='softmax')
])

# 모델 컴파일
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])

# 모델 학습
model.fit(train_images, train_labels, epochs=10, batch_size=16, validation_split=0.2)

# 모델 평가
test_loss, test_acc = model.evaluate(test_images, test_labels)

# 모델을 h5 형식으로 저장
model.save('my_model.h5')


# 저장된 모델 파일 불러오기
#loaded_model = tf.keras.models.load_model('my_model.h5')

# 모델 구조 출력
#loaded_model.summary()




