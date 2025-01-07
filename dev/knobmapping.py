# Helper script to calculate mapping from knob value to LFO frequencies. Uses thre
# different exponential functions for slow, mid and fast frequencies.

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def fit(x, y):
    def f(x, a, b, c):
        return a+b*(x**c)

    params, _ = curve_fit(f, x, y, maxfev=100000)
    a, b, c = params

    x_new = np.linspace(x[0], x[-1], 50)
    y_new = f(x_new, a, b, c)

    return params, x_new, y_new


def fits():
    plt_params = []
    for vals in [
        # Mapping of knob values (0-1) and frequencies (1/s or hz).
        [(0.0, 1/300), (0.1, 1/120), (0.2, 1/40), (0.3, 1/20)],
        [(0.3, 1/20), (0.5, 1/10), (0.6, 1/5), (0.7, 1/3)],
        [(0.7, 1/3), (0.8, 0.8), (0.9, 4), (1, 20)],
    ]:
        vals = np.array(vals)
        x, y = vals.T
        params, x_new, y_new = fit(x, y)
        print(x[0], x[-1], list(params))
        plt_params += [x, y, 'o', x_new, y_new]

    plt.plot(*plt_params)
    plt.yscale('log')

    # plt.xlim([x[0], x[-1]])
    plt.show()

    return params

def output():
    vals = []
    x = 0
    while x <= 1.0:
        if x <= 0.3:
            a, b, c = [0.0031048470476956495, 0.4748929230518831, 1.9212435592371293]
        elif x <= 0.8:
            a, b, c = [0.03924103972533662, 1.3730312965143276, 4.299823207425811]
        else:
            a, b, c = [0.24191984131798305, 19.75880469177711, 15.771876134848176]

        y = a+b*(x**c)
        # print(x, y)
        vals.append(int(y * 2**12))
        # vals.append(y)
        x += 1.0/255

    print(', '.join(str(v) for v in vals))

if __name__ == '__main__':
    # print Q12 hz values
    output()

    # calculate parameters for exponential functions
    # print(fits())


