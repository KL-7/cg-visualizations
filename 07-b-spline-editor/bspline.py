from __future__ import division

import math


def bspline(points, m):
    '''
        points - control points
        m - degree of B-splines
    '''
    M = len(points) - 1
    n = M - m + 1
    knots = [0] * m + [k / n for k in xrange(n + 1)] + [1] * m

    # uncomment to make two central knots equal
#    if n > 1:
#        knots = [0] * m + [k / (n - 1) for k in xrange(n)] + [1] * m
#        knots.insert(len(knots) // 2, knots[len(knots) // 2])
#    if n > 2:
#        knots = [0] * m + [k / (n - 2) for k in xrange(n - 1)] + [1] * m
#        knots.insert(len(knots) // 2, knots[len(knots) // 2])
#        knots.insert(len(knots) // 2, knots[len(knots) // 2])

    def curve(t):
        if t == 1:
            return points[-1]

        k = m
        while k + 1 < len(knots) and t > knots[k + 1]: k += 1
        # values of N[m, k-m],...,N[m, k] at t
        Nk = [1] + [0] * m

        V = lambda m, i, t: (t - knots[i]) / (knots[i + m] - knots[i]) \
                            if knots[i] != knots[i + m] \
                            else 0

        for i in xrange(1, m + 1):
            for j in xrange(i, -1, -1):
                # count N[i, k-j]
                if j:
                    Nk[j] = Nk[j] * V(i, k - j, t) + Nk[j - 1] * (1 - V(i, k - j + 1, t))
                else:
                    Nk[j] = Nk[j] * V(i, k - j, t)

        Nk.reverse()

        return [sum(p[j] * N for p, N in zip(points[k-m:k+1], Nk)) for j in (0, 1)]

    return curve


if __name__ == '__main__':
    import matplotlib.pyplot as plt

#    control_points = ((2, 3), (1, 1), (0, 0), (2, 1), (3, 0), (3, 3), (4, 1), (6, 3))
    control_points = ((0, 1), (1, 3), (3, 1), (1, 0))
    degree = 2

    curve = bspline(control_points, degree)

    N = 50
    points = map(curve, (i / N for i in xrange(N + 1)))
    x, y = ([p[j] for p in points] for j in (0, 1))
    plt.plot(x, y)

    x, y = ([p[j] for p in control_points] for j in (0, 1))
    plt.plot(x, y)

    plt.grid(True)
    plt.show()
