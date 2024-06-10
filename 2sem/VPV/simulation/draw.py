import matplotlib.pyplot as plt
import matplotlib.animation as animation


def animate(i):
    data = open('stock.txt', 'r').read()
    lines = data.split('\n')

    xs = []
    ys = []

    for line in lines:
        if line:
            x, y = [float(_) for _ in line.split(',')]
            xs.append(x)
            ys.append(y)

    ax.clear()
    ax.plot(xs, ys)

    plt.xlabel('N')
    plt.ylabel('Force')


fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()