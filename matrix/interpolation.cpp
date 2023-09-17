#include <iostream>
#include <array>
#include <tuple>


/**
 * Bilinear Interpolation
 * u = (x - x1) / (x2 - x1)
 * v = (y - y1) / (y2 - y1)
 * 
 * w(x,y) = {(1-u) * (1-v) * w(x1,y1)} + {u * (1-v) * w(x2,y1)} + {(1-u) * v * w(x1,y2)} + {u * v * w(x2,y2)}
 * where, w(xi,yj) = weight (or value) at coordinates xi & yj
 * 
 * since, here x1 = 0 & y1 = 0,
 * therefore, u = x, v = y
*/

/**
 * Input the weights (or values) of the 4 coordinates- (x1,y1) [x1y1], (x1,y2) [x1y2], (x2,y1) [x2y1], (x2,y2) [x2y2]
 * and the coordinate (x,y) [xy] for which the weight (value) needs to be interpolated where coordinates (x,y) must be normalised between 0 and 1 ([0,1]).
 * Calculates and returns the bilinear interpolation of the coordinate (x,y).
*/
double interpolate(const double &x1y1, const double &x1y2, const double &x2y1, const double &x2y2, const std::tuple<double, double> &xy) {
    const auto &[x, y] = xy;
    return ((1-x) * (1-y) * x1y1) + (x * (1-y) * x2y1) + ((1-x) * y * x1y2) + (x * y * x2y2);
}


int main() {
    std::tuple<double, double> xy{0.5, 1.0};
    double x1y1{1.0}, x1y2{2.0}, x2y2{4.0}, x2y1{3.0};

    double result = interpolate(x1y1, x1y2, x2y1, x2y2, xy);

    std::cout << result << std::endl;

    return 0;
}
