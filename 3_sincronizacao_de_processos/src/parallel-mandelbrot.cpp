#include <omp.h>

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

#define MAX_ITERATIONS 1000
#define IMG_WIDTH 1920
#define IMG_HEIGHT 1080

typedef struct ComplexNumber {
    double real, img;
} ComplexNumber;

unsigned char *calcMandelbrot() {
    unsigned char *canvas =
        new unsigned char[IMG_WIDTH * IMG_HEIGHT * sizeof(unsigned char)];

    /* The 'for' loop is parallelized, and iterations are dynamically divided to
    prevent load imbalance! */
#pragma omp parallel for shared(canvas) schedule(dynamic)
    for (int yIterator = 0; yIterator < IMG_HEIGHT; yIterator++) {
        for (int xIterator = 0; xIterator < IMG_WIDTH; xIterator++) {
            ComplexNumber z = {0.0, 0.0};
            double iterationCount = MAX_ITERATIONS;

            /* Centers the points (X, Y) and maps them to their equivalents in a
             * plane between -2 and 2. The maximum interval is defined as 4.0,
             * which corresponds to the range. */

            double xComplexPos =
                ((xIterator - (IMG_WIDTH / 2.0)) * 4.0 / IMG_WIDTH);
            double yComplexPos =
                (yIterator - (IMG_HEIGHT / 2.0)) * 4.0 / IMG_WIDTH;

            for (int iterations = 0; iterations < MAX_ITERATIONS;
                 iterations++) {
                double real2 = z.real * z.real;
                double img2 = z.img * z.img;
                bool isInsideSet = (real2 + img2) <= 4.0;

                if (!isInsideSet) {
                    iterationCount = iterations;
                    break;
                }

                /* Fc(z) = z^2 + c
                x = Re(z^2 +c) = Re(z^2) + Re(c) = x^2 - y^2 + Re(c)
                y = Im(z^2 +c) = Im(z^2) + Im(c) = 2xy + Im(c) */
                z.img = 2 * z.real * z.img + yComplexPos;
                z.real = real2 - img2 + xComplexPos;
            }

            /* Since the Canvas is shared among all threads, this ensures that
             * only one thread updates its value at a time. */

#pragma omp critical
            {
                unsigned char pixelColor =
                    (unsigned char)(255 *
                                    (iterationCount / (double)MAX_ITERATIONS));
                canvas[yIterator * IMG_WIDTH + xIterator] = pixelColor;
            }
        }
    }

    return canvas;
}

int main() {
    double workingTime = omp_get_wtime();
    unsigned char *canvas = calcMandelbrot();
    string filename = "mandelbrot-canvas.pgm";

    ofstream output_file(filename.c_str(), ios::binary);
    if (output_file.is_open()) {
        output_file << "P5\n" << IMG_WIDTH << " " << IMG_HEIGHT << "\n255\n";
        output_file.write(reinterpret_cast<char *>(canvas),
                          IMG_WIDTH * IMG_HEIGHT * sizeof(unsigned char));
        output_file.close();

        workingTime = omp_get_wtime() - workingTime;
        cout << "The file "
             << "\033[1;32m" << filename << "\033[0m"
             << " was created successfully." << endl;
        cout << "Duration: " << workingTime << "s" << endl;
    } else {
        cout << "Unable to open the file." << endl;
    }

    delete (canvas);

    return 0;
}
