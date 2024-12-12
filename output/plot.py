import os
import matplotlib.pyplot as plt

directory = 'output/'

out_files = [f for f in os.listdir(directory) if f.startswith('Out_') and f.endswith('_OMP')]

data = []

for out_file in out_files:
    file_path = os.path.join(directory, out_file)
    with open(file_path, 'r') as f:
        content = f.readlines()
        threads = int(content[0].split('=')[1].strip())
        exec_time = float(content[1].split('=')[1].strip().split()[0])
        data.append((threads, exec_time))

data.sort()

threads, exec_times = zip(*data)

baseline_time = exec_times[0]
speedup = [baseline_time / t for t in exec_times]

fig, ax1 = plt.subplots()

ax1.set_xlabel('Threads')
ax1.set_ylabel('Execution Time (seconds)', color='tab:blue')
ax1.plot(threads, exec_times, marker='o', color='tab:blue', label='Execution Time')
ax1.tick_params(axis='y', labelcolor='tab:blue')

ax2 = ax1.twinx()
ax2.set_ylabel('Speedup', color='tab:orange')
ax2.plot(threads, speedup, marker='o', color='tab:orange', label='Speedup')
ax2.tick_params(axis='y', labelcolor='tab:orange')

fig.tight_layout(rect=[0, 0, 1, 0.95])
plt.title('Performance Analysis')
plt.show()