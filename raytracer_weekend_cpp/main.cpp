#include <iostream>
#include <spdlog/spdlog.h>
#include <fstream> // Required for file operations
int main() {

    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    int image_width = 1280;
    int image_height = 720;
    std::ofstream MyFile("example.ppm");
    MyFile << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int row = 0; row < image_height; row++) {
        std::clog << "\rScanlines remaining: " << (image_height - row) << ' ' << std::flush;
        for (int col = 0; col < image_width; col++) {
            auto r =  static_cast<double>(col) / (image_width-1);
            auto g = static_cast<double>(row) / (image_height-1);
            auto b = 0;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            MyFile << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    MyFile.close();

    // Test spdlog
    std::clog << "\rDone.                 \n";
    return 0;
}