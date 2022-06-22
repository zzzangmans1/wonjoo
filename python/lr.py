import numpy as np
import matplotlib.pyplot as plt

# 선형 회귀 모델 실습

# 공부 시간 x와 성적 y의 넘파이 배열을 만듭니다.
x = np.array([2,4,6,8])
y = np.array([81,93,91,97])

# 데이터의 분포를 그래프로 나타냅니다.
plt.scatter(x, y)
plt.show()

# 기울기 a의 값과 절편 b의 값을 초기화합니다.
a = 0
b = 0

# 학습률을 정합니다.
lr = 0.03

# 몇 번 반복될지 설정합니다.
epochs = 2001

# x 값이 총 몇 개인지 셉니다.
n = len(x)

# 경사 하강법을 시작합니다.
for i in range(epochs): # 에포크 수만큼 반복합니다.
  y_pred = a * x + b  # 예측 값을 구하는 식입니다.
  error = y - y_pred  # 실제 값과 비교한 오차를 error로 놓습니다.

  a_diff = (2/n) * sum(-x * (error))  # 오차 함수를 a로 편미분한 값입니다.
  b_diff = (2/n) * sum(-(error))      # 오차 함수를 b로 편미분한 값입니다.

  a = a - lr * a_diff                 # 학습률을 곱해 기존의 a 값을 업데이트합니다.
  b = b - lr * b_diff                 # 학습률을 곱해 기존의 b 값을 업데이트합니다.

  if i % 100 == 0:                    # 100번 반복될 때마다 현재의 a 값, b 값을 출력합니다.
    print("epoch=%.f, 기울기=%.04f, 절편=%.04f" % (i, a, b))

# 앞서 구한 최종 a 값을 기울기, b 값을 y 절편에 대입해 그래프를 그립니다.
y_pred = a *  x + b

# 그래프를 출력합니다.
plt.scatter(x,y)
plt.plot(x, y_pred, 'r')
plt.show()
