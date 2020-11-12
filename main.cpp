//#define NDEBUG
#include <iostream>
#include "headers/simple_strategy.h"
#include "headers/extensible_simple_strategy.h"
#include "headers/Custom_allocator.h"
#include "headers/list_struct.h"
#include <map>
#include <algorithm>

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

    print_head("Custom_simple_allocator+std::map");

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

    print_head("Custom_extensible_simple_allocator+std::map");

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

    print_head("std::allocator+std::map");

    for(int i = 0; i < SZ+2; i++)
        MapFactSTL.try_emplace(i, i);

    for (const auto & [i, v] : MapFactSTL)
        std::cout << i << " " << v << std::endl;


//-----------------------------------------------------------------------------------------------------------------------------------------

    print_head("CustomList+std::allocator");

    LinkedList<int> Third;

    for (int i = 0; i < SZ; i++)
        Third.Add(MyFactorial(i));

    for(auto it = Third.begin(); it != Third.end(); ++it)
        std::cout << *it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

    print_head("CustomList+Custom_simple_allocator");

    LinkedList<int, Custom_allocator<int, SZ, simple_strategy>> llist_CustAlloc;

    for (int i = 0; i < SZ + 2; i++) {
        try {
            llist_CustAlloc.Add(MyFactorial(i));
        }
        catch(const std::exception& e) {
            std::cout << "Erorr: " << e.what() << " has occurred on the " << i << "th element while trying for allocate." << std::endl;
        }
    }

    //for(auto it = llist_CustAlloc.begin(); it != llist_CustAlloc.end(); ++it)
    for(const auto& it : llist_CustAlloc)
        std::cout << it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

    print_head("CustomList+Custom_ext_simple_allocator");

    LinkedList<int, Custom_allocator<int, SZ, ext_simple_strategy>> llist_CustExtAlloc;

    for (int i = 0; i < SZ + 2; i++)
            llist_CustExtAlloc.Add(MyFactorial(i));

    for(const auto& it : llist_CustExtAlloc)
        std::cout << it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

    print_head("Copy_ctor_LinkedList");

    LinkedList<int, Custom_allocator<int, SZ, ext_simple_strategy>> llist_1;

    for (int i = 0; i < SZ; i++)
            llist_1.Add(MyFactorial(i));

    for(const auto& it : llist_1)
        std::cout << it << std::endl;

    print_head("Copy_with_the_same_allocators");

    auto llist_2 = llist_1;

    for(const auto& it : llist_2)
        std::cout << it << std::endl;

    print_head("Copy_with_different_allocators");

    LinkedList<int> llist_3 = llist_1;

    for(const auto& it : llist_3)
        std::cout << it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------
    {

    print_head("Move_ctor_LinkedList");

    LinkedList<int, Custom_allocator<int, SZ, ext_simple_strategy>> llist_1;

    for (int i = 0; i < SZ; i++)
            llist_1.Add(MyFactorial(i));

    for(const auto& it : llist_1)
        std::cout << it << std::endl;

    print_head("Move_with_the_same_allocators");

    auto llist_2 = std::move(llist_1);

    for(const auto& it : llist_2)
        std::cout << it << std::endl;

    print_head("Move_with_different_allocators");

    LinkedList<int> llist_3 = std::move(llist_2);

    for(const auto& it : llist_3)
        std::cout << it << std::endl;
    }
//-----------------------------------------------------------------------------------------------------------------------------------------

    [[maybe_unused]] char a;
    a = getchar();
    return 0;
}
