#include <iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

int main(){
    srand(time(NULL));
    
    SkipList<std::string , std::string> skiplist(6);

    // insert_element ����
    std::cout << "-----------------insert_element ���� , ��ʼ-----------------" << std::endl;
    skiplist.insert_element("1" , "one");
    skiplist.insert_element("2" , "two");
    skiplist.insert_element("3" , "three");
    skiplist.insert_element("abc" , "����1");
    skiplist.insert_element("����" , "����2");

    std::cout << "skiplist size after insert_element: " << skiplist.size() << std::endl;
    std::cout << "-----------------insert_element ���� , ����-----------------" << std::endl << std::endl << std::endl;

    // search_element ����
    std::cout << "-----------------search_element ���� , ��ʼ-----------------" << std::endl;
    skiplist.search_element("3");
    skiplist.search_element("abc");
    skiplist.search_element("����");
    std::cout << "-----------------search_element ���� , ����-----------------" << std::endl << std::endl << std::endl;

    // delete_element ����
    std::cout << "-----------------delete_element ���� , ��ʼ-----------------" << std::endl;
    skiplist.delete_element("3");//�ɹ�
    skiplist.delete_element("5");//ʧ��
    std::cout << "skiplist size after delete_element: " << skiplist.size() << std::endl;
    std::cout << "-----------------delete_element ���� , ����-----------------" << std::endl << std::endl << std::endl;

    //update_element ����
    std::cout << "-----------------update_element ���� , ��ʼ-----------------" << std::endl;
    skiplist.update_element("abc" , "update ����3" , true);
    skiplist.update_element("5" , "update_false ����4" , false);
    skiplist.update_element("5" , "update_true ����5" , true);
    std::cout << "skiplist size after update_element: " << skiplist.size() << std::endl;
    std::cout << "-----------------update_element ���� , ����-----------------" << std::endl << std::endl << std::endl;

    // dump_file ����
    std::cout << "-----------------dump_file ���� , ��ʼ-----------------" << std::endl;
    skiplist.dump_file();
    std::cout << "-----------------dump_file ���� , ����-----------------" << std::endl << std::endl << std::endl;

    //display ����
    std::cout << "-----------------display ���� , ��ʼ-----------------" << std::endl;
    skiplist.display_list();
    std::cout << "-----------------display ���� , ����-----------------" << std::endl << std::endl << std::endl;

    //clear ����
    std::cout << "-----------------clear ���� , ��ʼ-----------------" << std::endl;
    skiplist.clear();
    std::cout << "skiplist size after clear: " << skiplist.size() << std::endl;
    std::cout << "-----------------clear ���� , ����-----------------" << std::endl << std::endl << std::endl;

    //load_file ����
    std::cout << "-----------------load_file ���� , ��ʼ-----------------" << std::endl;
    skiplist.load_file();
    std::cout << "skiplist size after load_file: " << skiplist.size() << std::endl;
    skiplist.display_list();
    std::cout << "-----------------load_file ���� , ����-----------------" << std::endl << std::endl << std::endl;

    return 0;
}