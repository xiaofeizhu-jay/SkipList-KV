#include <iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

int main(){
    srand(time(NULL));
    
    SkipList<std::string , std::string> skiplist(6);

    // insert_element ≤‚ ‘
    std::cout << "-----------------insert_element ≤‚ ‘ , ø™ º-----------------" << std::endl;
    skiplist.insert_element("1" , "one");
    skiplist.insert_element("2" , "two");
    skiplist.insert_element("3" , "three");
    skiplist.insert_element("abc" , "≤‚ ‘1");
    skiplist.insert_element("÷–Œƒ" , "≤‚ ‘2");

    std::cout << "skiplist size after insert_element: " << skiplist.size() << std::endl;
    std::cout << "-----------------insert_element ≤‚ ‘ , Ω· ¯-----------------" << std::endl << std::endl << std::endl;

    // search_element ≤‚ ‘
    std::cout << "-----------------search_element ≤‚ ‘ , ø™ º-----------------" << std::endl;
    skiplist.search_element("3");
    skiplist.search_element("abc");
    skiplist.search_element("÷–Œƒ");
    std::cout << "-----------------search_element ≤‚ ‘ , Ω· ¯-----------------" << std::endl << std::endl << std::endl;

    // delete_element ≤‚ ‘
    std::cout << "-----------------delete_element ≤‚ ‘ , ø™ º-----------------" << std::endl;
    skiplist.delete_element("3");//≥…π¶
    skiplist.delete_element("5");// ß∞‹
    std::cout << "skiplist size after delete_element: " << skiplist.size() << std::endl;
    std::cout << "-----------------delete_element ≤‚ ‘ , Ω· ¯-----------------" << std::endl << std::endl << std::endl;

    //update_element ≤‚ ‘
    std::cout << "-----------------update_element ≤‚ ‘ , ø™ º-----------------" << std::endl;
    skiplist.update_element("abc" , "update ≤‚ ‘3" , true);
    skiplist.update_element("5" , "update_false ≤‚ ‘4" , false);
    skiplist.update_element("5" , "update_true ≤‚ ‘5" , true);
    std::cout << "skiplist size after update_element: " << skiplist.size() << std::endl;
    std::cout << "-----------------update_element ≤‚ ‘ , Ω· ¯-----------------" << std::endl << std::endl << std::endl;

    // dump_file ≤‚ ‘
    std::cout << "-----------------dump_file ≤‚ ‘ , ø™ º-----------------" << std::endl;
    skiplist.dump_file();
    std::cout << "-----------------dump_file ≤‚ ‘ , Ω· ¯-----------------" << std::endl << std::endl << std::endl;

    //display ≤‚ ‘
    std::cout << "-----------------display ≤‚ ‘ , ø™ º-----------------" << std::endl;
    skiplist.display_list();
    std::cout << "-----------------display ≤‚ ‘ , Ω· ¯-----------------" << std::endl << std::endl << std::endl;

    //clear ≤‚ ‘
    std::cout << "-----------------clear ≤‚ ‘ , ø™ º-----------------" << std::endl;
    skiplist.clear();
    std::cout << "skiplist size after clear: " << skiplist.size() << std::endl;
    std::cout << "-----------------clear ≤‚ ‘ , Ω· ¯-----------------" << std::endl << std::endl << std::endl;

    //load_file ≤‚ ‘
    std::cout << "-----------------load_file ≤‚ ‘ , ø™ º-----------------" << std::endl;
    skiplist.load_file();
    std::cout << "skiplist size after load_file: " << skiplist.size() << std::endl;
    skiplist.display_list();
    std::cout << "-----------------load_file ≤‚ ‘ , Ω· ¯-----------------" << std::endl << std::endl << std::endl;

    return 0;
}