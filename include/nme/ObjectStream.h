#ifndef NME_OBJECT_STREAM_H
#define NME_OBJECT_STREAM_H

#include "Object.h"
#include "QuickVec.h"
#include <string>

#ifdef HXCPP_JS_PRIME
#include <emscripten.h>
#include <emscripten/bind.h>
#endif

namespace nme
{


struct ObjectStreamOut
{
   QuickVec<unsigned char> data;
   bool parentToo;

   ObjectStreamOut(bool inParentToo=true) : parentToo(inParentToo)
   {
   }
   virtual ~ObjectStreamOut() { }

   static ObjectStreamOut *createEncoder(int inFlags);

   inline bool empty() const { return data.empty(); }

   virtual void addObject(Object *inObj) = 0;

   inline int addInt(int inVal)
   {
      data.append((unsigned char *)&inVal,4);
      return inVal;
   }

   inline void append(const void *inData, int inBytes)
   {
      data.append((unsigned char *)inData, inBytes);
   }
   template<typename T>
   void add(const T& inData)
   {
      data.append((unsigned char *)&inData, sizeof(T));
   }
   template<typename T,int N>
   void addVec(const QuickVec<T,N> &inData)
   {
      addInt(inData.size());
      append(inData.ByteData(), inData.ByteCount());
   }
   void add(const std::wstring &inVal)
   {
      addInt(inVal.size());
      append(inVal.c_str(),inVal.size()*sizeof(wchar_t));
   }

   bool addBool(bool inValue)
   {
      add(inValue);
      return inValue;
   }

};


struct ObjectStreamIn
{
   bool newIds;
   const unsigned char *ptr;
   int len;

   ObjectStreamIn(const unsigned char *inPtr, int inLength)
       : ptr(inPtr), len(inLength)
   {
      newIds = false;
   }
   virtual ~ObjectStreamIn() { }

   static ObjectStreamIn *createDecoder(const unsigned char *inPtr, int inLength,int inFlags);

   virtual void linkAbstract(Object *inObject) { }


   inline int getInt()
   {
      if (len<=0)
         return 0;
      int result;
      memcpy(&result,ptr,4);
      ptr+=4;
      len-=4;
      return result;
   }
   inline const unsigned char *getBytes(int inLen)
   {
      const unsigned char *result = ptr;
      ptr+=inLen;
      len-=inLen;
      return result;
   }

   template<typename T>
   void get(T& outData)
   {
      memcpy(&outData, ptr, sizeof(T));
      ptr+=sizeof(T);
      len-=sizeof(T);
   }
   template<typename T,int N>
   void getVec(QuickVec<T,N> &outData)
   {
      int n = getInt();
      outData.resize(n);
      int size = n*sizeof(T);
      memcpy(outData.ByteData(), getBytes(size),size);
   }
   void get(std::wstring &outVal)
   {
      int size = getInt();
      wchar_t *data = (wchar_t *)getBytes(size*sizeof(wchar_t));
      outVal = std::wstring( data, data+size );
   }


   bool getBool()
   {
      if (len<=0)
      {
         printf("EOF!\n");
         return false;
      }
      bool result=false;
      get(result);
      return result;
   }

   virtual Object *decodeObject() = 0;


   template<typename T>
   void getObject(T *&outObject,bool inAddRef=true)
   {
      if (getBool())
      {
         Object *obj = decodeObject();
         outObject = dynamic_cast<T*>(obj);
         if (obj && !outObject)
         {
            printf("got object(%d), but wrong type %p\n", obj->getObjectType(), obj);
            *(int *)0=0;
         }
         else if (obj && inAddRef)
            obj->IncRef();
         else if (!obj)
            printf("Bad obj logic\n");
      }
      else
      {
         outObject = 0;
      }
   }
};


}
#endif




