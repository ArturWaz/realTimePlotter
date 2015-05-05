#include "SharedTable.h"


/*
template <class T>
SharedTable<T>::SharedTable(boost::interprocess::create_only_t o, char const *name, boost::interprocess::mode_t mode, size_t size):
        sharedMemory_(boost::interprocess::create_only,name,mode),
        mtx(boost::interprocess::create_only,(std::string(name)+std::string("_mtx")).c_str()),
        createOnly_(true) {
    sharedMemory_.truncate(size*sizeof(T));
    mappedRegion_ = boost::interprocess::mapped_region(sharedMemory_,mode);
}

template <class T>
SharedTable<T>::SharedTable(boost::interprocess::open_only_t o, char const *name, boost::interprocess::mode_t mode):
        sharedMemory_(boost::interprocess::open_only,name,mode),
        mtx(boost::interprocess::open_only,
            (std::string(name)+std::string("_mtx")).c_str()),
        createOnly_(false) {
    mappedRegion_ = boost::interprocess::mapped_region(sharedMemory_,mode);
}


template <class T>
SharedTable<T>::~SharedTable() {
    if (createOnly_) {
        boost::interprocess::named_mutex::remove((std::string(sharedMemory_.get_name())+std::string("_mtx")).c_str());
        boost::interprocess::shared_memory_object::remove(sharedMemory_.get_name());
    }
}


*/