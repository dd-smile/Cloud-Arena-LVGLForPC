/*
 * @Description: socket通用库与线程池
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-09-21 11:28:17
 * @LastEditTime: 2023-09-22 15:59:05
 * @FilePath: /avant_stadiumForPC/lv_node/src/display/connect_common.h
 */
#ifndef __SERVER_TCP_H__
#define __SERVER_TCP_H__

typedef struct {
    void *(*function)(void *);  /*函数指针，回调函数*/
    void *arg;                  /*上面函数的参数*/
} threadpool_task_t;            /*各子线程任务结构体*/

typedef struct threadpool_s {
    pthread_mutex_t lock;            /*用于锁住本结构体*/
    pthread_mutex_t thread_counter;  /*记录忙状态线程个数的锁 -- busy_thread_num*/
    pthread_cond_t queue_not_full;   /*当队列任务满时，添加任务的线程阻塞，等待此条件变量*/
    pthread_cond_t queue_not_empty;  /*任务队列不为空时，通知等待任务的线程*/

    pthread_t *workers_tid;          /*存放线程池中每个线程的tid，数组*/
    pthread_t manager_tid;           /*存管理线程tid*/
    threadpool_task_t *task_queue;   /*任务队列*/

    int min_thread_num;                /*线程池最小线程数*/
    int max_thread_num;                /*线程池最大线程数*/
    int live_thread_num;               /*当前存活线程个数*/
    int busy_thread_num;               /*忙线程个数*/
    int wait_exit_thr_num;           /*要销毁的线程个数*/

    int queue_front;                 /*task_queue队头下表*/
    int queue_rear;                  /*task_queue队尾下表*/
    int queue_size;                  /*task_queue队列中实际任务数*/
    int queue_max_size;              /*task_queue队列可容纳任务上限*/

    int shutdown;                    /*标志位，线程池使用状态，true或者false*/
} threadpool_t;   //　线程池结构体

#define DEFAULT_TIME 			60
#define MIN_WAIT_TASK_NUM  		10
#define DEFAULT_THREAD_VERY  	5
#define NEUTRAL_BUFFER_SIZE 1024
extern threadpool_t *thp;

int createSocket();  
int bindSocket(int lfd, unsigned short port);  
int setListen(int lfd);  
int acceptConn(int lfd, struct sockaddr_in *addr); 
int connectToHost(int fd, const char* ip, unsigned short port); 
int closeSocket(int fd);  

int writen(int fd, const char* msg, int size);
int sendMsg(int cfd, char* msg, int len);
int readn(int fd, char* buf, int size);
int recvMsg(int cfd, char** msg);

threadpool_t *threadpool_create(int min_thread_num, int max_thread_num, int queue_max_size);
void *manager_thread(void *threadpool);
void *workers_thread(void *threadpool);
int threadpool_add(threadpool_t *pool, void *function(void *arg), void *arg);
int threadpool_distory(threadpool_t *pool);
int threadpool_free(threadpool_t *pool);
int is_thread_alive(pthread_t tid);

int socketconnected(int sockfd);


void create_serverTemHum();

void *create_client_light();
void *create_client_zhongli();
void *create_client_abesn();




#endif // __DEVICE_H__
