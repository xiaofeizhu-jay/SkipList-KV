#include<iostream>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<mutex>
#include<fstream>

#define STORE_PATH "./store/dumpFile" 

std::mutex mtx , mtx1;
std::string delimiter = ":";

template<typename K , typename V>
class Node{
    public:
        Node() {};
        Node(K k , V v , int);
        ~Node();
        K get_key() const;
        V get_value() const;
        void set_value(V);
        Node<K,V> **forward;
        int node_level;
        int update_element(const K key , const V value , bool flag = false);//���½ڵ�,���м���ֵ��һ����־�����������־����Ϊtrue�����ʾ�ýڵ���һ���½ڵ�
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
};

template<typename K , typename V>
class SkipList{
    public:
        SkipList(int);
        ~SkipList();
        int get_random_level();      //生成随机层数
        Node<K,V>* create_node(K,V,int);    //创建节点
        std::string insert_element(K,V);   //插入元素
        std::string update_element(K,V,bool);   //更新元素
        std::string display_list();    //显示链表
        std::string search_element(K);   //查找元素
        std::string delete_element(K);   //删除元素
        std::string dump_file();   //将链表数�?写入文件
        std::string load_file();   //从文件中加载链表数据
        std::string clear();   //清空链表
        int size();   //返回链表大小
    private:
        void get_key_value_from_string(const std::string& str , std::string* key , std::string* value);
        bool is_valid_string(const std::string& str);

    private:
        int _max_level;     //最大层�?
        int _skip_list_level;   //当前层数
        Node<K,V>* _header;   //头节�?

        std::ofstream _file_writer;  //文件写入�?
        std::ifstream _file_reader;  //文件读取�?

        int _element_count;   //元素�?�?
};

template<typename K , typename V>
SkipList<K,V>::SkipList(int max_level){
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;
    K k;
    V v;
    this->_header = new Node<K,V>(k,v,max_level);
};

template<typename K , typename V>
SkipList<K,V>::~SkipList(){
    if(_file_writer.is_open())
        _file_writer.close();
    if(_file_reader.is_open())
        _file_reader.close();
    delete _header;
};

template<typename K , typename V>
int SkipList<K,V>::get_random_level(){
    int k = 1;
    while(rand()%2)
        k++;
    k = (k < _max_level) ? k : _max_level;
    return k;
};

template<typename K , typename V>
Node<K,V>* SkipList<K,V>::create_node(const K key , const V value , int level){
    Node<K,V>* n = new Node<K,V>(key,value,level);
    return n;
};

template<typename K , typename V>
std::string SkipList<K,V>:: insert_element(const K key , const V value){
    mtx.lock();
    std::string res;
    Node<K,V>* current = this->_header;
    Node<K,V> *update[_max_level+1];
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    for(int i = _skip_list_level ; i >= 0 ; i--){
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    }
    current = current->forward[0];
    if(current != NULL && current->get_key() == key){
        std::cout << "key : " << key << " , exists" << std::endl << std::endl;
        res = "key" + key + "exists";
        mtx.unlock();
        return res;
    }
    if(current == NULL || current->get_key() != key){
        int random_level = get_random_level();
        if(random_level > _skip_list_level){
            for(int i = _skip_list_level+1 ; i < random_level + 1 ; i++)
                update[i] = _header;
            _skip_list_level = random_level;
        }
        Node<K,V>* inserted_node = create_node(key , value , random_level);
        for(int i = 0 ; i <= random_level ; i++){
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        std::cout << "Successfully inserted key : " << key << " , value : " << value << std::endl << std::endl;
        res = "Successfully inserted key : " + key + " , value : " + value;
        // std::cout << std::endl;
        _element_count++;
    }
    mtx.unlock();
    return res;
}

template<typename K , typename V>
std::string SkipList<K,V>::display_list(){
    mtx.lock();
    std::string res;
    std::cout << "Displaying Skiplist : " << std::endl;
    res = "Displaying Skiplist : \n";
    for(int i = 0 ; i <= _skip_list_level ; i++){
        Node<K,V>* node = this->_header->forward[i];
        std::cout << "Level " << i << " : ";
        res += "Level " + std::to_string(i) + " : ";
        while(node != NULL){ 
            std::cout << node->get_key() << ":" << node->get_value() << " ";
            res += node->get_key() + ":" + node->get_value() + " ";
            node = node->forward[i];
        }
        res += "\n";
        std::cout << std::endl;
    }
    mtx.unlock();
    return res;
}

template<typename K , typename V>
std::string SkipList<K,V>::clear(){
    std::string res;
    std::cout << "Clearing Skiplist : " << std::endl;
    res = "Clearing Skiplist : \n";
    Node<K,V> *node = this->_header->forward[0];
    while(node != NULL){
        Node<K,V> *temp = node;
        node = node->forward[0];
        delete temp;
    }
    for(int i = 0 ; i <= _max_level ; i++)
        _header->forward[i] = NULL;
    this->_skip_list_level = 0;
    this->_element_count = 0;
    return res;
}

template<typename K , typename V>
std::string SkipList<K,V>::dump_file(){
    mtx.lock();
    std::string res;
    std::cout << "Dumping Skiplist to file : " << std::endl;
    res = "Dumping Skiplist to file : \n";
    _file_writer.open(STORE_PATH);
    Node<K,V> *node = this->_header->forward[0];
    while(node != NULL){
        _file_writer << node->get_key() << ":" << node->get_value() << std::endl;
        std::cout << "Dumped key : " << node->get_key() << " , value : " << node->get_value() << std::endl;
        res += "Dumped key : " + node->get_key() + " , value : " + node->get_value() + "\n";
        node = node->forward[0];
    }
    _file_writer.flush();
    _file_writer.close();
    mtx.unlock();
    return res;
}

template<typename K , typename V>
std::string SkipList<K,V>::load_file(){
    std::lock_guard<std::mutex> lock(mtx1); // RAII方式管理�?
    std::string res;
    _file_reader.open(STORE_PATH);
    std::cout << "Loading Skiplist from file : " << std::endl;
    res = "Loading Skiplist from file : \n";
    std::string line;
    std::string *key = new std::string();
    std::string *value = new std::string();

    while(std::getline(_file_reader , line)){
        get_key_value_from_string(line , key , value);
        if(key->empty() || value->empty())
            continue;
        insert_element(*key , *value);
        std::cout << "Loaded key : " << *key << " , value : " << *value << std::endl << std::endl;
        res += "Loaded key : " + *key + " , value : " + *value + "\n";
    }
    _file_reader.close();
    return res;
}

template<typename K , typename V>
void SkipList<K,V>::get_key_value_from_string(const std::string& str , std::string* key , std::string* value){
    if(!is_valid_string(str))
        return ;
    *key = str.substr(0 , str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1 , str.length());
}

template<typename K , typename V>
bool SkipList<K,V>::is_valid_string(const std::string& str){
    if(str.empty())
        return false;
    if(str.find(delimiter) == std::string::npos)
        return false;
    return true;
};

template<typename K , typename V>
int SkipList<K,V>::size(){
    return _element_count;
}

template<typename K , typename V>
std::string SkipList<K,V>::delete_element(K key){
    mtx.lock();
    std::string res;
    Node<K,V> *current = this->_header;
    Node<K,V> *update[_max_level+1];
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    for(int i = _skip_list_level ; i >= 0 ; i--){
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    }
    current = current->forward[0];

    if(current != NULL && current->get_key() == key){
        for(int i = 0 ; i <= _skip_list_level ; i++){
            if(update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }
        while(_skip_list_level > 0 && _header->forward[_skip_list_level] == 0)
            _skip_list_level--;
        std::cout << "Successfully deleted key : " << key << std::endl << std::endl;
        res = "Successfully deleted key : " + key;
        _element_count--;
    }
    else{
        std::cout << "Key : " << key << " , not found" << std::endl << std::endl;
        res = "Key : " + key + " , not found";
        mtx.unlock();
        return res;
    }
    mtx.unlock();
    return res;
}

template<typename K , typename V>
std::string SkipList<K,V>::search_element(K key){
    mtx.lock();
    std::string res;
    Node<K,V> *current = _header;

    for(int i = _skip_list_level ; i >= 0 ; i--){
        if(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
    }
    current = current->forward[0];
    if(current != NULL && current->get_key() == key){
        std::cout << "Key : " << key << " , exists : " << current->get_value() << std::endl << std::endl;
        res = "Key : " + key + " , exists : " + current->get_value();
        mtx.unlock();
        return res;
    }
    else{
        std::cout << "Key : " << key << " , not exists" << std::endl << std::endl;
        res = "Key : " + key + " , not exists";
        mtx.unlock();
        return res;
    }
}

template<typename K , typename V>
std::string SkipList<K,V>::update_element(K key , const V value , bool flag){
    mtx1.lock();
    std::string res;
    Node<K,V> *current = this->_header;
    Node<K,V> *update[_max_level+1];
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    for(int i = _skip_list_level ; i >= 0 ; i--){
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    }
    current = current->forward[0];

    if(current != NULL && current->get_key() == key){
        std::cout << "key : " << key << " , old value : " << current->get_value() << std::endl;
        res = "key : " + key + " , old value : " + current->get_value();
        current->set_value(value);
        std::cout << "key : " << key << " , new value : " << current->get_value() << std::endl;
        res += "key : " + key + " , new value : " + current->get_value();
        mtx1.unlock();
        return res;
    } 
    else if(flag){
        res = insert_element(key , value);
        mtx1.unlock();
        return res;
    }
    else{
        std::cout << "key : " << key << " , not found" << std::endl;
        res = "key : " + key + " , not found";
        mtx1.unlock();
        return res;
    }
}