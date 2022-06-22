import numpy as np
import matplotlib.pyplot as plt

# 다중 선형 회귀 모델

# 공부 시간 x1과 과외 시간 x2, 성적 y의 넘파이 배열을 만듭니다.
x1 = np.array([2,4,6,8])
x2 = np.array([0, 4, 2 , 3])
y = np.array([81, 93, 91, 97])

# 데이터의 분포를 그래프로 나타냅니다.
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter3D(x1, x2, y)
plt.show()

# 기울기 a의 값과 절편 b의 값을 초기화합니다.
a1 = 0
a2 = 0
b = 0

# 학습률을 정합니다.
lr = 0.01

# 몇 번 반복될지 설정합니다.
epochs = 2001

# x 값이 총 몇 개인지 셉니다. x1과 x2의 수가 같으므로 x1만 세겠습니다.
n = len(x1)

# 경사 하강법을 시작합니다.
for i in range(epochs):         # 에포크 수만큼 반복합니다.
  y_pred = a1 * x1 + a2 * x2 + b  # 예측 값을 구하는 식을 세웁니다.
  error = y - y_pred              # 실제 값과 비교한 오차를 error로 놓습니다.

  a1_diff = (2/n) * sum(-x1 * (error))  # 오차 함수를 a1로 편미분한 값입니다.
  a2_diff = (2/n) * sum(-x2 * (error))  # 오차 함수를 a2로 편미분한 값입니다.
  b_diff = (2/n) * sum(-(error))        # 오차 함수를 b로 편미분한 값입니다.

  a1 = a1 - lr * a1_diff                # 학습률을 곱해 기존의 a1 값을 업데이트합니다/
  a2 = a2 - lr * a2_diff                # 학습률을 곱해 기존의 a2 값을 업데이트합니다.
  b = b - lr * b_diff                   # 학습률을 곱해 기존의 b 값을 업데이트합니다.

  if i % 100 == 0:  # 100번 반복될 때마다 출력
    print("epoch=%.f, 기울기1=%.04f, 기울기2=%.04f, 절편=%.04f" % (i, a1, a2, b))

# 실제 점수와 예측된 점수를 출력합니다.
print("실제 점수: ", y)
print("예측 점수: ",y_pred)
