//#define NDEBUG
#include <iostream>
#include "headers/simple_strategy.h"
#include "headers/extensible_simple_strategy.h"
#include "headers/Custom_allocator.h"
#include <map>

static const int SZ = 10;

int MyFactorial(int n){
	int res = 1;
    for (int i = 2; i <= n; i++)
		res *= i;
	return res;
}

int main(int, char**){

    auto print_head = [](const std::string& str){
        std::size_t length = str.length() < 80 ? 40 - str.length()/2 : 0;
        std::string filler(length, '_');
        std::cout << filler << str << filler << std::endl;
    };
//------------------------------------------------------------------------------------------------------------------------------------
    auto MapFactCustom =
        std::map<int,
                 int,
                 std::less<int>,
                 Custom_allocator<std::pair<const int, int>, SZ, simple_strategy>>{};

    print_head("Custom_simple_allocator");

    for(int i = 0; i < SZ+2; i++){
        try {
            MapFactCustom.try_emplace(i, MyFactorial(i));
        }
        catch(const std::exception& e) {
            std::cout << "Erorr: " << e.what() << " has occurred on the " << i << "th element while trying for allocate." << std::endl;
        }
    }

    for(const auto & [i, v] : MapFactCustom)
        std::cout << i << " " << v << std::endl;
//-----------------------------------------------------------------------------------------------------------------------------------------

auto MapFactCustomExt =
        std::map<int,
                 int,
                 std::less<int>,
                 Custom_allocator<std::pair<const int, int>, SZ, ext_simple_strategy>>{};

    print_head("Custom_extensible_simple_allocator");

    for(int i = 0; i < SZ+32; i++){
        try {
            MapFactCustomExt.try_emplace(i, i);
        }
        catch(const std::exception& e) {
            std::cout << "Erorr: " << e.what() << " has occurred on the " << i << "th element while trying for allocate." << std::endl;
        }
    }

    for(const auto & [i, v] : MapFactCustomExt)
        std::cout << i << " " << v << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------
    auto MapFactSTL = std::map<int, int>{};

    print_head("STL_allocator");

    for(int i = 0; i < SZ+2; i++)
        MapFactSTL.try_emplace(i, i);

    for (const auto & [i, v] : MapFactSTL)
        std::cout << i << " " << v << std::endl;

    return 0;
}
