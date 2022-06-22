import numpy as np

# 최소 제곱법

# 공부한 시간과 점수를 각각 x, y라는 이름의 넘파이 배열로 만듭니다.
x = np.array([2, 4, 6, 8])
y = np.array([81, 93, 91, 97])

# x의 평균값을 구합니다.
mx = np.mean(x)

# y의 평균값을 구합니다.
my = np.mean(y)

# 출력으로 확인합니다.
print("x의 평균값: ", mx)
print("y의 평균값: ", my)

# 기울기 공식의 분모 부분
divisor = sum([(i-mx)**2 for i in x])

# 기울기 공식의 분자 부분
def top(x, mx, y, my):
  d = 0
  for i in range(len(x)):
    d += (x[i] - mx) * (y[i] - my)
  return d
dividend = top(x, mx, y, my)

# 출력으로 확인
print("분모: ", divisor)
print("분자: ", dividend)

# 기울기 a를 구하는 공식
a = dividend / divisor

# y 절편 b를 구하는 공식
b = my - (mx*a)

# 출력으로 확인
print("기울기 a = ", a)
print("y 절편 b = ", b)
