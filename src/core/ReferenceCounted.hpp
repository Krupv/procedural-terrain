// #pragma once
// #include "../main.hpp"
//
//
//
// template <class T>
// class IReferenceCounted {
// public:
// 	typedef IReferenceCounted ref;
//
// 	IReferenceCounted() { m_refCount = 1; };
// 	virtual ~IReferenceCounted() { };
//
// 	inline unsigned int incRef() const
// 	{
// 		return m_refCount++;
// 	}
//
// 	inline unsigned int  decRef() const
// 	{
// 		return m_refCount--;
// 	}
//
// 	inline unsigned int  getRefCount() const { return m_refCount; };
//
// 	inline int* drop() const
// 	{
// 		if (decRef() <= 0)
// 			delete this;
// 		return nullptr;
// 	}
//
// 	//implemented in 'child' classes
// 	inline virtual T* getRef() = 0;
//
//
// private:
// 	mutable unsigned int m_refCount;
//
// };
//
// //template <class T>
// //class ReferenceCounted : public IReferenceCounted {
// //public:
// //	typedef IReferenceCounted super;
// //
// //	ReferenceCounted() { };
// //	~ReferenceCounted() { };
// //
// //	inline virtual T* add() = 0;
// //};
