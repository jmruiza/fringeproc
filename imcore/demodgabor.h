/**************************************************************************
Copyright (c) 2012, Julio C. Estrada
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

+ Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

+ Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************/

#ifndef DEMODGABOR_H
#define DEMODGABOR_H

#ifndef SWIG
#include <opencv2/core/core.hpp>
#include "gabor_gears.h"
#endif

class Scanner;

/**
 * Implemments an adaptive Gabor filter guided by the local frequencies.
 *
 * @author Julio C. Estrada
 * @note The data processed is double precision.
 */
class DemodGabor
{
public:
  /**
   * Default constructor.
   */
  DemodGabor();
  /**
   * Builds the filter to process the given image.
   *
   * @param I the image to process
   */

#ifndef SWIG
  DemodGabor(const cv::Mat I);

  /**
   * Returns the real part of the output
   * @return the real part of the output
   */
  cv::Mat getFr();
  /**
   * Returns the imaginary part of the output
   * @return the imaginary part of the output
   */
  cv::Mat getFi();
  /**
   * Returns the estimated local frequencies at x-direction
   * @return the estimated local frequencies
   */
  cv::Mat getWx();
  /**
   * Returns the estimated local frequenciesat y-direction
   * @return the estimated local frequencies
   */
  cv::Mat getWy();
  /**
   * Returns the image being processed.
   * @return the image data
   */
  cv::Mat getInput();
#endif

  /**
    Resets the internal state of the object.

    This can be usefull when we whant to reinitialize the demodulation
    process.
    */
  void reset();

  /**
   * Sets the minimum local frequency to be processed by the scanner.
   *
   * This is a radial frequency, that is, it is the maximum frequency magnitud
   * that is going to be processed by the filter.
   *
   * @param minf the minimum radial frequency to be processed.
   */
  DemodGabor& setScanMinf(double minf);

  /**
   * Removes the background component from the input data.
   *
   * It removes the DC term or background illumination component.
   */
  void removeDC();

  /**
    Sets the number of iterations that the gabor filter is applied in each 
    point.

    When the convolution is applied, the local frequencies are estimated.
    Given the nuew local frequencies, we can appliy the Gabor filter againg
    iteratively. Here we say how many iterations we whant to apply in each
    scanned pixel.

    @param iters the number of iterations.
    */
  DemodGabor& setIters(const int iters);
  /**
    Sets the number of iterations for the first pixel to process.

    It is usefull to iterate more than once in the first pixel to refine the
    local frequency estimation in this pixel called the seed pixel.

    @param iters the number of iterations.
    */
  DemodGabor& setSeedIters(const int iters);
  /**
    Sets the kernel size of the gabor filter.

    Actually, this is the maximum size that the gabor filtar can have in both
    directions while it is adapted.
  */
  DemodGabor& setKernelSize(const double size);
  DemodGabor& setMaxfq(const double w);
  DemodGabor& setMinfq(const double w);
  DemodGabor& setTau(const double tau);
  DemodGabor& setStartPixel(const cv::Point pixel);
  DemodGabor& setCombFreqs(const bool comb);
  DemodGabor& setCombSize(const int size);
  cv::Point getStartPixel();


  /**
   * Executes the filtering operation.
   */
  void run();

  bool runInteractive(Scanner& scan);
private:
  /** The image matrix being processed */
  cv::Mat_<double> m_I;
  /** The real part of the output */
  cv::Mat_<double> m_fr;
  /** The imaginary part of the output */
  cv::Mat_<double> m_fi;
  /** The obtained frequencies at x-direction */
  cv::Mat_<double> m_fx;
  /** The obtained frequencies at y-direction */
  cv::Mat_<double> m_fy;
  /** Label field marking the pixels already visited */
  cv::Mat_<uchar> m_visited;
  cv::Point m_startPixel;

  /** The minimum radial frequency to process */
  double m_scanMinf;
  int m_iters;
  int m_seedIters;
  int m_combSize;
  bool m_combFreqs;
  double m_kernelSize;
  double m_maxfq;
  double m_minfq;
  double m_tau;

};

#endif // DEMODGABOR_H
