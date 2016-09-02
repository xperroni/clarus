/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of FFTW/CV.

FFTW/CV is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FFTW/CV is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FFTW/CV. If not, see <http://www.gnu.org/licenses/>.
*/

#include <clarus/fftw/plan.hpp>

//#define PLAN_MODE FFTW_MEASURE
#define PLAN_MODE FFTW_PATIENT

namespace clarus
{

namespace fftw
{

Plan::Plan()
{
  transform = NULL;
  count = 0;
  rows = 0;
  cols = 0;
}

Plan::Plan(int count, int m, int n, Transform *transform)
{
  this->transform = transform;
  this->count = count;
  this->rows = rows;
  this->cols = cols;
}

void Plan::execute()
{
  CV_Assert(transform != NULL);
  fftw_execute(transform->plan);
}

void Plan::execute(double *buffer)
{
  CV_Assert(transform != NULL);
  transform->execute(buffer);
}

Plan::ForwardR2C::ForwardR2C(int count, int m, int n, double *S)
{
  fftw_complex *F = (fftw_complex*) S;
  int size[] = {m, n};
  int dist_S = m * (n + 2);
  int dist_F = dist_S / 2;

  plan = fftw_plan_many_dft_r2c(2, size, count,
                                S, NULL, 1, dist_S, // input
                                F, NULL, 1, dist_F, // output
                                PLAN_MODE);
}

Plan::BackwardC2R::BackwardC2R(int count, int m, int n, double *S)
{
  fftw_complex *F = (fftw_complex*) S;
  int size[] = {m, n};
  int dist_S = m * (n + 2);
  int dist_F = dist_S / 2;

  plan = fftw_plan_many_dft_c2r(2, size, count,
                                F, NULL, 1, dist_F, // input
                                S, NULL, 1, dist_S, // output
                                PLAN_MODE);
}

Plan::Transform::~Transform()
{
  fftw_destroy_plan(plan);
}

void Plan::ForwardR2C::execute(double *S)
{
  fftw_complex *F = (fftw_complex*) S;
  fftw_execute_dft_r2c(plan, S, F);
}

void Plan::BackwardC2R::execute(double *S)
{
  fftw_complex *F = (fftw_complex*) S;
  fftw_execute_dft_c2r(plan, F, S);
}

Plan FORWARD_R2C(int count, int m, int n, double *S)
{
  return Plan(count, m, n, new Plan::ForwardR2C(count, m, n, S));
}

Plan BACKWARD_C2R(int count, int m, int n, double *S)
{
  return Plan(count, m, n, new Plan::BackwardC2R(count, m, n, S));
}

} // namespace fftw

} // namespace clarus
