import numpy as np
from bokeh.models import Label
from bokeh.plotting import figure, save
from bokeh.layouts import column, row

def less_than(arr, lim):
    return arr[np.where(arr > lim)]

lower_limit = 0.1


p = figure(
    title="Energy distribution with 10x10m and difference when using 30x30",
    background_fill_color="#fafafa",
)

with open("./with_wall/Energy.txt") as f:
    energies_10 = np.array(list(map(float, f.read().split(",")[:-1])))
hist, edges = np.histogram(less_than(energies_10, lower_limit), bins=100)

p.quad(
    top=hist,
    bottom=0,
    left=edges[:-1],
    right=edges[1:],
    fill_color="navy",
    line_color="white",
    alpha=0.5,
)



with open("./with_wall_30/Energy.txt") as f:
    energies_30 = np.array(list(map(float, f.read().split(",")[:-1])))
diff_hist, edges = np.histogram(less_than(energies_30 - energies_10, lower_limit), bins=100)

p.quad(
    top=diff_hist,
    bottom=0,
    left=edges[:-1],
    right=edges[1:],
    fill_color="red",
    line_color="white",
    alpha=0.5,
)

max_diff = 0
for i in range(len(diff_hist)):
    if hist[i] != 0:
        if max_diff < diff_hist[i]/hist[i]:
            max_diff = (diff_hist[i]/hist[i]) * 100
print(diff_hist)
print(max_diff, "%")

p.y_range.start = 0
p.xaxis.axis_label = "Energy"
p.yaxis.axis_label = "Number of events"
p.grid.grid_line_color = "white"
text = f"Maximum difference of {round(max_diff, 3)}%"
p.add_layout(Label(x=15, y=100, text=text))
save(p, "difference_between_10x10_and_30x30.html")

