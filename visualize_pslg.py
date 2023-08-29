import fileinput
import numpy as np
import pylab as pl
from matplotlib import collections  as mc
import matplotlib.cm as cm

lines = []
colors = []
for line in fileinput.input():
    tk = line.split(" ")
    lines.append([(float(tk[0]), float(tk[1])), (float(tk[2]), float(tk[3]))])
    colors.append(int(tk[4]))

cmap = cm.get_cmap('tab10')  # Choose a colormap for mapping values to colors
norm = pl.Normalize(vmin=min(colors), vmax=max(colors))  # Normalize values

lc = mc.LineCollection(lines, linewidths=2, cmap=cmap, norm=norm)
lc.set_array(np.array(colors))
fig, ax = pl.subplots()
ax.add_collection(lc)
ax.autoscale()
ax.margins(0.1)

# Add text annotations next to each line segment
for line, color in zip(lines, colors):
    x = (line[0][0] + line[1][0]) / 2  # x-coordinate for annotation
    y = (line[0][1] + line[1][1]) / 2  # y-coordinate for annotation
    ax.text(x, y, str(color), color='black', ha='center', va='center')

pl.show()
