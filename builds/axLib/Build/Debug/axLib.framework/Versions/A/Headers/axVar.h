/*******************************************************************************
 * Copyright (c) 2015 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or axVar later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT axVar WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#ifndef __AX_VAR__
#define __AX_VAR__

// http://codereview.stackexchange.com/questions/20058/a-c11-any-class

#include <type_traits>
#include <utility>
#include <typeinfo>
#include <string>
#include <cassert>

//using namespace std;

//template<class T>
//using StorageType = typename decay<typename remove_reference<T>::type>::type;

template <class T>
using StorageType = typename std::decay<T>::type;

class axVar
{
public:
    bool is_null() const;
    bool not_null() const;
    
    template<typename U>
    axVar(U&& value) :
    ptr(new Derived<StorageType<U>>(std::forward<U>(value)))
    {
        
    }
    
    template<class U>
    bool is() const
    {
        typedef StorageType<U> T;
        
        auto derived = dynamic_cast<Derived<T>*>(ptr);
        
        return derived;
    }
    
    template<class U>
    StorageType<U>& as()
    {
        typedef StorageType<U> T;
        
        auto derived = dynamic_cast<Derived<T>*>(ptr);
        
        if(!derived)
        {
            throw std::bad_cast();
        }
        
        return derived->value;
    }
    
    template<class U>
    operator U()
    {
        return as<StorageType<U>>();
    }
    
    axVar() :
    ptr(nullptr)
    {
        
    }
    
    axVar(axVar& that) :
    ptr(that.clone())
    {
        
    }
    
    axVar(axVar&& that) :
    ptr(that.ptr)
    {
        that.ptr = nullptr;
    }
    
    axVar(const axVar& that) :
    ptr(that.clone())
    {
        
    }
    
    axVar(const axVar&& that) :
    ptr(that.clone())
    {
        
    }
    
    axVar& operator=(const axVar& a)
    {
        if(ptr == a.ptr)
        {
            return *this;
        }
        
        auto old_ptr = ptr;
        
        ptr = a.clone();
        
        if(old_ptr)
        {
            delete old_ptr;
        }
        
        return *this;
    }
    
    axVar& operator=(axVar&& a)
    {
        if(ptr == a.ptr)
        {
            return *this;
        }
        
        
        std::swap(ptr, a.ptr);
        
        return *this;
    }
    
    ~axVar()
    {
        if(ptr)
        {
            delete ptr;
        }
        
    }
    
private:
    class Base
    {
    public:
        virtual ~Base() {}
        virtual Base* clone() const = 0;
    };
    
    template<typename T>
    struct Derived : Base
    {
        template<typename U>
        Derived(U&& value) :
        value(std::forward<U>(value))
        {
        }
        
        T value;
        
        Base* clone() const
        {
            return new Derived<T>(value);
        }
    };
    
    Base* clone() const
    {
        if(ptr)
        {
            return ptr->clone();
        }
        else
        {
            return nullptr;
        }
    }
    
    Base* ptr;
};

#endif //__AX_VAR__


