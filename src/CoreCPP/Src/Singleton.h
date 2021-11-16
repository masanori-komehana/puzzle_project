/*
 * Singleton.h
 *
 *  Created on: 2021/11/16
 *      Author: st202109
 */

#ifndef SRC_SINGLETON_H_
#define SRC_SINGLETON_H_

 template<class T>
 class Singleton
 {
 public:
     static inline T& GetInstance()
     {
         static T instance;
         return instance;
     }

 protected:
     Singleton() {}
     virtual ~Singleton() {}

 private:
     void operator=(const Singleton& obj) {}
     Singleton(const Singleton &obj) {}
 };

#endif /* SRC_SINGLETON_H_ */
