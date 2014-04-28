#include "circle.hpp"

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multifit_nlin.h>

#include <cmath>
#include <sstream>

CircleModel::CircleModel():
    x0(0.0),
    y0(0.0),
    r1(1.0),
    r2(1.0)
{
    // Nothing to do.
}

CircleModel::CircleModel(double x, double y, double r):
    x0(x),
    y0(y),
    r1(r),
    r2(r * r)
{
    // Nothing to do.
}

static int circle_f(const gsl_vector *x, void *params, gsl_vector *f) {
    const CircleModel::Data &data = *((const CircleModel::Data*) params);

    CircleModel::Domain x0 = gsl_vector_get(x, 0);
    CircleModel::Domain y0 = gsl_vector_get(x, 1);
    CircleModel::Domain r2 = pow(gsl_vector_get(x, 2), 2.0);

    for (int i = 0, n = data.size(); i < n; i++) {
        const CircleModel::Datum &p = data[i];
        CircleModel::Domain x2 = pow(p.first - x0, 2.0);
        CircleModel::Domain y2 = pow(p.second - y0, 2.0);
        CircleModel::Domain d2 = x2 + y2 - r2;
        gsl_vector_set(f, i, d2);
    }

    return GSL_SUCCESS;
}

static int circle_df(const gsl_vector *x, void *params, gsl_matrix *J) {
    const CircleModel::Data &data = *((const CircleModel::Data*) params);

    CircleModel::Domain x0 = gsl_vector_get(x, 0);
    CircleModel::Domain y0 = gsl_vector_get(x, 1);
    CircleModel::Domain r2 = -2 * gsl_vector_get(x, 2);

    for (size_t i = 0, n = data.size(); i < n; i++) {
        const CircleModel::Datum &p = data[i];
        gsl_matrix_set(J, i, 0, 2 * (p.first - x0));
        gsl_matrix_set(J, i, 1, 2 * (p.second - y0));
        gsl_matrix_set(J, i, 2, r2);
    }

    return GSL_SUCCESS;
}

static int circle_fdf(const gsl_vector *x, void *params, gsl_vector *f, gsl_matrix *J) {
    circle_f(x, params, f);
    circle_df(x, params, J);
    return GSL_SUCCESS;
}

inline double distance(double a_x, double a_y, double b_x, double b_y) {
    return pow(pow(a_x - b_x, 2.0) + pow(a_y - b_y, 2.0), 0.5);
}

void CircleModel::fit(Data &data) {
    const Datum &p1 = data[0];
    Domain a_x = p1.first;
    Domain a_y = p1.second;

    const Datum &p2 = data[1];
    Domain b_x = p2.first;
    Domain b_y = p2.second;

    const Datum &p3 = data[2];
    Domain c_x = p3.first;
    Domain c_y = p3.second;

    Domain a_2 = pow(a_x, 2) + pow(a_y, 2);
    Domain b_2 = pow(b_x, 2) + pow(b_y, 2);
    Domain c_2 = pow(c_x, 2) + pow(c_y, 2);
    Domain d = 2 * (a_x * (b_y - c_y) + b_x * (c_y - a_y) + c_x * (a_y - b_y));

    Domain x = (a_2 * (b_y - c_y) + b_2 * (c_y - a_y) + c_2 * (a_y - b_y)) / d;
    if (!isnan(x)) {
        x0 = x;
    }

    Domain y = (a_2 * (c_x - b_x) + b_2 * (a_x - c_x) + c_2 * (b_x - a_x)) / d;
    if (!isnan(y)) {
        y0 = y;
    }

    Domain a = ::distance(a_x, a_y, b_x, b_y);
    Domain b = ::distance(b_x, b_y, c_x, c_y);
    Domain c = ::distance(c_x, c_y, a_x, a_y);
    Domain s = pow((a + b + c) * (-a + b + c) * (a - b + c) * (a + b - c), 0.5);

    Domain r = a * b * c / s;
    if (!isnan(r)) {
        r1 = r;
        r2 = pow(r1, 2.0);
    }

/*
    size_t n = data.size();
    gsl_vector *x = gsl_vector_alloc(3);
    gsl_vector_set(x, 0, x0);
    gsl_vector_set(x, 1, y0);
    gsl_vector_set(x, 2, r1);

    gsl_multifit_fdfsolver *s = gsl_multifit_fdfsolver_alloc(gsl_multifit_fdfsolver_lmder, n, 3);
    gsl_multifit_function_fdf fdf = {circle_f, NULL, NULL, n, 3, &data};
    gsl_multifit_fdfsolver_set(s, &fdf, x);

    double e = 0.0001;
    //for (int i = 0; i < 100; i++) {
    //    gsl_multifit_fdfsolver_iterate(s);
    //}

    gsl_multifit_fdfsolver_driver(s, 1000, e, e);

    gsl_vector *b = gsl_multifit_fdfsolver_position(s);
    x0 = gsl_vector_get(b, 0);
    y0 = gsl_vector_get(b, 1);
    r1 = fabs(gsl_vector_get(b, 2));
    r2 = pow(r1, 2.0);

    gsl_multifit_fdfsolver_free(s);
    gsl_vector_free(x);
*/
}

CircleModel::Distance CircleModel::distance(const Datum &datum) const {
    Domain x2 = pow(datum.first - x0, 2.0);
    Domain y2 = pow(datum.second - y0, 2.0);
    return fabs(x2 + y2 - r2);
}

CircleModel::Distance CircleModel::distance(const CircleModel &that) const {
    Domain x2 = pow(x0 - that.x0, 2.0);
    Domain y2 = pow(y0 - that.y0, 2.0);
    Domain r2 = pow(r1 - that.r1, 2.0);
    return (x2 + y2 + r2);
}

std::string CircleModel::parametric() {
    std::ostringstream stream;
    stream << "set object 1 circle at " << x0 << "," << y0 << " size first " << r1 << "\n";
    return stream.str();
}
