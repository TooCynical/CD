import matplotlib.pyplot as plt
import matplotlib.patches as patches

import sys

class Rectangle:
    def __init__(self, x, y, w, h):
        self._x = x
        self._y = y
        self._w = w
        self._h = h

# This function returns a triple (width, height, [rectangles])
# given a file representing a floorplan, and a file containing
# dimensions for rectangles.
def rectangles_from_file(fplan_fp, dim_fp):
    count = 0
    rectangles = []
    with open(fplan_fp) as fplan_file:
        with open(dim_fp) as dim_file:
            for fplan_line, dim_line in zip(fplan_file, dim_file):
                if (count == 0):
                    width = int(fplan_line.split()[0])
                    height = int(fplan_line.split()[1])
                else:
                    rectangles.append(Rectangle(fplan_line.split()[0], \
                                                fplan_line.split()[1], \
                                                dim_line.split()[0], \
                                                dim_line.split()[1]))
                count += 1
    return width, height, rectangles


# Get width, height and rectangles from files.
if (len(sys.argv) < 4):
    print "Usage: python drawer <floorplan> <dimensions> <output>"
    exit(1)
fplan_fp = sys.argv[1]
dim_fp = sys.argv[2]
width, height, rectangles = rectangles_from_file(fplan_fp, dim_fp)

# Make a width x height figure and add an axis to draw the rectangles.
figure = plt.figure()
axis = figure.add_subplot(111, aspect='equal')
axis.set_xlim([0, width])
axis.set_ylim([0, height])

colors = ['b','g','r','c','m','y','k','w']

# Draw each rectangle, looping over 8 available colors.
count = 0
for rect in rectangles:
    axis.add_patch(
        patches.Rectangle(
            (rect._x, rect._y),   
            rect._w,          
            rect._h,
            facecolor=colors[count % len(colors)]
        )
    )
    count += 1

# Save the figure.
save_fp = sys.argv[3]
figure.savefig(save_fp, dpi=90, bbox_inches='tight')