import matplotlib.pyplot as plt
import numpy as np

def erlang_b(i, c):
    if c <= 0: return 1
    temp = i * erlang_b(i, c - 1)
    return temp / (c + temp)

def erlang_c(i, c):
    if i >= c: return 1.0
    E_b = erlang_b(i, c)
    return E_b / (1 - (i / c) * (1 - E_b))

def queue_len(i, c):
    if i >= c: return float('inf')
    E_c = erlang_c(i, c)
    return (i * E_c) / (c - i)

n = 93

plt.figure(figsize=(10, 6))
x = np.linspace(0, 350, 100)
plt.plot(x, [erlang_b(i, 2 * n) for i in x])
plt.xlabel("Интенсивность нагрузки")
plt.ylabel("Вероятность блокировки")
plt.grid()
plt.show()

plt.figure(figsize=(10, 6))
x = np.linspace(1, 250, 100)
plt.plot(x, [erlang_b(n, i) for i in x])
plt.xlabel("Число устройств")
plt.ylabel("Вероятность блокировки")
plt.grid()
plt.show()

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
x = np.linspace(0, 2 * n - 30, 100)
ax1.plot(x, [erlang_c(i, 2 * n) for i in x])
ax1.set_xlabel("Интенсивность нагрузки")
ax1.set_ylabel("Вероятность ожидания")
ax1.grid()
ax2.plot(x, [queue_len(i, 2 * n) for i in x])
ax2.set_xlabel("Интенсивность нагрузки")
ax2.set_ylabel("Средняя длина очереди")
ax2.grid()
plt.show()

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
x = np.linspace(n + 1, 250, 100)
ax1.plot(x, [erlang_c(n, i) for i in x])
ax1.set_xlabel("Число устройств")
ax1.set_ylabel("Вероятность ожидания")
ax1.grid()
ax2.plot(x, [queue_len(n, i) for i in x])
ax2.set_xlabel("Число устройств")
ax2.set_ylabel("Средняя длина очереди")
ax2.grid()
plt.show()
