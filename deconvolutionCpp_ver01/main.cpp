#include <iostream>
#include "fftw3.h"

static int raw_image_l;
static float* raw_image;

static int conv_image_l;
static float* conv_image;

static int kernel_l;
static float* kernel;

void generateImage(
	float* (&conv_image), int (&conv_image_l),
	float* (&raw_image), int (&raw_image_l),
	float* (&kernel), int (&kernel_l));

void directConvolution(
	float* (&conv_image), int (&conv_image_l),
	const float* const (&raw_image), int const (&raw_image_l),
	const float* const (&kernel), int const (&kernel_l));

void fftConvolution(
	float* (&conv_image), int(&conv_image_l),
	const float* const (&raw_image), int const (&raw_image_l),
	const float* const (&kernel), int const (&kernel_l));

int main() {

	generateImage(
		conv_image, conv_image_l, 
		raw_image, raw_image_l, 
		kernel, kernel_l
	); // reserves memory for all 3 images that is not automatically released

	std::cout << "raw_image:" << std::endl;
	for (int i = 0; i < raw_image_l; i++) {
		std::cout << raw_image[i] << " ";
	}
	std::cout << std::endl << std::endl;

	std::cout << "kernel:" << std::endl;
	for (int i = 0; i < kernel_l; i++) {
		std::cout << kernel[i] << " ";
	}
	std::cout << std::endl << std::endl;

	std::cout << "conv_image:" << std::endl;
	for (int i = 0; i < conv_image_l; i++) {
		std::cout << conv_image[i] << " ";
	}
	std::cout << std::endl << std::endl;

	free(raw_image);
	free(kernel);
	free(conv_image);

}

void generateImage(
	float* (&conv_image), int (&conv_image_l), 
	float* (&raw_image), int (&raw_image_l), 
	float* (&kernel), int (&kernel_l)) {
	// generates an example image to be deconvolved
	// reserves memory for both image and kernel

	raw_image_l = 21;
	raw_image = (float*)malloc(raw_image_l * sizeof(float));

		// last working test code
	for (int i = 0; i < raw_image_l; i++) { raw_image[i] = 0;}

	raw_image[10] = 1;
	raw_image[12] = 2;

	kernel_l = 11;
	kernel = (float*)malloc(kernel_l * sizeof(float));

	for (int i = 0; i < kernel_l; i++) { kernel[i] = 0; }

	kernel[4] = 0.9;
	kernel[5] = 0.6;
	kernel[6] = 0.3;

	directConvolution(
		conv_image, conv_image_l,
		raw_image, raw_image_l,
		kernel, kernel_l
	); // reserves memory for conv_image that is not automatically released

}

void directConvolution(
	float* (&conv_image), int (&conv_image_l),
	const float* const (&raw_image), int const (&raw_image_l),
	const float* const (&kernel), int const (&kernel_l)) {

	conv_image_l = raw_image_l + kernel_l - 1;
	conv_image = (float*)malloc(conv_image_l * sizeof(float));

	for (int i = 0; i < conv_image_l; i++) { conv_image[i] = 0; }

	for (int i = 0; i < kernel_l - 1; i++) {
		for (int j = 0; j <= i; j++) {
			conv_image[i] = conv_image[i] + raw_image[j] * kernel[i - j];
		}
	}

	for (int i = kernel_l - 1; i < raw_image_l; i++) {
		for (int j = 0; j < kernel_l; j++) {
			conv_image[i] = conv_image[i] + raw_image[i + j - kernel_l + 1] * kernel[j];
		}
	}

	for (int i = 0; i < kernel_l - 1; i++) {
		for (int j = 0; i + j < kernel_l - 1; j++) {
			conv_image[raw_image_l + i] = conv_image[raw_image_l + i] + raw_image[raw_image_l - kernel_l + 1 + i + j] * kernel[kernel_l - 1 - j];
		}
	}

}

void fftConvolution(
	float* (&conv_image), int (&conv_image_l),
	const float* const (&raw_image), int const (&raw_image_l),
	const float* const (&kernel), int const (&kernel_l)) {

	conv_image_l = raw_image_l + kernel_l - 1;
	conv_image = (float*)malloc(conv_image_l * sizeof(float));

	fftw_complex *raw_image_fftw_complex, *fft_raw_image_fftw_complex;
	fftw_complex *raw_image_fftw_complex_2; // DEBUG
	fftw_plan p, q;

	raw_image_fftw_complex = (fftw_complex*)malloc(raw_image_l * sizeof(fftw_complex));
	fft_raw_image_fftw_complex = (fftw_complex*)malloc(raw_image_l * sizeof(fftw_complex));
	raw_image_fftw_complex_2 = (fftw_complex*)malloc(raw_image_l * sizeof(fftw_complex)); // DEBUG

	for (int i = 0; i < raw_image_l; i++) {
		raw_image_fftw_complex[i][0] = raw_image[i];
		raw_image_fftw_complex[i][1] = 0;
	}

	p = fftw_plan_dft_1d(raw_image_l, raw_image_fftw_complex, fft_raw_image_fftw_complex, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);

	q = fftw_plan_dft_1d(raw_image_l, fft_raw_image_fftw_complex, raw_image_fftw_complex_2, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(q);
	fftw_destroy_plan(q);

	for (int i = 0; i < raw_image_l; i++) {
		std::cout << "index " << i << ":" << std::endl;
		std::cout << "   before: " << raw_image_fftw_complex[i][0] << " + " << raw_image_fftw_complex[i][1] << " I " << std::endl;
		std::cout << "   before: " << raw_image_fftw_complex_2[i][0] << " + " << raw_image_fftw_complex_2[i][1] << " I " << std::endl;
		std::cout << std::endl;
	}

	free(raw_image_fftw_complex);
	free(fft_raw_image_fftw_complex);

}
