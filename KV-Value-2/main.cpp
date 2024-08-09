#include<iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

// int main(){
//     srand(time(NULL));
//     SkipList<std::string , std::string> skiplist(6);

//     // insert_element test
//     std::cout << "-----------------insert_element test , start-----------------" << std::endl;
//     skiplist.insert_element("1" , "one");
//     skiplist.insert_element("2" , "two");
//     skiplist.insert_element("3" , "three");
//     skiplist.insert_element("abc" , "²âÊÔ1");
//     skiplist.insert_element("ÖĞÎÄ" , "²âÊÔ2");

//     std::cout << "skiplist size after insert_element: " << skiplist.size() << std::endl;
//     std::cout << "-----------------insert_element test , end-------------------" << std::endl << std::endl << std::endl;


//     // search_element ²âÊÔ
//     std::cout << "-----------------search_element ²âÊÔ , ¿ªÊ¼-------------------" << std::endl;
//     skiplist.search_element("3");
//     skiplist.search_element("abc");
//     skiplist.search_element("ÖĞÎÄ");
//     skiplist.search_element("5");
//     std::cout << "-----------------search_element ²âÊÔ , ½áÊø-------------------" << std::endl << std::endl << std::endl;


//     // delete_element ²âÊÔ
//     std::cout << "-----------------delete_element ²âÊÔ , ¿ªÊ¼-------------------" << std::endl;
//     skiplist.delete_element("3");//³É¹¦
//     skiplist.delete_element("5");//Ê§°Ü
//     std::cout << "skiplist size after delete_element: " << skiplist.size() << std::endl;
//     std::cout << "-----------------delete_element ²âÊÔ , ½áÊø-------------------" << std::endl << std::endl << std::endl;

//     //update_element ²âÊÔ
//     std::cout << "-----------------update_element ²âÊÔ , ¿ªÊ¼-------------------" << std::endl;
//     skiplist.update_element("abc" , "update ²âÊÔ3" , true);
//     skiplist.update_element("5" , "update_false ²âÊÔ4" , false);
//     skiplist.update_element("5" , "update_true ²âÊÔ5" , true);
//     std::cout << "skiplist size after update_element: " << skiplist.size() << std::endl;
//     std::cout << "-----------------update_element ²âÊÔ , ½áÊø-------------------" << std::endl << std::endl << std::endl;

//     // dump_file ²âÊÔ
//     std::cout << "-----------------dump_file ²âÊÔ , ¿ªÊ¼------------------------" << std::endl;
//     skiplist.dump_file();
//     std::cout << "-----------------dump_file ²âÊÔ , ½áÊø------------------------" << std::endl << std::endl << std::endl;

//     //display ²âÊÔ
//     std::cout << "-----------------display ²âÊÔ , ¿ªÊ¼--------------------------" << std::endl;
//     skiplist.display_list();
//     std::cout << "-----------------display ²âÊÔ , ½áÊø--------------------------" << std::endl << std::endl << std::endl;

//     //clear ²âÊÔ
//     std::cout << "-----------------clear ²âÊÔ , ¿ªÊ¼----------------------------" << std::endl;
//     skiplist.clear();
//     std::cout << "skiplist size after clear: " << skiplist.size() << std::endl;
//     std::cout << "-----------------clear ²âÊÔ , ½áÊø----------------------------" << std::endl << std::endl << std::endl;

//     //load_file ²âÊÔ
//     std::cout << "-----------------load_file ²âÊÔ , ¿ªÊ¼-----------------" << std::endl;
//     skiplist.load_file();
//     std::cout << "skiplist size after load_file: " << skiplist.size() << std::endl;
//     skiplist.display_list();
//     std::cout << "-----------------load_file ²âÊÔ , ½áÊø-----------------" << std::endl << std::endl << std::endl;

//     return 0;
// }
int main() {
    srand(time(NULL));
    SkipList<std::string, std::string> skiplist(6);
    std::string key, value;
    int command;

    while (true) {
        std::cout << "-----------------SkipList Test-----------------" << std::endl;
        std::cout << "Enter command: (1) insert, (2) search,  (3) delete " << std::endl;
        std::cout << "               (4) update, (5) display, (6) dump "   << std::endl;
        std::cout << "               (7) load,   (8) clear,   (9) exit: "  << std::endl;
        std::cout << "                           (0) size               "  << std::endl;
        std::cout << "-----------------------------------------------" << std::endl;
        std::cin >> command;

        switch (command) {
            case 0:
                std::cout << "Size: " << skiplist.size() << std::endl;
                break;
            case 1:
                std::cout << "Enter key: ";
                std::cin >> key;
                std::cout << "Enter value: ";
                std::cin >> value;
                skiplist.insert_element(key, value);
                break;
            case 2:
                std::cout << "Enter key: ";
                std::cin >> key;
                skiplist.search_element(key);
                break;
            case 3:
                std::cout << "Enter key: ";
                std::cin >> key;
                skiplist.delete_element(key);
                break;
            case 4:
                std::cout << "Enter key: ";
                std::cin >> key;
                std::cout << "Enter new value: ";
                std::cin >> value;
                std::cout << "Insert if not exists? (1 for yes, 0 for no): ";
                bool flag;
                std::cin >> flag;
                skiplist.update_element(key, value, flag);
                break;
            case 5:
                skiplist.display_list();
                break;
            case 6:
                skiplist.dump_file();
                break;
            case 7:
                skiplist.load_file();
                break;
            case 8:
                skiplist.clear();
                break;
            case 9:
                return 0;
            default:
                std::cout << "Unknown command. Please try again." << std::endl;
                break;
        }
    }
    return 0;
}
