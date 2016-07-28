"""
Simple demo of a scatter plot.
"""
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

N = 100000
x = np.random.randint(low=0,high=1000,size=N)
y = np.random.randint(low=0,high=1000,size=N)
colors = np.random.rand(N)
area = 10  # 0 to 15 point radiuses

'''
fig1 = plt.figure()
ax1 = fig1.add_subplot(111, aspect='equal')
ax1.add_patch(
    patches.Rectangle(
        (0.1, 0.1),   # (x,y)
        0.5,          # width
        0.5,          # height
		fill=False 
    )
)
'''
rectX1 = np.random.randint(low=0,high=900)
rectY1 = np.random.randint(low=0,high=900)

f = open('F:\\dev\\github\\find_nearest\\out.txt', 'w')
f.seek(0)
f.truncate()
listX = x.tolist()
listY = y.tolist()
for index in range(0, x.size):
	f.write('[{0},{1}];'.format(listX[index], listY[index]))
	#print ('[{0},{1}],'.format(listX[0], listY[0]))

f.write('[{0},{1},{2},{3}];'.format(rectX1, rectY1, 100, 100))
f.close()

plt.figure().add_subplot(111, aspect='equal').add_patch(
    patches.Rectangle(
        (np.random.randint(low=0,high=900), np.random.randint(low=0,high=900)),   # (x,y)
        100,          # width
        100,          # height
		fill=False 
    )
)
plt.scatter(x, y, s=area, c="yellow", edgecolors="black")
#plt.axhspan(0.25, 0.75, facecolor='0.5', alpha=0.5)
plt.show()
