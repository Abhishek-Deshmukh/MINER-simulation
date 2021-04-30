import numpy as np
import matplotlib.pyplot as plt


def less_than(arr, lim):
    return arr[np.where(arr > lim)]


lower_limit = 0.1


plt.clf()
fig, ax = plt.subplots()
ax.semilogx()


# importing data
with open("./with_wall_30/Energy.txt") as f:
    energies_30 = np.array(list(map(float, f.read().split(",")[:-1])))
with open("./with_wall/Energy.txt") as f:
    energies_10 = np.array(list(map(float, f.read().split(",")[:-1])))

# 10x10
hist_10, edges = np.histogram(less_than(energies_10, lower_limit), bins=100)
ax.bar(
    edges[:-1],
    hist_10,
    width=np.diff(edges),
    edgecolor="black",
    align="edge",
    label="10x10",
)
# print(hist_10)

# 30x30
hist_30, edges = np.histogram(less_than(energies_30, lower_limit), bins=100)
ax.bar(
    edges[:-1],
    hist_30,
    width=np.diff(edges),
    edgecolor="black",
    align="edge",
    label="30x30",
)


diff_hist = hist_30 - hist_10
# ax.bar(
#     edges[:-1],
#     diff_hist,
#     width=np.diff(edges),
#     edgecolor="black",
#     align="edge",
#     label="30x30 - 10x10",
# )

max_diff = 0
for i in range(len(diff_hist)):
    if hist_10[i] != 0:
        if max_diff < diff_hist[i] / hist_10[i]:
            max_diff = (diff_hist[i] / hist_10[i]) * 100
print(diff_hist)
print(max_diff, "%")

ax.text(7, 122, f"Maximum difference of:\n {round(max_diff, 3)}%")
plt.grid()
plt.legend()
plt.title("Comparing energy distribution for 10x10m² vs 30x30m² CRY input")
plt.xlabel("Energy (MeV)")
plt.ylabel("Number of events")
plt.savefig("diff_10_30.png")
