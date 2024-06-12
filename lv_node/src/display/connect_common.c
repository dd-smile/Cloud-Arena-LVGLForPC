/*
 * @Description: socket通用库与线程池
 * @Author: da
 * @LastEditors: da
 * @Date: 2023-09-21 11:28:17
 * @LastEditTime: 2023-09-22 15:59:05
 * @FilePath: /avant_stadiumForPC/lv_node/src/display/connect_common.c
 */

#include "ui_app.h"

threadpool_t *thp;
static char *s_common_connect_err;

/**
 * 创建套接字
 * @return      　　　返回创建的文件描述符
 * */
int createSocket()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1)
    {
        s_common_connect_err = strerror(errno);
		log_err_printf(s_common_connect_err);
        // perror("socket");
        return -1;
    }

    //printf("套接字创建成功, fd=%d\n", fd);
    return fd;
}

/**
 * 绑定本地的IP和端口
 * @param lfd         创建的文件描述符
 * @param port        端口号  
 * */
int bindSocket(int lfd, unsigned short port)
{
    char pp[24] = {0};

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);   //大端端口
    saddr.sin_addr.s_addr = INADDR_ANY;  // 0 = 0.0.0.0

    printf("服务器的IP地址: %s, 端口: %d\n",
          inet_ntop(AF_INET, &saddr.sin_addr.s_addr, pp, sizeof(pp)),
          ntohs(saddr.sin_port));
    // 设置ＳＯ_ＲＥＵＳＥＡＤＤＲ，快速重启服务器，防止出现Address in use
    int on = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); // 端口复用

    int ret = bind(lfd, (struct sockaddr*)&saddr, sizeof(saddr));

    if (ret == -1)
    {
        s_common_connect_err = strerror(errno);
		log_err_printf(s_common_connect_err);
        // perror("bind");
        return -1;
    }

    printf("套接字绑定成功, ip: %s, port: %d\n",
           inet_ntoa(saddr.sin_addr), port);
    return ret;
}

/**
 * 设置监听
 * @param lfd         创建的文件描述符
 * */
int setListen(int lfd)
{
    int ret = listen(lfd, 128);
    if (ret == -1)
    {
        s_common_connect_err = strerror(errno);
		log_err_printf(s_common_connect_err);
        // perror("listen");
        return -1;
    }

    printf("设置监听成功...\n");
    return ret;
}

/**
 * 阻塞并等待客户端的连接
 * @param lfd         创建的文件描述符
 * @param addr        端口号  
 * */
int acceptConn(int lfd, struct sockaddr_in *addr)
{
    int cfd = -1;

    if (addr == NULL)
    {
        cfd = accept(lfd, NULL, NULL);
    }
    else
    {
        int addrlen = sizeof(struct sockaddr_in);
        cfd = accept(lfd, (struct sockaddr*)addr, &addrlen);
    }

    if (cfd == -1)
    {
        s_common_connect_err = strerror(errno);
		log_err_printf(s_common_connect_err);
        // perror("accept");
        return -1;
    }   

    printf("成功和客户端建立连接...\n");
    return cfd; 
}

/**
 * 连接服务器
 * @param fd          创建的文件描述符
 * @param ip          服务器IP地址
 * @param addr        端口号  
 * */
int connectToHost(int fd, const char* ip, unsigned short port)
{
    // 2. 连接服务器IP port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &saddr.sin_addr.s_addr);
    int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));

    if (ret == -1)
    {
        // s_common_connect_err = strerror(errno);
		// log_err_printf(s_common_connect_err);
        //perror("connect");
        return -1;
    }

    printf("成功和服务器建立连接...\n");
    return ret;
}

/**
 * 关闭套接字
 * @param fd         创建的文件描述符
 * */
int closeSocket(int fd)
{
    int ret = close(fd);

    if (ret == -1)
    {
        s_common_connect_err = strerror(errno);
		log_err_printf(s_common_connect_err);
        // perror("close");
    }

    return ret;
}

/**
 * 检测TCP连接是否断开
 * @param sockfd         创建的文件描述符
 * */
int socketconnected(int sockfd)
{
	struct tcp_info info;//其实我们就是使用到tcp_info结构体的tcpi_state成员变量来存取socket的连接状态，
	int len = sizeof(info);//如果此返回值为1，则说明socket连接正常，如果返回值为0，则说明连接异常。
							//所以我们也可以直接用一个整形变量来存这个值，然后进行判断即可。
	if (sockfd <= 0)
    {
		return 0;
    }

	getsockopt(sockfd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *) & len);

	if ((info.tcpi_state == 1)) 
	{
		//printf("socket connected\n");
		return 1;
	} 
    else 
    {
		//printf("socket disconnected\n");
		return 0;
	}
}

/*
函数描述: 发送指定的字节数
函数参数:
    - fd: 通信的文件描述符(套接字)
    - msg: 待发送的原始数据
    - size: 待发送的原始数据的总字节数
函数返回值: 函数调用成功返回发送的字节数, 发送失败返回-1
*/
int writen(int fd, const char* msg, int size)
{
    int left = size;
    int nwrite = 0;
    const char* p = msg;

    while (left > 0)
    {
        if ((nwrite = write(fd, msg, left)) > 0)
        {
            p += nwrite;
            left -= nwrite;
        }
        else if (nwrite == -1)
        {
            return -1;
        }
    }

    return size;
}

/*
函数描述: 发送带有数据头的数据包
函数参数:
    - cfd: 通信的文件描述符(套接字)
    - msg: 待发送的原始数据
    - len: 待发送的原始数据的总字节数
函数返回值: 函数调用成功返回发送的字节数, 发送失败返回-1
*/
int sendMsg(int cfd, char* msg, int len)
{
   if (msg == NULL || len <= 0 || cfd <=0)
   {
       return -1;
   }

   // 申请内存空间: 数据长度 + 包头4字节(存储数据长度)
   char* data = (char*)malloc(len+4);
   int bigLen = htonl(len);
   memcpy(data, &bigLen, 4);
   memcpy(data+4, msg, len);
   // 发送数据
   int ret = writen(cfd, data, len+4);
   // 释放内存
   free(data);
   return ret;
}

/*
函数描述: 接收指定的字节数
函数参数:
    - fd: 通信的文件描述符(套接字)
    - buf: 存储待接收数据的内存的起始地址
    - size: 指定要接收的字节数
函数返回值: 函数调用成功返回发送的字节数, 发送失败返回-1
*/
int readn(int fd, char* buf, int size)
{
    char* pt = buf;
    int count = size;
    while (count > 0)
    {
        int len = recv(fd, pt, count, 0);
        if (len == -1)
        {
            return -1;
        }
        else if (len == 0)
        {
            return size - count;
        }
        pt += len;
        count -= len;
    }
    return size;
}

/*
函数描述: 接收带数据头的数据包
函数参数:
    - cfd: 通信的文件描述符(套接字)
    - msg: 一级指针的地址，函数内部会给这个指针分配内存，用于存储待接收的数据，这块内存需要使用者释放
函数返回值: 函数调用成功返回接收的字节数, 发送失败返回-1
*/
int recvMsg(int cfd, char** msg)
{
    // 接收数据
    // 1. 读数据头
    int len = 0;
    readn(cfd, (char*)&len, 4);
    len = ntohl(len);
    printf("数据块大小: %d\n", len);

    // 根据读出的长度分配内存，+1 -> 这个字节存储\0
    char *buf = (char*)malloc(len+1);
    int ret = readn(cfd, buf, len);

    if (ret != len)
    {
        close(cfd);
        free(buf);
        return -1;
    }

    buf[len] = '\0';
    *msg = buf;

    return ret;
}


/*--------------------------------创建服务器执行线程--------------------------------*/

/**
 * 创建接收温湿度数据的服务器
*/
void create_serverTemHum()
{
  //创建监听的套接字
  g_lfd = createSocket();
  bindSocket(g_lfd, 9266);
  setListen(g_lfd);

  pthread_t tid_listen;  //用于监听
  pthread_create(&tid_listen, NULL, listening_temphum, NULL);
}



/*--------------------------------连接服务器执行线程--------------------------------*/



/**
 * 检测与灯光服务器的TCP连接是否断开，断开的话进行重新连接
 * @param parg         线程传入的参数
 * */
static void *JudgmentConnection(void *parg)
{
  g_light_fd = createSocket();  //创建套接字
  connectToHost(g_light_fd, VSU_SERVER_IP, VSU_SERVER_PORT);  //连接服务器
  while (1)
  {
    if (socketconnected(g_light_fd) == 0)
    {
      closeSocket(g_light_fd);
      g_light_fd = createSocket();  //创建套接字
      connectToHost(g_light_fd, VSU_SERVER_IP, VSU_SERVER_PORT);  //连接服务器
    }
    usleep(50 * 1000);
  }
}


static void *JudgmentZhongliConnection(void *parg)
{

  fd_set readfds;
  struct timeval tv;
	int retval;
  char buffer[NEUTRAL_BUFFER_SIZE] = {0};
  char hex_data[NEUTRAL_BUFFER_SIZE] = {0};
  char bin_data[NEUTRAL_BUFFER_SIZE] = {0};

  g_neutral_fd = createSocket();  //创建套接字
  connectToHost(g_neutral_fd, ZHONGLI_SERVER_IP, ZHONGLI_SERVER_PORT);  //连接服务器
  while (1)
  {

    // 初始化文件描述符集合
    FD_ZERO(&readfds);
    FD_SET(g_neutral_fd, &readfds);

    // 设置超时时间
    tv.tv_sec = 5;  // 设置秒
    tv.tv_usec = 0; // 设置微秒

    retval = select(g_neutral_fd + 1, &readfds, NULL, NULL, &tv);


    if (retval > 0)
		{
			if (FD_ISSET(g_neutral_fd, &readfds)) 
			{
				
				int len = recv(g_neutral_fd, buffer, 1024, 0);
        
				if (len > 0)
				{
					printf("接收到的16进制数据: %s\n", buffer);

          // 提取第2到第5个数据
          strncpy(hex_data, buffer + 6, 11);
          hex_data[11] = '\0';

          // 转换为二进制
          hex_to_bin(hex_data, bin_data);

          printf("二进制数据：%s\n", bin_data);

          // 分别提取4组二进制数据
          char pos_8_1[9], pos_15_9[9], rev_8_1[9], rev_15_9[9];
          strncpy(pos_8_1, bin_data, 8);
          pos_8_1[8] = '\0';
          strncpy(pos_15_9, bin_data + 8, 8);
          pos_15_9[8] = '\0';
          strncpy(rev_8_1, bin_data + 16, 8);
          rev_8_1[8] = '\0';
          strncpy(rev_15_9, bin_data + 24, 8);
          rev_15_9[8] = '\0';

          // 打印二进制数据
          printf("正转8-1 ");
          print_switch_states(pos_8_1, 1, "正转8-1");
          printf("正转15-9 ");
          print_switch_states(pos_15_9, 8, "正转15-9");
          printf("反转8-1 ");
          print_switch_states(rev_8_1, 1, "反转8-1");
          printf("反转15-9 ");
          print_switch_states(rev_15_9, 8, "反转15-9");
				}
			}
		}


    if (socketconnected(g_neutral_fd) == 0)
    {
      closeSocket(g_neutral_fd);
      g_neutral_fd = createSocket();  //创建套接字
      connectToHost(g_neutral_fd, VSU_SERVER_IP, VSU_SERVER_PORT);  //连接服务器
    }
    usleep(50 * 1000);
  }
}


/**
 * 检测与艾比森ｐｌｃ服务器的TCP连接是否断开，断开的话进行重新连接
 * @param parg         线程传入的参数
 * */
static void *JudgmentabesnConnection(void *parg)
{
  g_plc_fd = createSocket();  //创建套接字
  connectToHost(g_plc_fd, ABESN_PLC_SERVER_IP, ABESN_PLC_SERVER_PORT);
  while (1)
  {
    if (socketconnected(g_plc_fd) == 0)
    {
      closeSocket(g_plc_fd);
      g_plc_fd = createSocket();  //创建套接字
      connectToHost(g_plc_fd, ABESN_PLC_SERVER_IP, ABESN_PLC_SERVER_PORT);  //连接服务器
    }
    usleep(100 * 1000);
  }
}



/**
 * 创建modbus TCP灯光连接
 * */
void *create_client_light()
{   
    pthread_t tid;
    pthread_create(&tid, NULL, JudgmentConnection, NULL);
}

/**
 * 创建modbus TCP中立电箱连接
 * */
void *create_client_zhongli()
{   
    pthread_t tid;
    pthread_create(&tid, NULL, JudgmentZhongliConnection, NULL);
}

/**
 * 创建modbus TCP 艾比森的ｌｅｄ电源控制
*/
void *create_client_abesn()
{  
  pthread_t tid;
  pthread_create(&tid, NULL, JudgmentabesnConnection, NULL);
}




/*--------------------------------线程池--------------------------------*/


/***************************************************************************
* 								创建线程池
*函数名: 
*		 threadpool_create()
*参  数:
*		 min_thread_num	    ：线程池中最小线程数量
*		 max_thread_num		：线程池中最大线程数量
*		 queue_max_size		：任务队列的最大长度
*作  用: 
*		 创建一个指定大小的线程池
*内  容:
*		1）线程池基本参数
*		2）工作线程
*		3）管理线程
*		4）任务队列
*		5）互斥锁、条件变量
****************************************************************************/
threadpool_t *threadpool_create(int min_thread_num, int max_thread_num, int queue_max_size)
{
    int i;
    threadpool_t *pool = NULL;
    
    do 
    {
        pool = (threadpool_t *)malloc(sizeof(threadpool_t));
        if (pool == NULL) 
        {
            printf("malloc threadpool fail\n");
            goto err_1;
        }
        
        pool->min_thread_num  = min_thread_num;
        pool->max_thread_num  = max_thread_num;
        pool->busy_thread_num = 0;
        pool->live_thread_num = min_thread_num;
        pool->queue_size 	  = 0;
        pool->queue_max_size  = queue_max_size;
        pool->queue_front 	  = 0;
        pool->queue_rear 	  = 0;
        pool->shutdown        = 0;
        
        /*根据最大线程上限数，给工作线程数据开辟空间，并清零*/
        pool->workers_tid = (pthread_t *)malloc(sizeof(pthread_t) * max_thread_num);
        if (pool->workers_tid == NULL) 
        {
            printf("malloc workers_tid fail\n");
            goto err_2;
        }
        memset(pool->workers_tid, 0, sizeof(pthread_t) * max_thread_num);
        
        /* 队列开辟空间 */
        pool->task_queue = (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_max_size);
        if (pool->task_queue == NULL) 
        {
            printf("malloc task_queue fail\n");
            goto err_3;
        }
        
        /* 初始化互斥锁，条件变量 */
        if (pthread_mutex_init(&(pool->lock), NULL) != 0			|| 
            pthread_mutex_init(&(pool->thread_counter), NULL) != 0	|| 
            pthread_cond_init(&(pool->queue_not_empty), NULL) != 0	|| 
            pthread_cond_init(&(pool->queue_not_full), NULL)  != 0) {
                printf("init the lock or cond fail\n");
                goto err_4;
        }
        
        /* 启动 min_thread_num 个 work thread */
        for (i = 0; i < min_thread_num; i++) 
        {
            pthread_create(&(pool->workers_tid[i]), NULL, workers_thread,(void *)pool); /*pool指向当前线程池*/
            printf("start thread  0x%x...\n", (unsigned int)pool->workers_tid[i]);
        }
		/*创建管理者线程*/
        pthread_create(&(pool->manager_tid), NULL, manager_thread, (void *)pool);
        
    } while(0);
     
	return pool;
    //threadpool_free(pool);    /*前面代码调用失败，释放poll存储空间*/	
    err_4:
	/*需要销毁互斥锁和条件变量*/
	free(pool->task_queue);	
    err_3:
	free(pool->workers_tid);
    err_2:
	free(pool);
    err_1:	
    return NULL;
}

/***************************************************************************
* 								管理者线程
*函数名: 
*		manager_thread()
*参  数:
*		threadpool	: 使用的线程池
*作  用: 
*		 根据任务的数量动态调整线程池大小
*内  容:
*		1）获取当前线程池中存在的线程和任务队列中积累的任务
*		2）根据需求动态调整线程池中的线程的数量
*缺  点：
*		使用了太多的互斥锁和条件变量，效率上值得商榷
****************************************************************************/
void *manager_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool;
    int i;
    
    while (!pool->shutdown) 
    {
        sleep(DEFAULT_TIME);   /*定时对线程池管理*/
        
        pthread_mutex_lock(&(pool->lock));
        int queue_size = pool->queue_size;
        int live_thread_num = pool->live_thread_num;/*线程池中存在的线程数量*/
        pthread_mutex_unlock(&(pool->lock));
        
        pthread_mutex_lock(&(pool->thread_counter));
        int busy_thread_num = pool->busy_thread_num;
        pthread_mutex_unlock(&(pool->thread_counter));
        
        /* 创建新线程算法，任务数大于最小线程池个数，
         * 且存活的线程数小于最大线程数时
         */
        if (queue_size >= MIN_WAIT_TASK_NUM && live_thread_num < pool->max_thread_num)
        {
            pthread_mutex_lock(&(pool->lock));
            int add = 0;
            
            /* 一次增加 DEFAULT_THREAD_VERY 个线程*/
            for (i = 0; i < pool->max_thread_num && add < DEFAULT_THREAD_VERY
                    && pool->live_thread_num < pool->max_thread_num; i++) 
            {
                if (pool->workers_tid[i] == 0 || !is_thread_alive(pool->workers_tid[i])) 
                {
                    pthread_create(&(pool->workers_tid[i]), NULL, workers_thread,(void *)pool);
                    add++;
                    pool->live_thread_num++;
                }
            }
            
            pthread_mutex_unlock(&(pool->lock));
        }
        
        /* 销毁多余的空闲线程算法，忙线程 x2 小于存活的线程数 且 
         * 存活的线程数大于最小线程数时
         */
        if (busy_thread_num * 2 < live_thread_num && live_thread_num > pool->min_thread_num) 
        {
            /*一次销毁 DEFAULT_THREAD_VERY 个线程*/
            pthread_mutex_lock(&(pool->lock));
            pool->wait_exit_thr_num = DEFAULT_THREAD_VERY;
            pthread_mutex_unlock(&(pool->lock));
            
            for (i = 0; i < DEFAULT_THREAD_VERY; i++) 
            {
                /*通知处在空闲状态的线程，他们会自行终止，线程自杀*/
                pthread_cond_signal(&(pool->queue_not_empty));
            }
        }
    }
    
    return NULL;
}

/***************************************************************************
* 								工作线程处理函数
*函数名: 
*		 workers_thread()
*参  数:
*		 threadpool	    ：包含线程池中所有的参数
*作  用: 
*		 等待分配任务并执行之
*内  容:
*		1）睡眠等待分配任务
*		2）是否终结本线程
*		3）从任务队列上取任务，更改线程busy状态
*		4）执行任务
*		5）恢复为空闲状态
*缺  点：
*		使用了太多的互斥锁和条件变量，效率上值得商榷
****************************************************************************/

void *workers_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t *)threadpool;
    threadpool_task_t task;

    while(1) 
    {
        /* Lock must be taken to wait on condition variable */
        /* 刚创建出线程，等待任务队列里面有任务，否则阻塞等待任务队列里有任务再唤醒
         * 接收任务
         */
        pthread_mutex_lock(&(pool->lock));
        
        /* queue_size == 0 说明没有任务，调wait 阻塞在条件变量上，若有任务，跳过该while */
        while((pool->queue_size == 0) && (!pool->shutdown)) 
        {
            //printf("Workers'thread ID 0x%x is waiting\n", (unsigned int)pthread_self());
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));
            
            /* 清除指定数目的空闲线程，如果要结束的线程个数大于0，结束线程 */
            if (pool->wait_exit_thr_num > 0) 
            { 
                /* 如果线程池里的线程个数大于最小值时可以结束当前线程 */
                if (pool->live_thread_num > pool->min_thread_num) 
                {
                    printf("Workers'thread ID 0x%x is exiting\n", (unsigned int)pthread_self());
                    pool->live_thread_num--;
					pool->wait_exit_thr_num--;
                    pthread_mutex_unlock(&(pool->lock));
                    pthread_exit(NULL);
                }
            }
        }
        
        /*如果关闭了线程池，自行退出处理*/
        if (pool->shutdown == 1) 
        {
			printf("Workers'thread ID 0x%x is exiting\n", (unsigned int)pthread_self());
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }
        
        /*从任务队列里获取任务，是一个出队操作*/
        task.function = pool->task_queue[pool->queue_front].function;
        task.arg = pool->task_queue[pool->queue_front].arg;

        /*出队，模拟环形队列*/
        pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;
        pool->queue_size--;
        
        /*通知可以有新的任务添加进来*/
        pthread_cond_broadcast(&(pool->queue_not_full));
        
        /*任务取出后，立即将线程池锁释放*/
        pthread_mutex_unlock(&(pool->lock));
        
		/*设置当前线程忙状态*/
        pthread_mutex_lock(&(pool->thread_counter));       /*忙状态线程数变量锁*/
        pool->busy_thread_num++;                           /*忙状态线程数+1*/
        pthread_mutex_unlock(&(pool->thread_counter));
		
		/*执行任务*/
        (*(task.function))(task.arg);                  
        
        /*由忙状态切换为空闲状态*/
        pthread_mutex_lock(&(pool->thread_counter));
        pool->busy_thread_num--;
        pthread_mutex_unlock(&(pool->thread_counter));
    }
    
    return NULL;
}

/***************************************************************************
* 						向线程池的任务队列中添加一个任务
*函数名: 
*		threadpool_add()
*参  数:
*		pool		: 使用的线程池
*		function	：任务的执行函数
*		arg			：任务执行参数的参数
*作  用: 
*		 向线程池的任务队列中添加一个任务
*内  容:
*		1）任务队列是否已满
*		2）添加任务
*		3）唤醒在任务队列上睡眠的线程
*缺  点：
*		使用了太多的互斥锁和条件变量，效率上值得商榷
****************************************************************************/
int threadpool_add(threadpool_t *pool, void *function(void *arg), void *arg)
{
    pthread_mutex_lock(&(pool->lock));
    
    /*为真，队列已满，调wait等待*/
    while ((pool->queue_size == pool->queue_max_size) && (!pool->shutdown)) 
    {
        pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));
    }
    
    if (pool->shutdown) 
    {
        pthread_mutex_unlock(&(pool->lock));
		return 0;
    }
    
    // /*清空工作线程 调用回调函数的参数 arg*/
    // if (pool->task_queue[pool->queue_rear].arg != NULL) {
    //     free(pool->task_queue[pool->queue_rear].arg);
    //     pool->task_queue[pool->queue_rear].arg = NULL;
    // }
    /*添加任务到任务队列里面*/
    pool->task_queue[pool->queue_rear].function = function;
    pool->task_queue[pool->queue_rear].arg = arg;
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;
    pool->queue_size++;
    
    /*添加完任务后，队列不为空，唤醒线程池中等待处理任务的线程*/
    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->lock));
    
    return 0;
}

/***************************************************************************
* 								销毁线程池
*函数名: 
*		threadpool_distory()
*参  数:
*		threadpool	: 要销毁的线程池
*作  用: 
*		 销毁线程池
*内  容:
*		1）发送任务，销毁线程
*		2）回收线程资源
*缺  点：
*		无
****************************************************************************/
int threadpool_distory(threadpool_t *pool)
{
    int i;
    if (pool == NULL) 
    {
        return -1;
    }

    pool->shutdown = 1;
    
    /*先销毁管理线程*/
    pthread_join(pool->manager_tid, NULL);
    
    for (i = 0; i < pool->live_thread_num; i++) 
    {/*通知所有空闲线程*/
        pthread_cond_broadcast(&(pool->queue_not_empty));
    }
    for (i = 0; i < pool->live_thread_num; i++) 
    {/*回收所有管理者线程资源*/
        pthread_join(pool->workers_tid[i], NULL);
    }
    threadpool_free(pool);
    
    return 0;
}
/***************************************************************************
* 								释放线程池资源
*函数名: 
*		threadpool_free()
*参  数:
*		threadpool	: 要释放的线程池
*作  用: 
*		 释放线程池资源
*内  容:
*		1）释放任务队列
*		2）销毁互斥锁和条件变量
*		3）释放线程池
*缺  点：
*		无
****************************************************************************/
int threadpool_free(threadpool_t *pool)
{
    if (pool == NULL) 
    {
        printf("thread pool is already free\n");
        return -1;
    }
    
    if (pool->task_queue) 
    {
        free(pool->task_queue);
    }
    
    if (pool->workers_tid) 
    {
        free(pool->workers_tid);
        pthread_mutex_lock(&(pool->lock));
        pthread_mutex_destroy(&(pool->lock));
        pthread_mutex_lock(&(pool->thread_counter));
        pthread_mutex_destroy(&(pool->thread_counter));
        pthread_cond_destroy(&(pool->queue_not_empty));
        pthread_cond_destroy(&(pool->queue_not_full));
    }
    free(pool);
    pool = NULL;
    
    return 0;
}

/***************************************************************************
* 							判断当前线程是否存在
*函数名: 
*		is_thread_alive()
*参  数:
*		tid	: 待查询的线程PID
*作  用: 
*		 判断当前线程是否存在
*内  容:
*		1）发送0信号来判断
*缺  点：
*		无
****************************************************************************/
int is_thread_alive(pthread_t tid)
{
    int kill_rc = pthread_kill(tid, 0); /*发0号信号，测试线程是否存活*/
    if (kill_rc == ESRCH) 
    {
        return 0;
    }   
    return 1;
}


