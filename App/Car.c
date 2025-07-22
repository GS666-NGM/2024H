#include "Car.h"

#define CAR_START_STACK                 128
#define CAR_START_PRIORITY              1
TaskHandle_t car_start_handle;
void Car_Start(void* pv);

#define DATA_TASK_STACK                 128
#define DATA_TASK_STACK_PRIORITY        5
TaskHandle_t data_task_handle;
void Data_Task(void* pv);

#define CONTROL_TASK_STACK              128*2
#define CONTROL_TASK_STACK_PRIORITY     4
TaskHandle_t control_task_handle;
void Control_Task(void* pv);

#define SHOW_TASK_STACK                 128*2
#define SHOW_TASK_STACK_PRIORITY        3
TaskHandle_t show_task_handle;
void Show_Task(void* pv);

#define DEBUG_TASK_STACK                 128
#define DEBUG_TASK_STACK_PRIORITY        2
TaskHandle_t debug_task_handle;
void Debug_Task(void* pv);

void  Car_Init(void)
{
    xTaskCreate( (TaskFunction_t)  Car_Start,
                (char *) " Car_Start", 
                (configSTACK_DEPTH_TYPE)  CAR_START_STACK,
                (void *) NULL,
                (UBaseType_t)  CAR_START_PRIORITY,
                (TaskHandle_t *) & car_start_handle );
    vTaskStartScheduler();
}

void  Car_Start(void* pv)
{
    taskENTER_CRITICAL();
    
    xTaskCreate( (TaskFunction_t) Data_Task,
                (char *) "Data_Task", 
                (configSTACK_DEPTH_TYPE) DATA_TASK_STACK,
                (void *) NULL,
                (UBaseType_t) DATA_TASK_STACK_PRIORITY,
                (TaskHandle_t *) &data_task_handle );
                
    xTaskCreate( (TaskFunction_t) Control_Task,
                (char *) "Control_Task", 
                (configSTACK_DEPTH_TYPE) CONTROL_TASK_STACK,
                (void *) NULL,
                (UBaseType_t) CONTROL_TASK_STACK_PRIORITY,
                (TaskHandle_t *) &control_task_handle );
                
    xTaskCreate( (TaskFunction_t) Show_Task,
                (char *) "Show_Task", 
                (configSTACK_DEPTH_TYPE) SHOW_TASK_STACK,
                (void *) NULL,
                (UBaseType_t) SHOW_TASK_STACK_PRIORITY,
                (TaskHandle_t *) &show_task_handle );
    xTaskCreate( (TaskFunction_t) Debug_Task,
                (char *) "Debug_Task", 
                (configSTACK_DEPTH_TYPE) DEBUG_TASK_STACK,
                (void *) NULL,
                (UBaseType_t) DEBUG_TASK_STACK_PRIORITY,
                (TaskHandle_t *) &debug_task_handle );
                
    vTaskDelete(NULL);
                
    taskEXIT_CRITICAL();
}
/*===========================================
上面是任务初始化，下面是执行的任务
//==============================================*/
//imudata angle;
//short enl, enr;
//float x, y; 
//float tx, ty;
//char debugrxdata[30];
//uint8_t debugflag;
//uint8_t positionflag = 10;

uint8_t tick1, tick2, tick3;
void Data_Task(void* pv)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    
    while(1)
    {
        tick1++;
//        //imu获取数据可能有bug，用这个来重置
//        if((uint32_t*)imu_buffer[0] == 0 || (uint32_t*)imu_buffer[1] == 0) 
//        {
//            MX_DMA_Init();
//            MX_USART2_UART_Init();
//            SET_BIT(huart2.Instance->CR3, USART_CR3_DMAR);
//            SET_BIT(huart2.Instance->CR3, USART_CR3_EIE);
//            HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*) imu_buffer, 44);
//        }
//        
//        JY901S_DataConverse(&angle);//获取角度
//        Get_Encoder(&enl, &enr);//获取编码值
//        Get_Position(enl, enr, angle, &x, &y);
////        //给控制任务通知
        xTaskNotifyGive(control_task_handle);//给控制任务通知
        xTaskDelayUntil(&pxPreviousWakeTime, 10);
    }
}


//sacc_typedef s_velocity;
//sacc_typedef s_turn;
//float pwm_l;
//float pwm_r;

void Control_Task(void* pv)
{
//    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
//    PID_Init(&velocity_pid,220, 10.0, 0, 0);//220, 10.0, 0, 0
//    PID_Init(&gyro_pid, 7.5, 0.7, 0, 0);//9.0, 0.7, 1, 0
//    PID_Init(&turn_pid, 6.0, 0.0007, 2, 0);//7.5, 0.0007, 0, 0
//    PID_Init(&position_pid, 2.0, 0.01, 0, 0);//2.2, 0.01, 0, 0
//    position_pid.lastError = 1000;
//    Sacc_Init(&s_velocity, 650.0f, 200.0f, 20.0f, 5.0f);
//    Sacc_Init(&s_turn, 550.0f, 200.0f, 20.0f, 1.0f);
    
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);//等待通知
        
//        //位置环串速度环
//        if(positionflag != 10)
//        {
//            float position_out;
//            if(turn_pid.setpoint == s_turn.current && positionflag == 1)
//                positionflag = 2;
//            position_out = PID_Position(&position_pid, tx, ty, x, y);
//            velocity_pid.setpoint = position_out;
//        }
//        else//xbox来控制速度
//        {
//            acc_calculate(&s_velocity);
//            velocity_pid.setpoint = s_velocity.current;
//        }
//        
//        //转向环s形加速
//        acc_calculate(&s_turn);
//        s_turn.current > 180 ? s_turn.current = -180 : 0;
//        s_turn.current < -180 ? s_turn.current = 180 : 0;
//        turn_pid.setpoint = s_turn.current;
//        
//        float velocity_out = PID_Velocity(&velocity_pid, (enl+enr));//速度环 
//        float turn_out = PID_Turn(&turn_pid, angle.yaw);//转向环
//        gyro_pid.setpoint = -turn_out;//串给角速度环
//        float gyro_out = PID_Gyro(&gyro_pid, angle.gz);//角速度环
//        pwm_l = velocity_out + gyro_out;//获取总pwm
//        pwm_r = velocity_out - gyro_out;
//        PID_Clamp(pwm_l, PWM_MIN, PWM_MAX);//限幅
//        PID_Clamp(pwm_r, PWM_MIN, PWM_MAX);
//        Set_Motor(pwm_l, pwm_r);//输入给电机
////        xTaskDelayUntil(&pxPreviousWakeTime, 10);
        tick2++;    
    }
}

void Show_Task(void* pv)
{
    float turntemp = 0;
    while(1)
    {
        tick3++;
//        //XB给速度、角度的目标值
//       if(xUART8.ReceiveNum != 0)
//       {
//           xUART8.ReceiveNum = 0;
//           Get_Data_Xbox(xUART8.ReceiveData);
////            if (XboxData[0] != 0 && XboxData[0] != 1 && XboxData[1] != 0 && XboxData[1] != 1)
////            {
////                
////            }
////           
//           calculate_target_speeds(XboxData[2], XboxData[3], &turntemp, &s_velocity.target);
//          //死区防止静止时抖动
//          if (s_velocity.target < 3.99f && s_velocity.target > -3.99f)
//          {
//              s_velocity.target = 0;
//          }
//          if (turntemp < 0.4f && turntemp > 0.4f)
//          {
//              turntemp = 0;
//          }
//          //s形加速时间计算
//          s_turn.target += turntemp;
//          s_turn.target <-180 ? s_turn.target += 360 : 0;
//          s_turn.target > 180 ? s_turn.target -= 360 : 0;
//          Angle_Constrain(&s_turn);
//          s_velocity.delta  = s_velocity.target - s_velocity.current;
//          time_calculate(&s_velocity);
//          time_calculate(&s_turn);
//       }
       vTaskDelay(50);
    }
}

void Debug_Task(void* pv)
{
    while(1)
    {
        
//        printf("%.2f, %.2f\r\n", mileage, position_pid.setpoint);
//        if(debugflag == 1)
//        {
//            
//            if(positionflag != 10)
//            {
//                sscanf(debugrxdata, "%4f,%4f", &tx, &ty);
//                s_turn.target = -atan2(tx-x, ty-y)* 57.296;
//                Angle_Constrain(&s_turn); 
//                time_calculate(&s_turn);
//                positionflag = 1;
//            }
//            if(debugrxdata[0] == 0x41)
//                positionflag = 0;
//            
//            debugflag = 0;
//        }
        vTaskDelay(100);
    }
}

/*===================================================================================*/
//各种回调

//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
//{
//   
//    if(huart->Instance == USART2)
//    {
//        if(imurxflag == 0)
//        {
//            memcpy(jytempdata, imu_buffer, 44);
//            imurxflag = 1;
//            imurxsize = Size;
//        }
//            HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*) imu_buffer, 44);
//    }
//    
//    if (huart == &huart8)                                                                    // 判断串口
//    {
//        __HAL_UNLOCK(huart);                                                                 // 解锁串口状态
//        xUART8.ReceiveNum  = Size;                                                          // 把接收字节数，存入结构体xUSART8.ReceiveNum，以备使用
//        memset(xUART8.ReceiveData, 0, sizeof(xUART8.ReceiveData));                         // 清0前一帧的接收数据
//        memcpy(xUART8.ReceiveData, xUART8.BuffTemp, Size);                                 // 把新数据，从临时缓存中，复制到xUSART8.ReceiveData[], 以备使用
//        HAL_UARTEx_ReceiveToIdle_DMA(&huart8, xUART8.BuffTemp, sizeof(xUART8.BuffTemp));   // 再次开启DMA空闲中断; 每当接收完指定长度，或者产生空闲中断时，就会来到这个
//// 其实，在CubeMX配置中，DMA有一个选项 ：Mode的circular, 可以让DMA进行连续地的工作，接收完成后，无需在回调函数里再次开启DMA 。但是，目前的CubeMX版本(V6.10），这个参数的选择，会使我们上面的DMA接收与发送，相冲突。那我们二选一好了，自行手工调用。
//    }
//    
//}

//void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
//{
//    if(huart->Instance == USART1)
//    {
//        debugflag = 1;
//        HAL_UART_Receive_IT(&huart1, (uint8_t*)debugrxdata, 30);
//    }
//}
