/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#include <clarus/vision/cvmat.hpp>
#include <clarus/vision/fourier.hpp>

#include <stdexcept>

cv::Size fourier::fit(int width, int height)
{
  return cv::Size(cv::getOptimalDFTSize(width), cv::getOptimalDFTSize(height));
}

cv::Size fourier::fit(const cv::Size &size)
{
  return fit(size.width, size.height);
}

cv::Mat fourier::convolve(const cv::Mat &data, const cv::Mat &kernel)
{
  if (data.rows < kernel.rows || data.cols < kernel.cols)
    throw std::runtime_error("Kernel must fit inside data matrix across all dimensions");

  cv::Size size = data.size();
  cv::Size optimal = fit(size);

  cv::Mat fourier;
  cv::Mat data_f = transform(data, 0, optimal);
  cv::Mat kernel_f = transform(kernel, 0, optimal);
  cv::mulSpectrums(data_f, kernel_f, fourier, 0);

  return inverse(fourier, size);
}

cv::Mat fourier::multiply(const cv::Mat &a, const cv::Mat &b, bool conjugate_b)
{
  cv::Mat c;
  cv::mulSpectrums(a, b, c, 0, conjugate_b);
  return c;
}

cv::Mat fourier::normalize(const cv::Mat &data)
{
  cv::Mat data_d = cvmat::convert(data, CV_64F);
/*
  double minVal = 0.0;
  double maxVal = 0.0;
  cv::minMaxLoc(data_d, &minVal, &maxVal);
  double median = (maxVal + minVal) / 2.0;

  return data_d - median;
*/
  cv::Mat averaged = data_d - cv::mean(data_d)[0];

  double maxVal = 0.0;
  cv::minMaxLoc(cv::abs(averaged), NULL, &maxVal);
  if (maxVal != 0)
    averaged /= maxVal;

  return averaged;
}

cv::Mat fourier::correlate(const cv::Mat &data, const cv::Mat &kernel, bool clip)
{
  if (data.rows < kernel.rows || data.cols < kernel.cols)
    throw std::runtime_error("Kernel must fit inside data matrix across all dimensions");

  cv::Size size = data.size();
  cv::Size optimal = fit(size);

  cv::Mat fourier;
  cv::Mat data_f = transform(normalize(data), 0, optimal);
  cv::Mat kernel_f = transform(normalize(kernel), 0, optimal);
  cv::mulSpectrums(data_f, kernel_f, fourier, 0, true);

  if (clip)
  {
    size.width  = 1 + size.width  - kernel.cols;
    size.height = 1 + size.height - kernel.rows;
  }

  return inverse(fourier, size);
}

inline float fourier_step(int wf, int ws, int width)
{
  float n = width / wf;
  return (width - ws) / n;
}

cv::Mat fourier::tiles(const cv::Mat &data, int wf)
{
  int rows = data.rows - data.rows % wf;
  int cols = data.cols - data.cols % wf;

  int ws = (wf - 1) * 2;
  float step_i = fourier_step(wf, ws, rows);
  float step_j = fourier_step(wf, ws, cols);

  int patches_i = rows / wf;
  int patches_j = cols / wf;

  cv::Mat out(rows, cols, CV_64F, cv::Scalar(0));
  for (float i = 0; i < patches_i; i++)
  {
    for (float j = 0; j < patches_j; j++)
    {
      cv::Rect roi_patch(round(j * step_j), round(i * step_i), ws, ws);
      cv::Mat patch(data, roi_patch);
      cv::Mat mag = fourier::magnitude(fourier::transform(patch));

      cv::Rect roi_mag(j * wf, i * wf, wf, wf);
      mag.copyTo(out(roi_mag));
    }
  }

  return out;
}

cv::Mat fourier::transform(const cv::Mat &data)
{
  return transform(data, 0, data.size());
}

cv::Mat fourier::transform(const cv::Mat &data, int flags)
{
  return transform(data, flags, data.size());
}

cv::Mat fourier::transform(const cv::Mat &data, const cv::Size &size)
{
  return transform(data, 0, size);
}

cv::Mat fourier::transform(const cv::Mat &data, int flags, const cv::Size &size)
{
  cv::Mat padded;
  cv::Size fitted = fit(size);
  int m = fitted.height - data.rows;
  int n = fitted.width - data.cols;
  cv::copyMakeBorder(data, padded, 0, m, 0, n, cv::BORDER_CONSTANT, cv::Scalar::all(0));

  cv::Mat fourier;
  cv::dft(padded, fourier, flags, data.rows);

  return fourier;
}

cv::Mat fourier::inverse(const cv::Mat &fourier, const cv::Size &optimal)
{
  cv::Size size = optimal;
  if (size.width == 0 || size.height == 0)
    size = fourier.size();

  cv::Mat real;
  cv::dft(fourier, real, cv::DFT_INVERSE + cv::DFT_SCALE, size.height);

  return real(cv::Rect(0, 0, size.width, size.height));
}

cv::Mat fourier::magnitude(const cv::Mat &fourier)
{
  std::vector<cv::Mat> plane;
  cv::split(fourier, plane);

  cv::Mat mag;
  cv::magnitude(plane[0], plane[1], mag);
//  mag += cv::Scalar::all(1);
//  cv::log(mag, mag);
  return mag;
}

cv::Mat fourier::phase(const cv::Mat &fourier)
{
  std::vector<cv::Mat> plane;
  cv::split(fourier, plane);

  cv::Mat pha;
  cv::phase(plane[0], plane[1], pha);
//  cv::normalize(pha, pha, 0, 127, CV_MINMAX);
  return pha;
}

cv::Mat fourier::bgr(const cv::Mat &fourier)
{
  const cv::Size &size = fourier.size();
  cv::Mat hls(size, CV_8UC3);

  std::vector<cv::Mat> plane;
  cv::split(fourier, plane);

  cv::Mat mag;
  cv::magnitude(plane[0], plane[1], mag);
  mag += cv::Scalar::all(1);
  cv::log(mag, mag);
  cv::normalize(mag, mag, 127, 255, CV_MINMAX);

  cv::Mat pha;
  cv::phase(plane[1], plane[0], pha);
  cv::normalize(pha, pha, 0, 127, CV_MINMAX);

  for (int i = 0, m = size.height; i < m; i++)
  {
    for (int j = 0, n = size.width; j < n; j++)
    {
      cv::Vec3b &pixel = hls.at<cv::Vec3b>(i, j);
      pixel[0] = (uchar) pha.at<float>(i, j);
      pixel[1] = (uchar) mag.at<float>(i, j);
      pixel[2] = 255;
    }
  }

  cv::Mat rgb;
  cv::cvtColor(hls, rgb, CV_HLS2BGR);
  return rgb;
}

cv::Mat fourier::swap(const cv::Mat &fourier)
{
  int rows = fourier.rows;
  int cols = fourier.cols;

  int x = cols / 2;
  int y = rows / 2;

  cv::Rect q0(0, 0, x, y);   // Top-Left
  cv::Rect q1(x, 0, x, y);  // Top-Right
  cv::Rect q2(0, y, x, y);  // Bottom-Left
  cv::Rect q3(x, y, x, y); // Bottom-Right

  cv::Mat swapped(fourier.size(), fourier.type());

  cv::Mat fourier_q0(fourier, q0);
  cv::Mat fourier_q1(fourier, q1);
  cv::Mat fourier_q2(fourier, q2);
  cv::Mat fourier_q3(fourier, q3);

  cv::Mat swapped_q0(swapped, q0);
  cv::Mat swapped_q1(swapped, q1);
  cv::Mat swapped_q2(swapped, q2);
  cv::Mat swapped_q3(swapped, q3);

  fourier_q0.copyTo(swapped_q3);
  fourier_q1.copyTo(swapped_q2);
  fourier_q2.copyTo(swapped_q1);
  fourier_q3.copyTo(swapped_q0);

  return swapped;
}
