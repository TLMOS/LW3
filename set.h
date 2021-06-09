#include <cstdlib>
#include <string>
#include <stdexcept>
#include <iostream>
#include "collection_interface.h"
#include "binary_tree.h"
#pragma once

template<class T>
class Set : public ICollection<T>
{
private:
	BTree<T>* items_;

public:
	~Set();
	Set();
	Set(T* keys, size_t count);
	Set(const Set<T>& set);
	Set(std::string src);

	void Add(T k);
	void Remove(T k);
	bool Contains(T k);
	bool Contains(Set<T>* set);
	bool Equals(Set<T>* set);

	Set<T>* Union(Set<T>* set);
	Set<T>* Intersection(Set<T>* set);
	Set<T>* Subtraction(Set<T>* set);

	Set<T>* Map(T(*f)(T));
	Set<T>* Where(bool (*f)(T));
	T Reduce(T(*f)(T, T), T c);

	std::string ToString();

	//ICollection
	virtual T GetFirst() const;
	virtual T GetLast() const;
	virtual T Get(size_t index);
	virtual size_t GetSize() const;
	virtual const T& operator[](size_t index);
};

template<class T>
inline Set<T>::~Set()
{
	delete items_;
}

template<class T>
inline Set<T>::Set()
{
	items_ = new BTree<T>();
}

template<class T>
inline Set<T>::Set(T* keys, size_t count)
{
	items_ = new BTree<T>();
	for (int i = 0; i < count; i++)
		Add(keys[i]);
}

template<class T>
inline Set<T>::Set(const Set<T>& set)
{
	items_ = new BTree<T>(*set.items_);
}

template<class T>
inline Set<T>::Set(std::string src)
{
	items_ = new BTree<T>();
	src = src.substr(1, src.length() - 2);
	
	size_t comma_index = src.find(",");
	while (comma_index != std::string::npos)
	{
		T item = TFromString(src.substr(0, comma_index));
		Add(item);
		src = src.substr(comma_index + 2, src.length() - comma_index - 2);
		comma_index = src.find(",");
	}
	if (src.length() > 0)
	{
		T item = TFromString(src.substr(0, comma_index));
		Add(item);
	}
}

template<class T>
inline void Set<T>::Add(T k)
{
	if (!items_->Contains(k))
		items_->Insert(k);
}

template<class T>
inline void Set<T>::Remove(T k)
{
	items_->Remove(k);
}

template<class T>
inline bool Set<T>::Contains(T k)
{
	return items_->Contains(k);
}

template<class T>
inline bool Set<T>::Contains(Set<T>* set)
{
	for (int i = 0; i < set->GetSize(); i++)
	{
		if (!items_->Contains(set->Get(i)))
			return false;
	}
	return true;
}

template<class T>
inline bool Set<T>::Equals(Set<T>* set)
{
	if (set->GetSize() != GetSize())
		return false;
	else
	{
		for (int i = 0; i < set->GetSize(); i++)
		{
			if (!items_->Contains(set->Get(i)))
				return false;
		}
		return true;
	}
}

template<class T>
inline Set<T>* Set<T>::Union(Set<T>* set)
{
	Set<T>* result = new Set<T>(*this);
	for (int i = 0; i < set->GetSize(); i++)
	{
		T item = set->Get(i);
		result->Add(item);
	}
	return result;
}

template<class T>
inline Set<T>* Set<T>::Intersection(Set<T>* set)
{
	Set<T>* result = new Set<T>();
	for (int i = 0; i < set->GetSize(); i++)
	{
		T item = set->Get(i);
		if (Contains(item))
			result->Add(item);
	}
	return result;
}

template<class T>
inline Set<T>* Set<T>::Subtraction(Set<T>* set)
{
	Set<T>* result = new Set<T>();
	for (int i = 0; i < GetSize(); i++)
	{
		T item = Get(i);
		if (!set->Contains(item))
			result->Add(item);
	}
	return result;
}

template<class T>
inline Set<T>* Set<T>::Map(T(*f)(T))
{
	Set<T>* set = new Set<T>();
	set->items_ = items_->Map(f);
	return set;
}

template<class T>
inline Set<T>* Set<T>::Where(bool(*f)(T))
{
	Set<T>* set = new Set<T>();
	set->items_ = items_->Where(f);
	return set;
}

template<class T>
inline T Set<T>::Reduce(T(*f)(T, T), T c)
{
	return items_->Reduce(f, c);
}

template<class T>
inline std::string Set<T>::ToString()
{
	std::string str = "{";
	if (GetSize() > 0)
	{
		str += "" + std::to_string(Get(0));
		for (int i = 1; i < GetSize(); i++)
			str += ", " + std::to_string(Get(i));
	}
	str += "}";
	return str;
}

template<class T>
inline T Set<T>::GetFirst() const
{
	return items_->GetFirst();
}

template<class T>
inline T Set<T>::GetLast() const
{
	return items_->GetLast();
}

template<class T>
inline T Set<T>::Get(size_t index)
{
	return items_->Get(index);
}

template<class T>
inline size_t Set<T>::GetSize() const
{
	return items_->GetSize();
}

template<class T>
inline const T& Set<T>::operator[](size_t index)
{
	return (*items_)[index];
}
