#include <iostream>

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
	const float* const (&raw_image), const int (&raw_image_l),
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
	float* (&kernel), int (&kernel_l)
) {
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
	const float* const (&raw_image), const int const (&raw_image_l),
	const float* const (&kernel), const int const (&kernel_l)
){

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
