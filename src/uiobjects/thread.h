// Thread.h
#ifndef __THREAD_H__
#define __THREAD_H__
// #############################################################################
#include <pthread.h>
#define THREAD_STILL_ACTIVE 2
#define THREAD_NOT_INITIATED 1
#define THREAD_FINISHED 0

#include "uiobjects/Constant.h"
//#include "uiobjects/gmutex.h"
#include <mutex>


// =============================================================================
template<class T>
class Thread
{
    // new type Method: pointer to a object's method (this call)
    typedef uint32_t (T::* Method)(void);
// -----------------------------------------------------------------------------
protected:
        pthread_t hThread;
        pthread_attr_t attr;
        void *threadReturn;


private:
    uint32_t    threadID;     // thread id - 0 until started
    T*          object;          // the object which owns the method
    Method      method;          // the method of the object

    //static volatile uint32_t status; //Para controlar el estado en UNIX
    uint32_t status;            //Para controlar el estado en UNIX
//    pthread_mutex_t hSingleStart;
    std::mutex hSingleStart;
    std::mutex hSingleRun;
    uint32_t ret;


    // This function gets executed by a concurrent thread.
    static void *run(void * thread_obj)
    {
        Thread<T>* thread = (Thread<T>*)thread_obj;
//        const std::lock_guard<std::mutex> lock(thread->hSingleRun);
        thread->ret = (thread->object->*thread->method) ();
        pthread_attr_destroy(&thread->attr);
        pthread_exit((void*) (uintptr_t) thread->ret);
        return (void*) (uintptr_t) thread->ret;
    }
    // Prevent copying of threads: No sensible implementation!
    Thread(const Thread<T>& other) {}
    // Prevent assignment of threads: No sensible implementation!
    Thread<T>& operator =(const Thread<T>& other) {}
// -----------------------------------------------------------------------------
public:
    /* Creates a new Thread object. object: the one which method should be
    executed. method: pointer to the object's method. */
    explicit Thread(T* object, uint32_t ( T::* method)(void)){

        this->object        = object;
        this->method        = method;
        this->threadID      = 0;
        setStatus(THREAD_NOT_INITIATED);
//        pthread_mutex_init(&hSingleStart, 0);
    }
// -----------------------------------------------------------------------------
    ~Thread(void){
        //std::cout << "Thread::Destructor"<<std::endl;
        //pthread_mutex_destroy(&hSingleStart);

    }
// -----------------------------------------------------------------------------
    T* getObject(){
        return object;
    }
    void setObject(T* pObject){
        object = pObject;
    }
// -----------------------------------------------------------------------------
    /* Starts executing the objects method in a concurrent thread. True if the
    thread was started successfully; otherwise false. */
    bool start(){
        bool ret = false;
        if (getStatus() != THREAD_STILL_ACTIVE){
            uint32_t rc;
            const std::lock_guard<std::mutex> lock(hSingleStart);
            setStatus(THREAD_STILL_ACTIVE);
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
            //El cuarto parametro es esta propia clase para poder acceder a las propiedades
            //desde el metodo run estatico
            rc = pthread_create(&hThread, &attr, Thread<T>::run, (void *)this);
            ret = (rc == 0);
//        } else {
//            std::cout << "Error:Thread already running" << std::endl;
        }
        return ret;
    }
// -----------------------------------------------------------------------------
    // Blocks the calling thread until this thread has stopped.
    inline int join(){
        int rc;
        rc = pthread_join(hThread, &threadReturn);

        if (rc != 0){
//            std::cout << "Error:unable to join," << rc << std::endl;
            return -1;
        }
        setStatus(THREAD_FINISHED);
//        std::cout << "join: completed thread";
//        std::cout << "  exiting with threadReturn :" << threadReturn << std::endl;
        return *((int*)(&threadReturn));
    }

// -----------------------------------------------------------------------------
    inline bool isRunning(){
        return getStatus() == THREAD_STILL_ACTIVE;
    }
// -----------------------------------------------------------------------------
    // Getter & Setter
// -----------------------------------------------------------------------------
    inline pthread_t getThreadHandle(){
        return hThread;
    }
// -----------------------------------------------------------------------------
    inline uint32_t getThreadID(){
        pthread_t ptid = pthread_self();
        memcpy(&this->threadID, &ptid, std::min(sizeof(this->threadID), sizeof(ptid)));
        return this->threadID;
    }

    inline uint32_t getExitCode(){
        return *((int*)(&threadReturn));
    }

    uint32_t inline getStatus(){
        return this->status;
    }

    inline void setStatus(uint32_t var){
        this->status = var;
//        std::cout << "status asignning: " << status << std::endl;
    }
// -----------------------------------------------------------------------------
};

//template <class T> volatile uint32_t Thread<T>::status;
//template <class T> uint32_t Thread<T>::ret;
// #############################################################################

#endif // __THREAD_H__
