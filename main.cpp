#define NDEBUG

#include <iostream>
#include "headers/simple_strategy.h"
#include "headers/extensible_simple_strategy.h"
#include "headers/extensible_realoc_strategy.h"
#include "headers/Custom_allocator.h"
#include "headers/list_struct.h"
#include <map>

static const int SZ = 10;

int MyFactorial(int n){
    int res = 1;
    for(int i = 2; i <= n; ++i){
        res *= i;
    }
	return res;
}

template<typename T>
using LLi = LinkedList<T, Custom_allocator<T, SZ, ext_simple_strategy>>;

int main(int, char**){

    auto print_head = [](const std::string& str){
        std::size_t length = str.length() < 80 ? 40 - str.length()/2 : 0;
        std::string filler(length, '_');
        std::cout << filler << str << filler << std::endl;
    };

    [[maybe_unused]] auto print_addr = [](const auto& cont){
        std::cout << "Head address: " << cont.Address_of_Head() << ". Head data address: " << cont.Address_of_Head_Data() << std::endl;
    };

    [[maybe_unused]] auto print_addr_all = [](auto& cont){
        for(auto it = cont.begin(); it != cont.end(); ++it)
            std::cout << "Value = " << *it << (*it < 10'000'000 ? "\t" : "") << "Data address: " << it.pointer_to_node() << std::endl;
    };

    print_head("_");
    print_head("MAIN_TASK");
    print_head("_");

//-----------------------------------------------------------------------------------------------------------------------------------------
    auto MapFactSTL = std::map<int, int>{};

    print_head("std::allocator+std::map");

    for(int i = 0; i < SZ; i++)
        MapFactSTL.try_emplace(i, MyFactorial(i));

    for (const auto & [i, v] : MapFactSTL)
        std::cout << i << " " << v << std::endl;


//------------------------------------------------------------------------------------------------------------------------------------
    auto MapFactCustom =
        std::map<int,
                 int,
                 std::less<int>,
                 Custom_allocator<std::pair<const int, int>, SZ, simple_strategy>>{};

    print_head("Custom_simple_allocator+std::map");

    for(int i = 0; i < SZ; i++){
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

    print_head("CustomList+std::allocator");

    LinkedList<int> Third;

    for (int i = 0; i < SZ; i++){
        Third.Add(i);
    }

    for(auto it = Third.begin(); it != Third.end(); ++it)
        std::cout << *it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

    print_head("CustomList+Custom_simple_allocator(here are allocating all 10 elements, that were reserved)");

    LinkedList<int, Custom_allocator<int, SZ, simple_strategy>> llist_CustAlloc;

    for (int i = 0; i < SZ; i++) {
        try {
            llist_CustAlloc.Add(i);
        }
        catch(const std::exception& e) {
            std::cout << "Erorr: " << e.what() << " has occurred on the " << i << "th element while trying for allocate." << std::endl;
        }
    }

    for(const auto& it : llist_CustAlloc)
        std::cout << it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

    print_head("CustomList+Custom_simple_allocator(here are trying to allocate 12 elements)");

    LinkedList<int, Custom_allocator<int, SZ, simple_strategy>> llist_CustAlloc_12;

    for (int i = 0; i < SZ+2; i++) {
        int fact = MyFactorial(i);
        try {
            llist_CustAlloc_12.Add(fact);
        }
        catch(const std::exception& e) {
            std::cout << "Erorr: " << e.what() << " has occurred on the " << i << "th element while trying for allocate." << std::endl;
        }
    }

    for(const auto& it : llist_CustAlloc_12)
        std::cout << it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------
    print_head("_");
    print_head("EXTAND_TASK");
    print_head("_");
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

    auto MapFactCustomDeal =
        std::map<int,
                 int,
                 std::less<int>,
                 Custom_allocator<std::pair<const int, int>, SZ, ext_realloc_strategy>>{};

    print_head("Custom_extensible_realloc_allocator+std::map");

    for(int i = 0; i < SZ; i++){
        try {
            MapFactCustomDeal.try_emplace(i, MyFactorial(i));
        }
        catch(const std::exception& e) {
            std::cout << "Erorr: " << e.what() << " has occurred on the " << i << "th element while trying for allocate." << std::endl;
        }
    }

    for(const auto & [i, v] : MapFactCustomDeal)
        std::cout << i << " " << v << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

    print_head("CustomList+Custom_ext_simple_allocator");

    LinkedList<int, Custom_allocator<int, SZ, ext_simple_strategy>> llist_CustExtAlloc;

    for (int i = 0; i < SZ + 2; i++)
            llist_CustExtAlloc.Add(MyFactorial(i));

    for(const auto& it : llist_CustExtAlloc)
        std::cout << it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

    print_head("Copy_ctor_of_LinkedList");

    LinkedList<int, Custom_allocator<int, SZ, ext_simple_strategy>> llist_1, llist_4;

    for (int i = 0; i < SZ; i++){
            int fct = MyFactorial(i);
            llist_1.Add(fct);
            llist_4.Add(fct);
    }

    for(const auto& it : llist_1)
        std::cout << it << std::endl;

    print_head("Copy_with_the_same_allocators");

    auto llist_2 = llist_1;

    for(const auto& it : llist_2)
        std::cout << it << std::endl;

    print_head("Copy_with_different_allocators");

    LinkedList<int> llist_3 = llist_4;

    for(const auto& it : llist_3)
        std::cout << it << std::endl;

//-----------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------
{

    print_head("Move_ctor_LinkedList");

    LLi<bunch<int>> llist_1;
    for (int i = 0; i < SZ; i++){
        bunch<int> bnc{i, MyFactorial(i)};
        llist_1.Add(bnc);
    }

    DEBUG_MODE(print_addr(llist_1));

    for(const auto& [f, s] : llist_1)
        std::cout << f << ' ' << s << std::endl;

    print_head("Move_with_the_same_allocators");

    auto llist_2 = std::move(llist_1);

    DEBUG_MODE(print_addr(llist_2));

    for(const auto& [f, s] : llist_2)
        std::cout << f << ' ' << s << std::endl;

    print_head("Move_with_different_allocators");

    LinkedList<bunch<int>> llist_3;
    for (int i = 0; i < SZ; i++){
        bunch<int> bnc{i, MyFactorial(i)};
        llist_3.Add(bnc);
    }

    DEBUG_MODE(print_addr(llist_3));

    for(const auto& [f, s] : llist_3)
        std::cout << f << ' ' << s << std::endl;

    LinkedList<bunch<int>, Custom_allocator<bunch<int>, SZ, ext_simple_strategy>> llist_4 = std::move(llist_3);

    DEBUG_MODE(print_addr(llist_4));

    for(const auto& [f, s] : llist_4)
        std::cout << f << ' ' << s << std::endl;

    std::cout << llist_4.front().f << ' ' << llist_4.empty() << std::endl;
    llist_4.clear();
    std::cout << llist_4.empty() << std::endl;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
    print_head("checking_iterators_LinkedList");

    LinkedList<int, Custom_allocator<int, SZ, ext_realloc_strategy>> llist_30;
    for (int i = 0; i < SZ + 1; i++){
            int fct = MyFactorial(i);
            llist_30.Add(fct);
    }
    print_addr_all(llist_30);
//-----------------------------------------------------------------------------------------------------------------------------------------
    std::cout << "replace zero element." << std::endl;
    auto it_l30 = std::begin(llist_30);
    *it_l30 = 222;
    std::cout << "zero: " << *it_l30 << std::endl;
    std::cout << "element after \"before_begin\"." << std::endl;
    auto it_bef_beg = llist_30.before_begin();
    std::cout << "zero: " << *++it_bef_beg << std::endl;
    ++it_l30;
    std::cout << "element after twice oparation ++." << std::endl;
    std::cout << "second: " << *++it_l30 << std::endl;
    std::cout << "element after twice oparation std::next" << std::endl;
    std::cout << "third: " << *std::next(std::next(it_l30)) << std::endl;
//-----------------------------------------------------------------------------------------------------------------------------------------
    print_head("initial_data_of_LinkedList");
    print_addr_all(llist_30);
//-----------------------------------------------------------------------------------------------------------------------------------------
     print_head("remove_two_elements");
    std::cout << "element with value: " << it_l30.pointer_to_node()->next->data << " will be erased by address: " << it_l30.pointer_to_node()->next << std::endl;
    it_l30 = llist_30.erase_after(it_l30); //must return an iterator to the element after the deleted
    std::cout << "element with value: " << it_l30.pointer_to_node()->next->data << " will be erased by address: " << it_l30.pointer_to_node()->next << std::endl;
    llist_30.erase_after(it_l30);
//-----------------------------------------------------------------------------------------------------------------------------------------
    print_head("data_after_erasing_from_LinkedList");
    print_addr_all(llist_30);

    llist_30.Add(1515); //Make sure that the addresses of the new elements match the addresses of the earlier erased ones.
    llist_30.Add(2222); //This confirms that an allocator with the ext_realloc_strategy is reusing memory.
//-----------------------------------------------------------------------------------------------------------------------------------------
    print_head("data_after_inserting_to_LinkedList");
    print_addr_all(llist_30);

    std::cout << std::endl << "Make sure that the addresses of the new elements match the addresses of the earlier erased ones." << std::endl;
    std::cout << "This confirms that an allocator with the ext_realloc_strategy is reusing memory." << std::endl;
//-----------------------------------------------------------------------------------------------------------------------------------------
/*
    [[maybe_unused]] char a;
    a = getchar();
*/
    return 0;
}
