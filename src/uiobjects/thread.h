// Thread.h
#ifndef __THREAD_H__
#define __THREAD_H__
// #############################################################################
#include <pthread.h>
#define THREAD_STILL_ACTIVE 0x103
#define THREAD_NOT_INITIATED 0x102
#define THREAD_FINISHED 0x101

#include "Constant.h"
#include "gmutex.h"


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
    uint32_t   threadID;     // thread id - 0 until started
    T*      object;          // the object which owns the method
    Method  method;          // the method of the object

    //static volatile uint32_t status; //Para controlar el estado en UNIX
    volatile uint32_t status; //Para controlar el estado en UNIX
    GMutex *hSingleStart;
    GMutex *hFinishRun;
    static uint32_t ret;

    // This function gets executed by a concurrent thread.
    static void *run(void * thread_obj)
    {
        Thread<T>* thread = (Thread<T>*)thread_obj;
        thread->hFinishRun->Lock();
        ret = (thread->object->*thread->method) ();
        thread->setStatus(THREAD_FINISHED);
//        cout << "exiting thread now" << endl;
        thread->hFinishRun->Unlock();
        pthread_exit(&ret);
        return &ret;
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
        
        hSingleStart = new GMutex();
        hFinishRun = new GMutex();
    }
// -----------------------------------------------------------------------------
    ~Thread(void){
        //std::cout << "Thread::Destructor"<<std::endl;
        pthread_attr_destroy(&attr);
        delete hSingleStart;
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
        if (getStatus() != THREAD_STILL_ACTIVE){
            volatile int rc;
            hSingleStart->Lock();
            // Initialize and set thread joinable
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
            //El cuarto parametro le pasa argumentos a la funcion que llamamos
            rc = pthread_create(&hThread, &attr, Thread<T>::run, this);
            if (rc != 0){
//                std::cout << "Error:unable to create thread," << rc << std::endl;
                return false;
            }
            setStatus(THREAD_STILL_ACTIVE);
            hSingleStart->Unlock();
            return true;
        } else {
//            std::cout << "Error:Thread already running" << std::endl;
            return false;
        }
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
        pthread_attr_destroy(&attr);
//        std::cout << "join: completed thread";
//        std::cout << "  exiting with threadReturn :" << threadReturn << std::endl;
        return *((int*)(&threadReturn));
    }
    
//    // -----------------------------------------------------------------------------
//    /* Asks the thread to exit nicely. Thread function must implement checks.
//    return value indicates if the interrupt could be placed not if the thread
//    reacts on the interrupt. true indicates success, false an error. */
//    inline bool interrupt()
//    {
//        return false;
//    }
//// -----------------------------------------------------------------------------
//    /* True if an interrupt request was set, otherwise false. */
//    inline bool isInterrupted()
//    {
//        return false;
//    }
//// -----------------------------------------------------------------------------
//    /* True if an interrupt request was set, otherwise false. Waits for millisec
//    milliseconds for the interrupt to take place. */
//    inline bool isInterrupted(uint32_t millisec){
//        return false;
//    }
// -----------------------------------------------------------------------------
    inline volatile bool isRunning(){
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
//        uint32_t threadId = 0;
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
//        hFinishRun->Lock();
        this->status = var;
//        hFinishRun->Unlock();
//        std::cout << "status asignning: " << status << std::endl;
    }
// -----------------------------------------------------------------------------
};

//template <class T> volatile uint32_t Thread<T>::status;
template <class T> uint32_t Thread<T>::ret;
// #############################################################################

#endif // __THREAD_H__
