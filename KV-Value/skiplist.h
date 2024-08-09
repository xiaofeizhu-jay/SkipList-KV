#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>

#define STORE_FILE "store/dumpFile"

std::mutex mtx , mtx1;
std::string delimiter = ":";

//ģ����������ģ����Ķ�����Ҫ��������ģ�������Ȼ�����ࡣ
template<typename K , typename V>
class Node{
    public:
        Node() {}             //Ĭ�Ϲ���
        Node(K k , V v , int);//���������캯��
        ~Node();              //��������
        K get_key() const;    //��ȡkey
        V get_value() const;  //��ȡvalue
        void set_value(V);    //����value
        Node<K,V> **forward;  //ָ����һ���ָ������
        int node_level;       //�ڵ����
        int update_element(const K key , const V value , bool flag = false); //���½ڵ�,���м���ֵ��һ����־�����������־����Ϊtrue�����ʾ�ýڵ���һ���½ڵ�
    private:
        K key;
        V value;
};

template<typename K , typename V>
Node<K,V>::Node(const K k , const V v , int level){
    this->key = k;
    this->value = v;
    this->node_level = level;

    this->forward = new Node<K,V>*[level+1];

    memset(this->forward , 0 , sizeof(Node<K,V>*)*(level+1));
};

template<typename K , typename V>
Node<K,V>::~Node(){
    delete[] forward;
}

template<typename K , typename V>
K Node<K,V>::get_key() const{
    return key;
};

template<typename K , typename V>
V Node<K,V>::get_value() const{
    return value;
};

template<typename K , typename V>
void Node<K,V>::set_value(V v){
    this->value = v;
}

template<typename K , typename V>
class SkipList{
    public:
        SkipList(int);
        ~SkipList();
        int get_random_level();         //��ȡ�������
        Node<K,V>* create_node(K,V,int);//�����ڵ�
        int insert_element(K,V);        //����ڵ�
        int update_element(K,V,bool);   //���½ڵ�
        void display_list();            //��ʾ����
        bool search_element(K);         //���ҽڵ�
        void delete_element(K);         //ɾ���ڵ�
        void dump_file();               //����������д���ļ�
        void load_file();               //���ļ��м�����������
        void clear();                   //�������
        int size();                     //��ȡ�����С

    private:
        void get_key_value_from_string(const std::string& str , std::string* key , std::string* value); //���ַ����л�ȡ��ֵ��
        bool is_vaild_string(const std::string& str); //�ж��ַ����Ƿ���Ч

    private:
        int _max_level;                 //����������
        int _skip_list_level;           //����ǰ����
        Node<K,V>* _header;             //����ͷ�ڵ�

        std::ofstream _file_writer;     //�ļ�д����
        std::ifstream _file_reader;     //�ļ���ȡ��

        int _element_count;             //����Ԫ�ظ���
};

//�����µ�������
template<typename K , typename V>
Node<K,V>* SkipList<K,V>::create_node(const K k , const V v , int level){
    Node<K,V> *n = new Node<K,V>(k,v,level);
    return n;
}

//���룬����0��ʾ����ɹ���1��ʾ�Ѿ�����
template<typename K , typename V>
int SkipList<K,V>::insert_element(const K key , const V value){
    mtx.lock();   //����
    Node<K,V> *current = this->_header;    //��ͷ�ڵ㿪ʼ
    // ~ update ��һ��ָ�����飬�����ڴ���ָ�룬ָ�� node �ڵ�
    Node<K,V> *update[_max_level+1];       //���ڼ�¼ÿ��Ľڵ�
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    //����߲㿪ʼ���ң��ҵ�ÿ��Ĳ���λ��
    for(int i = _skip_list_level ; i >= 0 ; i--){
        //�����ǰ�����һ���ڵ㲻Ϊ�գ�����һ���ڵ�� key С��Ҫ����� key
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current; //��¼��ǰ��Ľڵ�
    }

    current = current->forward[0]; //��ǰ�ڵ�ָ����һ���ڵ�
    //�����ǰ�ڵ�� key ����Ҫ����� key�����ʾ�Ѿ�����
    if(current != NULL && current->get_key() == key){
        std::cout << "key : " << key << " , exists" << std::endl;
        mtx.unlock(); //����
        return 1;
    }

    // �����ǰ current �����ڣ����� current->get_key > key
    if(current == NULL || current->get_key() != key){
        // �������һ��������Ҳ����forward����ĳ���
        int random_level = get_random_level();
        // ~ ���� update ���飬ԭ�� [_skip_list_level random_level] ��Χ�ڵ� NULL ��Ϊ _header
        if(random_level > _skip_list_level){
            for(int i = _skip_list_level+1 ; i < random_level+1 ; i++)
                update[i] = _header;
            _skip_list_level = random_level;
        }
        // ~ ����һ���µĽڵ�
        Node<K,V> *inserted_node = create_node(key , value , random_level);
        // ~ ���½ڵ���뵽ÿһ���������
        for(int i = 0 ; i <= random_level ; i++){
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        std::cout << "Successfully inserted key : " << key << ",value : " << value << std::endl;
        _element_count++;
    }
    mtx.unlock(); //����
    return 0;
}

//��ӡ����
template<typename K , typename V>
void SkipList<K,V>::display_list(){
    std::cout << " ***** Skip List *****" << std::endl;
    for(int i = 0 ; i <= _skip_list_level ; i++){
        Node<K,V> *node = this->_header->forward[i];
        std::cout << "Level " << i << " : ";
        while(node != NULL){
            std::cout << node->get_key() << ":" << node->get_value() << " ";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}

//�������
template<typename K , typename V>
void SkipList<K,V>::clear(){
    std::cout << "clear skip list" << std::endl;
    Node<K,V> *node = this->_header->forward[0];
    //�ӵ�һ�㿪ʼɾ��
    while(node != NULL){
        Node<K,V> *temp = node;
        node = node->forward[0];
        delete temp;
    }
    //���³�ʼ��
    for(int i = 0 ; i <= _max_level ; i++)
        this->_header->forward[i] = 0;
    this->_skip_list_level = 0;
    this->_element_count = 0;
}

//������д���ļ�
template<typename K , typename V>
void SkipList<K,V>::dump_file(){
    std::cout << "dump file...." << std::endl;
    _file_writer.open(STORE_FILE);
    Node<K,V> *node = this->_header->forward[0];
    //ֻд�� key:value
    while(node != NULL){
        _file_writer << node->get_key() << ":" << node->get_value() << std::endl;//д���ļ�
        std::cout << node->get_key() << ":" << node->get_value() <<  ";" << std::endl;//��ӡ
        node = node->forward[0];
    }
    _file_writer.flush();
    _file_writer.close();
    return ;
}

//���ļ��м�����������
template<typename K , typename V>
void SkipList<K,V>::load_file(){
    _file_reader.open(STORE_FILE);
    std::cout << "load file...." << std::endl;
    std::string line;
    std::string *key = new std::string();
    std::string *value = new std::string();

    while(getline(_file_reader , line)){
        get_key_value_from_string(line , key , value);//key��value��һ��ָ��string�����ָ��
        if(key->empty() || value->empty())
            continue;
        insert_element(*key , *value);//����������̣�level�ᷢ���仯����֮ǰ��SkipList��ͬ
        std::cout << "key : " << *key << " , value : " << *value << std::endl;
    }
    _file_reader.close();
}

//��ȡ�����С
template<typename K , typename V>
int SkipList<K,V>::size(){
    return _element_count;
}

//���ַ����л�ȡ��ֵ��
template<typename K  , typename V>
void SkipList<K,V>::get_key_value_from_string(const std::string& str , std::string* key , std::string* value){
    if(!is_vaild_string(str))
        return;
    *key = str.substr(0 , str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1 , str.length());
}

//�ж��ַ����Ƿ���Ч
template<typename K , typename V>
bool SkipList<K,V>::is_vaild_string(const std::string& str){
    if(str.empty())
        return false;
    if(str.find(delimiter) == std::string::npos)
        return false;
    return true;
}

//ɾ���ڵ�
template<typename K , typename V>
void SkipList<K,V>::delete_element(K key){
    mtx.lock();
    Node<K,V> *current = this->_header;
    Node<K,V> *update[_max_level + 1];
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    //����߲㿪ʼ����
    for(int i = _skip_list_level ; i >= 0 ; i--){
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];
    //�����ǰ�ڵ�� key ������Ҫɾ���� key�����ʾ������
    if(current != NULL && current->get_key() == key){
        for(int i = 0 ; i <= _skip_list_level ; i++){
            if(update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }
        //����ȷ��_skip_list_level����Ϊ����ɾ����Ԫ�ز���ǡ���ǵ�ǰ�����������
        while(_skip_list_level > 0 && _header->forward[_skip_list_level] == 0)
            _skip_list_level--;
        std::cout << "Successfully deleted key : " << key << std::endl;
        _element_count--;
    }
    else{
        std::cout << "key : " << key << " , not exists , please check your input !" << std::endl;
        mtx.unlock();
        return;
    }
    mtx.unlock();
    return;
}

//���ҽڵ�
template<typename K , typename V>
bool SkipList<K,V>::search_element(K key){
    Node<K,V> *current = _header;

    //����߲㿪ʼ����
    for(int i = _skip_list_level ; i >= 0 ; i--){
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key){
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if(current != NULL && current->get_key() == key){
        std::cout << "key : " << key << " , value : " << current->get_value() << std::endl;
        return true;
    }
    std::cout << "key : " << key << " , not exists" << std::endl;
    return false;
}

//���½ڵ�
template <typename K, typename V>
int SkipList<K,V>::update_element(const K key, const V value, bool flag){
    mtx1.lock(); //����
    Node<K,V> *current = this->_header;
    Node<K,V> *update[_max_level+1];
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    //����߲㿪ʼ����
    for(int i = _skip_list_level ; i >= 0 ; i--){
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    } 
    current = current->forward[0];
    
    //�����ǰ�ڵ�� key ����Ҫ���µ� key�������
    if(current != NULL && current->get_key() == key){
        std::cout << "key : " << key << " , old value : " << current->get_value() << std::endl;
        current->set_value(value);
        std::cout << "key : " << key << " , new value : " << current->get_value() << std::endl;
        mtx1.unlock(); //����
        return 1;  // ~ ����Ԫ���Ѿ����ڣ�ֻ���޸Ĳ��������� 1
    }
    if(flag){
        SkipList<K,V>::insert_element(key , value);
        mtx1.unlock(); //����
        return 0;  // ~ ����Ԫ�ز����ڣ��������������� 0
    }
    else{
        std::cout << "key : " << key << " , not exists , please check your input !" << std::endl;
        mtx1.unlock(); //����
        return -1;  // ~ ����Ԫ�ز����ڣ������������� -1
    }
}

//���캯��
template<typename K , typename V>
SkipList<K,V>::SkipList(int max_level){
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;

    K k;
    V v;
    this->_header = new Node<K,V>(k , v , _max_level);
};

//��������
template<typename K , typename V>
SkipList<K,V>::~SkipList(){
    if(_file_writer.is_open())
        _file_writer.close();
    if(_file_reader.is_open())
        _file_reader.close();
    delete _header;
}

//��ȡ�������
template<typename K , typename V>
int SkipList<K,V>::get_random_level(){
    int k = 1;
    while(rand()%2)
        k++;
    k = (k < _max_level) ? k : _max_level;
    return k;
};