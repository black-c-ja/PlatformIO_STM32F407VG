# FreeRTOS CMSIS-RTOS API 使用指南

## 1. 内核管理函数

### 初始化与启动
```c
osStatus_t osKernelInitialize(void);  // 初始化RTOS内核
osStatus_t osKernelStart(void);       // 启动RTOS内核调度器
```

### 内核状态控制
```c
osKernelState_t osKernelGetState(void);  // 获取当前内核状态
int32_t osKernelLock(void);              // 锁定调度器
int32_t osKernelUnlock(void);            // 解锁调度器
```

### 时钟管理
```c
uint32_t osKernelGetTickCount(void);     // 获取系统节拍计数
uint32_t osKernelGetTickFreq(void);      // 获取系统节拍频率
uint32_t osKernelGetSysTimerCount(void); // 获取系统定时器计数
```

## 2. 线程管理

### 线程创建与控制
```c
osThreadId_t osThreadNew(
    osThreadFunc_t func,           // 线程函数
    void *argument,                // 传递给线程的参数
    const osThreadAttr_t *attr     // 线程属性
);

// 线程属性结构体
typedef struct {
    const char *name;              // 线程名称
    uint32_t attr_bits;           // 属性位
    void *cb_mem;                 // 控制块内存
    uint32_t cb_size;            // 控制块大小
    void *stack_mem;             // 栈内存
    uint32_t stack_size;         // 栈大小
    osPriority_t priority;       // 线程优先级
} osThreadAttr_t;
```

### 线程控制函数
```c
osStatus_t osThreadYield(void);                    // 线程让出CPU
osStatus_t osThreadSuspend(osThreadId_t thread_id);// 挂起线程
osStatus_t osThreadResume(osThreadId_t thread_id); // 恢复线程
osStatus_t osThreadTerminate(osThreadId_t thread_id);// 终止线程
```

### 线程状态查询
```c
osThreadState_t osThreadGetState(osThreadId_t thread_id); // 获取线程状态
osPriority_t osThreadGetPriority(osThreadId_t thread_id); // 获取线程优先级
uint32_t osThreadGetStackSpace(osThreadId_t thread_id);   // 获取线程剩余栈空间
```

## 3. 同步原语

### 信号量
```c
osSemaphoreId_t osSemaphoreNew(
    uint32_t max_count,           // 最大计数值
    uint32_t initial_count,       // 初始计数值
    const osSemaphoreAttr_t *attr // 信号量属性
);

osStatus_t osSemaphoreAcquire(
    osSemaphoreId_t semaphore_id, // 信号量ID
    uint32_t timeout              // 超时时间
);

osStatus_t osSemaphoreRelease(osSemaphoreId_t semaphore_id); // 释放信号量
```

### 互斥量
```c
osMutexId_t osMutexNew(const osMutexAttr_t *attr);  // 创建互斥量

osStatus_t osMutexAcquire(
    osMutexId_t mutex_id,        // 互斥量ID
    uint32_t timeout            // 超时时间
);

osStatus_t osMutexRelease(osMutexId_t mutex_id);    // 释放互斥量
```

### 事件标志组
```c
osEventFlagsId_t osEventFlagsNew(const osEventFlagsAttr_t *attr); // 创建事件标志组

uint32_t osEventFlagsSet(
    osEventFlagsId_t ef_id,     // 事件标志组ID
    uint32_t flags              // 要设置的标志位
);

uint32_t osEventFlagsWait(
    osEventFlagsId_t ef_id,     // 事件标志组ID
    uint32_t flags,             // 等待的标志位
    uint32_t options,           // 选项
    uint32_t timeout           // 超时时间
);
```

## 4. 内存管理

### 内存池
```c
osMemoryPoolId_t osMemoryPoolNew(
    uint32_t block_count,        // 内存块数量
    uint32_t block_size,         // 每个内存块大小
    const osMemoryPoolAttr_t *attr // 内存池属性
);

void *osMemoryPoolAlloc(
    osMemoryPoolId_t mp_id,     // 内存池ID
    uint32_t timeout           // 超时时间
);

osStatus_t osMemoryPoolFree(
    osMemoryPoolId_t mp_id,     // 内存池ID
    void *block                // 要释放的内存块
);
```

## 5. 消息队列

```c
osMessageQueueId_t osMessageQueueNew(
    uint32_t msg_count,          // 消息数量
    uint32_t msg_size,           // 消息大小
    const osMessageQueueAttr_t *attr // 消息队列属性
);

osStatus_t osMessageQueuePut(
    osMessageQueueId_t mq_id,    // 消息队列ID
    const void *msg_ptr,         // 消息指针
    uint8_t msg_prio,            // 消息优先级
    uint32_t timeout            // 超时时间
);

osStatus_t osMessageQueueGet(
    osMessageQueueId_t mq_id,    // 消息队列ID
    void *msg_ptr,               // 消息接收缓冲区
    uint8_t *msg_prio,           // 消息优先级
    uint32_t timeout            // 超时时间
);
```

## 6. 定时器

```c
osTimerId_t osTimerNew(
    osTimerFunc_t func,          // 定时器回调函数
    osTimerType_t type,          // 定时器类型(一次性/周期性)
    void *argument,              // 传递给回调函数的参数
    const osTimerAttr_t *attr    // 定时器属性
);

osStatus_t osTimerStart(
    osTimerId_t timer_id,        // 定时器ID
    uint32_t ticks              // 定时器周期(系统节拍数)
);

osStatus_t osTimerStop(osTimerId_t timer_id);  // 停止定时器
```

## 使用注意事项

1. 所有API函数都以os开头，便于识别
2. 大多数函数都返回osStatus_t类型，用于指示操作是否成功
3. 超时参数通常可以设置为：
   - 0：立即返回
   - osWaitForever：永久等待
   - 其他值：等待指定的系统节拍数
4. 创建对象时需要注意内存分配，可以使用静态或动态分配
5. 在中断服务程序中只能使用特定的API函数（带ISR后缀的函数）

## 示例代码

### 创建线程
```c
void ThreadFunction(void *argument) {
    while (1) {
        // 线程代码
        osDelay(1000);  // 延时1000个系统节拍
    }
}

// 创建线程
osThreadAttr_t attr = {
    .name = "TestThread",
    .priority = osPriorityNormal,
    .stack_size = 128 * 4
};
osThreadId_t threadId = osThreadNew(ThreadFunction, NULL, &attr);
```

### 使用信号量
```c
// 创建信号量
osSemaphoreId_t semId = osSemaphoreNew(1, 0, NULL);

// 等待信号量
osSemaphoreAcquire(semId, osWaitForever);

// 释放信号量
osSemaphoreRelease(semId);
```

### 使用消息队列
```c
// 创建消息队列
osMessageQueueId_t queueId = osMessageQueueNew(16, sizeof(uint32_t), NULL);

// 发送消息
uint32_t msg = 123;
osMessageQueuePut(queueId, &msg, 0, osWaitForever);

// 接收消息
uint32_t received;
osMessageQueueGet(queueId, &received, NULL, osWaitForever);
```