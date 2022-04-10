#include "image.h"
#include "argparser.h"
#include "filters.h"

//My function is called "-my_filter" and needs int argument: -my_filter 50
int main(int argc, char* argv[]) {
    Arguments arg(argc, argv);
    Image my_image(arg);

    ApplyFilters(arg, my_image);

    my_image.Print(arg);

    return 0;
}

