#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>

#define STORE_FILE "store/dumpFile"

std::mutex mtx , mtx1;
std::string delimiter = ":";

//模板类声明。模板类的定义需要首先声明模板参数，然后定义类。
template<typename K , typename V>
class Node{
    public:
        Node() {}             //默认构造
        Node(K k , V v , int);//带参数构造函数
        ~Node();              //析构函数
        K get_key() const;    //获取key
        V get_value() const;  //获取value
        void set_value(V);    //设置value
        Node<K,V> **forward;  //指向下一层的指针数组
        int node_level;       //节点层数
        int update_element(const K key , const V value , bool flag = false); //更新节点,带有键、值和一个标志参数。如果标志参数为true，则表示该节点是一个新节点
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
        int get_random_level();         //获取随机层数
        Node<K,V>* create_node(K,V,int);//创建节点
        int insert_element(K,V);        //插入节点
        int update_element(K,V,bool);   //更新节点
        void display_list();            //显示跳表
        bool search_element(K);         //查找节点
        void delete_element(K);         //删除节点
        void dump_file();               //将跳表数据写入文件
        void load_file();               //从文件中加载跳表数据
        void clear();                   //清空跳表
        int size();                     //获取跳表大小

    private:
        void get_key_value_from_string(const std::string& str , std::string* key , std::string* value); //从字符串中获取键值对
        bool is_vaild_string(const std::string& str); //判断字符串是否有效

    private:
        int _max_level;                 //跳表最大层数
        int _skip_list_level;           //跳表当前层数
        Node<K,V>* _header;             //跳表头节点

        std::ofstream _file_writer;     //文件写入流
        std::ifstream _file_reader;     //文件读取流

        int _element_count;             //跳表元素个数
};

//创建新的跳表结点
template<typename K , typename V>
Node<K,V>* SkipList<K,V>::create_node(const K k , const V v , int level){
    Node<K,V> *n = new Node<K,V>(k,v,level);
    return n;
}

//插入，返回0表示插入成功，1表示已经存在
template<typename K , typename V>
int SkipList<K,V>::insert_element(const K key , const V value){
    mtx.lock();   //加锁
    Node<K,V> *current = this->_header;    //从头节点开始
    // ~ update 是一个指针数组，数组内存存放指针，指向 node 节点
    Node<K,V> *update[_max_level+1];       //用于记录每层的节点
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    //从最高层开始查找，找到每层的插入位置
    for(int i = _skip_list_level ; i >= 0 ; i--){
        //如果当前层的下一个节点不为空，且下一个节点的 key 小于要插入的 key
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current; //记录当前层的节点
    }

    current = current->forward[0]; //当前节点指向下一个节点
    //如果当前节点的 key 等于要插入的 key，则表示已经存在
    if(current != NULL && current->get_key() == key){
        std::cout << "key : " << key << " , exists" << std::endl;
        mtx.unlock(); //解锁
        return 1;
    }

    // 如果当前 current 不存在，或者 current->get_key > key
    if(current == NULL || current->get_key() != key){
        // 随机生成一个层数，也就是forward数组的长度
        int random_level = get_random_level();
        // ~ 更新 update 数组，原本 [_skip_list_level random_level] 范围内的 NULL 改为 _header
        if(random_level > _skip_list_level){
            for(int i = _skip_list_level+1 ; i < random_level+1 ; i++)
                update[i] = _header;
            _skip_list_level = random_level;
        }
        // ~ 创建一个新的节点
        Node<K,V> *inserted_node = create_node(key , value , random_level);
        // ~ 将新节点插入到每一层的链表中
        for(int i = 0 ; i <= random_level ; i++){
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        std::cout << "Successfully inserted key : " << key << ",value : " << value << std::endl;
        _element_count++;
    }
    mtx.unlock(); //解锁
    return 0;
}

//打印跳表
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

//清空跳表
template<typename K , typename V>
void SkipList<K,V>::clear(){
    std::cout << "clear skip list" << std::endl;
    Node<K,V> *node = this->_header->forward[0];
    //从第一层开始删除
    while(node != NULL){
        Node<K,V> *temp = node;
        node = node->forward[0];
        delete temp;
    }
    //重新初始化
    for(int i = 0 ; i <= _max_level ; i++)
        this->_header->forward[i] = 0;
    this->_skip_list_level = 0;
    this->_element_count = 0;
}

//将跳表写入文件
template<typename K , typename V>
void SkipList<K,V>::dump_file(){
    std::cout << "dump file...." << std::endl;
    _file_writer.open(STORE_FILE);
    Node<K,V> *node = this->_header->forward[0];
    //只写入 key:value
    while(node != NULL){
        _file_writer << node->get_key() << ":" << node->get_value() << std::endl;//写入文件
        std::cout << node->get_key() << ":" << node->get_value() <<  ";" << std::endl;//打印
        node = node->forward[0];
    }
    _file_writer.flush();
    _file_writer.close();
    return ;
}

//从文件中加载跳表数据
template<typename K , typename V>
void SkipList<K,V>::load_file(){
    _file_reader.open(STORE_FILE);
    std::cout << "load file...." << std::endl;
    std::string line;
    std::string *key = new std::string();
    std::string *value = new std::string();

    while(getline(_file_reader , line)){
        get_key_value_from_string(line , key , value);//key和value是一个指向string对象的指针
        if(key->empty() || value->empty())
            continue;
        insert_element(*key , *value);//重新载入过程，level会发生变化，与之前的SkipList不同
        std::cout << "key : " << *key << " , value : " << *value << std::endl;
    }
    _file_reader.close();
}

//获取跳表大小
template<typename K , typename V>
int SkipList<K,V>::size(){
    return _element_count;
}

//从字符串中获取键值对
template<typename K  , typename V>
void SkipList<K,V>::get_key_value_from_string(const std::string& str , std::string* key , std::string* value){
    if(!is_vaild_string(str))
        return;
    *key = str.substr(0 , str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1 , str.length());
}

//判断字符串是否有效
template<typename K , typename V>
bool SkipList<K,V>::is_vaild_string(const std::string& str){
    if(str.empty())
        return false;
    if(str.find(delimiter) == std::string::npos)
        return false;
    return true;
}

//删除节点
template<typename K , typename V>
void SkipList<K,V>::delete_element(K key){
    mtx.lock();
    Node<K,V> *current = this->_header;
    Node<K,V> *update[_max_level + 1];
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    //从最高层开始查找
    for(int i = _skip_list_level ; i >= 0 ; i--){
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];
    //如果当前节点的 key 不等于要删除的 key，则表示不存在
    if(current != NULL && current->get_key() == key){
        for(int i = 0 ; i <= _skip_list_level ; i++){
            if(update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }
        //重新确定_skip_list_level，因为可能删除的元素层数恰好是当前跳表的最大层数
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

//查找节点
template<typename K , typename V>
bool SkipList<K,V>::search_element(K key){
    Node<K,V> *current = _header;

    //从最高层开始查找
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

//更新节点
template <typename K, typename V>
int SkipList<K,V>::update_element(const K key, const V value, bool flag){
    mtx1.lock(); //加锁
    Node<K,V> *current = this->_header;
    Node<K,V> *update[_max_level+1];
    memset(update , 0 , sizeof(Node<K,V>*)*(_max_level+1));

    //从最高层开始查找
    for(int i = _skip_list_level ; i >= 0 ; i--){
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    } 
    current = current->forward[0];
    
    //如果当前节点的 key 等于要更新的 key，则更新
    if(current != NULL && current->get_key() == key){
        std::cout << "key : " << key << " , old value : " << current->get_value() << std::endl;
        current->set_value(value);
        std::cout << "key : " << key << " , new value : " << current->get_value() << std::endl;
        mtx1.unlock(); //解锁
        return 1;  // ~ 插入元素已经存在，只是修改操作，返回 1
    }
    if(flag){
        SkipList<K,V>::insert_element(key , value);
        mtx1.unlock(); //解锁
        return 0;  // ~ 插入元素不存在，创建了它，返回 0
    }
    else{
        std::cout << "key : " << key << " , not exists , please check your input !" << std::endl;
        mtx1.unlock(); //解锁
        return -1;  // ~ 插入元素不存在，不创建，返回 -1
    }
}

//构造函数
template<typename K , typename V>
SkipList<K,V>::SkipList(int max_level){
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;

    K k;
    V v;
    this->_header = new Node<K,V>(k , v , _max_level);
};

//析构函数
template<typename K , typename V>
SkipList<K,V>::~SkipList(){
    if(_file_writer.is_open())
        _file_writer.close();
    if(_file_reader.is_open())
        _file_reader.close();
    delete _header;
}

//获取随机层数
template<typename K , typename V>
int SkipList<K,V>::get_random_level(){
    int k = 1;
    while(rand()%2)
        k++;
    k = (k < _max_level) ? k : _max_level;
    return k;
};