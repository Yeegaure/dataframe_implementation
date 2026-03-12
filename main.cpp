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

    auto df_loaded = loader.load("../data/Sales.csv");
    if (!df_loaded) {
        std::cerr << "Failed to load CSV\n";
        return 1;
    }

    DataFrame df = *df_loaded;

    auto order_str = df["Order_Quantity"];
    auto order = DataFrame::to_int_vector(order_str);
    auto unit_cost_str = df["Unit_Cost"];
    auto unit_cost = DataFrame::to_int_vector(unit_cost_str);

    auto s = order.size();
    std::cout << s;


    // auto s = df.shape();
    // std::cout << s <<'\n';
    // df.add_row({
    //     "2026-05-22","22","May","2014","47","Adults (35-64)","F",
    //     "Australia","Victoria","Accessories","Bike Racks",
    //     "Hitch Rack - 4-Bike","4","45","120","199","180","379"
    // });
    //
    // auto s1 = df.shape();
    // std::cout << s1 <<'\n';


    std::cout << "file saved\n";

    // std::vector<double> x = {1, 2, 3, 4, 5};
    // std::vector<double> y = {10, 20, 15, 30, 25};
    //
    plt::plot(order, unit_cost);
    plt::title("order vs unit_cost Plot");
    plt::save("new_plot.png");
    IMG_Downloader downloader("new_plot.png");
    downloader.save("final_plot");


    return 0;
}


