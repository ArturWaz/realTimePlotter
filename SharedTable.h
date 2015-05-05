//
// Created by geox on 05.05.15.
//

#ifndef ONLINEPLOTTER_SHAREDTABLE_H
#define ONLINEPLOTTER_SHAREDTABLE_H


#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>



template <class T>
class SharedTable {
    static_assert(std::is_arithmetic<T>::value, "\nOnly basic types are supported.\n");

    void operator=(SharedTable const &) {}

    inline T &get_(size_t i) { return (static_cast<T*>(mappedRegion_.get_address()))[i]; }

    class Exception : public std::exception {
        char const *info;
    public:
        explicit Exception(char const *info): info(info) {}
        virtual char const *what() const throw() { return info; }
    };


    boost::interprocess::mapped_region mappedRegion_;
    boost::interprocess::named_mutex mtx;

#ifdef __linux__ // linux

    boost::interprocess::shared_memory_object sharedMemory_;
    bool createOnly_;

public:

    SharedTable(boost::interprocess::create_only_t o, char const *name, boost::interprocess::mode_t mode, size_t size)
            : sharedMemory_(boost::interprocess::create_only,name,mode),
              mtx(boost::interprocess::create_only,(std::string(name)+std::string("_mtx")).c_str()),
              createOnly_(true) {
        sharedMemory_.truncate(size*sizeof(T));
        mappedRegion_ = boost::interprocess::mapped_region(sharedMemory_,mode);
    }
    SharedTable(boost::interprocess::open_only_t o, char const *name, boost::interprocess::mode_t mode)
            : sharedMemory_(boost::interprocess::open_only,name,mode),
              mtx(boost::interprocess::open_only,(std::string(name)+std::string("_mtx")).c_str()),
              createOnly_(false) {
        mappedRegion_ = boost::interprocess::mapped_region(sharedMemory_,mode);
    }
    ~SharedTable() {
        if (createOnly_) {
            boost::interprocess::named_mutex::remove((std::string(sharedMemory_.get_name())+std::string("_mtx")).c_str());
            boost::interprocess::shared_memory_object::remove(sharedMemory_.get_name());
        }
    }

#else // windows

    boost::interprocess::windows_shared_memory sharedMemory_;

public:

    SharedTable::SharedTable(boost::interprocess::create_only_t, char const *name, boost::interprocess::mode_t mode, size_t size)
                : sharedMemory_(boost::interprocess::create_only,name,mode,size),
                  mtx(boost::interprocess::create_only,(std::string(name)+std::string("_mtx")).c_str()) {
        sharedMemory_.truncate(size*sizeof(T));
        mappedRegion_.mapped_region(sharedMemory_,mode);
    }
    SharedTable::SharedTable(boost::interprocess::open_only_t, char const *name, boost::interprocess::mode_t mode)
                : sharedMemory_(boost::interprocess::open_only,name,mode),
                  mtx(boost::interprocess::open_only,(std::string(name)+std::string("_mtx")).c_str()) {
        mappedRegion_.mapped_region(sharedMemory_,mode);
    }
    SharedTable::~SharedTable() {}

#endif


    inline size_t size() const { return mappedRegion_.get_size()/sizeof(T); }
    inline size_t length() const { return mappedRegion_.get_size()/sizeof(T); }


    inline void readData(T *table) {
        mtx.lock();
        std::memcpy(table,mappedRegion_.get_address(),mappedRegion_.get_size());
        mtx.unlock();
    }

    inline void writeData(T *table) {
        mtx.lock();
        std::memcpy(mappedRegion_.get_address(),table,mappedRegion_.get_size());
        mtx.unlock();
    }


    void readData(std::vector<T> &v) {
        if (v.size() != size()) throw Exception("Sizes of shared table and std::vector are different, function: SharedTable::getTable(std::vector<T> &)");
        mtx.lock();
        for (int i = 0; i < v.size(); ++i) {
            v[i] = get_(i);
        }
        mtx.unlock();
    }

    void writeData(std::vector<T> const &v) {
        if (v.size() != size()) throw Exception("Sizes of shared table and std::vector are different, function: SharedTable::setTable(std::vector<T> const &)");
        mtx.lock();
        for (int i = 0; i < v.size(); ++i) {
            get_(i) = v[i];
        }
        mtx.unlock();
    }

};


#endif //ONLINEPLOTTER_SHAREDTABLE_H
