#include "include/CSVLoader.h"
#include "include/DataFrame.h"
#include "DF_Downloader.h"
#include "matplotlibcpp.h"
#include <iostream>
#include <exception>

#include "IMG_Downloader.h"

int main() {
    CSVLoader loader;
    namespace plt = matplotlibcpp;
/*
    auto df_loaded = loader.load("../data/Sales.csv");
    if (!df_loaded) {
        std::cerr << "Failed to load CSV\n";
        return 1;
    }

    DataFrame df = *df_loaded;

    df.add_row({
        "2026-05-22","22","May","2014","47","Adults (35-64)","F",
        "Australia","Victoria","Accessories","Bike Racks",
        "Hitch Rack - 4-Bike","4","45","120","199","180","379"
    });

    DF_Downloader downloader(df);
    downloader.save("name.csv");

    std::cout << "file saved\n";
    */
    std::vector<double> x = {1, 2, 3, 4, 5};
    std::vector<double> y = {10, 20, 15, 30, 25};

    try {
        plt::plot(x, y);
        plt::title("Simple Test Plot");
        plt::save("plot.png");
        IMG_Downloader downloader("plot.png");
        downloader.save("final_plot.png");
    }
    catch (const std::exception& e) {
        std::cerr << "Plotting error: " << e.what() << "\n";
    }

    return 0;
}


