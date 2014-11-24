//                                      __
//     ____ ___  ___  ____ ___  ____   / /__   __
//    / __ `__ \/ _ \/ __ `__ \/ __ \ / __/ | / /
//   / / / / / /  __/ / / / / / /_/ // /_ | |/ /
//  /_/ /_/ /_/\___/_/ /_/ /_/\____(_)__/ |___/
//
//
//  Created by Memo Akten, www.memo.tv
//
//  ofxMSAControlFreakGui
//

#pragma once

//#include <vector>
//#include <map>
//#include <string>
#include "ofMain.h"

namespace msa {
    
    template<typename keyType, typename T>
    class OrderedPointerMap {
    public:
        
        OrderedPointerMap();
        virtual ~OrderedPointerMap();
        
        // get size
        int size() const;
        
        // add new item
        // item will be cloned, stored internally and added to an stl::map and stl::vector as a pointer
        // pointer will be owned and deleted by this class (unless set to not own, see below)
        // returns a reference to the new pointer added
        T& push_back(const keyType& key, const T& t);
        
        // add new item
        // this pointer will be added directly to the stl::map and stl::vector
        // pointer will be owned and deleted by this class (unless set to not own, see below)
        // returns a reference to the (same) pointer
        T& push_back(const keyType& key, T* t);

        
        // return reference to the stored object
        // throws an exception of the index or key doesn't exist
        T& at(int index) const;         // get by index
        T& at(const keyType& key) const;       // get by key
        
        // [] operator overloads for above
        // these also throw an exception if the index or key doesn't exist
        T& operator[](int index) const;
        T& operator[](const keyType& key) const;
        
        // get the key for item at index. returns "" if doesn't exist
        keyType keyFor(int index) const;
        
        // get the index for item with key. returns -ve if doesn't exist
        int indexFor(const keyType& key) const;
        
        // see if key exists
        bool exists(const keyType& key) const;
        
        // change key
        void changeKey(int index, const keyType& newKey);
        void changeKey(const keyType& oldKey, const keyType& newKey);

        // erase
        void erase(int index);
        void erase(const keyType& key);
        
        // set or get whether the instance of OrderedPointerMap owns the objects
        // if OwnsObjects is true (default), all pointers will be deleted automatically (i.e. when erasing items or destroying the container)
        // otherwise, it's the apps responsibility to delete the pointers
        void setOwnsObjects(bool b);
        bool getOwnsObjects();
        
        // clear, and delete pointers (if owns them)
        void clear();
        

        // ADVANCED
        // if you know the index and the key
        // fast erase without any validity checks
        void fastErase(int index, const keyType& key);
        
    private:
        map<keyType, pair<int, T*> > _map;
        vector< pair<keyType, T*> >	_vector;
        bool _bOwnsObjects;
        
        void validateIndex(int index, string errorMessage) const;
        void validateKey(const keyType& key, string errorMessage) const;
        
        
        // if something is erased, the indices in the map need to be updated
        void updateMapIndices();
    };
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    OrderedPointerMap<keyType, T>::OrderedPointerMap() {
        setOwnsObjects(true);
    }
    
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    OrderedPointerMap<keyType, T>::~OrderedPointerMap() {
        clear();
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::clear() {
        if(_bOwnsObjects) {
            for(int i=0; i<_vector.size(); i++) delete _vector[i].second;
        }
        
        _vector.clear();
        _map.clear();
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    int OrderedPointerMap<keyType, T>::size() const {
        // if these aren't equal, something went wrong somewhere. not good!
        if(_map.size() != _vector.size()) throw runtime_error("msa::OrderedPointerMap::size() - map size doesn't equal vector size");
        return _vector.size();
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    T& OrderedPointerMap<keyType, T>::push_back(const keyType& key, const T& t) {
        if(exists(key)) {
            throw invalid_argument("msa::OrderedPointerMap::push_back(keyType, T&) - key already exists");
            return at(key);
        } else {
            return push_back(key, new T(t));
        }
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    T& OrderedPointerMap<keyType, T>::push_back(const keyType& key, T* t) {
        if(exists(key)) {
            throw invalid_argument("msa::OrderedPointerMap::push_back(keyType, T*) - key already exists");
            return at(key);
        } else {
            _vector.push_back(pair<keyType, T*>(key, t));
            _map[key] = pair<int, T*>(_vector.size()-1, t);
            size();	// to validate if correctly added to both containers, should be ok
            return *t;
        }
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    T& OrderedPointerMap<keyType, T>::at(int index) const {
        validateIndex(index, "msa::OrderedPointerMap::at(int)");
        return *_vector[index].second;
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    T& OrderedPointerMap<keyType, T>::at(const keyType& key) const {
        validateKey(key, "msa::OrderedPointerMap::at(keyType)");
        return *_map.at(key).second;
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    T& OrderedPointerMap<keyType, T>::operator[](int index) const {
        return at(index);
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    T& OrderedPointerMap<keyType, T>::operator[](const keyType& key) const {
        return at(key);
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    keyType OrderedPointerMap<keyType, T>::keyFor(int index) const {
        validateIndex(index, "msa::OrderedPointerMap::keyFor(int)");
        return _vector[index].first;
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    int OrderedPointerMap<keyType, T>::indexFor(const keyType& key) const {
        validateKey(key, "msa::OrderedPointerMap::indexFor(keyType)");
        return _map.at(key).first;
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::changeKey(int index, const keyType& newKey) {
        validateIndex(index, "msa::OrderedPointerMap::changeKey(int)");

        // erase from map, and reinsert
        _map.erase(keyFor(index));
        _map[newKey] = pair<int, T*>(index, _vector[index].second);
        
        // change key from the vector
        _vector.at(index).first = newKey;
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::changeKey(const keyType& oldKey, const keyType& newKey) {
        validateKey(oldKey, "msa::OrderedPointerMap::changeKey(keyType)");
        changeKey(indexFor(oldKey), newKey);
    }
    


    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::erase(int index) {
        validateIndex(index, "msa::OrderedPointerMap::erase(int)");
        fastErase(index, keyFor(index));
        size(); // validate map and vector have same sizes to make sure everything worked alright
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::erase(const keyType& key) {
        validateKey(key, "msa::OrderedPointerMap::erase(keyType)");
        fastErase(indexFor(key), key);
        size(); // validate map and vector have same sizes to make sure everything worked alright
    }

    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::fastErase(int index, const keyType& key) {
        if(_bOwnsObjects) delete _vector[index].second;
        _map.erase(key);
        _vector.erase(_vector.begin() + index);
        updateMapIndices();
    }

    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::setOwnsObjects(bool b) {
        _bOwnsObjects = b;
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    bool OrderedPointerMap<keyType, T>::getOwnsObjects() {
        return _bOwnsObjects;
    }
    

    //--------------------------------------------------------------
    template<typename keyType, typename T>
    bool OrderedPointerMap<keyType, T>::exists(const keyType& key) const {
        return _map.find(key) != _map.end();
    }

    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::validateIndex(int index, string errorMessage) const {
        if(index<0 || index >= _vector.size()) throw invalid_argument(errorMessage + " - index doesn't exist");
    }
    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::validateKey(const keyType& key, string errorMessage) const {
        if(!exists(key)) throw invalid_argument(errorMessage + " - key doesn't exist");
    }

    
    //--------------------------------------------------------------
    template<typename keyType, typename T>
    void OrderedPointerMap<keyType, T>::updateMapIndices() {
        for(int i=0; i<_vector.size(); i++) {
            const keyType& key = _vector[i].first;
            _map[key].first = i;
        }
    }


    
}